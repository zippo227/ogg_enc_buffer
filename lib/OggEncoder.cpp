#include "stdafx.h"
#include "OggEncoder.h"
#include "StreamEncoder.h"

AudioTools::StreamEncoder* streamEnc = nullptr;

bool InitEncoder()
{
	streamEnc = new AudioTools::StreamEncoder();

	bool suc = (streamEnc != nullptr);
	return suc;
}

int EncodeBuffer(void* buffer, int length)
{
	int numSamplesProcessed = 0;
	if (streamEnc != nullptr)
	{
		numSamplesProcessed = streamEnc->EncodeBuffer(buffer, length);
	}

	return numSamplesProcessed;
}

bool CloseEncoder()
{
	if (streamEnc != nullptr)
	{
		//TODO: Submit empty buffer to close
		delete streamEnc;
		streamEnc = nullptr;
		return true;
	}

	return false;
}