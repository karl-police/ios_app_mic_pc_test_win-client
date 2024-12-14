#include "ClassFactory.h"
#include "CustomTestMic_APO.h"
#include "Resource.h"


#define WIN32_LEAN_AND_MEAN

static HINSTANCE hInstance;

// TODO: There are things that have a "_Ref_count_obj" instead of this?
//static LONG dll_refCount;



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
/*static void IncrementRefCount() {
    InterlockedIncrement(&dll_refCount);
}
static void DecrementRefCount() {
    InterlockedDecrement(&dll_refCount);
}*/



// This here seems to be important.
HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    
    ClassFactory* factory = new ClassFactory();

    if (rclsid != CustomTestMic_APO_GUID)
        return CLASS_E_CLASSNOTAVAILABLE;

    if (factory == NULL)
        return E_OUTOFMEMORY;

    // Increment Reference Count here
    //IncrementRefCount();

    HRESULT hResult = factory->QueryInterface(riid, ppv);
    factory->Release();

    return hResult;
}






//===========================================================================
//
// DllRegisterServer
//
// Purpose: Called during setup or by regsvr32.
//
// Return:  NOERROR if registration successful, error otherwise.
//===========================================================================
HRESULT WINAPI DllRegisterServer() {
    HRESULT hResult = RegisterAPO();

    if (FAILED(hResult)) {
        UnregisterAPO();
        return hResult;
    }
}


//===========================================================================
//
// DllUnregisterServer
//
// Purpose: Called when it is time to remove the registry entries.
//
// Return:  NOERROR if registration successful, error otherwise.
//===========================================================================
HRESULT WINAPI DllUnregisterServer() {
    
}



//===========================================================================
//
// ::DllCanUnloadNow
//
// Purpose: Called periodically by OLE in order to determine if the
//          DLL can be freed.
//
// Return: S_OK if there are no objects in use and the class factory 
//          isn't locked.
//
//===========================================================================
HRESULT WINAPI DllCanUnloadNow()
{
    // It is okay to unload
    // if there are no objects or locks on the ClassFactory.

    // If there are issues, perhaps add refCount here as well somehow.

    if (ClassFactory::lockCount == 0) {
        return S_OK;
    }
    else {
        return S_FALSE;
    }
}