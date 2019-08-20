#include "stdafx.h"
#include "EndFrame.h"



DWORD InternalValue_HUDSAYPOSITION_DVAR = NULL;

/*
#include <io.h>
#include <iostream>
#include <fcntl.h>
void CreateConsole(std::string name)
{
	AllocConsole();

	HANDLE lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	XASSERT(lStdHandle);

	int hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	XASSERT(hConHandle);

	SetConsoleTitleA(name.c_str());
	SetConsoleTextAttribute(lStdHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED);

	FILE *fp = _fdopen(hConHandle, "w");
	XASSERT(fp);

	/ *Won't work on 64bit with Visual Studio 2015 :(
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);* /

	//http://stackoverflow.com/questions/32185512/output-to-console-from-a-win32-gui-application-on-windows-10
	freopen_s(&fp, "CONOUT$", "w", stdout);
}
*/

/************************************************************************/
/* Steam Check                                                          */
/************************************************************************/

/************************************************************************/
/* Offsets - Pointers                                                   */
/************************************************************************/

void PatchBytes(void * lpAddress, const char * szBytes, int nLen)
{
	// Needed by VirtualProtect.
	DWORD dwBack = 0;
	SafeVirtualProtect(lpAddress, nLen, PAGE_READWRITE, &dwBack);

	// Write Byte-After-Byte.
	for (int i = 0; i < nLen; i++)
		*(BYTE *)((DWORD)lpAddress + i) = szBytes[i];

	// Restore old protection.
	SafeVirtualProtect(lpAddress, nLen, dwBack, &dwBack);
}

void GetOffsets() {
		
	HMODULE iw5mp_module = nullptr;
	while (!(iw5mp_module = GetModuleHandleA("BlackOpsMP.exe")))
	{		
		SafeSleep(119);
	}	

	DWORD iw5mp_size = Hook.GetModuleInfo(iw5mp_module).SizeOfImage;
	XASSERT(iw5mp_size);

	Offsets::cg_entities = 0x2C483EFC;
	Offsets::ci = 0x2C3F6DA8;
	Offsets::cg = 0x2C397B80;
	Offsets::refdef = 0x2C3DAC80;
	Offsets::rdViewAngles = 0x2C3DAB2C;

	Offsets::Fonts = 0x037A14A4;
	Offsets::Fonts_Dev = 0x00D51214;

	Offsets::R_AddCmdDrawText = 0x6F44A0;
	Offsets::R_AddDrawStretchPic = 0x6F3F70;
	Offsets::DrawRotatedPic = 0x6F4110;
	Offsets::Rtag = 0x4202D0;
	Offsets::BG_GetWeapondef = 0x553510;
	Offsets::GetWeapon = 0x529FF0;

	Offsets::Gettag1 = 0x615EA0;
	Offsets::Gettag2 = 0x4B2F50;

	Offsets::white = 0xD4F188;	


	Offsets::HUDSAYPOSITION_DVAR = 0xCBF804;	
	Offsets::HUDSAYPOSITION_EXCEPTION = 0x7CA365;

}

void GetPointers()
{
	cg_entities = (centity_t *)Offsets::cg_entities;
	ci = (clientInfo_t *)Offsets::ci;
	cg = (cg_t *)Offsets::cg;

	refdef = (refdef_t *)Offsets::refdef;
	rdViewAngles = (rdViewAngles_t *)Offsets::rdViewAngles;

	Fonts = (Fonts_t *)Offsets::Fonts;
	Fonts_Dev = (Fonts_Dev_t *)Offsets::Fonts_Dev;

	//========================================================================

	R_AddCmdDrawText = (R_AddCmdDrawText_t)Offsets::R_AddCmdDrawText;
	R_AddDrawStretchPic = (R_AddDrawStretchPic_t)Offsets::R_AddDrawStretchPic;
	DrawRotatedPic = (CG_DrawRotatedPicPhysical_t)Offsets::DrawRotatedPic;
	Rtag = (RegisterTag_t)Offsets::Rtag;
	BG_GetWeapondef = (BG_GetWeapondef_t)Offsets::BG_GetWeapondef;
	GetWeapon = (GetWeapon_t)Offsets::GetWeapon;
	

	InternalValue_HUDSAYPOSITION_DVAR = *(DWORD*)Offsets::HUDSAYPOSITION_DVAR;
	
}

/************************************************************************/
/* Threads                                                              */
/************************************************************************/

HANDLE SetNullPtrThreadHandle = nullptr;


void SetNullPtrThread()
{
	while (2019)
	{
		//*(DWORD *)Offsets::PACKETDUPLICATION_DVAR = 0; //"cl_packetdup"	
		*(DWORD *)Offsets::HUDSAYPOSITION_DVAR = 0; //"cg_hudSayPosition"			

		SafeSleep(5017);
	}
}


/************************************************************************/
/* Hooks                                                                */
/************************************************************************/

long __stdcall pVEH_Hook(_EXCEPTION_POINTERS *pInfo)
{	
	/*if (pInfo->ContextRecord->Eip == Offsets::PACKETDUPLICATION_EXCEPTION)
	{
		pInfo->ContextRecord->Ecx = / *0x59D6A20* / 0x059DAA20; // check it live.
		WritePacket();
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else */if (pInfo->ContextRecord->Eip == Offsets::HUDSAYPOSITION_EXCEPTION)
	{
		pInfo->ContextRecord->Eax = InternalValue_HUDSAYPOSITION_DVAR;
		RenderAll();
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	
	return EXCEPTION_CONTINUE_SEARCH;
}






/************************************************************************/
/* Hook Class                                                           */
/************************************************************************/

Hook_t Hook;

char Hook_t::m_szDllDirectory[MAX_PATH] = { 0 };

MODULEINFO Hook_t::GetModuleInfo(HMODULE hModule)
{
	MODULEINFO modinfo = { 0 };

	//Can't be safe, otherwise it'll crash.
	/*Safe*/GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

void Hook_t::MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	DWORD dwOldProtect = 0;

	// give the paged memory read/write permissions

	SafeVirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// calculate the distance between our address and our target location
	// and subtract the 5bytes, which is the size of the jmp
	// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes

	DWORD dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

	// overwrite the byte at pAddress with the jmp opcode (0xE9)

	*pAddress = 0xE9;

	// overwrite the next 4 bytes (which is the size of a DWORD)
	// with the dwRelAddr

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

	// overwrite the remaining bytes with the NOP opcode (0x90)
	// NOP opcode = No OPeration

	for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;

	// restore the paged memory permissions saved in dwOldProtect

	DWORD dwBkup = 0;

	SafeVirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

}

void Hook_t::playSound(LPVOID HackPath) {	
	PlaySound(reinterpret_cast<char*>(HackPath), nullptr, SND_APPLICATION | SND_FILENAME | SND_ASYNC);
}

void Hook_t::CreateFolder(const char * path)
{
	if (!CreateDirectory(path, NULL))	
		return;	
}

void Hook_t::FinishThread(HANDLE hThread)
{
	if (hThread != nullptr)
	{
		DWORD dwExit = 0;
		GetExitCodeThread(hThread, &dwExit);
		TerminateThread(hThread, dwExit);
		CloseHandle(hThread);
		hThread = nullptr;
	}
}

void Hook_t::ExecMainThread()
{
	// Level 0
	/*CreateConsole("Output");
	printf("Dev console\n");
	std::cout << "cout works\n\n";*/

	// Level 1
	GetOffsets();
	GetPointers();		

	AddVectoredExceptionHandler(17, pVEH_Hook);
	SetNullPtrThreadHandle = SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(SetNullPtrThread), nullptr, 0, nullptr);		

}

void Hook_t::ExecCleaningThread()
{	
	FinishThread(SetNullPtrThreadHandle);	
	*(DWORD *)Offsets::HUDSAYPOSITION_DVAR = InternalValue_HUDSAYPOSITION_DVAR;
	RemoveVectoredExceptionHandler(pVEH_Hook);	
}










