extern "C"
{
#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>
}

#include "BufferEncoder.h"

namespace AudioTools
{
	StreamEncoder::StreamEncoder()
	{
		//Initialize the info 
		vorbis_info_init(&mVorbisInfo);

		if (vorbis_encode_init(&mVorbisInfo, 2, 44100, 100, 80, 60) != 0)
		{
			//Error
		}

		if (vorbis_analysis_init(&mVorbisDspState, &mVorbisInfo) != 0)
		{
			//Error
		}

		vorbis_comment_init(&mVorbisComment);
		//vorbis_comment_add(&mVorbisComment, "Comments");

		if (vorbis_analysis_headerout(&mVorbisDspState, &mVorbisComment, &mOggPacketIdentification, &mOggPacketComment, &mOggPacketCodes) != 0)
		{
			//Error
		}

		if (vorbis_block_init(&mVorbisDspState, &mVorbisBlock) != 0)
		{
			//Error
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
		float** bufferChannels = static_cast<float**>(buffer);

		for (int i = 0; i < length; i++)
		{
			//Assume two channels here
			allocatedChannels[0][i] = bufferChannels[0][i];
			allocatedChannels[1][i] = bufferChannels[1][i];
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