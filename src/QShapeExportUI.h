#pragma once
#include "OCCT_ShapeExport.h"

class QShapeExportUI:
	public OCCT_ShapeExportUI
{
public:
	QShapeExportUI();
	virtual ~QShapeExportUI();
private:
	virtual OCCT_Utf8String getSavePathFolder(ModelFileType & FileType) override;
	virtual OCCT_Utf8String getSavePath(ModelFileType & FileType) override;

	virtual void showPathSuccess(const OCCT_Utf8String & Path) override;
	virtual void showPathError(const OCCT_Utf8String & Path) override;
	virtual void showError(const OCCT_Utf8String & ErrorMsg) override;
	virtual void showSuccess(const OCCT_Utf8String & SuccessMsg) override;

	virtual void BeginWaitCursor() override;
	virtual void EndWaitCursor() override;
};

