#pragma once
#include <NCollection_UtfString.hxx>

typedef NCollection_Utf8String			OCCT_Utf8String; //÷ÿ∂®“Â

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, const OCCT_Utf8String & String)
{
	Utf8String += String;
	return Utf8String;
}

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, const char * ConstStr)
{
	Utf8String += ConstStr;
	return Utf8String;
}

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, double Val)
{
	char Buffer[20];
	Sprintf(Buffer, "%lf", Val);
	Utf8String += Buffer;
	return Utf8String;
}

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, int Val)
{
	char Buffer[20];
	Sprintf(Buffer, "%d", Val);
	Utf8String += Buffer;
	return Utf8String;
}

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, char Val)
{
	char Buffer[3];
	Sprintf(Buffer, "%c", Val);
	Utf8String += Buffer;
	return Utf8String;
}

inline OCCT_Utf8String & operator << (OCCT_Utf8String & Utf8String, unsigned Val)
{
	char Buffer[20];
	Sprintf(Buffer, "%d", Val);
	Utf8String += Buffer;
	return Utf8String;
}