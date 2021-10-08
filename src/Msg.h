#pragma once
#include "OCCT_Utf8String.h"
#include <NCollection_Map.hxx>

#define Msg_Info    (0x0)
#define Msg_Waring  (0x1)
#define Msg_Error   (0x2)

class Messager
{
public:
	Messager() {};
	virtual ~Messager() {};
	virtual void ShowMessage(int Type, const OCCT_Utf8String & Message) = 0;
	virtual void DebugMessage(int Type, const OCCT_Utf8String & Message) = 0;
};

class Msg
{
public:
	static void AddMessager(Messager * pMsgr);
	static void RemoveMessager(Messager * pMsgr);

	static void SetCurrentMessager(Messager * pMsgr);
	static void RemoveCurrentMessager();
	static Messager * GetCurrentMessager() {return m_CurrentMsgr;}

	static void ShowInfo(const OCCT_Utf8String & Message, bool Global = false);
	static void ShowWaring(const OCCT_Utf8String & Message, bool Global = false);
	static void ShowError(const OCCT_Utf8String & Message, bool Global = false);

	static void ShowInfo(bool Global,const char* theFormat, ...);
	static void ShowWaring(bool Global,const char* theFormat, ...);
	static void ShowError(bool Global,const char* theFormat, ...);

	static void DebugInfo(const OCCT_Utf8String & Message, bool Global = false);
	static void DebugWaring(const OCCT_Utf8String & Message, bool Global = false);
	static void DebugError(const OCCT_Utf8String & Message, bool Global = false);

	static void DebugInfo(bool Global, const char* theFormat, ...);
	static void DebugWaring(bool Global, const char* theFormat, ...);
	static void DebugError(bool Global, const char* theFormat, ...);
private:
	Msg() {}
	virtual ~Msg() {}
protected:
	static NCollection_Map<Messager *> m_Msgrs;
	static Messager *       m_CurrentMsgr;
};


