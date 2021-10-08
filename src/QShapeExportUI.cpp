#include "QShapeExportUI.h"
#include "ComplainUtf8.h"
#include "Msg.h"
#include <QString>
#include <QFileDialog>
#include <QApplication>

QShapeExportUI::QShapeExportUI()
{
}

QShapeExportUI::~QShapeExportUI()
{
}

OCCT_Utf8String QShapeExportUI::getSavePathFolder(ModelFileType & FileType)
{
	QString Title = "导出几何模型";
	QString Dir = ".";
	QString Filter =
		"STEP文件(*.step *.stp)"
		";;BREP文件(*.brep)"
		";;IGES文件(*.iges *.igs)"
		";;STL文件(*.stl)";
	QString SelectFilter;
	QString fileName = QFileDialog::getSaveFileName(nullptr, Title, Dir, Filter,&SelectFilter);
	if (fileName.isEmpty())
	{
		FileType = MFT_NULL;
		return "";
	}
	if (SelectFilter == "STEP文件(*.step *.stp)")
		FileType = MFT_STEP;
	else if (SelectFilter == "BREP文件(*.brep)")
		FileType = MFT_BREP;
	else if (SelectFilter == "IGES文件(*.iges *.igs)")
		FileType = MFT_IGES;
	else if (SelectFilter == "STL文件(*.stl)")
		FileType = MFT_STL;
	else
		FileType = MFT_NULL;
	return fileName.toUtf8().data();
}

OCCT_Utf8String QShapeExportUI::getSavePath(ModelFileType & FileType)
{
	QString Title = "导出几何模型";
	QString Dir = ".";
	QString Filter =
		"STEP文件(*.step *.stp)"
		";;BREP文件(*.brep)"
		";;IGES文件(*.iges *.igs)"
		";;STL文件(*.stl)";
	QString SelectFilter;
	QString fileName = QFileDialog::getSaveFileName(nullptr, Title, Dir, Filter, &SelectFilter);
	if (fileName.isEmpty())
	{
		FileType = MFT_NULL;
		return "";
	}
	if (SelectFilter == "STEP文件(*.step *.stp)")
		FileType = MFT_STEP;
	else if (SelectFilter == "BREP文件(*.brep)")
		FileType = MFT_BREP;
	else if (SelectFilter == "IGES文件(*.iges *.igs)")
		FileType = MFT_IGES;
	else if (SelectFilter == "STL文件(*.stl)")
		FileType = MFT_STL;
	else
		FileType = MFT_NULL;
	return fileName.toUtf8().data();
}

void QShapeExportUI::showPathSuccess(const OCCT_Utf8String & Path)
{
	Standard_Character Buffer[1024] = { 0 };
	Sprintf(Buffer, "文件[%s]打开成功!", Path.ToCString());
	Msg::ShowInfo(OCCT_Utf8String(Buffer));
}

void QShapeExportUI::showPathError(const OCCT_Utf8String & Path)
{
	Standard_Character Buffer[1024] = { 0 };
	Sprintf(Buffer, "文件[%s]打开失败!", Path.ToCString());
	Msg::ShowError(OCCT_Utf8String(Buffer));
}

void QShapeExportUI::showError(const OCCT_Utf8String & ErrorMsg)
{
	Msg::ShowError(ErrorMsg);
}

void QShapeExportUI::showSuccess(const OCCT_Utf8String & SuccessMsg)
{
	Msg::ShowInfo(SuccessMsg);
}

void QShapeExportUI::BeginWaitCursor()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void QShapeExportUI::EndWaitCursor()
{
	QApplication::restoreOverrideCursor();
}
