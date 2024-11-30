#include "CustomTestMic_APO.hpp"
#include "ClassFactory.h"



ClassFactory::ClassFactory() {
	refCount = 1;
}

/*ClassFactory::~ClassFactory() {
	InterlockedDecrement(&refCount);
}*/


HRESULT __stdcall ClassFactory::CreateInstance(IUnknown* pUnkOuter, const IID& riid, void** ppvObject)
{
	if (pUnkOuter != NULL && riid != __uuidof(IUnknown))
		return E_NOINTERFACE;

	CustomTestMic_APO* apo = new CustomTestMic_APO(pUnkOuter);
	if (apo == NULL)
		return E_OUTOFMEMORY;

	HRESULT hr = apo->NonDelegatingQueryInterface(riid, ppvObject);

	apo->NonDelegatingRelease();
	return hr;
}


HRESULT __stdcall ClassFactory::LockServer(BOOL bLock) {
	if (bLock)
		InterlockedIncrement(&lockCount);
	else
		InterlockedDecrement(&lockCount);

	return S_OK;
}