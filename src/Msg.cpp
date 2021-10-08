#include "Msg.h"
#include <stdarg.h>
#include <Standard_CLocaleSentry.hxx>

#ifdef __APPLE__
// There are a lot of *_l functions availalbe on Mac OS X - we use them
#define SAVE_TL()
#elif defined(_MSC_VER)
// MSVCRT has equivalents with slightly different syntax
#define SAVE_TL()
#define strtod_l(thePtr, theNextPtr, theLocale)                _strtod_l(thePtr, theNextPtr, theLocale)
#define vprintf_l(theLocale, theFormat, theArgPtr)             _vprintf_l(theFormat, theLocale, theArgPtr)
#define vsprintf_l(theBuffer, theLocale, theFormat, theArgPtr) _vsprintf_l(theBuffer, theFormat, theLocale, theArgPtr)
#define vfprintf_l(theFile,   theLocale, theFormat, theArgPtr) _vfprintf_l(theFile,   theFormat, theLocale, theArgPtr)
#else
// glibc provides only limited xlocale implementation:
// strtod_l/strtol_l/strtoll_l functions with explicitly specified locale
// and newlocale/uselocale/freelocale to switch locale within current thread only.
// So we switch to C locale temporarily
#define SAVE_TL() Standard_CLocaleSentry aLocaleSentry;
#ifndef OCCT_CLOCALE_POSIX2008
// glibc version for android platform use locale-independent implementation of
// strtod, strtol, strtoll functions. For other system with locale-depended
// implementations problems may appear if "C" locale is not set explicitly.
#if !defined(__ANDROID__) && !defined(__QNX__) && !defined(__MINGW32__)
#error System does not support xlocale. Import/export could be broken if C locale did not specified by application.
#endif
#define strtod_l(thePtr, theNextPtr, theLocale)              strtod(thePtr, theNextPtr)
#endif
#define vprintf_l(theLocale, theFormat, theArgPtr)             vprintf(theFormat, theArgPtr)
#define vsprintf_l(theBuffer, theLocale, theFormat, theArgPtr) vsprintf(theBuffer, theFormat, theArgPtr)
#define vfprintf_l(theFile,   theLocale, theFormat, theArgPtr) vfprintf(theFile,   theFormat, theArgPtr)
#endif

NCollection_Map<Messager *> Msg::m_Msgrs = NCollection_Map<Messager *>();
Messager * Msg::m_CurrentMsgr = nullptr;

void Msg::AddMessager(Messager * pMsgr)
{
	if (pMsgr != nullptr && m_Msgrs.Contains(pMsgr) == false)
		m_Msgrs.Add(pMsgr);
}

void Msg::RemoveMessager(Messager * pMsgr)
{
	if (pMsgr != nullptr)
		m_Msgrs.Remove(pMsgr);
}

void Msg::SetCurrentMessager(Messager * pMsgr)
{
	m_CurrentMsgr = pMsgr;
}

void Msg::RemoveCurrentMessager()
{
	m_CurrentMsgr = nullptr;
}

void Msg::ShowInfo(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->ShowMessage(Msg_Info, Message);
	}
	else
	{
		if(m_CurrentMsgr)
			m_CurrentMsgr->ShowMessage(Msg_Info, Message);
	}
}

void Msg::ShowWaring(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->ShowMessage(Msg_Waring, Message);
	}
	else
	{
		if (m_CurrentMsgr)
			m_CurrentMsgr->ShowMessage(Msg_Waring, Message);
	}
}

void Msg::ShowError(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->ShowMessage(Msg_Error, Message);
	}
	else
	{
		if (m_CurrentMsgr)
			m_CurrentMsgr->ShowMessage(Msg_Error, Message);
	}
}

void Msg::ShowInfo(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::ShowInfo(theBuffer, Global);
}

void Msg::ShowWaring(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::ShowWaring(theBuffer, Global);
}

void Msg::ShowError(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::ShowError(theBuffer, Global);
}


void Msg::DebugInfo(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->DebugMessage(Msg_Info, Message);
	}
	else
	{
		if (m_CurrentMsgr)
			m_CurrentMsgr->DebugMessage(Msg_Info, Message);
	}
}

void Msg::DebugWaring(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->DebugMessage(Msg_Waring, Message);
	}
	else
	{
		if (m_CurrentMsgr)
			m_CurrentMsgr->DebugMessage(Msg_Waring, Message);
	}
}

void Msg::DebugError(const OCCT_Utf8String & Message, bool Global)
{
	if (Global)
	{
		NCollection_Map<Messager *>::Iterator Itor(m_Msgrs);
		for (; Itor.More(); Itor.Next())
			Itor.Value()->DebugMessage(Msg_Error, Message);
	}
	else
	{
		if (m_CurrentMsgr)
			m_CurrentMsgr->DebugMessage(Msg_Error, Message);
	}
}

void Msg::DebugInfo(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::DebugInfo(theBuffer, Global);
}

void Msg::DebugWaring(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::DebugWaring(theBuffer, Global);
}

void Msg::DebugError(bool Global, const char * theFormat, ...)
{
	char theBuffer[1024];
	SAVE_TL();
	va_list argp;
	va_start(argp, theFormat);
	int result = vsprintf_l(theBuffer, Standard_CLocaleSentry::GetCLocale(), theFormat, argp);
	va_end(argp);
	Msg::DebugError(theBuffer, Global);
}

