#pragma once
#include "Msg.h"

#include <QListView>
#include <QStandardItemModel>

class GUI_Message : public QListView
	,public Messager
{
	Q_OBJECT
public:
	GUI_Message(QWidget *parent);
	void ShowMessage(int Type, const OCCT_Utf8String & Message) override;
	void DebugMessage(int Type, const OCCT_Utf8String & Message) override;
	~GUI_Message();

	void SetCurrentMessager();
protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
private:
	QStandardItemModel * p_Model;
};
