#pragma once
#include "OCCT_ShapeList.h"
#include "OCCT_Utf8StringList.h"

class OCCT_ShapeImportUI
{
public:
	OCCT_ShapeImportUI() {};
	virtual ~OCCT_ShapeImportUI() {};

	virtual OCCT_Utf8String getExtUpper(const OCCT_Utf8String & Utf8String) = 0;
	virtual OCCT_Utf8StringList getLoadPathList() = 0;
	virtual OCCT_Utf8String getLoadPath() = 0;

	virtual void showPathSuccess(const OCCT_Utf8String & Path) = 0;
	virtual void showPathError(const OCCT_Utf8String & Path) = 0;
	virtual void showError(const OCCT_Utf8String & ErrorMsg) = 0;
	virtual void showSuccess(const OCCT_Utf8String & ErrorMsg) = 0;
	virtual void BeginWaitCursor() = 0;
	virtual void EndWaitCursor() = 0;
};

class OCCT_ShapeImport
{
public:
	OCCT_ShapeImport(OCCT_ShapeImportUI * pUI);
	virtual ~OCCT_ShapeImport();

	OCCT_ShapeList ImportModel();
	OCCT_ShapeList ImportModel(const OCCT_Utf8String & Path);
private:
	OCCT_ShapeImportUI * p_UI;
};

