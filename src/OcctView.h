#pragma once

#include <Standard_WarningsDisable.hxx>
#include <QWidget>
#include <QAction>
#include <QList>
#include <Standard_WarningsRestore.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <AIS_Point.hxx>
class TopoDS_Shape;
class QRubberBand;

//class COMMONSAMPLE_EXPORT View: public QWidget
class OcctView : public QWidget
{
	Q_OBJECT
protected:
	enum CurrentAction3d {
		CurAction3d_Nothing,
		CurAction3d_SingleOperation,
		CurAction3d_DragEvent,
		CurAction3d_DynamicZooming,
		CurAction3d_WindowZooming,
		CurAction3d_DynamicPanning,
		CurAction3d_GlobalPanning,
		CurAction3d_DynamicRotation
	};

	enum ViewAction {
		ViewFitAllId, ViewFitAreaId, ViewZoomId, ViewPanId, ViewGlobalPanId, ViewRotationId,
		ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId, ViewRightId,
		ViewAxoId, ViewResetId, ViewHlrOffId, ViewHlrOnId
	};
	enum RaytraceAction { ToolRaytracingId, ToolShadowsId, ToolReflectionsId, ToolAntialiasingId };
public:
	OcctView(Handle(AIS_InteractiveContext) theContext, QWidget* parent);

	~OcctView();

	Handle(AIS_InteractiveContext)&     getXYZContext() { return myXYZContext; }
	const Handle(V3d_View)&       getViewc() { return myView; }
	virtual void init();
	bool dump(Standard_CString theFile);
	QList<QAction*>*       getViewActions();
	QList<QAction*>*       getRaytraceActions();
	void noActiveActions();

	void EnableRaytracing();
	void DisableRaytracing();
	void SetRaytracedShadows(bool theState);
	void SetRaytracedReflections(bool theState);
	void SetRaytracedAntialiasing(bool theState);
	bool IsRaytracingMode() const { return myIsRaytracing; }
	bool IsShadowsEnabled() const { return myIsShadowsEnabled; }
	bool IsReflectionsEnabled() const { return myIsReflectionsEnabled; }
	bool IsAntialiasingEnabled() const { return myIsAntialiasingEnabled; }

	/*!
	Get paint engine for the OpenGL viewer. [ virtual public ]
	*/
	virtual QPaintEngine*     paintEngine() const { return 0; }
signals:
	void LUP();
	void selectionChanged();
	void SingleSelect(const Handle_AIS_InteractiveContext & myContext, const Handle_AIS_InteractiveObject & hAisObj);
	void SingleOperation_Begin(const Handle_V3d_View & myView, const Handle_AIS_InteractiveContext & myContext, QPoint Point);
	void SingleOperation_Move(const Handle_V3d_View & myView, const Handle_AIS_InteractiveContext & myContext, QPoint Point);
	void SingleOperation_End(const Handle_V3d_View & myView, const Handle_AIS_InteractiveContext & myContext, QPoint Point);
public slots:
	void fitAll();
	void fitArea();
	void zoom();
	void pan();
	void globalPan();
	void front();
	void back();
	void top();
	void bottom();
	void left();
	void right();
	void axo();
	void rotation();
	void reset();
	void hlrOn();
	void hlrOff();
	void updateToggled(bool);
	void onBackground();
	void onScreenShot();
	void onRaytraceAction();
protected:
	virtual void paintEvent(QPaintEvent*);
	virtual void resizeEvent(QResizeEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void wheelEvent(QWheelEvent *event);  // ¹öÂÖÊÂ¼þ

	int   getSelectNb();
	Handle(V3d_View)&  getView();
	Handle(AIS_InteractiveContext)& 	getContext();
	void  	activateCursor(const CurrentAction3d);
	void  	Popup(const int x, const int y);
	CurrentAction3d  	getCurrentMode();


	Handle(AIS_Point) AisPnt;

	virtual void onLButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onMButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onRButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onLButtonUp(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onMButtonUp(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onRButtonUp(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);
	virtual void onMouseMove(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point);

private:
	void initCursors();
	void initViewActions();
	void initRaytraceActions();
	void DragEvent(const int x, const int y, const int TheState);
	void InputEvent(const int x, const int y);
	void MoveEvent(const int x, const int y);
	void MultiMoveEvent(const int x, const int y);
	void MultiDragEvent(const int x, const int y, const int TheState);
	void MultiInputEvent(const int x, const int y);
	void DrawRectangle(const int MinX, const int MinY,
		const int MaxX, const int MaxY, const bool Draw);
private:
	bool  myIsRaytracing;
	bool  myIsShadowsEnabled;
	bool  myIsReflectionsEnabled;
	bool  myIsAntialiasingEnabled;

	bool  myDrawRect;      // set when a rect is used for selection or magnify 
	Handle(V3d_View)					myView;
	Handle(AIS_InteractiveContext)		myContext;
	Handle(AIS_InteractiveContext)		myXYZContext;
	CurrentAction3d myCurrentMode;
	Standard_Integer        myXmin;
	Standard_Integer        myYmin;
	Standard_Integer        myXmax;
	Standard_Integer        myYmax;
	Standard_Real  myCurZoom;
	Standard_Boolean        myHlrModeIsOn;
	QList<QAction*>*        myViewActions;
	QList<QAction*>*        myRaytraceActions;
	QMenu* myBackMenu;
	QRubberBand*  myRectBand; //!< selection rectangle rubber band
};


