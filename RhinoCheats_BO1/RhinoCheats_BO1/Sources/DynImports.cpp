#include "stdafx.h"

void *get_proc_address(HMODULE module, const char *proc_name)
{
	char *modb = reinterpret_cast<char *>(module);
	if (modb == nullptr)
		return nullptr;

	IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)modb;
	if (dos_header == nullptr)
		return nullptr;
	IMAGE_NT_HEADERS *nt_headers = (IMAGE_NT_HEADERS *)(modb + dos_header->e_lfanew);
	if (nt_headers == nullptr)
		return nullptr;
	IMAGE_OPTIONAL_HEADER *opt_header = &nt_headers->OptionalHeader;
	if (opt_header == nullptr)
		return nullptr;
	IMAGE_DATA_DIRECTORY *exp_entry = (IMAGE_DATA_DIRECTORY *)(&opt_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	if (exp_entry == nullptr)
		return nullptr;
	IMAGE_EXPORT_DIRECTORY *exp_dir = (IMAGE_EXPORT_DIRECTORY *)(modb + exp_entry->VirtualAddress);
	if (exp_dir == nullptr)
		return nullptr;

	void **func_table = (void **)(modb + exp_dir->AddressOfFunctions);
	WORD *ord_table = (WORD *)(modb + exp_dir->AddressOfNameOrdinals);
	char **name_table = (char **)(modb + exp_dir->AddressOfNames);
	void *address = nullptr;

	DWORD i;

	/* is ordinal? */
	if (((DWORD)proc_name >> 16) == 0) {
		WORD ordinal = LOWORD(proc_name);
		DWORD ord_base = exp_dir->Base;
		/* is valid ordinal? */
		if (ordinal < ord_base || ordinal > ord_base + exp_dir->NumberOfFunctions)
			return nullptr;

		/* taking ordinal base into consideration */
		address = (void *)(modb + (DWORD)func_table[ordinal - ord_base]);
	}
	else {
		/* import by name */
		for (i = 0; i < exp_dir->NumberOfNames; i++) {
			/* name table pointers are rvas */
			if (strcmp(proc_name, modb + (DWORD)name_table[i]) == 0)
				address = (void *)(modb + (DWORD)func_table[ord_table[i]]);
		}
	}
	/* is forwarded? */
	if ((char *)address >= (char *)exp_dir &&
		(char *)address < (char *)exp_dir + exp_entry->Size) {
		char *dll_name, *func_name;
		HMODULE frwd_module;
		dll_name = _strdup((char *)address);
		if (!dll_name)
			return nullptr;
		
		func_name = strchr(dll_name, '.');
		*func_name++ = 0;

		/* is already loaded? */
		frwd_module = GetModuleHandle(dll_name);
		if (!frwd_module)
			frwd_module = LoadLibrary(dll_name);

		if (frwd_module)
			address = get_proc_address(frwd_module, func_name);
		else
			address = nullptr;

		free(dll_name);
	}
	return address;
}

ULONG_PTR dyn_call(char* dll, char* func) 
{	
	if (func == nullptr)
		return 0;
	if (strlen(func) == 0)
		return 0;

	HMODULE hMod = LoadLibraryA(dll);
	if (hMod == nullptr)
		return 0; // error

	ULONG_PTR ptr = reinterpret_cast<ULONG_PTR>(get_proc_address(hMod, func));

	return ptr;
}


/************************************************************************/
/* CreateThread                                                         */
/************************************************************************/
HANDLE WINAPI SafeCreateRemoteThread(
	HANDLE hProcess,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
)
{
	return FNC(CreateRemoteThread, "kernel32.dll")(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

HANDLE WINAPI SafeCreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId)
{

	return SafeCreateRemoteThread(GetCurrentProcess(), lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}


/************************************************************************/
/* MessageBox                                                           */
/************************************************************************/
int WINAPI SafeMessageBox(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType)
{
	return FNC(MessageBoxA, "user32.dll")(hWnd, lpText, lpCaption, uType);
}

/************************************************************************/
/* Sleep                                                                */
/************************************************************************/
VOID WINAPI SafeSleep(DWORD dwMilliseconds)
{
	FNC(Sleep, "kernel32.dll")(dwMilliseconds);
}

/************************************************************************/
/* VirtualProtect                                                       */
/************************************************************************/
BOOL WINAPI SafeVirtualProtect(
	_In_  LPVOID lpAddress,
	_In_  SIZE_T dwSize,
	_In_  DWORD  flNewProtect,
	_Out_ PDWORD lpflOldProtect
) {
	return FNC(VirtualProtect, "kernel32.dll")(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}