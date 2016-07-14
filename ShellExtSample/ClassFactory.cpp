
#include "stdafx.h"
#include "ClassFactory.h"
#include "MenuExtension.h"

extern HINSTANCE	g_inst;
extern UINT			g_cRefThisDll;

CClassFactory::CClassFactory()
{
	m_cRef = 0L;
	g_cRefThisDll++;
}

CClassFactory::~CClassFactory()
{
	g_cRefThisDll--;
}


STDMETHODIMP
CClassFactory::QueryInterface( REFIID riid, LPVOID FAR *ppv )
{
	*ppv = NULL;

	// Any interface on this object is the object pointer
	if( IsEqualIID( riid, IID_IUnknown )  ||  IsEqualIID( riid, IID_IClassFactory ) )
	{
		*ppv = (LPCLASSFACTORY)this;
		AddRef();
		return NOERROR;
	}

	return E_NOINTERFACE;
}	

STDMETHODIMP_(ULONG)
CClassFactory::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG)
CClassFactory::Release()
{
	if( --m_cRef )
		return m_cRef;
	delete this;
	return 0L;
}

STDMETHODIMP
CClassFactory::CreateInstance( LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppvObj )
{
	*ppvObj = NULL;

	if( pUnkOuter )
		return CLASS_E_NOAGGREGATION;

	CMenuExtension *pMenuExt = new CMenuExtension();

	if( !pMenuExt )
		return E_OUTOFMEMORY;

	return pMenuExt->QueryInterface( riid, ppvObj );
}

STDMETHODIMP
CClassFactory::LockServer( BOOL fLock )
{
	return NOERROR;
}
