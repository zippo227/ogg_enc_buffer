// OggEncoder.h - Contains declaration of Function class
#pragma once

//Functions to be exposed in these libraries
extern "C" bool InitEncoder();
	
extern "C" bool CloseEncoder();
	
// Length of encoded bytes
extern "C" int EncodeBuffer(void* buffer, int length);

