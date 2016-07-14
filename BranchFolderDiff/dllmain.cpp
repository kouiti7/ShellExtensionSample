
#include "stdafx.h"

#include <guiddef.h>
#include "CLSID.h"
#include "ClassFactory.h"

HINSTANCE	g_inst = NULL;
UINT		g_cRefThisDll = 0;

BOOL APIENTRY
DllMain(
		HMODULE hModule,
        DWORD  ul_reason_for_call,
        LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		g_inst = (HINSTANCE)hModule;
		break;

	case DLL_PROCESS_DETACH:
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

_Check_return_
STDAPI
DllGetClassObject(
		_In_  REFCLSID rclsid,
		_In_  REFIID   riid,
		_Out_ LPVOID   *ppv )
{
	*ppv = NULL;

	if( IsEqualIID( rclsid, g_clsid ) )
	{
		CClassFactory *pcf = new CClassFactory;
		return pcf->QueryInterface( riid, ppv );
	}

	return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI
DllCanUnloadNow()
{
	return ( g_cRefThisDll == 0 ? S_OK : S_FALSE );
}
