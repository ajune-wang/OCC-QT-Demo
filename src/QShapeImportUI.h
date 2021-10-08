#pragma once
#include "OCCT_ShapeImport.h"

class QShapeImportUI:
	public OCCT_ShapeImportUI
{
public:
	QShapeImportUI();
	virtual ~QShapeImportUI();
private:
	virtual OCCT_Utf8String getExtUpper(const OCCT_Utf8String & Utf8String) override;
	virtual OCCT_Utf8StringList getLoadPathList() override;
	virtual OCCT_Utf8String getLoadPath() override;

	virtual void showPathSuccess(const OCCT_Utf8String & Path) override;
	virtual void showPathError(const OCCT_Utf8String & Path) override;
	virtual void showError(const OCCT_Utf8String & ErrorMsg) override;
	virtual void showSuccess(const OCCT_Utf8String & SuccessMsg) override;

	virtual void BeginWaitCursor() override;
	virtual void EndWaitCursor() override;
};

