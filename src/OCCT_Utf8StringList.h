#pragma once
#include "OCCT_Utf8String.h"
#include <NCollection_List.hxx>

typedef NCollection_List<OCCT_Utf8String> OCCT_Utf8StringList;

inline OCCT_Utf8StringList & operator<<(OCCT_Utf8StringList & List, const OCCT_Utf8String & String)
{
	List.Append(String);
	return List;
}

inline OCCT_Utf8StringList & operator+=(OCCT_Utf8StringList & List, const OCCT_Utf8String & String)
{
	List.Append(String);
	return List;
}

