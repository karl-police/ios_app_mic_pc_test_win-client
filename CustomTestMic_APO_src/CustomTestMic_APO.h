//#pragma once

#include <Unknwn.h>
#include <audioenginebaseapo.h>
#include <baseaudioprocessingobject.h>


#include "Resource.h"


class INonDelegatingUnknown
{
	virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& iid, void** ppv) = 0;
	virtual ULONG __stdcall NonDelegatingAddRef() = 0;
	virtual ULONG __stdcall NonDelegatingRelease() = 0;
};


class CustomTestMic_APO
	: public CBaseAudioProcessingObject, /*public IAudioProcessingObject, public IAudioProcessingObjectConfiguration,
		public IAudioProcessingObjectRT,*/ public IAudioSystemEffects, public INonDelegatingUnknown
{
public:
	CustomTestMic_APO(IUnknown* pUnkOuter);


	static const CRegAPOProperties<1> regMainProperties;
};