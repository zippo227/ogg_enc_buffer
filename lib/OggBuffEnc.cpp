#include "OggBuffEnc.h"
#include "BufferEncoder.h"

AudioTools::StreamEncoder* streamEnc = NULL;

bool InitEncoder()
{
	streamEnc = new AudioTools::StreamEncoder();

	bool suc = (streamEnc != NULL);
	return suc;
}

int EncodeBuffer(void* buffer, int length)
{
	int numSamplesProcessed = 0;
	if (streamEnc != NULL)
	{
		numSamplesProcessed = streamEnc->EncodeBuffer(buffer, length);
	}

	return numSamplesProcessed;
}

bool CloseEncoder()
{
	if (streamEnc != NULL)
	{
		//TODO: Submit empty buffer to close
		delete streamEnc;
		streamEnc = NULL;
		return true;
	}

	return false;
}