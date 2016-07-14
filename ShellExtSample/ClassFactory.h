
#pragma once
#define __CLASSFACTORY_H__

#include <Unknwnbase.h>

class CClassFactory : public IClassFactory
{
public:
	CClassFactory();
	~CClassFactory();

	//IUnknown members
	STDMETHODIMP			QueryInterface( REFIID, LPVOID FAR * );
	STDMETHODIMP_(ULONG)	AddRef();
	STDMETHODIMP_(ULONG)	Release();

	//IClassFactory members
	STDMETHODIMP			CreateInstance( LPUNKNOWN, REFIID, LPVOID FAR * );
	STDMETHODIMP			LockServer( BOOL );

protected:
	ULONG	m_cRef;
};
