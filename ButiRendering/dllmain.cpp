#include "stdafx.h"
#include"../Header/GetDLLInstance.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
HMODULE g_hModule;
BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    g_hModule = hModule;
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
HINSTANCE GetButiRenderingDLLHInstance() {
    return g_hModule;
}