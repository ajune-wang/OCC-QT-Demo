#include "mainwindow.h"
#include "mdichild.h"
#include "ICon.h"
#include "ComplainUtf8.h"
#include "Msg.h"
#include <QtWidgets>

void MainWindow::createActions()
{
	//文件
	{
		QMenu *fileMenu = menuBar()->addMenu(tr("文件&F"));

		const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
		newAct = new QAction(newIcon, tr("新建&N"), this);
		newAct->setShortcuts(QKeySequence::New);
		newAct->setStatusTip(tr("Create a new file"));
		connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
		fileMenu->addAction(newAct);

		const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
		openAct = new QAction(openIcon, tr("打开&O..."), this);
		openAct->setShortcuts(QKeySequence::Open);
		openAct->setStatusTip(tr("Open an existing file"));
		connect(openAct, &QAction::triggered, this, &MainWindow::open);
		fileMenu->addAction(openAct);

		const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
		saveAct = new QAction(saveIcon, tr("保存&S"), this);
		saveAct->setShortcuts(QKeySequence::Save);
		saveAct->setStatusTip(tr("Save the document to disk"));
		connect(saveAct, &QAction::triggered, this, &MainWindow::save);
		fileMenu->addAction(saveAct);

		const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(":/images/saveas.png"));
		saveAsAct = new QAction(saveAsIcon, tr("另存为&A..."), this);
		saveAsAct->setShortcuts(QKeySequence::SaveAs);
		saveAsAct->setStatusTip(tr("Save the document under a new name"));
		connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
		fileMenu->addAction(saveAsAct);
		fileMenu->addSeparator();

		closeAct = new QAction(tr("关闭&O"), this);
		closeAct->setStatusTip(tr("Close the active window"));
		connect(closeAct, &QAction::triggered,
			mdiArea, &QMdiArea::closeActiveSubWindow);
		fileMenu->addAction(closeAct);

		closeAllAct = new QAction(tr("关闭全部&A"), this);
		closeAllAct->setStatusTip(tr("Close all the windows"));
		connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);
		fileMenu->addAction(closeAllAct);

		fileMenu->addSeparator();
		QMenu *recentMenu = fileMenu->addMenu(tr("最近打开&R..."));
		connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileActions);
		recentFileSubMenuAct = recentMenu->menuAction();

		for (int i = 0; i < MaxRecentFiles; ++i) {
			recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
			recentFileActs[i]->setVisible(false);
		}

		recentFileSeparator = fileMenu->addSeparator();

		setRecentFilesVisible(MainWindow::hasRecentFiles());

		//! [0]
		const QIcon exitIcon = QIcon::fromTheme("application-exit");
		QAction *exitAct = fileMenu->addAction(exitIcon, tr("退出&X"), qApp, &QApplication::closeAllWindows);
		exitAct->setShortcuts(QKeySequence::Quit);
		exitAct->setStatusTip(tr("Exit the application"));
		fileMenu->addAction(exitAct);
		//! [0]
	}

	//窗口
	{
		windowMenu = menuBar()->addMenu(tr("窗口&W"));
		connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

		tileAct = new QAction(tr("标题&T"), this);
		tileAct->setStatusTip(tr("Tile the windows"));
		connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

		cascadeAct = new QAction(tr("排列&C"), this);
		cascadeAct->setStatusTip(tr("Cascade the windows"));
		connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);

		nextAct = new QAction(tr("下一个&X"), this);
		nextAct->setShortcuts(QKeySequence::NextChild);
		nextAct->setStatusTip(tr("Move the focus to the next window"));
		connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

		previousAct = new QAction(tr("上一个&V"), this);
		previousAct->setShortcuts(QKeySequence::PreviousChild);
		previousAct->setStatusTip(tr("Move the focus to the previous window"));
		connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

		windowMenuSeparatorAct = new QAction(this);
		windowMenuSeparatorAct->setSeparator(true);

		updateWindowMenu();

		menuBar()->addSeparator();
	}
	//MDI菜单
	{
		this->createMDIActions();
	}
	//帮助
	{
		QMenu *helpMenu = menuBar()->addMenu(tr("帮助&H"));
		QAction *aboutAct = helpMenu->addAction(tr("关于&A"), this, &MainWindow::about);
		aboutAct->setStatusTip(tr("Show the application's About box"));
		QAction *aboutQtAct = helpMenu->addAction(tr("关于&Qt"), qApp, &QApplication::aboutQt);
		aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	}
	//{//工具栏
	//	QToolBar *fileToolBar = addToolBar(tr("File"));
	//	fileToolBar->addAction(newAct);
	//	fileToolBar->addAction(openAct);
	//	fileToolBar->addAction(saveAct);
	//}
}

void MainWindow::createMDIActions()
{
	//视图
	{
		Mdi_ViewMenu = menuBar()->addMenu("视图&V");
	}
	//模型
	{
		QMenu *  Mdi_ModelMenu = menuBar()->addMenu("模型&M");
		ChildMenus += Mdi_ModelMenu;

		{
			QAction * ImportModelAct = new QAction(tr("导入模型&I"), this);
			ImportModelAct->setStatusTip(tr("导入模型到OCC系统"));
			connect(ImportModelAct, &QAction::triggered, this, [=]()
			{
				if (activeMdiChild())
				{
					activeMdiChild()->ImportModel();
					statusBar()->showMessage(tr("导入模型ing"), 2000);
				}
			});
			Mdi_ModelMenu->addAction(ImportModelAct);
		}
		{
			QAction * ExportModelAct = new QAction(tr("导出模型&E"), this);
			ExportModelAct->setStatusTip(tr("导出模型到文件"));
			connect(ExportModelAct, &QAction::triggered, this, [=]()
			{
				if (activeMdiChild())
				{
					activeMdiChild()->ExportModel();
					statusBar()->showMessage(tr("导出模型ing"), 2000);
				}
			});
			Mdi_ModelMenu->addAction(ExportModelAct);
		}

		//QAction * ImportModelAssAct = new QAction(tr("导入模型_装配体&I"), this);
		//ImportModelAssAct->setStatusTip(tr("导入模型_装配体到MBD标注"));
		//connect(ImportModelAssAct, &QAction::triggered, this, [=]() {
		//	if (activeMdiChild())
		//	{
		//		//activeMdiChild()->document()->ImportModelAss();
		//		statusBar()->showMessage(tr("导入模型_装配体ing"), 2000);
		//	}
		//});
		//Mdi_ModelMenu->addAction(ImportModelAssAct);
	}
	//选择
	{
		QMenu * Mdi_SelMenu = menuBar()->addMenu("选择&S");
		ChildMenus += Mdi_SelMenu;
		QAction * SelNaturalAct = new QAction(QIcon(ICON_SEL_SHAPE), tr("自然&N"), this);
		SelNaturalAct->setStatusTip(tr("选择自然模型"));
		connect(SelNaturalAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelNatural();
		});
		Mdi_SelMenu->addAction(SelNaturalAct);
		QAction * SelSolidAct = new QAction(QIcon(ICON_SEL_SOLID), tr("体&S"), this);
		SelSolidAct->setStatusTip(tr("选择模型——体"));
		connect(SelSolidAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelSolid();
		});
		Mdi_SelMenu->addAction(SelSolidAct);
		QAction * SelFaceAct = new QAction(QIcon(ICON_SEL_FACE), tr("面&F"), this);
		SelFaceAct->setStatusTip(tr("选择模型——面"));
		connect(SelFaceAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelFace();
		});
		Mdi_SelMenu->addAction(SelFaceAct);
		QAction * SelEdgeAct = new QAction(QIcon(ICON_SEL_EDGE), tr("边&E"), this);
		SelEdgeAct->setStatusTip(tr("选择模型——边"));
		connect(SelEdgeAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelEdge();
		});
		Mdi_SelMenu->addAction(SelEdgeAct);
		QAction * SelVertexAct = new QAction(QIcon(ICON_SEL_VERTEX), tr("点&E"), this);
		SelVertexAct->setStatusTip(tr("选择模型——点"));
		connect(SelVertexAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelVertex();
		});
		Mdi_SelMenu->addAction(SelVertexAct);
		QToolBar * Mdi_SelToolBar = this->addToolBar("拓扑选择工具栏");
		ChildToolBars += Mdi_SelToolBar;
		Mdi_SelToolBar->addAction(SelNaturalAct);
		Mdi_SelToolBar->addAction(SelSolidAct);
		Mdi_SelToolBar->addAction(SelFaceAct);
		Mdi_SelToolBar->addAction(SelEdgeAct);
		Mdi_SelToolBar->addAction(SelVertexAct);
	}
	//AIS
	{
		QMenu * Mdi_AISMenu = menuBar()->addMenu("显示&S");
		ChildMenus += Mdi_AISMenu;
		QAction * DisplayAllAct = new QAction(QIcon(ICON_AIS_DISPLAYALL), tr("显示所有&D"), this);
		DisplayAllAct->setStatusTip(tr("显示所有"));
		connect(DisplayAllAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjDisplayAll();
		});
		Mdi_AISMenu->addAction(DisplayAllAct);
		QAction * EraseAllAct = new QAction(QIcon(ICON_AIS_ERASEALL), tr("隐藏所有&E"), this);
		EraseAllAct->setStatusTip(tr("隐藏所有"));
		connect(EraseAllAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjEraseAll();
		});
		Mdi_AISMenu->addAction(EraseAllAct);
		QAction * SetColorAct = new QAction(QIcon(ICON_AIS_OBJCOLOR), tr("设置对象颜色&C"), this);
		SetColorAct->setStatusTip(tr("设置对象颜色"));
		connect(SetColorAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelColor();
		});
		Mdi_AISMenu->addAction(SetColorAct);
		QAction * SetTransparencyAct = new QAction(QIcon(ICON_AIS_TRANSPARENCY), tr("设置透明度&T"), this);
		SetTransparencyAct->setStatusTip(tr("设置透明度"));
		connect(SetTransparencyAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->SelTrans();
		});
		Mdi_AISMenu->addAction(SetTransparencyAct);
		QAction * HideAct = new QAction(QIcon(ICON_AIS_ERASE), tr("隐藏&H"), this);
		HideAct->setStatusTip(tr("隐藏"));
		connect(HideAct, &QAction::triggered, this, [=]() {
			if (activeMdiChild())activeMdiChild()->AisObjHide();
		});
		Mdi_AISMenu->addAction(HideAct);
		//QAction * SelColorAct = new QAction(QIcon(ICON_AIS_SELCOLOR), tr("设置选择高亮颜色&S"), this);
		//SelColorAct->setStatusTip(tr("设置选择高亮颜色"));
		//connect(SelColorAct, &QAction::triggered, this, [=]() {
		//	if (activeMdiChild())activeMdiChild()->SelColor();
		//});
		//Mdi_AISMenu->addAction(SelColorAct);
		QToolBar * Mdi_AISToolBar = this->addToolBar("显示工具栏");
		ChildToolBars += Mdi_AISToolBar;
		Mdi_AISToolBar->addAction(DisplayAllAct);
		Mdi_AISToolBar->addAction(EraseAllAct);
		Mdi_AISToolBar->addAction(SetColorAct);
		Mdi_AISToolBar->addAction(SetTransparencyAct);
		Mdi_AISToolBar->addAction(HideAct);
		//Mdi_AISToolBar->addAction(SelColorAct);
	}
	//测试
	{
		QMenu * Mdi_TestMenu = menuBar()->addMenu("测试&T");
		ChildMenus += Mdi_TestMenu;
		{
			QAction * TestActT = new QAction(tr("测试-测试1"), this);
			connect(TestActT, &QAction::triggered, this, [=]() {
				if (activeMdiChild())
				{
					activeMdiChild()->Test1();
					statusBar()->showMessage(tr("测试-测试1"), 2000);
				}
			});
			Mdi_TestMenu->addAction(TestActT);
		}
	}
}
