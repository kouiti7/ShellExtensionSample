
#include "stdafx.h"

#include "CLSID.h"

const GUID g_clsid = 
	{ 0x000000000, 0xFFFF, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

void
GUID2String( const GUID &guid, wstring &str )
{
	str.resize( 36 );

	swprintf_s( (wchar_t*)str.data(), 37, _T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );
}

bool
String2GUID( LPCTSTR str, GUID &guid )
{
	int i = _tcscanf_s( str, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				&guid.Data1, &guid.Data2, &guid.Data3,
				&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
				&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7] );
	return i == 11;
}
