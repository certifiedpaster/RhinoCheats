#pragma once

//=====================================================================================

class Hook_t
{	
public:
	static char m_szDllDirectory[MAX_PATH];

	static bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
		{
			if (*pData != *bMask && *szMask == 'x')
				return false;
		}

		return (*szMask) == NULL;
	}
	static DWORD FindPattern(DWORD dwAddress, DWORD dwSize, BYTE* pbMask, char* szMask)
	{
		for (DWORD i = 0; i < dwSize; i++)
		{
			if (DataCompare(reinterpret_cast<BYTE*>(dwAddress + i), pbMask, szMask))
				return dwAddress + i;
		}
		return 0;
	}
	static MODULEINFO GetModuleInfo(HMODULE hModule);	

	static void MakeJMP(BYTE * pAddress, DWORD dwJumpTo, DWORD dwLen);
	static void playSound(LPVOID HackPath);
	static void CreateFolder(const char * path);	
	static void FinishThread(HANDLE hThread);
	static void ExecMainThread();
	static void ExecCleaningThread();	
};

extern Hook_t Hook;

