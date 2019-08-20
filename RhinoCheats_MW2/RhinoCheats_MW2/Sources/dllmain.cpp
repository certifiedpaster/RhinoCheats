#include "stdafx.h"

bool WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		if (!lpReserved) 
		{
			SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.ExecMainThread), nullptr, 0, nullptr);
		}
		else
			return false;		
		break;

	case DLL_PROCESS_DETACH:
		SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.ExecCleaningThread), nullptr, 0, nullptr);
	}
	
	return true;
}

