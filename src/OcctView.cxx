#include "Icon.h"
#include "ComplainUtf8.h"
#include "OcctView.h"
#include "OcctWindow.h"

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QCursor>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMouseEvent>
#include <QRubberBand>
#include <QMdiSubWindow>
#include <QStyleFactory>
#include <QRegExpValidator>
#include <QRegExpValidator>

#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <AIS_ViewCube.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_TextureEnv.hxx>
#include <AIS_Point.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Trihedron.hxx>

// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftModifier

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlModifier

// for elastic bean selection
#define ValZWMin 1

static QCursor* defCursor = NULL;
static QCursor* handCursor = NULL;
static QCursor* panCursor = NULL;
static QCursor* globPanCursor = NULL;
static QCursor* zoomCursor = NULL;
static QCursor* rotCursor = NULL;

OcctView::OcctView(Handle(AIS_InteractiveContext) theContext,QWidget* parent)
	: QWidget(parent),
	myIsRaytracing(false),
	myIsShadowsEnabled(true),
	myIsReflectionsEnabled(false),
	myIsAntialiasingEnabled(false),
	myViewActions(0),
	myRaytraceActions(0),
	myBackMenu(NULL)
{
	myContext = theContext;
	{
		Handle(AIS_InteractiveContext) hAISContext = new AIS_InteractiveContext(theContext->CurrentViewer()); //创建交互式环境
		hAISContext->SetDisplayMode(AIS_Shaded, true);   //设置显示模式为遮蔽
		{
			{	//CUBE
				Handle(AIS_ViewCube) H_AisViewCube = new AIS_ViewCube();
				hAISContext->SetDisplayMode(AIS_Shaded, true);   //设置显示模式为遮蔽
				H_AisViewCube->SetBoxSideLabel(V3d_Xpos, "右视图");//!< (+Y+Z) view
				H_AisViewCube->SetBoxSideLabel(V3d_Ypos, "背视图");//!< (+X+Z) view
				H_AisViewCube->SetBoxSideLabel(V3d_Zpos, "俯视图");//!< (+Y+Z) view
				H_AisViewCube->SetBoxSideLabel(V3d_Xneg, "左视图");//!< (+Y+Z) view
				H_AisViewCube->SetBoxSideLabel(V3d_Yneg, "正视图");//!< (+Y+Z) view
				H_AisViewCube->SetBoxSideLabel(V3d_Zneg, "仰视图");//!< (+Y+Z) view
				H_AisViewCube->SetTransparency(0.8);

				H_AisViewCube->SetTransformPersistence(
					new Graphic3d_TransformPers(
						Graphic3d_TMF_TriedronPers,
						Aspect_TOTP_RIGHT_UPPER,
						Graphic3d_Vec2i(100, 100)));

				hAISContext->Display(H_AisViewCube, Standard_True);
			}

			{	//XYZ坐标系
				Handle(AIS_Point) Point = new AIS_Point(new Geom_CartesianPoint(0, 0, 0));
				hAISContext->Display(Point, Standard_False);
				Handle(AIS_Trihedron) Trihedron = new AIS_Trihedron(
					new Geom_Axis2Placement(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1))));
				Trihedron->SetSize(30);
				hAISContext->Display(Trihedron, Standard_False);
				hAISContext->SetZLayer(Trihedron, Graphic3d_ZLayerId_Topmost);
				hAISContext->SetTransformPersistence(Trihedron,
					Graphic3d_TransModeFlags::Graphic3d_TMF_ZoomPers);//禁止缩放
			}
		}
		myXYZContext = hAISContext;
	}

	myXmin = 0;
	myYmin = 0;
	myXmax = 0;
	myYmax = 0;
	myCurZoom = 0;
	myRectBand = 0;

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	myCurrentMode = CurAction3d_Nothing;
	myHlrModeIsOn = Standard_False;
	setMouseTracking(true);

	initViewActions();
	initCursors();

	setBackgroundRole(QPalette::NoRole);//NoBackground );
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy(Qt::StrongFocus);
	//setWindowFlag(Qt::MSWindowsOwnDC);

	init();
}

OcctView::~OcctView()
{
	delete myBackMenu;
}

void OcctView::init()
{
	if (myView.IsNull())
		myView = myContext->CurrentViewer()->CreateView();
	myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHEAT, 0.2, V3d_ZBUFFER);

	Handle(OcctWindow) hWnd = new OcctWindow(this);
	myView->SetWindow(hWnd);
	if (!hWnd->IsMapped())
		hWnd->Map();
	
	myView->MustBeResized();
	if (myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;

	myView->SetBackgroundColor(Quantity_NOC_BLACK);
}

void OcctView::paintEvent(QPaintEvent *)
{
	//  QApplication::syncX();
	myView->Redraw();
}

void OcctView::resizeEvent(QResizeEvent *)
{
	//  QApplication::syncX();
	if (!myView.IsNull())
	{
		myView->MustBeResized();
	}
}

void OcctView::fitAll()
{
	myView->FitAll();
	myView->ZFitAll();
	myView->Redraw();
}

void OcctView::fitArea()
{
	myCurrentMode = CurAction3d_WindowZooming;
}

void OcctView::zoom()
{
	myCurrentMode = CurAction3d_DynamicZooming;
}

void OcctView::pan()
{
	myCurrentMode = CurAction3d_DynamicPanning;
}

void OcctView::rotation()
{
	myCurrentMode = CurAction3d_DynamicRotation;
}

void OcctView::globalPan()
{
	// save the current zoom value
	myCurZoom = myView->Scale();
	// Do a Global Zoom
	myView->FitAll();
	// Set the mode
	myCurrentMode = CurAction3d_GlobalPanning;
}

void OcctView::front()
{
	myView->SetProj(V3d_Yneg);
}

void OcctView::back()
{
	myView->SetProj(V3d_Ypos);
}

void OcctView::top()
{
	myView->SetProj(V3d_Zpos);
}

void OcctView::bottom()
{
	myView->SetProj(V3d_Zneg);
}

void OcctView::left()
{
	myView->SetProj(V3d_Xneg);
}

void OcctView::right()
{
	myView->SetProj(V3d_Xpos);
}

void OcctView::axo()
{
	myView->SetProj(V3d_XposYnegZpos);
}

void OcctView::reset()
{
	myView->Reset();
}

void OcctView::hlrOff()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myHlrModeIsOn = Standard_False;
	myView->SetComputedMode(myHlrModeIsOn);
	myView->Redraw();
	QApplication::restoreOverrideCursor();
}

void OcctView::hlrOn()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myHlrModeIsOn = Standard_True;
	myView->SetComputedMode(myHlrModeIsOn);
	myView->Redraw();
	QApplication::restoreOverrideCursor();
}

void OcctView::SetRaytracedShadows(bool theState)
{
	myView->ChangeRenderingParams().IsShadowEnabled = theState;

	myIsShadowsEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void OcctView::SetRaytracedReflections(bool theState)
{
	myView->ChangeRenderingParams().IsReflectionEnabled = theState;

	myIsReflectionsEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void OcctView::onRaytraceAction()
{
	QAction* aSentBy = (QAction*)sender();

	if (aSentBy == myRaytraceActions->at(ToolRaytracingId))
	{
		bool aState = myRaytraceActions->at(ToolRaytracingId)->isChecked();

		QApplication::setOverrideCursor(Qt::WaitCursor);
		if (aState)
			EnableRaytracing();
		else
			DisableRaytracing();
		QApplication::restoreOverrideCursor();
	}

	if (aSentBy == myRaytraceActions->at(ToolShadowsId))
	{
		bool aState = myRaytraceActions->at(ToolShadowsId)->isChecked();
		SetRaytracedShadows(aState);
	}

	if (aSentBy == myRaytraceActions->at(ToolReflectionsId))
	{
		bool aState = myRaytraceActions->at(ToolReflectionsId)->isChecked();
		SetRaytracedReflections(aState);
	}

	if (aSentBy == myRaytraceActions->at(ToolAntialiasingId))
	{
		bool aState = myRaytraceActions->at(ToolAntialiasingId)->isChecked();
		SetRaytracedAntialiasing(aState);
	}
}

void OcctView::SetRaytracedAntialiasing(bool theState)
{
	myView->ChangeRenderingParams().IsAntialiasingEnabled = theState;

	myIsAntialiasingEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void OcctView::EnableRaytracing()
{
	if (!myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;

	myIsRaytracing = true;

	myContext->UpdateCurrentViewer();
}

void OcctView::DisableRaytracing()
{
	if (myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;

	myIsRaytracing = false;

	myContext->UpdateCurrentViewer();
}

void OcctView::updateToggled(bool isOn)
{
	QAction* sentBy = (QAction*)sender();

	if (!isOn)
		return;
	for (int i = ViewFitAllId; i < ViewHlrOffId; i++)
	{
		QAction* anAction = myViewActions->at(i);
		if ((anAction == myViewActions->at(ViewFitAreaId)) ||
			(anAction == myViewActions->at(ViewZoomId)) ||
			(anAction == myViewActions->at(ViewPanId)) ||
			(anAction == myViewActions->at(ViewGlobalPanId)) ||
			(anAction == myViewActions->at(ViewRotationId)))
		{
			if (anAction && (anAction != sentBy))
			{
				anAction->setCheckable(true);
				anAction->setChecked(false);
			}
			else
			{
				sentBy->setCheckable(false);
			}
		}
	}
}

void OcctView::initCursors()
{
	if (!defCursor)
		defCursor = new QCursor(Qt::ArrowCursor);
	if (!handCursor)
		handCursor = new QCursor(Qt::PointingHandCursor);
	if (!panCursor)
		panCursor = new QCursor(Qt::SizeAllCursor);
	if (!globPanCursor)
		globPanCursor = new QCursor(Qt::CrossCursor);
	if ( !zoomCursor )
	  zoomCursor = new QCursor(QPixmap(ICON_CURSOR_ZOOM));
	if ( !rotCursor )
	  rotCursor = new QCursor(QPixmap(ICON_CURSOR_ROTATE));
}

QList<QAction*>* OcctView::getViewActions()
{
	initViewActions();
	return myViewActions;
}

QList<QAction*>* OcctView::getRaytraceActions()
{
	initRaytraceActions();
	return myRaytraceActions;
}

void OcctView::initViewActions()
{
	if (myViewActions)
		return;

	myViewActions = new QList<QAction*>();

	QAction* a;
	a = new QAction(QIcon(ICON_VIEW_FITALL), QObject::tr("调整视图—自动"), this);
	a->setToolTip(QObject::tr("调整视图—自动"));
	a->setStatusTip(QObject::tr("自动调整视图至合适的大小"));
	connect(a, SIGNAL(triggered()), this, SLOT(fitAll()));
	myViewActions->insert(ViewFitAllId, a);

	a = new QAction(QIcon(ICON_VIEW_FITAREA), QObject::tr("调整视图—框选"), this);
	a->setToolTip(QObject::tr("调整视图—框选"));
	a->setStatusTip(QObject::tr("调整视图至框选的区域"));
	connect(a, SIGNAL(triggered()), this, SLOT(fitArea()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewFitAreaId, a);

	a = new QAction(QIcon(ICON_VIEW_ZOOM), QObject::tr("缩放视图"), this);
	a->setToolTip(QObject::tr("缩放视图"));
	a->setStatusTip(QObject::tr("拖拽视图控制缩放视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(zoom()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewZoomId, a);

	a = new QAction(QIcon(ICON_VIEW_PAN), QObject::tr("移动视图"), this);
	a->setToolTip(QObject::tr("移动视图"));
	a->setStatusTip(QObject::tr("移动视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(pan()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewPanId, a);

	a = new QAction(QIcon(ICON_VIEW_GLOBALPAN), QObject::tr("移动视图—位置"), this);
	a->setToolTip(QObject::tr("移动视图—位置"));
	a->setStatusTip(QObject::tr("移动视图至鼠标指定的位置"));
	connect(a, SIGNAL(triggered()), this, SLOT(globalPan()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewGlobalPanId, a);

	a = new QAction(QIcon(ICON_VIEW_ROTATE), QObject::tr("旋转视图"), this);
	a->setToolTip(QObject::tr("旋转视图"));
	a->setStatusTip(QObject::tr("旋转视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(rotation()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewRotationId, a);

	a = new QAction(QIcon(ICON_VIEW_FRONT), QObject::tr("正视图"), this);
	a->setToolTip(QObject::tr("正视图"));
	a->setStatusTip(QObject::tr("正视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(front()));
	myViewActions->insert(ViewFrontId, a);

	a = new QAction(QIcon(ICON_VIEW_BACK), QObject::tr("背视图"), this);
	a->setToolTip(QObject::tr("背视图"));
	a->setStatusTip(QObject::tr("背视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(back()));
	myViewActions->insert(ViewBackId, a);

	a = new QAction(QIcon(ICON_VIEW_TOP), QObject::tr("俯视图"), this);
	a->setToolTip(QObject::tr("俯视图"));
	a->setStatusTip(QObject::tr("俯视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(top()));
	myViewActions->insert(ViewTopId, a);

	a = new QAction(QIcon(ICON_VIEW_BOTTOM), QObject::tr("仰视图"), this);
	a->setToolTip(QObject::tr("仰视图"));
	a->setStatusTip(QObject::tr("仰视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(bottom()));
	myViewActions->insert(ViewBottomId, a);

	a = new QAction(QIcon(ICON_VIEW_LEFT), QObject::tr("左视图"), this);
	a->setToolTip(QObject::tr("左视图"));
	a->setStatusTip(QObject::tr("左视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(left()));
	myViewActions->insert(ViewLeftId, a);

	a = new QAction(QIcon(ICON_VIEW_RIGHT), QObject::tr("右视图"), this);
	a->setToolTip(QObject::tr("右视图"));
	a->setStatusTip(QObject::tr("右视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(right()));
	myViewActions->insert(ViewRightId, a);

	a = new QAction(QIcon(ICON_VIEW_AXO), QObject::tr("轴测视图"), this);
	a->setToolTip(QObject::tr("轴测视图"));
	a->setStatusTip(QObject::tr("轴测视图"));
	connect(a, SIGNAL(triggered()), this, SLOT(axo()));
	myViewActions->insert(ViewAxoId, a);

	a = new QAction(QIcon(ICON_VIEW_RESET), QObject::tr("视图—复原"), this);
	a->setToolTip(QObject::tr("视图—复原"));
	a->setStatusTip(QObject::tr("视图复原至初始位置"));
	connect(a, SIGNAL(triggered()), this, SLOT(reset()));
	myViewActions->insert(ViewResetId, a);

	QActionGroup* ag = new QActionGroup(this);

	a = new QAction(QIcon(ICON_VIEW_COMP_OFF), QObject::tr("MNU_HLROFF"), this);
	a->setToolTip(QObject::tr("TBR_HLROFF"));
	a->setStatusTip(QObject::tr("TBR_HLROFF"));
	connect(a, SIGNAL(triggered()), this, SLOT(hlrOff()));
	a->setCheckable(true);
	a->setChecked(true);
	ag->addAction(a);
	myViewActions->insert(ViewHlrOffId, a);

	a = new QAction(QIcon(ICON_VIEW_COMP_ON), QObject::tr("MNU_HLRON"), this);
	a->setToolTip(QObject::tr("TBR_HLRON"));
	a->setStatusTip(QObject::tr("TBR_HLRON"));
	connect(a, SIGNAL(triggered()), this, SLOT(hlrOn()));

	a->setCheckable(true);
	ag->addAction(a);
	myViewActions->insert(ViewHlrOnId, a);
}

void OcctView::initRaytraceActions()
{
	if (myRaytraceActions)
		return;

	myRaytraceActions = new QList<QAction*>();
	QString dir;
	QAction* a;

	a = new QAction(QPixmap(ICON_RAYTRACE_RAYTRACING), QObject::tr("MNU_TOOL_RAYTRACING"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_RAYTRACING"));
	a->setStatusTip(QObject::tr("TBR_TOOL_RAYTRACING"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolRaytracingId, a);

	a = new QAction(QPixmap(ICON_RAYTRACE_SHADOWS), QObject::tr("MNU_TOOL_SHADOWS"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_SHADOWS"));
	a->setStatusTip(QObject::tr("TBR_TOOL_SHADOWS"));
	a->setCheckable(true);
	a->setChecked(true);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolShadowsId, a);

	a = new QAction(QPixmap(ICON_RAYTRACE_REFLECTIONS), QObject::tr("MNU_TOOL_REFLECTIONS"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_REFLECTIONS"));
	a->setStatusTip(QObject::tr("TBR_TOOL_REFLECTIONS"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolReflectionsId, a);

	a = new QAction(QPixmap(ICON_RAYTRACE_ANTIALIASING), QObject::tr("MNU_TOOL_ANTIALIASING"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_ANTIALIASING"));
	a->setStatusTip(QObject::tr("TBR_TOOL_ANTIALIASING"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolAntialiasingId, a);
}

void OcctView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
		onLButtonDown(e->buttons(), e->modifiers(), e->pos());
	else if (e->button() == Qt::MidButton)
		onMButtonDown(e->buttons(), e->modifiers(), e->pos());
	else if (e->button() == Qt::RightButton)
		onRButtonDown(e->buttons(), e->modifiers(), e->pos());
}

void OcctView::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
		onLButtonUp(e->buttons(), e->modifiers(), e->pos());
	else if (e->button() == Qt::MidButton)
		onMButtonUp(e->buttons(), e->modifiers(), e->pos());
	else if (e->button() == Qt::RightButton)
		onRButtonUp(e->buttons(), e->modifiers(), e->pos());
}

void OcctView::mouseMoveEvent(QMouseEvent* e)
{
	onMouseMove(e->buttons(), e->modifiers(), e->pos());
}

void OcctView::wheelEvent(QWheelEvent * event)
{
	double scar = 1.0;
	double zDelta = event->delta();
	if (zDelta>0)
	{
		scar = 1.2;
		if (myView->Scale() <= 10000)
			myView->SetZoom(scar);
	}
	else
	{
		scar = 0.8;
		if (myView->Scale()>0.0001)
			myView->SetZoom(scar);
	}
}

void OcctView::activateCursor(const CurrentAction3d mode)
{
	switch( mode )
	{
	  case CurAction3d_DynamicPanning:
	    setCursor( *panCursor );
	    break;
	  case CurAction3d_DynamicZooming:
	    setCursor( *zoomCursor );
	    break;
	  case CurAction3d_DynamicRotation:
	    setCursor( *rotCursor );
	    break;
	  case CurAction3d_GlobalPanning:
	    setCursor( *globPanCursor );
	    break;
	  case CurAction3d_WindowZooming:
	    setCursor( *handCursor );
	    break;
	  case CurAction3d_DragEvent:
	  case CurAction3d_Nothing:
	  default:
	    setCursor( *defCursor );
	    break;
	}
}

void OcctView::onLButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point)
{
	//  save the current mouse coordinate in min
	myXmin = point.x();
	myYmin = point.y();
	myXmax = point.x();
	myYmax = point.y();

	if (nKeys & CASCADESHORTCUTKEY)
	{
		myCurrentMode = CurAction3d_DynamicZooming;
	}
	else
	{
		switch (myCurrentMode)
		{
		case CurAction3d_Nothing:
			if (nKeys & MULTISELECTIONKEY)
				MultiInputEvent(point.x(), point.y());
			else
				InputEvent(point.x(), point.y());

			if (this->getSelectNb() == 0)
			{
				myCurrentMode = CurAction3d_DragEvent;
				if (nKeys & MULTISELECTIONKEY)
					MultiDragEvent(myXmax, myYmax, -1);
				else
					DragEvent(myXmax, myYmax, -1);
			}
			else if(this->getSelectNb() == 1)
			{
				myCurrentMode = CurAction3d_SingleOperation;
				emit SingleOperation_Begin(myView, myContext, point);
				{
					myContext->InitSelected();
					emit SingleSelect(myContext, myContext->SelectedInteractive());
				}
			}
			break;
		case CurAction3d_SingleOperation:
		case CurAction3d_DragEvent:
			break;
		case CurAction3d_DynamicZooming:
			break;
		case CurAction3d_WindowZooming:
			break;
		case CurAction3d_DynamicPanning:
			break;
		case CurAction3d_GlobalPanning:
			break;
		case CurAction3d_DynamicRotation:
			if (myHlrModeIsOn)
			{
				myView->SetComputedMode(Standard_False);
			}
			myView->StartRotation(point.x(), point.y());
			break;
		default:
			throw Standard_Failure("incompatible Current Mode");
			break;
		}
	}
	activateCursor(myCurrentMode);
}

void OcctView::onMButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint /*point*/)
{
	if (nKeys & CASCADESHORTCUTKEY)
		myCurrentMode = CurAction3d_DynamicPanning;
	activateCursor(myCurrentMode);
}

void OcctView::onRButtonDown(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point)
{
	myXmin = point.x();
	myYmin = point.y();
	myXmax = point.x();
	myYmax = point.y();

	if (nKeys & CASCADESHORTCUTKEY)
	{
		if (myHlrModeIsOn)
		{
			myView->SetComputedMode(Standard_False);
		}
		myCurrentMode = CurAction3d_DynamicRotation;
		myView->StartRotation(point.x(), point.y());
	}
	activateCursor(myCurrentMode);
}

#include <TopoDS_Shape.hxx>
#include <BRepIntCurveSurface_Inter.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRepGProp_Face.hxx>
#include <AIS_Point.hxx>
#include <Geom_CartesianPoint.hxx>

void OcctView::onLButtonUp(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point)
{
	switch (myCurrentMode)
	{
	case CurAction3d_Nothing:
		break;
	case CurAction3d_SingleOperation:
		if (point != QPoint(myXmin, myYmin))
			emit SingleOperation_End(myView, myContext, point);
		else if (point == QPoint(myXmin, myYmin))
		{
			emit selectionChanged();
			//myContext->InitSelected();
			//TopoDS_Shape Shape = myContext->SelectedShape();
			//if (Shape.IsNull() == false && Shape.ShapeType() == TopAbs_FACE)
			//{
			//	Standard_Integer Xs = point.x();
			//	Standard_Integer Ys = point.y();
			//	Standard_Real Xv, Yv, Zv;
			//	Standard_Real Vx, Vy, Vz;
			//	myView->Convert(Xs, Ys, Xv, Yv, Zv); //向量点
			//	myView->Proj(Vx, Vy, Vz);     //向量方向
			//	gp_Pnt Pnt(Xv, Yv, Zv);
			//	gp_Dir Dir(Vx, Vy, Vz);
			//	gp_Lin ViewLine = gp_Lin(Pnt, Dir);
			//	TopoDS_Face TouchFace = TopoDS::Face(Shape);
			//	BRepIntCurveSurface_Inter IntCS;
			//
			//	IntCS.Init(TouchFace, ViewLine, Precision::Confusion());
			//
			//	if (IntCS.More()) {
			//		BRepGProp_Face G(TopoDS::Face(TouchFace));
			//		gp_Vec GetVec;
			//		gp_Pnt GetTouchPnt;
			//		G.Normal(IntCS.U(), IntCS.V(), GetTouchPnt, GetVec);
			//		gp_Dir GetNormalDir = gp_Dir(GetVec);

			//		if (AisPnt.IsNull())
			//		{
			//			AisPnt = new AIS_Point(new Geom_CartesianPoint(GetTouchPnt));
			//			myContext->Display(AisPnt,Standard_True);
			//		}
			//		else
			//		{
			//			AisPnt->SetComponent(new Geom_CartesianPoint(GetTouchPnt));
			//			myContext->Redisplay(AisPnt, Standard_True);
			//		}
			//		//return Standard_True;
			//	}
			//}
		}
		myCurrentMode = CurAction3d_Nothing;
		break;
	case CurAction3d_DragEvent:
		if ((point.x() == myXmin && point.y() == myYmin) == false)
		{
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
			myXmax = point.x();
			myYmax = point.y();
			if (nKeys & MULTISELECTIONKEY)
				MultiDragEvent(point.x(), point.y(), 1);
			else
				DragEvent(point.x(), point.y(), 1);
		}
		myCurrentMode = CurAction3d_Nothing;
		break;
	case CurAction3d_DynamicZooming:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_WindowZooming:
		DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);//,LongDash);
		myXmax = point.x();
		myYmax = point.y();
		if ((abs(myXmin - myXmax) > ValZWMin) ||
			(abs(myYmin - myYmax) > ValZWMin))
			myView->WindowFitAll(myXmin, myYmin, myXmax, myYmax);
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_DynamicPanning:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_GlobalPanning:
		myView->Place(point.x(), point.y(), myCurZoom);
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_DynamicRotation:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	default:
		throw Standard_Failure(" incompatible Current Mode ");
		break;
	}
	activateCursor(myCurrentMode);
}

void OcctView::onMButtonUp(Qt::MouseButtons /*nFlags*/, Qt::KeyboardModifiers nKeys, const QPoint /*point*/)
{
	myCurrentMode = CurAction3d_Nothing;
	activateCursor(myCurrentMode);
}

void OcctView::onRButtonUp(Qt::MouseButtons /*nFlags*/, Qt::KeyboardModifiers nKeys, const QPoint point)
{
	if (myCurrentMode == CurAction3d_Nothing)
	{
		if ((point.x() == myXmin && point.y() == myYmin) /*&& (nKeys & MULTISELECTIONKEY)*/)
			Popup(point.x(), point.y());
	}
	else
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		// reset tyhe good Degenerated mode according to the strored one
		//   --> dynamic rotation may have change it
		if (myHlrModeIsOn)
		{
			myView->SetComputedMode(myHlrModeIsOn);
			myView->Redraw();
		}
		QApplication::restoreOverrideCursor();
		myCurrentMode = CurAction3d_Nothing;
	}
	activateCursor(myCurrentMode);
}

void OcctView::onMouseMove(Qt::MouseButtons nFlags, Qt::KeyboardModifiers nKeys, const QPoint point)
{
	if (nFlags & Qt::LeftButton || nFlags & Qt::RightButton || nFlags & Qt::MidButton)
	{
		switch (myCurrentMode)
		{
		case CurAction3d_Nothing:
			myXmax = point.x();
			myYmax = point.y();
			break;
		case CurAction3d_SingleOperation:
			myXmax = point.x();
			myYmax = point.y();
			emit SingleOperation_Move(myView, myContext, point);
			break;
		case CurAction3d_DragEvent:
			if (nFlags & Qt::LeftButton)
			{
				myXmax = point.x();
				myYmax = point.y();
				DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
				if (nKeys & MULTISELECTIONKEY)
					MultiDragEvent(myXmax, myYmax, 0);
				else
					DragEvent(myXmax, myYmax, 0);
				DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_True);
			}
			break;
		case CurAction3d_DynamicZooming:
			myView->Zoom(myXmax, myYmax, point.x(), point.y());
			myXmax = point.x();
			myYmax = point.y();
			break;
		case CurAction3d_WindowZooming:
			myXmax = point.x();
			myYmax = point.y();
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_True);
			break;
		case CurAction3d_DynamicPanning:
			myView->Pan(point.x() - myXmax, myYmax - point.y());
			myXmax = point.x();
			myYmax = point.y();
			break;
		case CurAction3d_GlobalPanning:
			break;
		case CurAction3d_DynamicRotation:
			myView->Rotation(point.x(), point.y());
			myView->Redraw();
			break;
		default:
			throw Standard_Failure("incompatible Current Mode");
			break;
		}
	}
	else
	{
		myXmax = point.x();
		myYmax = point.y();
		if (nKeys & MULTISELECTIONKEY)
			MultiMoveEvent(point.x(), point.y());
		else
			MoveEvent(point.x(), point.y());
	}
}

void OcctView::DragEvent(const int x, const int y, const int TheState)
{
	// TheState == -1  button down
	// TheState ==  0  move
	// TheState ==  1  button up

	static Standard_Integer theButtonDownX = 0;
	static Standard_Integer theButtonDownY = 0;

	if (TheState == -1)
	{
		theButtonDownX = x;
		theButtonDownY = y;
	}

	if (TheState == 1)
	{
		myContext->Select(theButtonDownX, theButtonDownY, x, y, myView, Standard_True);
		emit selectionChanged();
	}
}

void OcctView::InputEvent(const int /*x*/, const int /*y*/)
{
	myContext->Select(Standard_True);
	if (!myXYZContext.IsNull())
		myXYZContext->Select(Standard_True);
	emit selectionChanged();
}

void OcctView::MoveEvent(const int x, const int y)
{
	myContext->MoveTo(x, y, myView, Standard_True);
	if (!myXYZContext.IsNull())
		myXYZContext->MoveTo(x, y, myView, Standard_True);
}

void OcctView::MultiMoveEvent(const int x, const int y)
{
	myContext->MoveTo(x, y, myView, Standard_True);
}

void OcctView::MultiDragEvent(const int x, const int y, const int TheState)
{
	static Standard_Integer theButtonDownX = 0;
	static Standard_Integer theButtonDownY = 0;

	if (TheState == -1)
	{
		theButtonDownX = x;
		theButtonDownY = y;
	}
	if (TheState == 0)
	{
		myContext->ShiftSelect(theButtonDownX, theButtonDownY, x, y, myView, Standard_True);
		emit selectionChanged();
	}
}

void OcctView::MultiInputEvent(const int /*x*/, const int /*y*/)
{
	myContext->ShiftSelect(Standard_True);
	emit selectionChanged();
}

void OcctView::Popup(const int /*x*/, const int /*y*/)
{
	//ApplicationCommonWindow* stApp = ApplicationCommonWindow::getApplication();
	//QMdiArea* ws = ApplicationCommonWindow::getWorkspace();
	//QMdiSubWindow* w = ws->activeSubWindow();
	//if ( myContext->NbSelected() )
	//{
	//  QList<QAction*>* aList = stApp->getToolActions();
	//  QMenu* myToolMenu = new QMenu( 0 );
	//  myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolWireframeId ) );
	//  myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolShadingId ) );
	//  myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolColorId ) );
	//      
	//  QMenu* myMaterMenu = new QMenu( myToolMenu );

	//  QList<QAction*>* aMeterActions = ApplicationCommonWindow::getApplication()->getMaterialActions();
	//      
	//  QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
	//  myMaterMenu = myToolMenu->addMenu( QPixmap( dir+QObject::tr("ICON_TOOL_MATER")), QObject::tr("MNU_MATER") );
	//  for ( int i = 0; i < aMeterActions->size(); i++ )
	//    myMaterMenu->addAction( aMeterActions->at( i ) );
	//     
	//  myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolTransparencyId ) );
	//  myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolDeleteId ) );
	//  addItemInPopup(myToolMenu);
	//  myToolMenu->exec( QCursor::pos() );
	//  delete myToolMenu;
	//}
	//else
	{
		if (!myBackMenu)
		{
			myBackMenu = new QMenu(0);
			
			//背景
			{
				QMenu * BackMenu = new QMenu("背景", myBackMenu);
				QAction* a = new QAction(QObject::tr("设置背景颜色"), this);
				a->setToolTip(QObject::tr("设置背景颜色"));
				connect(a, SIGNAL(triggered()), this, SLOT(onBackground()));
				BackMenu->addAction(a);

				a = new QAction(QObject::tr("截屏"), this);
				a->setToolTip(QObject::tr("截屏"));
				connect(a, SIGNAL(triggered()), this, SLOT(onEnvironmentMap()));
				BackMenu->addAction(a);
				myBackMenu->addMenu(BackMenu);
			}
			//剖视图
			{
				QMenu * ClipMenu = new QMenu("剖视图", myBackMenu);

				QAction* a = new QAction(QObject::tr("无剖视图"), this);
				a->setToolTip(QObject::tr("无剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("X轴正向"), this);
				a->setToolTip(QObject::tr("X轴正向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), gp::DX())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("X轴反向"), this);
				a->setToolTip(QObject::tr("X轴反向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), -gp::DX())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("Y轴正向"), this);
				a->setToolTip(QObject::tr("Y轴正向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), gp::DY())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("Y轴反向"), this);
				a->setToolTip(QObject::tr("Y轴反向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), -gp::DY())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("Z轴正向"), this);
				a->setToolTip(QObject::tr("Z轴正向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), gp::DZ())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("Z轴反向"), this);
				a->setToolTip(QObject::tr("Z轴反向-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
					myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(gp::Origin(), -gp::DZ())));
				});
				ClipMenu->addAction(a);

				a = new QAction(QObject::tr("自定义平面"), this);
				a->setToolTip(QObject::tr("自定义平面-剖视图"));
				connect(a, &QAction::triggered, this, [=]()
				{
					//创建对话框
					QDialog Dialog;
					Dialog.setWindowTitle("请输入一个自定义平面");
					Dialog.setMaximumHeight(40);
					//创建编辑框
					QRegExp regExp("-?[0-9]*\\.[0-9]*");
					QLineEdit * pEdits[6] = {nullptr};
					for (int index = 0; index < 6; index++)
					{
						QLineEdit * pEdit = new QLineEdit("0", &Dialog);
						pEdit->setValidator(new QRegExpValidator(regExp, pEdit));
						pEdit->setMinimumWidth(5);
						pEdits[index] = pEdit;
					}
					//布局
					QVBoxLayout * pLayout = new QVBoxLayout();
					QString LabelStr[2] = { "平面上一点","平面的方向" };
					for (int m = 0; m < 2; m++)
					{
						QHBoxLayout * pLayout_in = new QHBoxLayout();
						pLayout->addLayout(pLayout_in);
						pLayout_in->addWidget(new QLabel(LabelStr[m], &Dialog));
						for (int n = 0; n < 3; n++)
							pLayout_in->addWidget(pEdits[3 * m + n]);
					}
					QDialogButtonBox * pBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &Dialog);
					connect(pBox, &QDialogButtonBox::accepted, &Dialog, &QDialog::accept);
					connect(pBox, &QDialogButtonBox::rejected, &Dialog, &QDialog::reject);
					pLayout->addWidget(pBox);
					Dialog.setLayout(pLayout);
					if (QDialog::Accepted == Dialog.exec())
					{
						double P[6] = { 0 };
						for(int index = 0;index<6;index++)
							P[index] = pEdits[index]->text().toDouble();
						if (sqrt(P[3] * P[3] + P[4] * P[4] + P[5] * P[5]) > 1e-7)
						{
							gp_Pnt Pnt(P[0], P[1], P[2]);
							gp_Dir Dir(P[3], P[4], P[5]);
							myView->SetClipPlanes(new Graphic3d_SequenceOfHClipPlane());
							myView->AddClipPlane(new Graphic3d_ClipPlane(gp_Pln(Pnt, Dir)));
						}
						else
							QMessageBox::information(this, "ERROR", "向量的模长不能为零");
					}
				});
				ClipMenu->addAction(a);
				myBackMenu->addMenu(ClipMenu);
			}
		}

		myBackMenu->exec(QCursor::pos());
	}
}

void OcctView::DrawRectangle(const int MinX, const int MinY,
	const int MaxX, const int MaxY, const bool Draw)
{
	static Standard_Integer StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
	static Standard_Boolean m_IsVisible;

	StoredMinX = (MinX < MaxX) ? MinX : MaxX;
	StoredMinY = (MinY < MaxY) ? MinY : MaxY;
	StoredMaxX = (MinX > MaxX) ? MinX : MaxX;
	StoredMaxY = (MinY > MaxY) ? MinY : MaxY;

	QRect aRect;
	aRect.setRect(StoredMinX, StoredMinY, abs(StoredMaxX - StoredMinX), abs(StoredMaxY - StoredMinY));

	if (!myRectBand)
	{
		myRectBand = new QRubberBand(QRubberBand::Rectangle, this);
		myRectBand->setStyle(QStyleFactory::create("windows"));
		myRectBand->setGeometry(aRect);
		myRectBand->show();
	}

	if (m_IsVisible && !Draw) // move or up  : erase at the old position
	{
		myRectBand->hide();
		delete myRectBand;
		myRectBand = 0;
		m_IsVisible = false;
	}

	if (Draw) // move : draw
	{
		//aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
		m_IsVisible = true;
		myRectBand->setGeometry(aRect);
		//myRectBand->show();
	}
}

void OcctView::noActiveActions()
{
	for (int i = ViewFitAllId; i < ViewHlrOffId; i++)
	{
		QAction* anAction = myViewActions->at(i);
		if ((anAction == myViewActions->at(ViewFitAreaId)) ||
			(anAction == myViewActions->at(ViewZoomId)) ||
			(anAction == myViewActions->at(ViewPanId)) ||
			(anAction == myViewActions->at(ViewGlobalPanId)) ||
			(anAction == myViewActions->at(ViewRotationId)))
		{
			setCursor(*defCursor);
			anAction->setCheckable(true);
			anAction->setChecked(false);
		}
	}
}

void OcctView::onBackground()
{
	QColor aColor;
	Standard_Real R1;
	Standard_Real G1;
	Standard_Real B1;
	myView->BackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
	aColor.setRgb((Standard_Integer)(R1 * 255), (Standard_Integer)(G1 * 255), (Standard_Integer)(B1 * 255));
	QColor aRetColor = QColorDialog::getColor(aColor);
	if (aRetColor.isValid())
	{
		R1 = aRetColor.red() / 255.;
		G1 = aRetColor.green() / 255.;
		B1 = aRetColor.blue() / 255.;
		myView->SetBackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
	}
	myView->Redraw();
}

void OcctView::onScreenShot()
{
	QMessageBox::information(nullptr, "截图", "啥也没有!");
}

bool OcctView::dump(Standard_CString theFile)
{
	return myView->Dump(theFile);
}

int OcctView::getSelectNb()
{
	return myContext->NbSelected();
}

Handle(V3d_View)& OcctView::getView()
{
	return myView;
}

Handle(AIS_InteractiveContext)& OcctView::getContext()
{
	return myContext;
}

OcctView::CurrentAction3d OcctView::getCurrentMode()
{
	return myCurrentMode;
}



