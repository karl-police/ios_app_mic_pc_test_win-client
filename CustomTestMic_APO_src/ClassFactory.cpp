#include "CustomTestMic_APO.hpp"
#include "ClassFactory.h"




ClassFactory::ClassFactory() {
	m_refCount = 1;
}

ClassFactory::~ClassFactory() {
	m_refCount = 0;
}



HRESULT __stdcall ClassFactory::QueryInterface(const IID& riid, void** ppv) {
	if (ppv == NULL)
		return E_INVALIDARG;

	if (riid == IID_IClassFactory || riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}


ULONG __stdcall ClassFactory::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG __stdcall ClassFactory::Release()
{
	if (InterlockedDecrement(&m_refCount) == 0)
	{
		delete this;
		//return 0;
	}

	return m_refCount;
}



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