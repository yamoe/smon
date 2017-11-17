#pragma once


/*
#include "smon.h"

#if defined(WINDOWS)


#ifdef SMON_EXPORTS
# define SMON_API __declspec(dllexport)
#else
# define SMON_API __declspec(dllimport)
#endif



BOOL APIENTRY DllMain(HMODULE hModule,
DWORD  ul_reason_for_call,
LPVOID lpReserved
)
{
switch (ul_reason_for_call)
{
case DLL_PROCESS_ATTACH:
case DLL_THREAD_ATTACH:
case DLL_THREAD_DETACH:
case DLL_PROCESS_DETACH:
break;
}
return TRUE;
}


extern "C" {

SMON_API int smon_add(int i)
{
return i + 1;
}

}
#endif
*/


