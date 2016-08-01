// OggEncoder.h - Contains declaration of Function class
#pragma once

//Functions to be exposed in these libraries

extern "C" {
		
	extern bool InitEncoder();
	
	extern bool CloseEncoder();
	
	// Length of encoded bytes
	extern int EncodeBuffer(void* buffer, int length);
}

