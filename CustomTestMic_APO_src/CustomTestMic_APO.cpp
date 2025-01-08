#include "CustomTestMic_APO.h"


long CustomTestMic_APO::instCount = 0;

// Registry Properties
const CRegAPOProperties<1> CustomTestMic_APO::regMainProperties(
	CustomTestMic_APO_GUID,
	L"CustomTestMic_APO",
	L"Copyright (C) 2025",
	1,
	0,
	__uuidof(IAudioProcessingObject),
	(APO_FLAG)(APO_FLAG_FRAMESPERSECOND_MUST_MATCH | APO_FLAG_BITSPERSAMPLE_MUST_MATCH | APO_FLAG_INPLACE)
);


// Constructor
CustomTestMic_APO::CustomTestMic_APO(IUnknown* pUnkOuter)
	: CBaseAudioProcessingObject(regMainProperties)
{
	refCount = 1;

	if (pUnkOuter != NULL)
		this->pUnkOuter = pUnkOuter;
	else
		this->pUnkOuter = reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this));

	InterlockedIncrement(&instCount);
};
CustomTestMic_APO::~CustomTestMic_APO()
{
	InterlockedDecrement(&instCount);
};



HRESULT __stdcall CustomTestMic_APO::QueryInterface(const IID& iid, void** ppv)
{
	return pUnkOuter->QueryInterface(iid, ppv);
}
ULONG __stdcall CustomTestMic_APO::AddRef()
{
	return pUnkOuter->AddRef();
}
ULONG __stdcall CustomTestMic_APO::Release()
{
	return pUnkOuter->Release();
}


HRESULT __stdcall CustomTestMic_APO::GetLatency(HNSTIME* pTime)
{
	if (!pTime)
		return E_POINTER;

	if (!m_bIsLocked)
		return APOERR_ALREADY_UNLOCKED;

	*pTime = 0;

	return S_OK;
}

HRESULT __stdcall CustomTestMic_APO::Initialize(UINT32 cbDataSize, BYTE* pbyData)
{
	if ((NULL == pbyData) && (0 != cbDataSize))
		return E_INVALIDARG;
	if ((NULL != pbyData) && (0 == cbDataSize))
		return E_POINTER;
	if (cbDataSize != sizeof(APOInitSystemEffects))
		return E_INVALIDARG;


	return S_OK;
}

HRESULT __stdcall CustomTestMic_APO::IsInputFormatSupported(IAudioMediaType* pOutputFormat,
	IAudioMediaType* pRequestedInputFormat, IAudioMediaType** ppSupportedInputFormat)
{
	if (!pRequestedInputFormat)
		return E_POINTER;

	UNCOMPRESSEDAUDIOFORMAT inFormat;
	HRESULT hr = pRequestedInputFormat->GetUncompressedAudioFormat(&inFormat);
	if (FAILED(hr))
	{
		return hr;
	}

	UNCOMPRESSEDAUDIOFORMAT outFormat;
	hr = pOutputFormat->GetUncompressedAudioFormat(&outFormat);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CBaseAudioProcessingObject::IsInputFormatSupported(pOutputFormat, pRequestedInputFormat, ppSupportedInputFormat);

	return hr;
}



HRESULT __stdcall CustomTestMic_APO::LockForProcess(UINT32 u32NumInputConnections,
	APO_CONNECTION_DESCRIPTOR** ppInputConnections, UINT32 u32NumOutputConnections,
	APO_CONNECTION_DESCRIPTOR** ppOutputConnections)
{
	HRESULT hr;

	UNCOMPRESSEDAUDIOFORMAT outFormat;
	hr = ppOutputConnections[0]->pFormat->GetUncompressedAudioFormat(&outFormat);
	if (FAILED(hr))
		return hr;

	hr = CBaseAudioProcessingObject::LockForProcess(u32NumInputConnections, ppInputConnections,
		u32NumOutputConnections, ppOutputConnections);
	if (FAILED(hr))
		return hr;

	channelCount = outFormat.dwSamplesPerFrame;

	return hr;
}

HRESULT __stdcall CustomTestMic_APO::UnlockForProcess()
{
	return CBaseAudioProcessingObject::UnlockForProcess();
}



#pragma AVRT_CODE_BEGIN
//-------------------------------------------------------------------------
// Description:
//
//  Do the actual processing of data.
//
// Parameters:
//
//      u32NumInputConnections      - [in] number of input connections
//      ppInputConnections          - [in] pointer to list of input APO_CONNECTION_PROPERTY pointers
//      u32NumOutputConnections      - [in] number of output connections
//      ppOutputConnections         - [in] pointer to list of output APO_CONNECTION_PROPERTY pointers
//
// Return values:
//
//      void
//
// Remarks:
//
//  This function processes data in a manner dependent on the implementing
//  object.  This routine can not fail and can not block, or call any other
//  routine that blocks, or touch pagable memory.
//
void __stdcall CustomTestMic_APO::APOProcess(UINT32 u32NumInputConnections,
	APO_CONNECTION_PROPERTY** ppInputConnections, UINT32 u32NumOutputConnections,
	APO_CONNECTION_PROPERTY** ppOutputConnections)
{
	FLOAT32* pf32InputFrames, * pf32OutputFrames;

	// check APO_BUFFER_FLAGS.
	switch (ppInputConnections[0]->u32BufferFlags)
	{
		case BUFFER_VALID:
		case BUFFER_SILENT:
		{
			// get input pointer to connection buffer
			pf32InputFrames = reinterpret_cast<FLOAT32*>(ppInputConnections[0]->pBuffer);

			// get output pointer to connection buffer
			pf32OutputFrames = reinterpret_cast<FLOAT32*>(ppOutputConnections[0]->pBuffer);


			// pass along buffer flags
			ppOutputConnections[0]->u32BufferFlags = ppInputConnections[0]->u32BufferFlags;

			// Set the valid frame count.
			ppOutputConnections[0]->u32ValidFrameCount = ppInputConnections[0]->u32ValidFrameCount;

			break;
		}
	}
}
#pragma AVRT_CODE_END



HRESULT __stdcall CustomTestMic_APO::NonDelegatingQueryInterface(const IID& iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
		*ppv = static_cast<INonDelegatingUnknown*>(this);
	else if (iid == __uuidof(IAudioProcessingObject))
		*ppv = static_cast<IAudioProcessingObject*>(this);
	else if (iid == __uuidof(IAudioProcessingObjectRT))
		*ppv = static_cast<IAudioProcessingObjectRT*>(this);
	else if (iid == __uuidof(IAudioProcessingObjectConfiguration))
		*ppv = static_cast<IAudioProcessingObjectConfiguration*>(this);
	else if (iid == __uuidof(IAudioSystemEffects))
		*ppv = static_cast<IAudioSystemEffects*>(this);
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall CustomTestMic_APO::NonDelegatingAddRef()
{
	return InterlockedIncrement(&refCount);
}

ULONG __stdcall CustomTestMic_APO::NonDelegatingRelease()
{
	if (InterlockedDecrement(&refCount) == 0)
	{
		delete this;
		return 0;
	}

	return refCount;
}