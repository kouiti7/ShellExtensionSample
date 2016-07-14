
#include "stdafx.h"
#include "MenuExtension.h"
#include "dllmain.h"

#include <shellapi.h>
#include <stdarg.h>
#include <stdio.h>


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
	if( !n )
		hr = E_INVALIDARG;

	TCHAR path[MAX_PATH];
	for( int i = 0; i < n; i++ )
	{
		DragQueryFile( hDrop, i, path, MAX_PATH );
		OutputDebugString( path );
		OutputDebugString( _T("\n") );
	}

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
	if( uFlags & CMF_DEFAULTONLY )
		return MAKE_SCODE( SEVERITY_SUCCESS, FACILITY_NULL, 0 );

	UINT idCmd = idCmdFirst;

	BOOL bAppendItems = TRUE;
	
	if( ( uFlags & 0x000F ) == CMF_NORMAL )
		;
	else if( uFlags & CMF_VERBSONLY )
		;
	else if( uFlags & CMF_EXPLORE )
		;
	else
		bAppendItems = FALSE;
	if( !bAppendItems )
		return NOERROR;

	int added = 0;

//	InsertMenu( hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL );

	InsertMenu( hMenu, indexMenu++, MF_STRING | MF_BYPOSITION, idCmd, _T("ShellExtMenu") );

//	InsertMenu( hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL );

	added += 1;
	return ResultFromShort( added );
}


STDMETHODIMP
CMenuExtension::InvokeCommand( LPCMINVOKECOMMANDINFO lpcmi )
{
	if( HIWORD( lpcmi->lpVerb ) != 0 )
		return E_INVALIDARG;

	switch( LOWORD( lpcmi->lpVerb ) )
	{
	case 0:	// ShellExtMenu
		MessageBox( lpcmi->hwnd, _T("ShellExtMenu"), _T("test"), MB_OK );
		break;
	}

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
