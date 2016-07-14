
#ifndef __CLSID_H__
#define __CLSID_H__

#include <atlbase.h>
#include <tchar.h>
#include <string>

using namespace std;
extern const GUID g_clsid;

void GUID2String( const GUID &guid, wstring &str );
bool String2GUID( LPCTSTR str, GUID &guid );

#endif // __CLSID_H__
