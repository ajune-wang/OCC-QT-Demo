#include "mdichild.h"
#include "ComplainUtf8.h"
#include "GUI_Message.h"

#include <QtWidgets>
#include <QDockWidget>

MdiChild::MdiChild(QWidget * parent)
	:QMainWindow(parent)
{
	//InitMBD
	h_MyDoc = new MyDocument();
	h_MyViewer = new MyViewer(h_MyDoc);
	//Config
	this->setMenuBar(nullptr);
	setAttribute(Qt::WA_DeleteOnClose);
	isUntitled = true;
	//Init
	this->Init();
}

MdiChild::~MdiChild()
{
}

void MdiChild::SetCurrent()
{
	p_MessageWidget->SetCurrentMessager();
}

void MdiChild::createMDIActions()
{
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

QList<QAction*> MdiChild::GetDockActions()
{
	return QList<QAction*>()
		<< p_MessageDock->toggleViewAction();
		//<< p_TopoDock->toggleViewAction()
		//<< p_PropertyDock->toggleViewAction()
		//<< p_AddGD_T_Dock->toggleViewAction()
		//<< p_AddSR_Dock->toggleViewAction();
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool MdiChild::maybeSave()
{
    if (!document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
