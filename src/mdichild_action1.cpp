#include "mdichild.h"
#include "ComplainUtf8.h"
#include <QtWidgets>
#include <QDockWidget>

void MdiChild::newFile()
{
	static int sequenceNumber = 1;

	isUntitled = true;
	curFile = tr("My文件%1.occfile").arg(sequenceNumber++);
	setWindowTitle(curFile + "[*]");

	//connect(document(), &mdidocument::contentsChanged,
	//        this, &MdiChild::documentWasModified);
}

bool MdiChild::save()
{
	if (isUntitled) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool MdiChild::saveAs()
{
	QString Title = "SAVE AS";
	QString Filter = "My文件(*.occfile)"
		";;My文件2(*.occfile2)";
	QString fileName = QFileDialog::getSaveFileName(nullptr, Title, curFile, Filter);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

bool MdiChild::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("MDI"),
			tr("不能读取文件 %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QDataStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	//document()->LoadData(in); //LOAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	QApplication::restoreOverrideCursor();
	setCurrentFile(fileName);

	//connect(document(), &mdidocument::contentsChanged,
	//	this, &MdiChild::documentWasModified);

	return true;
}

bool MdiChild::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("MDI"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return false;
	}

	QDataStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	//document()->SaveData(out); //SAVE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	return true;
}



#include "ComplainUtf8.h"
#include "mdichild.h"
#include "QShapeImportUI.h"
#include "OCCT_ShapeList.h"
#include <AIS_Shape.hxx>

void MdiChild::AisObjDisplayAll()
{
	h_MyViewer->getAisContext()->DisplayAll(Standard_True);
}

void MdiChild::AisObjEraseAll()
{
	h_MyViewer->getAisContext()->EraseAll(Standard_True);
}

void MdiChild::AisObjHide()
{
	h_MyViewer->AisObjHide();
}

#include <QColorDialog>
namespace
{
	Quantity_Color GetColor(Quantity_Color oldColor)
	{
		QColor color = QColorDialog::getColor(Qt::red, nullptr, "颜色对话框", QColorDialog::ShowAlphaChannel);
		int R = color.red() / 256.0;
		int G = color.green() / 256.0;
		int B = color.blue() / 256.0;
		Quantity_Color Color(R, G, B, Quantity_TOC_RGB);
		return Color;
	}
	OCCT_Utf8String GetInPut(const OCCT_Utf8String & Title,QWidget * parent)
	{
		QDialog Dialog(parent);
		Dialog.setWindowTitle(Title.ToCString());
		Dialog.setMaximumHeight(40);
		QHBoxLayout * pLayout = new QHBoxLayout(&Dialog);
		QLineEdit * pEdit = new QLineEdit(&Dialog);
		//QRegExp regExp("[A-Z]*");
		//pEdit->setValidator(new QRegExpValidator(regExp, pEdit));
		pEdit->setMinimumWidth(300);
		QDialogButtonBox * pBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &Dialog);
		QDialog::connect(pBox, &QDialogButtonBox::accepted, &Dialog, &QDialog::accept);
		QDialog::connect(pBox, &QDialogButtonBox::rejected, &Dialog, &QDialog::reject);
		pLayout->addWidget(pEdit, 100);
		pLayout->addWidget(pBox);
		Dialog.setLayout(pLayout);
		if (QDialog::Accepted == Dialog.exec())
			return pEdit->text().toUtf8().data();
		else
			return "";
	}
}
void MdiChild::SelTrans()
{
	OCCT_Utf8String Str = GetInPut("设置透明度值",this);
	if (Str.IsEmpty())
		return;
	double val = atof(Str.ToCString());
	h_MyViewer->AisObjTransparency(val);
}


void MdiChild::SelColor()
{
	Quantity_Color Color = GetColor(Quantity_NOC_RED);
	h_MyViewer->AisObjColor(Color);
}

void MdiChild::SelUnColor()
{
	h_MyViewer->AisObjUnColor();
}

void MdiChild::SelUnTrans()
{
	h_MyViewer->AisObjUnTransparency();
}

void MdiChild::SelWireFrame()
{
	h_MyViewer->AisObjWireFrame();
}

void MdiChild::SelShaded()
{
	h_MyViewer->AisObjShaded();
}
