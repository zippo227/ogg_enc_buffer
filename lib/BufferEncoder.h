#pragma once


#ifdef __cplusplus
extern "C"
{
#endif

#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>

#ifdef __cplusplus
}
#endif

//Based on steps from: https://xiph.org/vorbis/doc/libvorbis/overview.html

namespace AudioTools
{
	class StreamEncoder
	{
	public:
		StreamEncoder();
		~StreamEncoder();

		int EncodeBuffer(void* buffer, int length);

	private:
		vorbis_info mVorbisInfo;
		vorbis_dsp_state mVorbisDspState;
		vorbis_comment mVorbisComment;

		ogg_packet mOggPacketIdentification;
		ogg_packet mOggPacketComment;
		ogg_packet mOggPacketCodes;

		vorbis_block mVorbisBlock;
	};
}