#include "mainwindow.h"
#include "mdichild.h"
#include "ICon.h"
#include "ComplainUtf8.h"
#include "Msg.h"
#include <QtWidgets>

MainWindow::MainWindow()
	:QMainWindow()
    ,mdiArea(new QMdiArea(this))
{
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setBackground(Qt::NoBrush);
	 //css图像尺寸不变
	mdiArea->setObjectName("mdiArea");
	mdiArea->setStyleSheet("#mdiArea{"
		"background-color:white;"
		"background-image:url(" IMG_BACK ");"
		//"border:1px solid red;"
		"background-repeat:no-repeat;"              //不重复
		"background-position:center;"
		"}");

    setCentralWidget(mdiArea);
	createActions();
    createStatusBar();
    updateMenus();
    readSettings();
    setUnifiedTitleAndToolBarOnMac(true);

	//把当前的文档窗口 设置为消息的焦点
	connect(mdiArea, &QMdiArea::subWindowActivated,
		this, [=](QMdiSubWindow * pMdiSubWindow)
	{
		if (pMdiSubWindow && pMdiSubWindow->widget())
			activeMdiChild()->SetCurrent();
		else
			Msg::RemoveCurrentMessager();
		updateMenus();
	});

	//setWindowTitle(tr("MDI")); 标题名
	//this->createMdiChild(); 启动时自动创建子窗口
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateMenus()
{
	bool hasMdiChild = (activeMdiChild() != 0);
	saveAct->setEnabled(hasMdiChild);
	saveAsAct->setEnabled(hasMdiChild);
	closeAct->setEnabled(hasMdiChild);
	closeAllAct->setEnabled(hasMdiChild);
	tileAct->setEnabled(hasMdiChild);
	cascadeAct->setEnabled(hasMdiChild);
	nextAct->setEnabled(hasMdiChild);
	previousAct->setEnabled(hasMdiChild);
	windowMenuSeparatorAct->setVisible(hasMdiChild);

	this->updateMDIMenus();
}

void MainWindow::updateMDIMenus()
{
	bool hasMdiChild = (activeMdiChild() != 0);

	//视图菜单
	Mdi_ViewMenu->menuAction()->setVisible(hasMdiChild);
	Mdi_ViewMenu->clear();
	if (hasMdiChild)
		Mdi_ViewMenu->addActions(activeMdiChild()->GetDockActions());
    for(auto pmenu : ChildMenus)
		pmenu->menuAction()->setVisible(hasMdiChild);
    for(auto ptoolbar : ChildToolBars)
		ptoolbar->setVisible(hasMdiChild);
}

void MainWindow::updateWindowMenu()
{
	windowMenu->clear();

	windowMenu->addAction(tileAct);
	windowMenu->addAction(cascadeAct);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAct);
	windowMenu->addAction(previousAct);
	windowMenu->addAction(windowMenuSeparatorAct);

	windowMenu->addSeparator();
	windowMenu->addAction(tr("切换布局方向&S"), this, &MainWindow::switchLayoutDirection);
	windowMenu->addSeparator();

	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	windowMenuSeparatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) {
		QMdiSubWindow *mdiSubWindow = windows.at(i);
		MdiChild *child = qobject_cast<MdiChild *>(mdiSubWindow->widget());

		QString text;
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		else {
			text = tr("%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		QAction *action = windowMenu->addAction(text, mdiSubWindow, [this, mdiSubWindow]() {
			mdiArea->setActiveSubWindow(mdiSubWindow);
		});
		action->setCheckable(true);
		action->setChecked(child == activeMdiChild());
	}
}

MdiChild *MainWindow::createMdiChild()
{
	MdiChild *child = new MdiChild(this);
	mdiArea->addSubWindow(child);
	child->showMaximized();
	return child;
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
}

void MainWindow::open()
{
	QString Title = "选择导入的几何模型";
	QString Dir = ".";
	QString Filter = "My文件(*.myocc *.myocc1)"
		";;My文件(*.myocc2 *.myocc3)";

	QString fileName = QFileDialog::getOpenFileName(nullptr, Title, Dir, Filter);
    if (!fileName.isEmpty())
        openFile(fileName);
}

bool MainWindow::openFile(const QString &fileName)
{
    if (QMdiSubWindow *existing = findMdiChild(fileName)) {
        mdiArea->setActiveSubWindow(existing);
        return true;
    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
    return succeeded;
}

bool MainWindow::loadFile(const QString &fileName)
{
    MdiChild *child = createMdiChild();
    const bool succeeded = child->loadFile(fileName);
	if (succeeded)
	{
		child->show();
		MainWindow::prependToRecentFiles(fileName);
	}
	else
	{
		child->close();
		mdiArea->closeActiveSubWindow();
		MainWindow::prependToRecentFiles(fileName,false);
	}
      
    
    return succeeded;
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }
static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}
static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void MainWindow::prependToRecentFiles(const QString &fileName,bool Add)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
	if(Add)
		recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void MainWindow::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void MainWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        openFile(action->data().toString());
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
    MdiChild *child = activeMdiChild();
    if (child && child->saveAs()) {
        statusBar()->showMessage(tr("File saved"), 2000);
        MainWindow::prependToRecentFiles(child->currentFile());
    }
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("关于"),
            tr("<b>OCC_QT_Demo</b> "
               "\t\t<b>作者：bilibili 蘑菇_AJUNE </b>"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

MdiChild *MainWindow::activeMdiChild() const
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName) const
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        QGuiApplication::setLayoutDirection(Qt::RightToLeft);
    else
        QGuiApplication::setLayoutDirection(Qt::LeftToRight);
}
