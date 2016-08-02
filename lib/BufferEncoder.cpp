extern "C"
{
#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>
}

#include "BufferEncoder.h"
#include <iostream>

using namespace std;

namespace AudioTools
{
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

	StreamEncoder::StreamEncoder()
	{
		//Initialize the info 
		vorbis_info_init(&mVorbisInfo);

		if (ErrorCheck(vorbis_encode_init(&mVorbisInfo, 2, 44100, 100, 80, 60)) == true)
		{
			//Error
			Write("vorbis_encode_init error");
			return;
		}

		if (ErrorCheck(vorbis_analysis_init(&mVorbisDspState, &mVorbisInfo)) == true)
		{
			//Error
			Write("vorbis_analysis_init error");
			return;
		}

		vorbis_comment_init(&mVorbisComment);
		//vorbis_comment_add(&mVorbisComment, "Comments");
		int vahCode = vorbis_analysis_headerout(&mVorbisDspState, &mVorbisComment, &mOggPacketIdentification, &mOggPacketComment, &mOggPacketCodes);
		if (ErrorCheck(vahCode) == true)
		{
			//Error
			Write("vorbis_analysis_init error");
			return;
		}

		if (ErrorCheck(vorbis_block_init(&mVorbisDspState, &mVorbisBlock)) == true)
		{
			//Error
			Write("vorbis_block_init error");
			return;
		}
	}

	int StreamEncoder::EncodeBuffer(void* buffer, int length)
	{
		/*
		While there is more audio to encode:

		Submit a chunk of audio data using vorbis_analysis_buffer and vorbis_analysis_wrote.
		Obtain all available blocks using vorbis_analysis_blockout in a loop. For each block obtained:
			Encode the block into a packet (or prepare it for bitrate management) using vorbis_analysis. (It's a good idea to always pass the blocks through the bitrate management mechanism; more information is on the vorbis_analysis page. It does not affect the resulting packets unless you are actually using a bitrate-managed mode.)
			If you are using bitrate management, submit the block using vorbis_bitrate_addblock and obtain packets using vorbis_bitrate_flushpacket.
			Output any obtained packets.
		*/

		float** allocatedChannels = vorbis_analysis_buffer(&mVorbisDspState, length);

		//I'm not sure of the way that the samples inside of buffer are laid out
		float* bufferChannels = static_cast<float*>(buffer);

		Write("Casted the buffer channels");

		for (int i = 0; i < length; i+=2)
		{
			//Assume two channels here
			allocatedChannels[0][i] = bufferChannels[i];
			allocatedChannels[1][i] = bufferChannels[i+1];
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
				if (vorbis_analysis(&mVorbisBlock, NULL) == 0)
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

	StreamEncoder::~StreamEncoder()
	{
		//Submit an empty buffer to indicate the end of input; this will result in an end - of - stream packet after all encoding steps are done to it.
		//	Destroy the structures using the appropriate vorbis_*_clear routines.

		vorbis_info_clear(&mVorbisInfo);
	}
}