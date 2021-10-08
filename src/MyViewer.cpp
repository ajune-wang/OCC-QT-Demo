#include "MyViewer.h"
#include "OCCT_GraphDriver.h"
#include "AIS_Shape.hxx"

IMPLEMENT_STANDARD_RTTIEXT(MyViewer, Standard_Transient)
MyViewer::MyViewer(const Handle(MyDocument) & hDoc)
	:h_Doc(hDoc)
{
	h_Viewer = OCCT_GraphDriver::CreateViewer();
	h_AisContext = OCCT_GraphDriver::CreateAISContext(h_Viewer);
	h_TempAisContext = OCCT_GraphDriver::CreateAISContext(h_Viewer);
	h_Viewer->Update();
}

MyViewer::~MyViewer()
{
}

const Handle(MyDocument) & MyViewer::getDocument()
{
	return h_Doc;
}

void MyViewer::selectionChanged()
{
}

void MyViewer::Display(const TopoDS_Shape & Shape)
{
	Handle(AIS_Shape) hAisShape = new AIS_Shape(Shape);
	h_AisContext->Display(hAisShape,Standard_True);
}

NCollection_List<Handle(AIS_InteractiveObject)> MyViewer::GetAisObj()
{
	NCollection_List<Handle(AIS_InteractiveObject)> Objs;
	h_AisContext->DisplayedObjects(Objs);
	return Objs;
}

NCollection_List<Handle(AIS_InteractiveObject)> MyViewer::GetSelAisObj()
{
	NCollection_List<Handle(AIS_InteractiveObject)> Objs;
	h_AisContext->InitSelected();
	for (; h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{

		const Handle(AIS_InteractiveObject) & AisObj = h_AisContext->SelectedInteractive();
		if (AisObj)
			Objs.Append(AisObj);
	}
	return Objs;
}


bool MyViewer::getAisObjColor(Quantity_Color & Color)
{
	Color = Quantity_NOC_YELLOW;
	h_AisContext->InitSelected();
	if (h_AisContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		H_AisObj->Color(Color);
		return true;
	}
	else
		return false;
}

void MyViewer::AisObjColor(Quantity_Color Color)
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		H_AisObj->SetColor(Color);
		H_AisObj->Redisplay();
	}
	h_AisContext->UpdateCurrentViewer();
}

void MyViewer::AisObjUnColor()
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		H_AisObj->UnsetColor();
		H_AisObj->Redisplay();
	}
	h_AisContext->UpdateCurrentViewer();
}

void MyViewer::AisObjTransparency(Standard_Real Transparency)
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		H_AisObj->SetTransparency(Transparency);
	}
}

void MyViewer::AisObjUnTransparency()
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		H_AisObj->UnsetTransparency();
	}
}

void MyViewer::AisObjDisplayAll()
{
	h_AisContext->DisplayAll(Standard_True);
}

void MyViewer::AisObjEraseAll()
{
	h_AisContext->EraseAll(Standard_True);
}

void MyViewer::AisObjHide()
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); /*h_AisContext->NextSelected()*/)
	{
		Handle(AIS_InteractiveObject) H_AisObj = h_AisContext->SelectedInteractive();
		if (!H_AisObj.IsNull())
		{
			h_AisContext->Erase(H_AisObj, Standard_False);
		}
	}
	h_AisContext->UpdateCurrentViewer();
}

void MyViewer::AisObjWireFrame()
{
	h_AisContext->SetDisplayMode(0, Standard_True);
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
	{
		h_AisContext->SetDisplayMode(h_AisContext->SelectedInteractive(), 0, Standard_True);
		h_AisContext->Redisplay(h_AisContext->SelectedInteractive(), Standard_True);
	}
	h_AisContext->UpdateCurrentViewer();
}

void MyViewer::AisObjShaded()
{
	for (h_AisContext->InitSelected(); h_AisContext->MoreSelected(); h_AisContext->NextSelected())
		h_AisContext->SetDisplayMode(h_AisContext->SelectedInteractive(), 1, Standard_False);
	h_AisContext->UpdateCurrentViewer();
}

Quantity_Color MyViewer::getAisSelColor()
{
	Handle(Prs3d_Drawer) theStyle = h_AisContext->SelectionStyle();
	return theStyle->Color();
}

#include <Prs3d_Drawer.hxx>
#include <Prs3d_PlaneAspect.hxx>
#include <Prs3d_PointAspect.hxx>

void MyViewer::AisSelColor(Quantity_Color Color)
{
	Handle(Prs3d_Drawer) theStyle = h_AisContext->SelectionStyle();
	theStyle->SetColor(Color);
	theStyle->SectionAspect()->SetColor(Color);
	theStyle->PlaneAspect()->EdgesAspect()->SetColor(Color);
	theStyle->PointAspect()->SetColor(Color);
	h_AisContext->UpdateCurrentViewer();
}
