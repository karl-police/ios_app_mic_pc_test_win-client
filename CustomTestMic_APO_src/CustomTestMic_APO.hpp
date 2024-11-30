#include <audioenginebaseapo.h>
#include <baseaudioprocessingobject.h>


#include "Resource.h"


class CustomTestMic_APO
	: public CBaseAudioProcessingObject, /*public IAudioProcessingObject, public IAudioProcessingObjectConfiguration,
		public IAudioProcessingObjectRT,*/ public IAudioSystemEffects
{
	
};