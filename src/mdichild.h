
#ifndef MDICHILD_H
#define MDICHILD_H

#include <QMainWindow>
#include "MyDocument.h"
#include "MyViewer.h"

class MdiChild : public QMainWindow
{
    Q_OBJECT
public:
    MdiChild(QWidget * parent);
	virtual ~MdiChild();

	QString userFriendlyCurrentFile();
	QString currentFile() { return curFile; }

	QList<QAction *> GetDockActions();
	Handle(MyDocument) document() { return h_MyDoc; }

	void SetCurrent();
	void createMDIActions();
public: //mdichild_action1
	void newFile();
	bool loadFile(const QString &fileName);
	bool save();
	bool saveAs();
	bool saveFile(const QString &fileName);
public: //mdichild_action2
	void ImportModel();
	void ExportModel();
	void SelNatural();
	void SelSolid();
	void SelFace();
	void SelEdge();
	void SelVertex();

	void AisObjDisplayAll();
	void AisObjEraseAll();
	void AisObjHide();
	void SelTrans();
	void SelUnTrans();
	void SelWireFrame();
	void SelShaded();
	void SelColor();
	void SelUnColor();

	void Test1();
protected slots: //mdichild_slots
	void selectionChanged();
protected:
    void closeEvent(QCloseEvent *event) override;
protected: //mdichild_init
	void Init();
private slots:
    void documentWasModified();
private:
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString					curFile;
    bool					isUntitled;

	//Occt
	class OcctView *			q3dView;

	//Message
	QDockWidget *				p_MessageDock;
	class GUI_Message *			p_MessageWidget;

	Handle(MyDocument)		h_MyDoc;
	Handle(MyViewer)		h_MyViewer;
};
#endif
