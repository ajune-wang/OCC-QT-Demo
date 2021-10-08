#include "MsgUI.h"

MsgUIer * MsgUI::p_UIer = nullptr;
MsgUI::MsgUI()
{
}

MsgUI::~MsgUI()
{
}

void MsgUI::SetUIer(MsgUIer * pUIer)
{
	p_UIer = pUIer;
}

//#include "GUI_Dialogs.h"
int MsgUI::GetChoose(const OCCT_Utf8String & Title, const OCCT_Utf8StringList & ChooseList)
{
	if (p_UIer)
		return p_UIer->GetChoose(Title, ChooseList);
	else
		return -1;
}

OCCT_Utf8String MsgUI::GetInPut(const OCCT_Utf8String & Title)
{
	if (p_UIer)
		return p_UIer->GetInPut(Title);
	else
		return "";
	//return GUI_Dialogs::GetInPut(Title.ToCString()).toUtf8().data();
}

Standard_Boolean MsgUI::YesOrNo(const OCCT_Utf8String & Title, const OCCT_Utf8String & Message)
{
	if (p_UIer)
		return p_UIer->YesOrNo(Title, Message);
	else
		return Standard_False;
	//return GUI_Dialogs::YesOrNo(Title.ToCString(),Message.ToCString());
}

NCollection_List<Standard_Integer> MsgUI::GetChoose(const OCCT_Utf8String & Title,
	const NCollection_DataMap<Standard_Integer, OCCT_Utf8String> & Map)
{
	if (p_UIer)
		return p_UIer->GetChoose(Title, Map);
	else
		return NCollection_List<Standard_Integer>();
}
