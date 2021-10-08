
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
	virtual ~MainWindow();
    bool openFile(const QString &fileName);
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void updateRecentFileActions();
    void openRecentFile();
    void about();
    void updateMenus();
	void updateMDIMenus();
    void updateWindowMenu();
    MdiChild *createMdiChild();
    void switchLayoutDirection();
private:
    enum { MaxRecentFiles = 5 };

    void createActions();
	void createMDIActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName,bool Add = true);
    void setRecentFilesVisible(bool visible);
    MdiChild *activeMdiChild() const;
    QMdiSubWindow *findMdiChild(const QString &fileName) const;

    QMdiArea *	mdiArea;
	QMenu *		windowMenu;
	QAction *	newAct;
	QAction *	openAct;
	QAction *	saveAct;
	QAction *	saveAsAct;
    QAction *	recentFileActs[MaxRecentFiles];
    QAction *	recentFileSeparator;
    QAction *	recentFileSubMenuAct;
    QAction *	closeAct;
    QAction *	closeAllAct;
    QAction *	tileAct;
    QAction *	cascadeAct;
    QAction *	nextAct;
    QAction *	previousAct;
    QAction *	windowMenuSeparatorAct;
private:
	QMenu *				Mdi_ViewMenu;
	QList<QMenu *>		ChildMenus;
	QList<QToolBar *>	ChildToolBars;
};

#endif
