// Test2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>

#ifdef __cplusplus
}
#endif

#include <iostream>

using namespace std;

vorbis_info mVorbisInfo;
vorbis_dsp_state mVorbisDspState;
vorbis_comment mVorbisComment;

ogg_packet mOggPacketIdentification;
ogg_packet mOggPacketComment;
ogg_packet mOggPacketCodes;

vorbis_block mVorbisBlock;

void Write(const char * error)
{
	cout << error;
}

bool ErrorCheck(int code)
{
	switch (code)
	{
	case 0:
		return false;
	case OV_EFAULT:
		Write("OV_EFAULT");
		break;
	case OV_EIMPL:
		Write("OV_EIMPL");
		break;
	default:
		break;
	}

	return true;
}

/*
While there is more audio to encode:

Submit a chunk of audio data using vorbis_analysis_buffer and vorbis_analysis_wrote.
Obtain all available blocks using vorbis_analysis_blockout in a loop. For each block obtained:
Encode the block into a packet (or prepare it for bitrate management) using vorbis_analysis. (It's a good idea to always pass the blocks through the bitrate management mechanism; more information is on the vorbis_analysis page. It does not affect the resulting packets unless you are actually using a bitrate-managed mode.)
If you are using bitrate management, submit the block using vorbis_bitrate_addblock and obtain packets using vorbis_bitrate_flushpacket.
Output any obtained packets.
*/

int EncodeBuffer(void* buffer, int length)
{
	float** allocatedChannels = vorbis_analysis_buffer(&mVorbisDspState, length);

	//I'm not sure of the way that the samples inside of buffer are laid out
	float* bufferChannels = static_cast<float*>(buffer);

	for (int i = 0; i < length; i+=2)
	{
		//Assume two channels here
		allocatedChannels[0][i] = bufferChannels[i];
		allocatedChannels[1][i] = bufferChannels[i + 1];
	}

	if (vorbis_analysis_wrote(&mVorbisDspState, length) != 0)
	{
		//Error
		return 0;
	}

	int processedSamplesCount = 0;

	int retValue = 1;
	while (retValue == 1)
	{
		retValue = vorbis_analysis_blockout(&mVorbisDspState, &mVorbisBlock) == 1;

		if (retValue >= 0)
		{
			//No Error
			int vaErrValue = vorbis_analysis(&mVorbisBlock, NULL);
			if (vaErrValue == 0)
			{
				vorbis_bitrate_addblock(&mVorbisBlock);

				ogg_packet samplePacket;

				int fpRetValue = 1;
				while (fpRetValue == 1)
				{
					fpRetValue = vorbis_bitrate_flushpacket(&mVorbisDspState, &samplePacket);
					if (fpRetValue >= 0)
					{
						//Store the bytes inside of buffer
						//memcpy(buffer, samplePacket.bytes, len)
						//processedSamplesCount += len;
					}
				}
			}
		}

		if (retValue == 0)
		{
			break;
		}
	}

	return processedSamplesCount;
}

int main()
{

	//Initialize the info 
	vorbis_info_init(&mVorbisInfo);

	if (ErrorCheck(vorbis_encode_init_vbr(&mVorbisInfo, 2, 44100, .4f)) == true)
	{
		//Error
		Write("vorbis_encode_init error");
		return 0;
	}

	if (ErrorCheck(vorbis_analysis_init(&mVorbisDspState, &mVorbisInfo)) == true)
	{
		//Error
		Write("vorbis_analysis_init error");
		return 0;
	}

	vorbis_comment_init(&mVorbisComment);
	//vorbis_comment_add(&mVorbisComment, "Comments");
	int vahCode = vorbis_analysis_headerout(&mVorbisDspState, &mVorbisComment, &mOggPacketIdentification, &mOggPacketComment, &mOggPacketCodes);
	if (ErrorCheck(vahCode) == true)
	{
		//Error
		Write("vorbis_analysis_init error");
		return 0;
	}

	if (ErrorCheck(vorbis_block_init(&mVorbisDspState, &mVorbisBlock)) == true)
	{
		//Error
		Write("vorbis_block_init error");
		return 0;
	}
	

	float* buffer = new float[1024]{ 0 };
	EncodeBuffer( (void*)buffer, 1024);

	vorbis_info_clear(&mVorbisInfo);

    return 0;
}

