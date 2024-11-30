#include "ClassFactory.h"
#include "CustomTestMic_APO.hpp"
#include "Resource.h"


#define WIN32_LEAN_AND_MEAN

static HINSTANCE hInstance;

// TODO: There are things that have a "_Ref_count_obj" instead of this?
static LONG dll_refCount;



extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Alternatively change it to switch case.

    if (DLL_PROCESS_ATTACH == dwReason)
    {
        // "::" ensures accessing global variable
        ::hInstance = hInstance;
    }
    // do necessary cleanup only if the DLL is being unloaded dynamically
    else if ((DLL_PROCESS_DETACH == dwReason) && (NULL == lpReserved))
    {
    }

    return TRUE;
}



// Functions to do things with the ref count
static void IncrementRefCount() {
    InterlockedIncrement(&dll_refCount);
}
static void DecrementRefCount() {
    InterlockedDecrement(&dll_refCount);
}



// This here seems to be important.
HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    
    ClassFactory* factory = new ClassFactory();

    if (rclsid != CustomTestMic_APO_GUID)
        return CLASS_E_CLASSNOTAVAILABLE;

    if (factory == NULL)
        return E_OUTOFMEMORY;

    // Increment Reference Count here
    IncrementRefCount();

    HRESULT hResult = factory->QueryInterface(riid, ppv);
    factory->Release();

    return hResult;
}



// TODO: Yet I don't know the purpose of this.
HRESULT WINAPI DllCanUnloadNow()
{
    return dll_refCount <= 0 ? S_OK : S_FALSE;
}