#pragma once

#define MAKESTR(x) #x
#define FNC(func, lib) (reinterpret_cast<PROTO_##func>(dyn_call(lib, MAKESTR(func))))

#define GET_INT(x)	(*reinterpret_cast<PINT>(x))  
#define XASSERT(x)  if( !x ) { SafeMessageBox( HWND_DESKTOP, STREAM("Error! "<< #x <<" is empty, closing everything.").c_str(), ALASKA17, MB_ICONERROR ); exit(-1); }  

ULONG_PTR dyn_call(char* dll, char* func);

//========================================================================
//Typedefs

typedef HANDLE(WINAPI *PROTO_CreateRemoteThread)(
	__in HANDLE hProcess,
	__in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes,
	__in SIZE_T dwStackSize,
	__in LPTHREAD_START_ROUTINE lpStartAddress,
	__in_opt LPVOID lpParameter,
	__in DWORD dwCreationFlags,
	__out_opt LPDWORD lpThreadId
	);

typedef int(WINAPI *PROTO_MessageBoxA) (
	__in_opt HWND hWnd,
	__in_opt LPCSTR lpText,
	__in_opt LPCSTR lpCaption,
	__in UINT uType
	);

typedef VOID(WINAPI *PROTO_Sleep)(
	__in DWORD dwMilliseconds
	);

typedef BOOL(WINAPI *PROTO_VirtualProtect)(_In_  LPVOID lpAddress,
	_In_  SIZE_T dwSize,
	_In_  DWORD  flNewProtect,
	_Out_ PDWORD lpflOldProtect);

//========================================================================
//Externs

HANDLE WINAPI SafeCreateThread(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId);

int WINAPI SafeMessageBox(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType);

VOID WINAPI SafeSleep(DWORD dwMilliseconds);

BOOL WINAPI SafeVirtualProtect(
	_In_  LPVOID lpAddress,
	_In_  SIZE_T dwSize,
	_In_  DWORD  flNewProtect,
	_Out_ PDWORD lpflOldProtect
);



