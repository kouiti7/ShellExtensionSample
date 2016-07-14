
#pragma once
#define __MENUEXTENSION_H__

#include <ShObjIdl.h>

class CMenuExtension : public IContextMenu, public IShellExtInit
{
public:
	CMenuExtension();
	~CMenuExtension();

    // IUnknown
    STDMETHODIMP            QueryInterface( REFIID, LPVOID FAR * );
    STDMETHODIMP_(ULONG)    AddRef();
    STDMETHODIMP_(ULONG)    Release();

    // IShellExtInit
    STDMETHODIMP	Initialize(
							LPCITEMIDLIST pIDFolder,
							LPDATAOBJECT pDataObj,
							HKEY hKeyID );

    // IContextMenu
    STDMETHODIMP	QueryContextMenu(
							HMENU hMenu,
							UINT indexMenu,
							UINT idCmdFirst,
							UINT idCmdLast,
							UINT uFlags );
    STDMETHODIMP	InvokeCommand( LPCMINVOKECOMMANDINFO lpcmi );

	STDMETHODIMP	GetCommandString(
							UINT_PTR idCmd, 
							UINT uFlags, 
							UINT FAR *reserved, 
				            _Out_writes_bytes_((uType & GCS_UNICODE) ? (cchMax * sizeof(wchar_t)) : cchMax) _When_(!(uType & (GCS_VALIDATEA | GCS_VALIDATEW)), _Null_terminated_)  CHAR *pszName,
							UINT cchMax );

protected:
	ULONG					m_cRef;
	LPDATAOBJECT			m_pDataObj;
};
