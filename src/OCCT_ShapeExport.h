#pragma once
#include "OCCT_ShapeList.h"
#include "OCCT_Utf8String.h"

enum OCCT_ShapeExportModelFileType
{
	MFT_BREP,
	MFT_STEP,
	MFT_IGES,
	MFT_STL,
	MFT_NULL
};

class OCCT_ShapeExportUI
{
public:
	typedef OCCT_ShapeExportModelFileType ModelFileType;
public:
	OCCT_ShapeExportUI() {};
	virtual ~OCCT_ShapeExportUI() {};

	virtual OCCT_Utf8String getSavePathFolder(ModelFileType & FileType) = 0;
	virtual OCCT_Utf8String getSavePath(ModelFileType & FileType) = 0;

	virtual void showPathSuccess(const OCCT_Utf8String & Path) = 0;
	virtual void showPathError(const OCCT_Utf8String & Path) = 0;
	virtual void showError(const OCCT_Utf8String & ErrorMsg) = 0;
	virtual void showSuccess(const OCCT_Utf8String & ErrorMsg) = 0;

	virtual void BeginWaitCursor() = 0;
	virtual void EndWaitCursor() = 0;
};

class OCCT_ShapeExport
{
	typedef OCCT_ShapeExportModelFileType ModelFileType;
public:
	OCCT_ShapeExport(OCCT_ShapeExportUI * pUI);
	virtual ~OCCT_ShapeExport();

	void ExportModel(OCCT_ShapeList theExportList);
	void ExportMutiModel(OCCT_ShapeList theExportList);
private:
	OCCT_ShapeExportUI * p_UI;
};

