#include "OCCT_ShapeExport.h"
#include "ComplainUtf8.h"
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopExp_Explorer.hxx>

#include <Interface_Static.hxx>
#include <XSControl_WorkSession.hxx>
#include <Transfer_TransientProcess.hxx>
#include <Transfer_ProcessForTransient.hxx>

#include <STEPControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <StlAPI_Writer.hxx>


OCCT_ShapeExport::OCCT_ShapeExport(OCCT_ShapeExportUI * pUI)
	:p_UI(pUI)
{
}


OCCT_ShapeExport::~OCCT_ShapeExport()
{
}

void OCCT_ShapeExport::ExportModel(OCCT_ShapeList theExportList)
{
	if (theExportList.IsEmpty())
		return;
	ModelFileType Type;
    const OCCT_Utf8String & FolderPath = p_UI->getSavePath(Type);
	p_UI->BeginWaitCursor();
	if (FolderPath.Size())
	{
		OCCT_Utf8String aFileName = FolderPath;
		switch (Type)
		{
		case MFT_STEP:
		{
			STEPControl_StepModelType StepControlType = STEPControl_AsIs;
			//aFileName += ".step";
			STEPControl_Writer aWriter;
            for(TopoDS_Shape shape:theExportList)
			{
				IFSelect_ReturnStatus status = aWriter.Transfer(shape, STEPControl_AsIs);
				if (status != IFSelect_RetDone)
				{
					p_UI->showError("STEP转换失败");
					return;
				}
			}
			aWriter.Write(aFileName.ToCString());
		}
		break;
		case MFT_IGES:
		{
			//aFileName += ".iges";
			IGESControl_Controller::Init();
			IGESControl_Writer aWriter(Interface_Static::CVal("XSTEP.iges.unit"),
				Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
            for(TopoDS_Shape shape : theExportList)
			{
				aWriter.AddShape(shape);
				aWriter.ComputeModel();
			}
			//这种复合模式的转换 最后得到的实体是一个整体 估计是复合实体得一步步拆分
			Standard_Integer result = aWriter.Write(aFileName.ToCString());
			if (!result)
				p_UI->showError("IGES转换失败");
		}
		break;
		case MFT_STL:
		case MFT_BREP:
		{
			TopoDS_Compound ResultShape;
			BRep_Builder MKCP;
			MKCP.MakeCompound(ResultShape);
            for(TopoDS_Shape shape : theExportList)
				MKCP.Add(ResultShape, shape);
			if (ResultShape.IsNull()) {
				p_UI->showError("转换失败,复合图形为空");
				return;
			}
			else
			{
				if (Type == MFT_BREP)//".BREP"
				{
					//aFileName += ".brep";
					Standard_Boolean status = BRepTools::Write(ResultShape, aFileName.ToCString());
					if (!status) {
						p_UI->showError("BREP转换失败");
						return;
					}
				}
				else if (Type == MFT_STL)//".STL"
				{
					//STL同brep 复合形状添加多个
					//aFileName += ".stl";
					if (ResultShape.IsNull()) {
						p_UI->showError("STL转换失败");
					}
					else
					{
						StlAPI_Writer myStlWriter;
						myStlWriter.Write(ResultShape, aFileName.ToCString());
					}
				}
			}

		}
		break;
		default:
			break;
		}
		p_UI->showSuccess("转换成功");
	}
	p_UI->EndWaitCursor();
}

void OCCT_ShapeExport::ExportMutiModel(OCCT_ShapeList theExportList)
{
	if (theExportList.IsEmpty())
		return;
	ModelFileType Type;
    const OCCT_Utf8String & FolderPath = p_UI->getSavePathFolder(Type);
	if (FolderPath.Size())
	{
		OCCT_Utf8String aFileName;
		STEPControl_StepModelType StepControlType = STEPControl_AsIs;
		bool Index = true;
        for(TopoDS_Shape shape : theExportList)
		{
			Standard_Character Buffer[30] = { 0 };
			if (!Index++)
			{
				Sprintf(Buffer, "[%d]", Index - 1);
			}
			aFileName += Buffer;
			if (Type == MFT_STEP)
			{
				aFileName += ".step";
				STEPControl_Writer aWriter;
				//Transfer 可以转换多个模型 相当于添加
				IFSelect_ReturnStatus status = aWriter.Transfer(shape, StepControlType);//STEPControl_ManifoldSolidBrep
				if (status != IFSelect_RetDone)
					p_UI->showError("STEP转换失败");
				aWriter.Write(aFileName.ToCString());
			}
			else if (Type == MFT_BREP)
			{
				aFileName += ".brep";
				//Brep 可以用 TopoDS_Compound RES;	BRep_Builder MKCP;MKCP.MakeCompound(RES);MKCP.Add(RES, aShape); 最后写入RES
				Standard_Boolean status = BRepTools::Write(shape, aFileName.ToCString());
				if (!status)
					p_UI->showError("BREP转换失败");
			}
			else if (Type == MFT_IGES)
			{
				aFileName += ".iges";
				IGESControl_Controller::Init();
				IGESControl_Writer aWriter(Interface_Static::CVal("XSTEP.iges.unit"),
					Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
				//iges AddShape 可以添加多个模型
				aWriter.AddShape(shape);
				aWriter.ComputeModel();
				Standard_Integer result = aWriter.Write(aFileName.ToCString());
				if (!result)
					p_UI->showError("IGES转换失败");
			}
			else if (Type == MFT_STL)//".STL"
			{
				//STL同brep 复合形状添加多个
				aFileName += ".stl";
				if (shape.IsNull())
					p_UI->showError("STL转换失败");
				else
				{
					StlAPI_Writer myStlWriter;
					myStlWriter.Write(shape, aFileName.ToCString());
				}
			}
		}
		p_UI->showSuccess("转换成功");
	}
}
