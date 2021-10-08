#pragma once
#include "MyDocument.h"
#include <Standard_Transient.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Manipulator.hxx>

class MyViewer
	:public Standard_Transient
{
	DEFINE_STANDARD_RTTIEXT(MyViewer, Standard_Transient)
public:
	MyViewer(const Handle(MyDocument) & hDoc);
	virtual ~MyViewer();

	const Handle(MyDocument) & getDocument();
	const Handle(AIS_InteractiveContext) & getAisContext() { return h_AisContext; }
	const Handle(AIS_InteractiveContext) & getTempAisContext() { return h_TempAisContext; }

	NCollection_List<Handle(AIS_InteractiveObject)> GetAisObj();
	NCollection_List<Handle(AIS_InteractiveObject)> GetSelAisObj();

	bool getAisObjColor(Quantity_Color & Color);
	void AisObjColor(Quantity_Color Color);
	void AisObjUnColor();
	void AisObjTransparency(Standard_Real Transparency);
	void AisObjUnTransparency();
	void AisObjDisplayAll();
	void AisObjEraseAll();
	void AisObjHide();
	void AisObjWireFrame();
	void AisObjShaded();

	Quantity_Color getAisSelColor();
	void AisSelColor(Quantity_Color Color);
	
	void selectionChanged();
	void Display(const TopoDS_Shape & Shape);
protected:
	Handle(MyDocument)					h_Doc;
	Handle(V3d_Viewer)					h_Viewer;
	Handle(AIS_InteractiveContext)		h_AisContext;
	Handle(AIS_InteractiveContext)		h_TempAisContext;

	//²Ù×÷Æ÷
	//======ÎÞ===============
};
DEFINE_STANDARD_HANDLE(MyViewer, Standard_Transient)
