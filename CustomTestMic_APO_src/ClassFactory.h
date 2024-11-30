#pragma once

#include <Unknwn.h>


class ClassFactory : public IClassFactory {
public:
	ClassFactory();
	//~ClassFactory();

	// IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	// IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, const IID& riid, void** ppvObject);
	virtual HRESULT __stdcall LockServer(BOOL bLock);


	static long lockCount;
private:
	long refCount;
};