#include <QApplication>
#include "ComplainUtf8.h"
#include "mdichild.h"
#include "OCCT_ShapeList.h"
#include "QShapeImportUI.h"
#include "QShapeExportUI.h"
#include <AIS_Shape.hxx>

void MdiChild::ImportModel()
{
	QShapeImportUI UI;
    const OCCT_ShapeList & Shapes = OCCT_ShapeImport(&UI).ImportModel();
    for(TopoDS_Shape shape : Shapes)
		h_MyViewer->Display(shape); 
}

void MdiChild::ExportModel()
{
	OCCT_ShapeList Shapes;
	Handle_AIS_InteractiveContext myContext = h_MyViewer->getAisContext();
	for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
		Shapes.Append(myContext->SelectedShape());
	if (Shapes.Size())
	{
		QShapeExportUI UI;
		OCCT_ShapeExport(&UI).ExportModel(Shapes);
	}
}

void MdiChild::SelNatural()
{
	h_MyViewer->getAisContext()->Deactivate();
	h_MyViewer->getAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_SHAPE));
}

void MdiChild::SelSolid()
{
	h_MyViewer->getAisContext()->Deactivate();
	h_MyViewer->getAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_SOLID));
}

void MdiChild::SelFace()
{
	h_MyViewer->getAisContext()->Deactivate();
	h_MyViewer->getAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
}

void MdiChild::SelEdge()
{
	h_MyViewer->getAisContext()->Deactivate();
	h_MyViewer->getAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
}

void MdiChild::SelVertex()
{
	h_MyViewer->getAisContext()->Deactivate();
	h_MyViewer->getAisContext()->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
}

#include "Msg.h"
#include <BRepPrimAPI_MakeBox.hxx>

void MdiChild::Test1()
{
	//测试代码1
	Handle_AIS_InteractiveContext myContext = h_MyViewer->getAisContext();
	{
		TopoDS_Shape Shape = BRepPrimAPI_MakeBox(100, 100, 100);
		Handle(AIS_Shape) hShape = new AIS_Shape(Shape);
		myContext->Display(hShape,Standard_False);
	}
	myContext->DisplayAll(Standard_True);
	Msg::ShowInfo("测试1——创建一个BOX并且显示！");
}
