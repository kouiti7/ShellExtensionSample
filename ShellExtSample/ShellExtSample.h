
#pragma once

#include "ShObjIdl.h"

class CShellExtSample : public IShellExtInit
{
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize( 
        _In_opt_  PCIDLIST_ABSOLUTE pidlFolder,
        _In_opt_  IDataObject *pdtobj,
        _In_opt_  HKEY hkeyProgID);
};
