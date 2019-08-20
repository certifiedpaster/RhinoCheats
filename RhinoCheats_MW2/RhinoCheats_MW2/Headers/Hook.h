#pragma once

/*
#define POINTER DWORD32
#define EXC Eip
#define REG Edi
#define REG_SIZE 6
#define PTR_SIZE 4

namespace VEH
{
	typedef void(*tCallback)();

	typedef struct
	{
		int iIndex;
		POINTER pBackup;
		POINTER pDvar;
		POINTER pException;
		tCallback Callback;
	} sVEHContext;

	static LPVOID VEHandler;
	static std::vector<sVEHContext> VEHContexts;

	static LONG CALLBACK VEHHook(LPEXCEPTION_POINTERS exception)
	{
		POINTER pRegister = reinterpret_cast<POINTER>(&exception->ContextRecord->REG);

		for (int i = 0; i < REG_SIZE; i++)
		{
			for (auto& VEHContext : VEHContexts)
			{
				if (*reinterpret_cast<POINTER*>(pRegister) == VEHContext.iIndex)
				{
					*reinterpret_cast<POINTER*>(pRegister) = VEHContext.pBackup;

					if (exception->ContextRecord->EXC == VEHContext.pException)
						VEHContext.Callback();

					return EXCEPTION_CONTINUE_EXECUTION;
				}
			}

			pRegister += PTR_SIZE;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	static void AddHook(POINTER dvar, POINTER exception, tCallback callback)
	{
		sVEHContext VEHContext;

		VEHContext.iIndex = static_cast<int>(VEHContexts.size());
		VEHContext.pBackup = *reinterpret_cast<POINTER*>(dvar);
		VEHContext.pDvar = dvar;
		VEHContext.pException = exception;
		VEHContext.Callback = callback;

		VEHContexts.push_back(VEHContext);
	}

	static void ForceExceptions()
	{
		VEHandler = AddVectoredExceptionHandler(FALSE, VEHHook);

		for (auto& VEHContext : VEHContexts)
			*reinterpret_cast<POINTER*>(VEHContext.pDvar) = VEHContext.iIndex;
	}

	static void RemoveHooks()
	{
		for (auto& VEHContext : VEHContexts)
			*reinterpret_cast<POINTER*>(VEHContext.pDvar) = VEHContext.pBackup;

		VEHContexts.clear();

		RemoveVectoredExceptionHandler(VEHandler);
	}
};

*/
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

