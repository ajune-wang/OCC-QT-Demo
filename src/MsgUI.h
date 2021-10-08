#pragma once
#include "OCCT_Utf8String.h"
#include "OCCT_Utf8StringList.h"
#include <NCollection_DataMap.hxx>

class MsgUIer
{
public:
	MsgUIer() {};
	virtual ~MsgUIer(){};
	virtual int GetChoose(const OCCT_Utf8String & Title, const OCCT_Utf8StringList & ChooseList) = 0;
	virtual OCCT_Utf8String GetInPut(const OCCT_Utf8String & Title) = 0;
	virtual Standard_Boolean YesOrNo(const OCCT_Utf8String & Title, const OCCT_Utf8String & Message) = 0;
	virtual NCollection_List<Standard_Integer> GetChoose(const OCCT_Utf8String & Title,
		const NCollection_DataMap<Standard_Integer, OCCT_Utf8String> & Map) = 0;
};

class MsgUI
{
public:
	MsgUI();
	virtual ~MsgUI();
	static void SetUIer(MsgUIer * pUIer);

	static int GetChoose(const OCCT_Utf8String & Title, const OCCT_Utf8StringList & ChooseList);
	static OCCT_Utf8String GetInPut(const OCCT_Utf8String & Title);
	static Standard_Boolean YesOrNo(const OCCT_Utf8String & Title,const OCCT_Utf8String & Message);
	static NCollection_List<Standard_Integer> GetChoose(const OCCT_Utf8String & Title,
		const NCollection_DataMap<Standard_Integer, OCCT_Utf8String> & Map);
private:
	static MsgUIer * p_UIer;
};

