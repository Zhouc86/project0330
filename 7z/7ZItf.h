#pragma once
#ifdef MY7Z_EXPORTS 
#define MY7Z_API  __declspec(dllexport)
#else
#define MY7Z_API __declspec(dllimport)
#endif
extern "C"
{
	bool MY7Z_API Decompress(const char * file);
}

