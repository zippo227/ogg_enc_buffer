// OggEncoder.h - Contains declaration of Function class
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
		
	extern bool InitEncoder();
	
	extern bool CloseEncoder();
	
	// Length of encoded bytes
	extern int EncodeBuffer(void* buffer, int length);

#ifdef __cplusplus
}
#endif

