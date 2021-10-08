#include "GUI_Message.h"
#include "ComplainUtf8.h"
#include <QDateTime>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QDebug>

GUI_Message::GUI_Message(QWidget *parent)
	: QListView(parent)
{
	p_Model = new QStandardItemModel(this);
	this->setModel(p_Model);
	Msg::AddMessager(this);
}

GUI_Message::~GUI_Message()
{
	Msg::RemoveMessager(this);
}

void GUI_Message::SetCurrentMessager()
{
	Msg::SetCurrentMessager(this);
}

void GUI_Message::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu RMenu;
	QModelIndex index = currentIndex();
	if (index.isValid())
	{
		QAction * pDeleteAct = new QAction("删除", this);
		connect(pDeleteAct, &QAction::triggered, this, [=]()
		{
			p_Model->removeRow(index.row(), index.parent());
		});
		RMenu.addAction(pDeleteAct);

		QAction * pCopyAct = new QAction("复制消息", this);
		connect(pCopyAct, &QAction::triggered, this, [=]()
		{
			QStandardItem * pItem = p_Model->itemFromIndex(index);
			QString Msg = pItem->text();
			char TimeBuffer[] = "[yyyy-MM-dd hh:mm:ss]  ->";
			Msg = Msg.mid(sizeof(TimeBuffer)-1,-1);
			QClipboard *clipboard = QApplication::clipboard();     //获取系统剪贴板指针
			clipboard->setText(Msg);
		});
		RMenu.addAction(pCopyAct);

		QAction * pCopyWithTimeAct = new QAction("复制时间消息", this);
		connect(pCopyWithTimeAct, &QAction::triggered, this, [=]()
		{
			QStandardItem * pItem = p_Model->itemFromIndex(index);
			QString Msg = pItem->text();
			QClipboard *clipboard = QApplication::clipboard();     //获取系统剪贴板指针
			clipboard->setText(Msg);
		});
		RMenu.addAction(pCopyWithTimeAct);
	}
	QAction * pClearAct = new QAction("清空", this);
	connect(pClearAct, &QAction::triggered, this, [=]()
	{
		p_Model->clear();
	});
	RMenu.addAction(pClearAct);
	RMenu.exec(QCursor::pos());
}

void GUI_Message::ShowMessage(int Type, const OCCT_Utf8String & Message)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("[yyyy-MM-dd hh:mm:ss]  ->");
	QStandardItem * pItem = new QStandardItem(current_date + Message.ToCString());
	pItem->setFlags(pItem->flags() & ~Qt::ItemFlag::ItemIsEditable);
	switch (Type)
	{
	case Msg_Error:
		QApplication::beep();
		pItem->setForeground(QColor(Qt::red));
		break;
	case Msg_Waring:
		pItem->setForeground(QColor(Qt::darkRed));
		QApplication::beep();
		break;
	default:
		break;
	}
	p_Model->insertRow(0,pItem);
	if (p_Model->rowCount() > 100)
		p_Model->removeRow(p_Model->rowCount()-1);
}

void GUI_Message::DebugMessage(int Type, const OCCT_Utf8String & Message)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString TimeMessage = current_date_time.toString("[yyyy-MM-dd hh:mm:ss]  ->");
	TimeMessage += QString(Message.ToCString());
	switch (Type)
	{
	case Msg_Error:
	case Msg_Waring:
		//QApplication::beep();
		break;
	default:
		break;
	}
	qDebug() << TimeMessage;
}
