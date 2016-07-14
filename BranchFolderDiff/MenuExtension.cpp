
#include "stdafx.h"
#include "MenuExtension.h"
#include "dllmain.h"
#include "Setting.h"

#include <shellapi.h>
#include <stdarg.h>
#include <stdio.h>

static CSetting setting;
static vector<CSettingData> diffArgs;

CMenuExtension::CMenuExtension()
{
}

CMenuExtension::~CMenuExtension()
{
}

STDMETHODIMP
CMenuExtension::QueryInterface( REFIID riid, LPVOID FAR *ppv )
{
	*ppv = NULL;

	if( IsEqualIID( riid, IID_IShellExtInit )  ||  IsEqualIID( riid, IID_IUnknown ) )
	{
		*ppv = (LPSHELLEXTINIT)this;
	}
	else if( IsEqualIID( riid, IID_IContextMenu ) )
	{
		*ppv = (LPCONTEXTMENU)this;
	}

	if( !*ppv )
		return E_NOINTERFACE;

	AddRef();
	return NOERROR;
}

STDMETHODIMP_(ULONG)
CMenuExtension::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG)
CMenuExtension::Release()
{
	if( --m_cRef )
		return m_cRef;
	delete this;
	return 0L;
}

STDMETHODIMP
CMenuExtension::Initialize(
		LPCITEMIDLIST pIDFolder,
		LPDATAOBJECT pDataObj,
		HKEY hRegKey )
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop;

	if( !pDataObj )
		return E_INVALIDARG;
	if( FAILED( pDataObj->GetData( &fmt, &stg ) ) )
		return E_INVALIDARG;

	hDrop = (HDROP)GlobalLock( stg.hGlobal );

	HRESULT hr = NOERROR;

	int n = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );
	if( n != 1 )
		hr = E_INVALIDARG;

	TCHAR path[MAX_PATH];
	DragQueryFile( hDrop, 0, path, MAX_PATH );
	if( !setting.GetDiffInfo( path, diffArgs ) )
		return NOERROR;

	GlobalUnlock( stg.hGlobal );
	ReleaseStgMedium( &stg );

	return NOERROR;
}

#define ResultFromShort(i)  ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(i)))

STDMETHODIMP
CMenuExtension::QueryContextMenu(
		HMENU hMenu,
		UINT indexMenu,
		UINT idCmdFirst,
		UINT idCmdLast,
		UINT uFlags )
{
	if( uFlags & CMF_DEFAULTONLY  ||  !diffArgs.size() )
		return NOERROR;

	UINT idCmd = idCmdFirst;

	if( ( uFlags & 0x000F ) == CMF_NORMAL )
		;
	else if( uFlags & CMF_VERBSONLY )
		;
	else if( uFlags & CMF_EXPLORE )
		;
	else
		return NOERROR;

	UINT start = indexMenu;

	InsertMenu( hMenu, indexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, NULL );

	for( auto iter = diffArgs.begin(); iter != diffArgs.end(); iter++ )
		InsertMenu( hMenu, indexMenu++, MF_STRING | MF_BYPOSITION, idCmd++, iter->name.c_str() );

	InsertMenu( hMenu, indexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, NULL );

	return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, diffArgs.size() + 2);
//	return ResultFromShort( indexMenu - start );
}

STDMETHODIMP
CMenuExtension::InvokeCommand( LPCMINVOKECOMMANDINFO lpcmi )
{
	if( HIWORD( lpcmi->lpVerb ) != 0 )
		return E_INVALIDARG;

	int idx = LOWORD( lpcmi->lpVerb );

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset( &si, 0, sizeof( STARTUPINFO ) );
	memset( &pi, 0, sizeof( PROCESS_INFORMATION ) );

	CreateProcess( NULL, (LPWSTR)diffArgs[idx].exeAndArgs.c_str(),
					NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return NOERROR;
}

STDMETHODIMP
CMenuExtension::GetCommandString(
		UINT_PTR idCmd,
		UINT uFlags,
		UINT FAR *reserved,
		LPSTR pszName,
		UINT cchMax )
{
	return NOERROR;
}
