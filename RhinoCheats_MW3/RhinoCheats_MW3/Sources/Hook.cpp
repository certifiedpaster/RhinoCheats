#include "stdafx.h"
#include "EndFrame.h"
#include "WritePackets.h"
#include "Host.h"

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

bool isTekno = false;
void Bypass(void);

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
	while (!(iw5mp_module = GetModuleHandleA("iw5mp.exe")))
	{		
		SafeSleep(119);
	}	

	DWORD iw5mp_size = Hook.GetModuleInfo(iw5mp_module).SizeOfImage;
	XASSERT(iw5mp_size);

	if (iw5mp_size == 109072384)
		isTekno = true;
	else
		isTekno = false;	

	Offsets::stats = isTekno ? 0x01CDBA54 : 0x01CE4E38;
	Offsets::stunflash_congrenade = isTekno ? 0x8DB5A0 : 0x8DD410;
	Offsets::stunflash_flashgrenade = isTekno ? 0x8DB808 : 0x8DD678;
	Offsets::victim = isTekno ? 0x5CC7A8C : 0x5CECE0C;
	Offsets::server_entityinfo = isTekno ? 0 : 0x1C2D45C;
	
	Offsets::awallcheck_1 = isTekno ? 0x4F0CF0 : 0x6A7310;
	Offsets::awallcheck_2 = isTekno ? 0x489F70 : 0x6A7350;
	Offsets::awallcheck_3 = isTekno ? 0x52A190 : 0x438F00;
	Offsets::awallcheck_4 = isTekno ? 0x46B370 : 0x432530;

	//========================================================================

	DWORD Xis5, Xis4, Xis3, Xis2, Xis1 = 0;
	if (!isTekno)
	{	
		Xis1 = GET_INT(reinterpret_cast<DWORD>(GetModuleHandleA("steam_api.dll")) + 0x0001824C);
		XASSERT(Xis1);

		Xis2 = GET_INT(Xis1);
		XASSERT(Xis2);
		Xis2 = Xis2 + 0x4;

		Xis3 = GET_INT(Xis2);
		XASSERT(Xis3);
		Xis3 = Xis3 + 0x8;

		Xis4 = GET_INT(Xis3);
		XASSERT(Xis4);
		Xis4 = Xis4 + 0x10;

		Xis5 = GET_INT(Xis4);
		XASSERT(Xis5);
		Xis5 = Xis5 + 0x7e;

		DWORD InternalTestOfXis5 = GET_INT(Xis5);
		XASSERT(InternalTestOfXis5);
	}
	else
	{		
		
		Xis5 = 0x05A7B1B4;
		DWORD InternalTestOfXis5 = GET_INT(Xis5);
		XASSERT(InternalTestOfXis5);

		PatchBytes((void*)0x4A578C, "\xC3", 1);			
	}	
	Offsets::namegame = Xis5;	


	DWORD vn = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x56\x8B\x74\x24\x08\xD9\x46\x04\xD9\x06\xD9\x46\x08\xD9\xC1\xDE\xCA\xD9\xC2\xDE\xCB\xD9\xC9\xDE\xC2\xDC\xC8\xDE\xC1\xD9\x5C\x24\x08\xD9\x44\x24\x08\xE8\x00\x00\x00\x00\xD9\x5C\x24\x08\xD9\x44\x24\x08\xD9\x5C\x24\x08\xD9\x44\x24\x08\xD9\xC0\xD9\xE0\xD8\x1D\x00\x00\x00\x00\xDF\xE0\xF6\xC4\x01\x75\x08\xD9\xE8\xD9\x5C\x24\x08\xEB\x04\xD9\x54\x24\x08\xD9\x44\x24\x08\xD9\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxx");
	XASSERT(vn);
	Offsets::normalize = vn;

	DWORD xg = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x14\x81\xC7\x00\x00\x00\x00\x83\xEB\x01\x75\xBF\x5F\x5E\x5B\x5D\xC3", "x????x????xxxxx????xxxxxxxxxx");
	XASSERT(xg);
	Offsets::cgt = GET_INT(xg + 1);

	DWORD xgs = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x53\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x74\x24\x7C", "x????x????x????xx????x????xxxx");
	XASSERT(xgs);
	Offsets::cgs = GET_INT(xgs + 1);

	DWORD xy = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x05\x00\x00\x00\x00\xF6\x80\xD0\x01\x00\x00\x01\x74\x1D\xD9\x40\x14", "x????xxxxxxxxxxxx");
	XASSERT(xy);
	Offsets::entity = GET_INT(xy + 1);

	DWORD xt = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xC1\x00\x00\x00\x00\x51\x56\x50", "xx????xxx");
	XASSERT(xt);
	Offsets::client = GET_INT(xt + 2);

	DWORD xf = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\x56\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x38", "x????xxx????????x????xxx");
	XASSERT(xf);
	Offsets::refdef = GET_INT(xf + 1);

	DWORD xxt = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x40\xD9\xEE\x8B\x4C\x24\x3C\x8B\x54\x24\x38\x50\x8B\x44\x24\x38\x51\x8B\x4C\x24\x38\x52\x8B\x54\x24\x38\x50", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	XASSERT(xxt);
	Offsets::text = xxt;

	DWORD xwh = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x51\x53\x57\x33\xDB", "xxxxx");
	XASSERT(xwh);
	Offsets::strWidth = xwh;

	DWORD xewt = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x57\x68\x00\x00\x00\x00", "x????x????xx????");
	XASSERT(xewt);
	Offsets::white = GET_INT(xewt + 1);

	DWORD xfs = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA1\x00\x00\x00\x00\x7B\x05\xA1\x00\x00\x00\x00\xC3", "x????xxx????x");
	XASSERT(xfs);
	Offsets::fonts_t = GET_INT(xfs + 1);

	DWORD xfvs = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x07\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x07\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x07\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00", "x????x????xxx????x????x????xxx????x????xxx????x????");
	XASSERT(xfvs);
	Offsets::fonts_dev_t = GET_INT(xfvs + 1);

	DWORD xdrs = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x04\x80\x38\x00\x75\x06\xA1\x00\x00\x00\x00", "xxxxxxxxxx????");
	XASSERT(xdrs);
	Offsets::shader = xdrs;

	DWORD xrtsa = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x54\x24\x04\x8B\xC2\x56\x8D\x70\x01\x8D\x9B\x00\x00\x00\x00\x8A\x08\x40\x84\xC9\x75\xF9\x2B\xC6\x6A\x07\x40\x50\x6A\x01", "xxxxxxxxxxxx????xxxxxxxxxxxxxxx");
	XASSERT(xrtsa);
	Offsets::regtag = xrtsa;

	DWORD xgtsa = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x8E\x00\x00\x00\x00\x50\x51\xE8\x00\x00\x00\x00\x83\xC4\x08\x85\xC0\x75\x01", "xx????xxx????xxxxxxx");
	XASSERT(xgtsa);
	Offsets::gettag = xgtsa;

	DWORD xctec = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x14\x83\xEC\x0C\x50\x8D\x4C\x24\x04", "xxxxxxxxxxxx");
	XASSERT(xctec);
	Offsets::cgtrace = xctec;

	DWORD xptw = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x0C\x85\x00\x00\x00\x00\x8B\x01\x80\x38\x69\x75\x18", "xxx????xxxxxxx");
	XASSERT(xptw);
	Offsets::getweapon = GET_INT(xptw + 3);

	DWORD xmtw = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xB8\x00\x00\x00\x00\xD9\x80\x00\x00\x00\x00\xD8\x00", "x????xx????xx");
	XASSERT(xmtw);
	Offsets::viewmatrix = GET_INT(xmtw + 1);
	

	DWORD xmon = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x14\x6A\x03\x8D\x54\x24\x10", "x????x????xxxxxxxxx");
	XASSERT(xmon);
	Offsets::zoom = GET_INT(xmon + 1);

	DWORD xieds = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x89\x1D\x00\x00\x00\x00\x89\x1D\x00\x00\x00\x00\x89\x1D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x08\x5B\xC3", "xx????xx????xx????x????xxxxx");
	XASSERT(xieds);
	Offsets::serverid = GET_INT(xieds + 2);

	DWORD xurea = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x3B\x35\x00\x00\x00\x00\x7C\xE9\x5F\x5B\x68\x00\x00\x00\x00", "xx????xxxxx????");
	XASSERT(xurea);
	Offsets::dvars = GET_INT(xurea + 2);

	DWORD xioup = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x10\x8B\xC5\xE8\x00\x00\x00\x00", "x????x????xxxxxx????");
	XASSERT(xioup);
	Offsets::input = GET_INT(xioup + 1);

	DWORD xynf = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x18\x56\x8B\x74\x24\x20\x85\xF6\x74\x13", "xxxxxxxxxxxx");
	XASSERT(xynf);
	Offsets::weaponspread = xynf;

	DWORD xynf2 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x18\x56\xE8\x00\x00\x00\x00\xDC\x0D\x00\x00\x00\x00", "xxxxx????xx????");
	XASSERT(xynf2);
	Offsets::randomspread = xynf2;

	DWORD xynf3 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xF7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x75\x24\xE8\x00\x00\x00\x00\x8B\x1D\x00\x00\x00\x00", "xx????????xxx????xx????");
	XASSERT(xynf3);
	Offsets::isplaying = GET_INT(xynf3 + 2);

	DWORD xynf4 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x24\xD9\x05\x00\x00\x00\x00\x53\x8B\x5C\x24\x30\xD9\x5C\x24\x04\xD9\x05\x00\x00\x00\x00", "xxxxx????xxxxxxxxxxx????");
	XASSERT(xynf4);
	Offsets::visible = xynf4;

	DWORD xynf5 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xC6\x00\x00\x00\x00\x89\x34\x85\x00\x00\x00\x00\x40\xF7\x44\x24\x00\x00\x00\x00\x00", "xx????xxx????xxxx?????");
	XASSERT(xynf5);
	Offsets::dvarbase = GET_INT(xynf5 + 2);

	DWORD xynf6 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA1\x00\x00\x00\x00\x8D\x04\x85\x00\x00\x00\x00\x8B\xC8\x81\xE9\x00\x00\x00\x00", "x????xxx????xxxx????");
	XASSERT(xynf6);
	Offsets::dvarsize = GET_INT(xynf6 + 1);

	DWORD xynf7 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x89\x08\x8B\x56\x08\x53", "xx????xxxxxxxxx");
	XASSERT(xynf7);
	Offsets::fullbright = GET_INT(xynf7 + 2);

	DWORD xynf8 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA1\x00\x00\x00\x00\x83\xF8\x03\x77\x0D\xFF\x24\x85\x00\x00\x00\x00", "x????xxxxxxxx????");
	XASSERT(xynf8);
	Offsets::screeematrix = xynf8;

	DWORD xynf9 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x0C\x8B\x44\x24\x18\xD9\x00\xD8\x25\x00\x00\x00\x00", "xxxxxxxxxxx????");
	XASSERT(xynf9);
	Offsets::worldtoscreen = xynf9;

	DWORD xynf10 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x50\xD9\x44\x24\x68\x56\xDC\x0D\x00\x00\x00\x00", "xxxxxxxxxx????");
	XASSERT(xynf10);
	Offsets::drawrotatedpic = xynf10;

	DWORD xynf11 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\x8D\x4C\x24\x18\x51\xD8\x44\x24\x18", "x????xxxxxxxxx");
	XASSERT(xynf11);
	Offsets::bounding = GET_INT(xynf11 + 1);

	DWORD xynf12 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\xD9\x47\x04\x0F\xB6\x46\x11\x51\x0F\xB6\x4E\x10", "xx????xxxxxxxxxxxx");
	XASSERT(xynf12);
	Offsets::hud_dpad_arrow = GET_INT(xynf12 + 2);

	DWORD xynf13 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x04\x56\x8B\x74\x24\x10\x57\x8B\x7C\x24\x10", "xxxxxxxxxxxxxx");
	XASSERT(xynf13);
	Offsets::weaponimpacttype = xynf13;
	
	DWORD xynf14 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x15\x00\x00\x00\x00\x52\x53\x57\xE8\x00\x00\x00\x00", "xx????xxxx????");
	XASSERT(xynf14);
	Offsets::newseed = GET_INT(xynf14 + 2);

	DWORD xynf15 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xC6\x00\x00\x00\x00\x56\xC7\x44\x24\x00\x00\x00\x00\x00", "xx????xxxx?????");
	XASSERT(xynf15);
	Offsets::serverset = GET_INT(xynf15 + 2);

	DWORD xynf16 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x51\x53\x55\x8B\x6C\x24\x14\x57\x8D\x58\x2C", "xxxxxxxxxxx");
	XASSERT(xynf16);
	Offsets::m_addr_TraceBullet = xynf16;

	DWORD xynf17 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x0C\x55\x8B\x6C\x24\x1C\x85\xED\x75\x07", "xxxxxxxxxxxx");
	XASSERT(xynf17);
	Offsets::m_addr_GetPenetrationPower = xynf17;

	DWORD xynf18 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x55\x8B\xEC\x53\x56\x57\x8B\x5D\x0C\x8B\x4D\x10\x81\xF9\x00\x00\x00\x00", "xxxxxxxxxxxxxx????");
	XASSERT(xynf18);
	Offsets::m_addr_CopyTrace = xynf18;

	DWORD xynf19 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x56\x8B\x74\x24\x0C\x56\xE8\x00\x00\x00\x00\x8B\x4C\x24\x0C\x0F\xB7\xC0", "xxxxxxx????xxxxxxx");
	XASSERT(xynf19);
	Offsets::m_addr_StepForward = xynf19;

	DWORD xynf20 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xD9\x1D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x8B\xF0", "xx????x????xx????xx");
	XASSERT(xynf20);
	Offsets::punch = GET_INT(xynf20 + 2);


	DWORD xb1 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x35\x00\x00\x00\x00\xD9\x46\x10\xE8\x00\x00\x00\x00\xD9\x46\x0C", "xx????xxxx????xxx");
	XASSERT(xb1);
	Offsets::HUDSAYPOSITION_DVAR = GET_INT(xb1 + 2);
	Offsets::HUDSAYPOSITION_EXCEPTION = xb1 + 6;


	DWORD xb2 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x2B\x41\x0C\x57", "xx????xxxx");
	XASSERT(xb2);
	Offsets::PACKETDUPLICATION_DVAR = GET_INT(xb2 + 2);
	Offsets::PACKETDUPLICATION_EXCEPTION = xb2 + 6;
	
	/*if (!isTekno)
	{	
		
	}
	else
	{		
		Offsets::ping = 0x10636D8;
	}*/
		
	DWORD pg = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA3\x00\x00\x00\x00\x8B\x5C\x24\x00\x8B\x6C\x24\x00\x8B\x0D\x00\x00\x00\x00\x23\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "x????xxx?xxx?xx????xx????x????");
	XASSERT(pg);
	Offsets::ping = GET_INT(pg + 1);

	DWORD bpc = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x5C\x57\x8B\xF8\xA1\x00\x00\x00\x00\x80\x78\x0C\x00\x75\x07", "xxxxxxx????xxxxxx");
	XASSERT(bpc);
	Offsets::bulletpenetrationcheck = bpc;

	DWORD bp = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x55\x8B\xAC\x24\x00\x00\x00\x00\x56\x57\x83\xFB\xFF", "xx????xxxx????xxxx????xxxxx");
	XASSERT(bp);
	Offsets::bulletpenetration = bp;

	DWORD bt = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8B\xF3\x69\xFF\x00\x00\x00\x00\x69\xF6\x00\x00\x00\x00\x55", "xx????xxxx????xx????x");
	XASSERT(bt);
	Offsets::bullettrail = GET_INT(bt + 2);

	DWORD ms = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x80\x3D\x00\x00\x00\x00\x00\x74\x25\x56", "xx?????xxx");
	XASSERT(ms);
	Offsets::mouse = GET_INT(ms + 2);

	DWORD nsn = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xD9\x1D\x00\x00\x00\x00\x8B\x5C\x24\x00\x55", "xx????xxx?x");
	XASSERT(nsn);
	Offsets::nospread_numerator = GET_INT(nsn + 2);

	DWORD kc = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x53\x75\x30", "xx?????xxx");
	XASSERT(kc);
	Offsets::killcam = GET_INT(kc + 2);

	DWORD ws = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xEC\x00\x00\x00\x00\x83\xBC\x24\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x53", "xx????xxx?????xx????x");
	XASSERT(ws);
	Offsets::weaponsound = ws;

	DWORD ssc = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x00\x8B\x4C\x24\x00\x83\xF8\x00\x75\x16", "xxx?xxx?xx?xx");
	XASSERT(ssc);
	Offsets::svgamesndconcmd = ssc;

	DWORD ht = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8B\xE8", "x????x????x????xx");
	XASSERT(ht);
	Offsets::host = GET_INT(ht + 1);

	DWORD sh = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x89\x90\x00\x00\x00\x00\x8B\x81\x00\x00\x00\x00\x89\x81\x00\x00\x00\x00\xC3", "xx????xx????xx????x");
	XASSERT(sh);
	Offsets::server_health = GET_INT(sh + 2);


	if (!isTekno)	
	{
		DWORD cb = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size,
			(PBYTE)"\x6A\x00\xE8\x00\x00\x00\x00\x8B\x44\x24\x00\x8A\x08\x83\xC4\x00\x80\xF9\x00\x74\x05\x80\xF9\x00\x75\x23\x8A\x48\x00\x80\xF9\x00\x7C\x1B\x80\xF9\x00\x7D\x16\x0F\xBE\xC9\x83\xC0\x00\x83\xE9\x00\x80\x38\x00\x75\x0C\x40\x80\x38\x00\x74\xFA\xEB\x04\x8B\x4C\x24\x00\x55\x33\xD2\x8D\x2C\x49\x56\x8D\x2C\xAD\x00\x00\x00\x00\x38\x10\x74\x07\x42\x80\x3C\x02\x00\x75\xF9\x8B\x75\x00\x8D\x0C\x16\x3B\x4D\x00\x7C\x0D\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x5E\x5D\xC3\x8B\x4D\x00\x57\x8D\x7A\x00\x03\xCE\x8B\xF0\x85\xFF\x74\x0B\x8A\x06\x88\x01\x41\x46\x83\xEF\x00\x75\xF5\x01\x55\x00\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x5F\x5E\x5D\xC3",
			"x?x????xxx?xxxx?xx?xxxx?xxxx?xx?xxxx?xxxxxxx?xx?xx?xxxxx?xxxxxxx?xxxxxxxxxx????xxxxxxxx?xxxx?xxxxx?xxx?x????xx?xxxxx?xxx?xxxxxxxxxxxxxxxx?xxxx?x?x????xx?xxxx");
		XASSERT(cb);
		Offsets::cbufaddtext = cb;
	}
	else
	{
		/*

		They're hooking this function...
		004C1030  -E9 CBEF71FF      JMP FFBE0000
		004C1035   0100             ADD DWORD PTR DS:[EAX],EAX
		004C1037   8B4424 0C        MOV EAX,DWORD PTR SS:[ESP+0xC]
		004C103B   8A08             MOV CL,BYTE PTR DS:[EAX]
		004C103D   83C4 04          ADD ESP,0x4
		004C1040   80F9 70          CMP CL,0x70
		004C1043   74 05            JE SHORT iw5mp.004C104A	

		...

		FFBE0000   FF3424           PUSH DWORD PTR SS:[ESP]
		FFBE0003   C74424 04 0000BF>MOV DWORD PTR SS:[ESP+0x4],0xFFBF0000
		FFBE000B   E8 B0D34210      CALL steam_ap.1000D3C0
		FFBE0010   C2 0400          RETN 0x4
		
		...

		1000D3C0   55               PUSH EBP
		1000D3C1   8BEC             MOV EBP,ESP
		1000D3C3   81EC 58020000    SUB ESP,0x258
		1000D3C9   A1 8C560410      MOV EAX,DWORD PTR DS:[0x1004568C]
		1000D3CE   33C5             XOR EAX,EBP
		1000D3D0   8945 FC          MOV DWORD PTR SS:[EBP-0x4],EAX
		1000D3D3   6A 04            PUSH 0x4
		1000D3D5   68 E4E10310      PUSH steam_ap.1003E1E4                   ; ASCII "cmd "
		1000D3DA   8B45 14          MOV EAX,DWORD PTR SS:[EBP+0x14]
		1000D3DD   50               PUSH EAX
		1000D3DE   E8 8DD40000      CALL steam_ap.1001A870
		1000D3E3   83C4 0C          ADD ESP,0xC
		1000D3E6   85C0             TEST EAX,EAX
		1000D3E8   75 77            JNZ SHORT steam_ap.1000D461
		1000D3EA   0FBE0D 76A50410  MOVSX ECX,BYTE PTR DS:[0x1004A576]
		1000D3F1   83F9 4D          CMP ECX,0x4D
		1000D3F4   75 6B            JNZ SHORT steam_ap.1000D461
		1000D3F6   BA 01000000      MOV EDX,0x1
		1000D3FB   B9 ECE10310      MOV ECX,steam_ap.1003E1EC                ; ASCII "cg_scoreboardpingtext"
		1000D400   E8 9B6AFFFF      CALL steam_ap.10003EA0
		1000D405   33D2             XOR EDX,EDX
		1000D407   B9 04E20310      MOV ECX,steam_ap.1003E204                ; ASCII "cg_scoreboardpinggraph"
		1000D40C   E8 8F6AFFFF      CALL steam_ap.10003EA0
		1000D411   C605 A9AC4400 01 MOV BYTE PTR DS:[0x44ACA9],0x1
		1000D418   BA 64000000      MOV EDX,0x64
		1000D41D   B9 28E00310      MOV ECX,steam_ap.1003E028                ; ASCII "cl_maxpackets"
		1000D422   E8 796AFFFF      CALL steam_ap.10003EA0
		1000D427   BA 01000000      MOV EDX,0x1
		1000D42C   B9 38E00310      MOV ECX,steam_ap.1003E038                ; ASCII "cl_packetdup"

		*/

		Offsets::cbufaddtext = 0x004C1030;
	}
	

	DWORD pd = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x56\x8B\x74\x24\x00\x8B\x86\x00\x00\x00\x00\x8B\x88\x00\x00\x00\x00\x51", "xxxx?xx????xx????x");
	XASSERT(pd);
	Offsets::playerdie = pd;


	DWORD ki = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xC6\x00\x00\x00\x00\xF7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00", "xx????xx????????xx????");
	XASSERT(ki);
	Offsets::key_input = GET_INT(ki + 2);


	DWORD sht = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x04\x8B\x40\x04\xC3", "xxxxxxxx");
	XASSERT(sht);
	Offsets::strHeight = sht;

	DWORD pas = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\x3D\x00\x00\x00\x00\x00\x74\x05\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x3B\x05\x00\x00\x00\x00\x75\x0D\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x8B\x4C\x24\x00\xFF\x05\x00\x00\x00\x00\x56\x8B\x35\x00\x00\x00\x00\x83\xC6\x00\x6A\x00\x89\x35\x00\x00\x00\x00\x51\xC7\x46\x04\x02\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x89\x06\x5E\xC3", "xx?????xxx????x????xx????xxx????x????xx?xxx?xx????xxx????xx?x?xx????xxxxx???x????xx?xxxx");
	XASSERT(pas);
	Offsets::pAddString = pas;

	DWORD kent = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8A\x15\x00\x00\x00\x00\x53\x8B\x5C\x24\x00\x55\x56", "xx????xxxx?xx");
	XASSERT(kent);
	Offsets::keyEvent = kent;	

	DWORD psd = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x00\x8B\x4C\x24\x00\x50\x51\x8D\x54\x24\x00\x52\x8D\x44\x24\x00\x50\x8B\x44\x24\x00\x8D\x4C\x24\x00\x51\x8D\x54\x24\x00\x52\x50\xE8\x00\x00\x00\x00\xD9\x44\x24\x00\x8B\x4C\x24\x00\x8B\x54\x24\x00\x83\xC4\x00", "xxx?xxx?xxxxx?xxxx?xxxx?xxx?xxxx?xxx????xxx?xxx?xxx?xx?");
	XASSERT(psd);
	Offsets::drawstretchpic = psd;

	DWORD ymv = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x55\x8B\xEC\x83\xE4\x00\x8B\x45\x00\x8B\x0D\x00\x00\x00\x00\x83\xEC\x00\x53", "xxxxx?xx?xx????xx?x");
	XASSERT(ymv);
	Offsets::vmNotify = ymv;


	DWORD evd = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xA1\x00\x00\x00\x00\x8B\x08\x56\x8B\x74\x24\x0C", "x????xxxxxxx");
	XASSERT(evd);
	Offsets::d3d_device = GET_INT(evd + 1);


	DWORD ampr = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x44\x53\x56\x57\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00", "xxxxxxx????x????");
	XASSERT(ampr);
	Offsets::rMapRestart = ampr;

	//========================================================================

	DWORD art_1, art_2, art_3, art_4, art_5, art_6, art_7, art_8, art_9, art_10, art_11, art_12, art_13;

	art_1 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x43\x83\xC4\x18\x3B\x5C\x24\x10\x72\x93", "xxxxxxxxxx");
	XASSERT(art_1);
	Offsets::d3d_ra_1 = art_1;

	art_2 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xC4\x18\x83\xEF\x01\x0F\x85\x00\x00\x00\x00", "xxxxxxxx????");
	XASSERT(art_2);
	Offsets::d3d_ra_2 = art_2;

	art_3 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x43\x83\xC4\x18\x3B\x5C\x24\x14\x72\x93", "xxxxxxxxxx");
	XASSERT(art_3);
	Offsets::d3d_ra_3 = art_3;

	art_4 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x43\x83\xC4\x10\x3B\xDD\x72\xBC\x5F\x5E\x5D\x5B\x83\xC4\x14\xC3\x83\xEC\x14", "xxxxxxxxxxxxxxxxxxx");
	XASSERT(art_4);
	Offsets::d3d_ra_4 = art_4;

	art_5 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x43\x83\xC4\x10\x3B\xDD\x72\xBC\x5F\x5E\x5D\x5B\x83\xC4\x14\xC3\x83\xEC\x10", "xxxxxxxxxxxxxxxxxxx");
	XASSERT(art_5);
	Offsets::d3d_ra_5 = art_5;	

	art_6 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xC2\x00\x00\x00\x00\x52\xE8\x00\x00\x00\x00\x83\xC4\x00\x5F\x5E\x5D\x5B\x83\xC4\x00\xC3", "xx????xx????xx?xxxxxx?x");
	XASSERT(art_6);
	Offsets::d3d_ra_6 = art_6 + 0xC;

	art_7 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xC4\x1C\x33\xC0\x89\x44\x24\x2C\x89\x44\x24\x20", "xxxxxxxxxxxxx");
	XASSERT(art_7);
	Offsets::d3d_ra_7 = art_7;

	art_8 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xC4\x1C\x5F\x5E\x5D\x5B\x83\xC4\x18\xC3", "xxxxxxxxxxx");
	XASSERT(art_8);
	Offsets::d3d_ra_8 = art_8;

	art_9 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x0F\xB7\x4F\x06\x46\x83\xC4\x40\x3B\xF1", "xxxxxxxxxx");
	XASSERT(art_9);
	Offsets::d3d_ra_9 = art_9;	

	art_10 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x54\x24\x00\x8D\x4C\x24\x00\x51\x81\xC2\x00\x00\x00\x00\x52\xE8\x00\x00\x00\x00\x83\xC4\x00\xC3", "xxx?xxx?xxx????xx????xx?x");
	XASSERT(art_10);
	Offsets::d3d_ra_10 = art_10 + 0x15;

	art_11 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xC4\x1C\x83\xC6\x20\x83\xEF\x01\x0F\x85\x00\x00\x00\x00\x8B\x8C\x24\x00\x00\x00\x00", "xxxxxxxxxxx????xxx????");
	XASSERT(art_11);
	Offsets::d3d_ra_11 = art_11;

	art_12 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x54\x83\x00\x18\x8B\x30", "xxxxxxxxx");
	XASSERT(art_12);
	Offsets::d3d_ra_12 = art_12;

	art_13= Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xC4\x14\x5F\x5E\x5D\x5B\x83\xC4\x18\xC3\xCC\xCC\xCC\xCC\xCC", "xxxxxxxxxxxxxxxx");
	XASSERT(art_13);
	Offsets::d3d_ra_13 = art_13;

	//========================================================================



	//first of 2.
	DWORD xex1 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x44\x24\x00\x0F\xBF\x04\x45\x00\x00\x00\x00\x85\xC0\x74\x0C\x69\xC0\x00\x00\x00\x00\x05\x00\x00\x00\x00\xC3\x33\xC0\xC3", "xxx?xxxx????xxxxxx????x????xxxx");
	XASSERT(xex1);
	Offsets::gtag_1 = xex1;
	
	DWORD xex2 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x51\x8B\x4C\x24\x00\x56\x57\x8B\x7C\x24\x00\x8D\x44\x24\x00", "xxxx?xxxxx?xxx?");
	XASSERT(xex2);
	Offsets::gtag_2 = xex2;

	//========================================================================

	DWORD xex3 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x8B\x2D\x00\x00\x00\x00\x85\xED\x74\x47", "xx????xxxx");
	XASSERT(xex3);
	Offsets::laser_1 = GET_INT(xex3 + 2);

	DWORD xex4 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x81\xFD\x00\x00\x00\x00\x0F\x8C\x00\x00\x00\x00\xD9\xEE", "xx????xx????xx");
	XASSERT(xex4);
	Offsets::laser_2 = GET_INT(xex4 + 2);

	DWORD xex5 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xBE\x00\x00\x00\x00\x83\x3E\x00\x74\x23", "x????xxxxx");
	XASSERT(xex5);
	Offsets::laser_3 = GET_INT(xex5 + 1);

	DWORD xex6 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x68\x00\x00\x00\x00\x51\xD9\x1C\x24\x74\x1C", "x????xxxxxx");
	XASSERT(xex6);
	Offsets::laser_4 = GET_INT(xex6 + 1);
	

	DWORD xex7 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x83\xEC\x00\x0F\xB7\x0D\x00\x00\x00\x00\x53\x55\x8B\x6C\x24\x00\x56", "xx?xxx????xxxxx?x");
	XASSERT(xex7);
	Offsets::laser_5 = xex7;

	//========================================================================

	DWORD xex8 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\x56\x53\x8B\xF0\xE8\x00\x00\x00\x00\x83\xC4\x04\x8D\x50\x01", "xxxxx????xxxxxx");
	XASSERT(xex8);
	Offsets::addicontotext = xex8;

	//========================================================================

	DWORD xex9 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xD9\x05\x00\x00\x00\x00\xDD\xE9\xDF\xE0\xF6\xC4\x00\x0F\x8A\x00\x00\x00\x00\x8B\x44\x24\x00\xDD\xD8\x85\xC0", "xx????xxxxxx?xx????xxx?xxxx");
	XASSERT(xex9);
	Offsets::jumpheight = GET_INT(xex9 + 2);

	DWORD xex10 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xD9\x05\x00\x00\x00\x00\xD9\x5C\x24\x00\xD9\x44\x24\x00\xD9\x1C\x24\xE8\x00\x00\x00\x00\xD9\x5C\x24\x00\xB9\x00\x00\x00\x00\xD9\x44\x24\x00\xD9\x54\x24\x00\xDC\x0D\x00\x00\x00\x00\x8B\x44\x24\x00\xD1\xF8", "xx????xxx?xxx?xxxx????xxx?x????xxx?xxx?xx????xxx?xx");
	XASSERT(xex10);
	Offsets::jumpladder = GET_INT(xex10 + 2);

	//========================================================================
		
	DWORD zex1, zex2, /*zex3,*/ zex4, zex5, zex6, zex7, zex8, zex9/*, zex10, zex11, zex12, zex13, zex14, zex15, zex16, zex17, zex18, zex19, zex20*/;
			 
	zex1 = Hook.FindPattern(reinterpret_cast<DWORD>(iw5mp_module), iw5mp_size, (PBYTE)"\xD9\x05\x00\x00\x00\x00\xD9\x1C\x24\xD9\xEE\xD9\x05\x00\x00\x00\x00\xDD\xE1\xDF\xE0\xDD\xD9", "xx????xxxxxxx????xxxxxx");
	XASSERT(zex1);
	Offsets::fb1 = GET_INT(zex1 + 2);

	zex2 = GET_INT(zex1 + 0xD);
	XASSERT(zex2);
	Offsets::fb2 = zex2;

	//more than 1 pattern:
	Offsets::fb3 = !isTekno ? 0x1065BE0 : 0x10637F4;

	zex4 = GET_INT(zex1 + 0x3A);
	XASSERT(zex4);
	Offsets::fb4 = zex4;

	zex5 = GET_INT(zex1 + 0x40);
	XASSERT(zex5);
	Offsets::fb5 = zex5;

	zex6 = GET_INT(zex1 + 0x50);
	XASSERT(zex6);
	Offsets::fb6 = zex6;

	zex7 = GET_INT(zex1 + 0x56);
	XASSERT(zex7);
	Offsets::fb7 = zex7;

	zex8 = GET_INT(zex1 + 0x60);
	XASSERT(zex8);
	Offsets::fb8 = zex8;

	zex9 = GET_INT(zex1 + 0x66);
	XASSERT(zex9);
	Offsets::fb9 = zex9;

	
	Offsets::fb10 = !isTekno ? 0x1065BEC : 0x1063800;

	Offsets::fb11 = !isTekno ? 0x1065BF0 : 0x1063804;

	Offsets::fb12 = !isTekno ? 0x1065BF4 : 0x1063808;

	Offsets::fb13 = !isTekno ? 0x1065BD4 : 0x10637E8;

	Offsets::fb14 = !isTekno ? 0x977668 : 0x975738;

	Offsets::fb15 = !isTekno ? 0x1065BD8 : 0x10637EC;

	Offsets::fb16 = !isTekno ? 0x977D6C: 0x975E3C;

	Offsets::fb17 = !isTekno ? 0x1065BDC : 0x10637F0;

	Offsets::fb18 = !isTekno ? 0x97766C : 0x97573C;

	Offsets::fb19 = !isTekno ? 0x1065C7C : 0x1063890;

	Offsets::fb20 = !isTekno ? 0xA058B8 : 0xA03940;

	//========================================================================
	

}

void GetPointers()
{
	punch = (Punch_t *)Offsets::punch;
	concGrenade = (stunflash_t*)Offsets::stunflash_congrenade;
	flashGrenade = (stunflash_t*)Offsets::stunflash_flashgrenade;
	cg_entities = (centity_t *)Offsets::entity;
	
	cg = (cg_t*)Offsets::cgt;
	cgs = (cgs_t *)Offsets::cgs;
	refdef = (refdef_t*)Offsets::refdef;
	pViewMatrix = (viewmatrix_t *)Offsets::viewmatrix;	
	Fonts = (Fonts_t *)Offsets::fonts_t;
	fonts_Dev = (Fonts_Dev_t *)Offsets::fonts_dev_t;
	ServerID = (int **)Offsets::serverid;
	pinput = (input_t*)Offsets::input;	
	pStats = (stats_t*)Offsets::stats;	
	 
	ServerSet = (ServerSet_t *)Offsets::serverset;
	ServerEntityInfo = (entityInfo_s *)Offsets::server_entityinfo;
	ServerHealth = (Health_s *)Offsets::server_health;

	key_input = (KInput_t *)Offsets::key_input;

	//========================================================================

	Trace = (G_TraceCapsule_t)Offsets::cgtrace;
	VectorNormalize = (VectorNormalize_t)Offsets::normalize;
	R_WeaponSpread = (BG_GetSpreadForWeapon_t)Offsets::weaponspread;
	
	SV_GameSendServerCommand = (SV_GameSendServerCommand_t)Offsets::svgamesndconcmd;
	Cbuf_AddText = (Cbuf_AddText_t)Offsets::cbufaddtext;
	player_die = (player_die_t)Offsets::playerdie;

	GSM = (ScrPlace_GetActivePlacement_t)Offsets::screeematrix;	
	WorldToScreenE = (CG_WorldPosToScreenPosReal_t)Offsets::worldtoscreen;
	DrawRotatedPic = (CG_DrawRotatedPicPhysical_t)Offsets::drawrotatedpic;
	RegisterShader = (Material_RegisterHandle_t)Offsets::shader;
	DrawGlowText = (CL_DrawTextPhysicalWithEffects_t)Offsets::text;
	StrWidth = (R_TextWidth_t)Offsets::strWidth;
	StrHeight = (R_TextHeight_t)Offsets::strHeight;
	

	//Autowall special functions...
	pWeaponImpacttype = (BG_WeaponBulletFire_GetMethodOfDeath_t)Offsets::weaponimpacttype;
	pfnGetBulletPenetration = (BG_GetSurfacePenetrationDepth_t)(Offsets::m_addr_GetPenetrationPower);
	pfnCopyTrace = (Com_Memcpy_t)(Offsets::m_addr_CopyTrace);
	pfnStepForward = (BG_AdvanceTrace_t)(Offsets::m_addr_StepForward);	

	psub_4F0CF0 = (tsub_4F0CF0)Offsets::awallcheck_1;
	psub_489F70 = (tsub_4F0CF0)Offsets::awallcheck_2;
	psub_52A190 = (tsub_52A190)Offsets::awallcheck_3;
	psub_46B370 = (tsub_46B370)Offsets::awallcheck_4;

	pScr_AddString = (Scr_AddString_t)Offsets::pAddString;	

	CL_KeyEvent = (CL_KeyEvent_t)Offsets::keyEvent;

	CL_DrawStretchPic = (t_CL_DrawStretchPic)Offsets::drawstretchpic;

	VM_Notify = (VM_Notify_t)Offsets::vmNotify;	
	
}


/************************************************************************/
/* Threads                                                              */
/************************************************************************/

HANDLE Handle_GetAKSThread = nullptr; //GetAsyncKeyStateThread
HANDLE Handle_ForceHostThread = nullptr;
HANDLE SetNullPtrThreadHandle = nullptr;

void GAKSThread()
{
	while (true)
	{
		Engine.SwitchTeamProcess();

		SafeSleep(100);
	}
}

void ForceHostThread()
{
	while (!isTekno)
	{
		if (Settings[host_forcehost].Value.bValue
			&& !*(int*)Offsets::host)
		{	
			switch (Settings[host_map].Value.iValue)
			{
			case 1:
				Engine.SendToConsole("ui_mapname mp_plaza2\n");
				break;

			case 2:
				Engine.SendToConsole("ui_mapname mp_mogadishu\n");
				break;

			case 3:
				Engine.SendToConsole("ui_mapname mp_bootleg\n");
				break;

			case 4:

				Engine.SendToConsole("ui_mapname mp_carbon\n");
				break;

			case 5:
				Engine.SendToConsole("ui_mapname mp_dome\n");
				break;

			case 6:
				Engine.SendToConsole("ui_mapname mp_exchange\n");
				break;

			case 7:
				Engine.SendToConsole("ui_mapname mp_lambeth\n");
				break;

			case 8:
				Engine.SendToConsole("ui_mapname mp_hardhat\n");
				break;

			case 9:
				Engine.SendToConsole("ui_mapname mp_interchange\n");
				break;

			case 10:
				Engine.SendToConsole("ui_mapname mp_alpha\n");
				break;

			case 11:
				Engine.SendToConsole("ui_mapname mp_bravo\n");
				break;

			case 12:
				Engine.SendToConsole("ui_mapname mp_radar\n");
				break;

			case 13:
				Engine.SendToConsole("ui_mapname mp_paris\n");
				break;

			case 14:
				Engine.SendToConsole("ui_mapname mp_seatown\n");
				break;

			case 15:
				Engine.SendToConsole("ui_mapname mp_underground\n");
				break;

			case 16:
				Engine.SendToConsole("ui_mapname mp_village\n");
				break;

			case 17:
				Engine.SendToConsole("ui_mapname mp_morningwood\n");
				break;

			case 18:
				Engine.SendToConsole("ui_mapname mp_park\n");
				break;

			case 19:
				Engine.SendToConsole("ui_mapname mp_qadeem\n");
				break;

			case 20:
				Engine.SendToConsole("ui_mapname mp_overwatch\n");
				break;

			case 21:
				Engine.SendToConsole("ui_mapname mp_italy\n");
				break;

			case 22:
				Engine.SendToConsole("ui_mapname mp_courtyard_ss\n");
				break;

			case 23:
				Engine.SendToConsole("ui_mapname mp_aground_ss\n");
				break;

			case 24:
				Engine.SendToConsole("ui_mapname mp_terminal_cls\n");
				break;
			}

			SafeSleep(100);

			switch (Settings[host_gametype].Value.iValue)
			{
			case 1:
				Engine.SendToConsole("ui_gametype dm\n");
				break;

			case 2:
				Engine.SendToConsole("ui_gametype sd\n");
				break;

			case 3:
				Engine.SendToConsole("ui_gametype war\n");
				break;

			case 4:
				Engine.SendToConsole("ui_gametype ctf\n");
				break;

			case 5:
				Engine.SendToConsole("ui_gametype dd\n");
				break;

			case 6:
				Engine.SendToConsole("ui_gametype dom\n");
				break;

			case 7:
				Engine.SendToConsole("ui_gametype koth\n");
				break;

			case 8:
				Engine.SendToConsole("ui_gametype sab\n");
				break;

			case 9:
				Engine.SendToConsole("ui_gametype grnd\n");
				break;

			case 10:
				Engine.SendToConsole("ui_gametype infect\n");
				break;
			}

			SafeSleep(100);

			if (Settings[host_killcam].Value.bValue)
				Engine.SendToConsole("scr_game_allowkillcam 1\n");
			else
				Engine.SendToConsole("scr_game_allowkillcam 0\n");		

			SafeSleep(100);

			Engine.SendToConsole(

				(char *)
				STREAM("party_gamesize " << Settings[host_partysize].Value.iValue << "\n").c_str()
			);

			SafeSleep(100);

			Engine.SendToConsole("party_minplayers 1\n");
		}

		SafeSleep(1000);
	}
}

void SetNullPtrThread()
{
	while (2019)
	{
		*(DWORD *)Offsets::PACKETDUPLICATION_DVAR = 0; //"cl_packetdup"	
		*(DWORD *)Offsets::HUDSAYPOSITION_DVAR = 0; //"cg_hudSayPosition"			

		SafeSleep(5017);
	}
}


/************************************************************************/
/* Hooks                                                                */
/************************************************************************/

CVMTHookManager* pD3DHook = nullptr;

DWORD _eax = 0;
DWORD dwEnd = 0x67EC7F;
DWORD dwNext = 0x67EB4D;
char *pData = nullptr;
void FixString(char *pChar)
{
	if (!strstr(pChar, "killiconheadshot"))
	{
		for (int i = 0; i < strlen(pChar); i++)
			if ((int)pChar[i] < 32)
				pChar[i] = 32;
	}	
	//else: will print the headshot icon...
}
__declspec(naked) void h_client_crasher_fix()
{
	__asm pushad	
	__asm mov _eax, eax

	pData = reinterpret_cast<char*>(_eax);
	if (pData == nullptr)
	{
		__asm popad
		__asm jmp[dwEnd];
	}
	else
	{
		FixString(pData);

		__asm popad
		__asm jmp[dwNext];
	}
}

DWORD _ecx = 0;
DWORD dwContinue = 0x5022E8;
char *pFixChar = nullptr;
void FixEndRound(char *pChar)
{	
	if (strstr(pChar, "endround") && 
		*(int*)Offsets::host && 
		Settings[host_antiendgame].Value.bValue)
		strcpy(pChar, "lemonade");
	

	pScr_AddString(pChar);
}
__declspec(naked) void h_endround_fix()
{	
	__asm pushad
	__asm mov _ecx, ecx

	pFixChar = reinterpret_cast<char*>(_ecx);
	FixEndRound(pFixChar);	

	__asm popad
	__asm jmp[dwContinue];	
}

char __cdecl CL_KeyEvent_Hook(int a1, int a2, int a3)
{
	__asm pushad	
	char retVal;	

	if (D3D::g_bMainWindowOpen)
	{		
		retVal = 1;
	}		
	else
	{
		retVal = CL_KeyEvent(a1, a2, a3);				
	}	

	_asm popad;

	return retVal;

}

void CL_DrawStretchPic_Hook(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, /*const*/ float *color, void *material) {
		
	if (!D3D::g_bWasInitialized)
		CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign, s1, t1, s2, t2, color, material);
	else
	{
		char *Shader = *reinterpret_cast<char**>(material);
		if (Shader)
		{
			//printf("Shader: %s\n", Shader);

			if (Settings[style_rainbow].Value.bValue)
			{
				if (!strcmp(Shader, "mw2_main_cloud_overlay")
					|| strstr(Shader, "navbar")
					|| !strcmp(Shader, "background_image")
					)
					color = reinterpret_cast<float*>(&ImVec4(D3D::colRainBow.x, D3D::colRainBow.y, D3D::colRainBow.z, color[3]));

			}
		}

		CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign, s1, t1, s2, t2, color, material);
	}	
}

int __cdecl VM_Notify_Hook(int a1, unsigned int a2, unsigned int *a3) {
		
	Host::VM_Notify(a1, a2, a3);

	return VM_Notify(a1, a2, a3);
}

long __stdcall pVEH_Hook(_EXCEPTION_POINTERS *pInfo)
{	
	if (pInfo->ContextRecord->Eip == Offsets::PACKETDUPLICATION_EXCEPTION)
	{
		pInfo->ContextRecord->Ecx = /*0x59D6A20*/ 0x059DAA20; // check it live.
		WritePacket();
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (pInfo->ContextRecord->Eip == Offsets::HUDSAYPOSITION_EXCEPTION)
	{
		pInfo->ContextRecord->Esi = /*0x59D60EC*/ 0x059DA0EC;
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

	strcpy(m_szDllDirectory, "C:\\RC\\");
	std::string soundsFolder = m_szDllDirectory + std::string("Sounds\\");	
	std::string texturesFolder = m_szDllDirectory + std::string("Textures\\");
	
	CreateFolder(m_szDllDirectory);	
	CreateFolder(soundsFolder.c_str());
	CreateFolder(texturesFolder.c_str());

	strcpy_s(D3D::texturesFolder, texturesFolder.c_str());
	sounds.MapSoundDirectory(soundsFolder.c_str());

	Bot_InitSettings();
	Bot_LoadSettings();	

	// Level 2
	if (!isTekno)
	{
		MakeJMP((BYTE *)/*0x0067EAE7*/ 0x0067EB47, (DWORD)h_client_crasher_fix, 0x6);
		MakeJMP((BYTE *)/*0x005021F3*/ 0x005022E3, (DWORD)h_endround_fix, 0x5);
		HookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);
	}	
	
	HookModule(GetCurrentThread(), CL_KeyEvent, CL_KeyEvent_Hook); //Console Fix		
	HookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);//Background Effect	
	

	// Level 3
 	if (!D3D::InitAndHookD3D())	
	{
		SafeMessageBox(HWND_DESKTOP, "Error! at Level 3", ALASKA17, MB_ICONERROR);		

		if (!isTekno)		
			UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);
		UnHookModule(GetCurrentThread(), CL_KeyEvent, CL_KeyEvent_Hook);
		UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);					

		exit(-1);
	} 	
	// Level 4
 	else
 	{
		AddVectoredExceptionHandler(17, pVEH_Hook);
		SetNullPtrThreadHandle = SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(SetNullPtrThread), nullptr, 0, nullptr);
		if (SetNullPtrThreadHandle)			
		{ 
			Handle_GetAKSThread = SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)GAKSThread, 0, 0, nullptr);
			Handle_ForceHostThread = SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHostThread, 0, 0, nullptr);

			// Level 5
			if (Handle_GetAKSThread && Handle_ForceHostThread)
			{
				SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.playSound), sounds.activated, 0, nullptr);
			}
			else
			{
				SafeMessageBox(HWND_DESKTOP, "Error! at Level 5", ALASKA17, MB_ICONERROR);
				
				FinishThread(SetNullPtrThreadHandle);
				RemoveVectoredExceptionHandler(pVEH_Hook);

				if (!isTekno)
					UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);
				UnHookModule(GetCurrentThread(), CL_KeyEvent, CL_KeyEvent_Hook);
				UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);				

				D3D::Restore_WndProc();

				if (pD3DHook)
				{
					pD3DHook->UnHook();
					pD3DHook = nullptr;
				}

				exit(-1);
			}
		}
		else 
		{
			SafeMessageBox(HWND_DESKTOP, "Error! at Level 4", ALASKA17, MB_ICONERROR);

			RemoveVectoredExceptionHandler(pVEH_Hook);

			if (!isTekno)
				UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);
			UnHookModule(GetCurrentThread(), CL_KeyEvent, CL_KeyEvent_Hook);
			UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);

			D3D::Restore_WndProc();

			if (pD3DHook)
			{
				pD3DHook->UnHook();
				pD3DHook = nullptr;
			}

			exit(-1);			
		}	
 	}


	if (isTekno)
		Bypass();

}

void Hook_t::ExecCleaningThread()
{	
	FinishThread(SetNullPtrThreadHandle);
	*(DWORD *)Offsets::PACKETDUPLICATION_DVAR = 0x059DAA20; //"cl_packetdup"	
	*(DWORD *)Offsets::HUDSAYPOSITION_DVAR = 0x059DA0EC; //"cg_hudSayPosition"
	RemoveVectoredExceptionHandler(pVEH_Hook);	

	if (!isTekno)
		UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);
	UnHookModule(GetCurrentThread(), CL_KeyEvent, CL_KeyEvent_Hook);
	UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);

	D3D::Restore_WndProc();

	if (pD3DHook)
	{
		pD3DHook->UnHook();
		pD3DHook = nullptr;
	}
		
	FinishThread(Handle_GetAKSThread);
	FinishThread(Handle_ForceHostThread);
	
}










//========================================================================


/*

  SMBIOS information can be found at http://dmtf.org/sites/default/files/standards/documents/DSP0134_2.7.1.pdf
*/

using namespace std;

typedef vector<uint8> BufferType;

#define JMP_HOOK_SIZE 5
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

typedef LONG(WINAPI *pZwQuerySystemInformation)(
	DWORD SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);
pZwQuerySystemInformation ZwQuerySystemInformation;

#define SYSTEMINFO_CLASS_FIRM_TABLE 0x4C
#define SMBIOS_STRUCTURE_SEPARATOR_SIZE 2
#define SMBIOS_STRUCTURE_SEPARATOR "\0\0"
#define SMBIOS_TABLE_SIGNATURE 0x52534D42

#define XNADDR_LEN 0x1C
#define PLAYER_NAME_MAXLEN 0x0C
#define ADDRESS_XUID 0x05A7B1D8
#define ADDRESS_PLAYER_INFO 0x05CCB138
#define ADDRESS_PLAYER_STATS 0x01CDAFBC
#define ADDRESS_XNADDRESS_BUFFER 0x00464A58
#define ADDRESS_UNNAMEDPLAYER_NAME 0x007E5AC4

#define RAND_STR_MASK_LEN 62
static const char *RAND_STR_MASK = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

//SMBIOS Raw Data changer
namespace SMBIOS
{

	typedef struct _StructureHeader
	{
		uint8 Type;
		uint8 FormattedSize;
		uint16 Handle; //Unique handle for this structure for later recall
		uint8 Data[];
	}StructureHeader;

	enum StructureType
	{
		BIOS_INFO_TYPE = 0x00,
		SYSTEM_INFO_TYPE = 0x01,
		BASEBOARD_INFO_TYPE = 0x02,
		CHASSIS_INFO_TYPE = 0x03,
		PROCESSOR_INFO_TYPE = 0x04,
		CACHE_INFO_TYPE = 0x07,
		PORTS_INFO_TYPE = 0x08,
		SYSTEMSLOTS_INFO_TYPE = 0x09,
		ONBOARDDEVS_INFO_TYPE = 0x0A,
		OEMSTRING_INFO_TYPE = 0x0B,
		SYSTEMCONFIG_INFO_TYPE = 0x0C,
		BIOSLANG_INFO_TYPE = 0x0D,
		GROUPASSOCS_INFO_TYPE = 0x0E,
		SYSLOG_INFO_TYPE = 0x0F,
		PHYSMEM_INFO_TYPE = 0x10,
		MEMDEV_INFO_TYPE = 0x11,
		MEMERROR32_INFO_TYPE = 0x12,
		MEMARRAYMAPPED_INFO_TYPE = 0x13,
		MEMDEVMAPPED_INFO_TYPE = 0x14,
		BUILTINPTRDEV_INFO_TYPE = 0x15,
		BATTERY_INFO_TYPE = 0x16,
		SYSRESET_INFO_TYPE = 0x17,
		HARDSEC_INFO_TYPE = 0x18,
		SYSPOWER_INFO_TYPE = 0x19,
		VOLTPROBE_INFO_TYPE = 0x1A,
		COOLINGDEV_INFO_TYPE = 0x1B,
		TEMPPROBE_INFO_TYPE = 0x1C,
		ELECPROBE_INFO_TYPE = 0x1D,
		OOBRA_INFO_TYPE = 0x1E,
		SYSBOOT_INFO_TYPE = 0x20,
		MEMERROR64_INFO_TYPE = 0x21,
		MNGDEV_INFO_TYPE = 0x22,
		MNGDEVCOMP_INFO_TYPE = 0x23,
		MNGDEVTHRES_INFO_TYPE = 0x24,
		MEMCHAN_INFO_TYPE = 0x25,
		IPMIDEV_INFO_TYPE = 0x26,
		POWERSUPPLY_INFO_TYPE = 0x27,
		ADDITIONAL_INFO_TYPE = 0x28,
		ONBOARDDEVSEX_INFO_TYPE = 0x29,
		MNGCTRLHOSTIF_INFO_TYPE = 0x2A,
		INACTIVE_INFO_TYPE = 0x7E,
		EOF_INFO_TYPE = 0x7F,
	};

	class AlterInfo
	{
	public:
		AlterInfo(uint8 *_buffer, uint32 _size) : m_Buffer(_buffer), m_BufferSize(_size), m_BufferPtr(sizeof(uint32) + sizeof(uint32)) {} //Skip version and length

		void Process(void)
		{
			int32 currentStLen = 0;
			while ((currentStLen = GetStructureLen()) > (SMBIOS_STRUCTURE_SEPARATOR_SIZE + sizeof(uint32)))
			{
				StructureHeader* pHeader = ((StructureHeader *)&m_Buffer[m_BufferPtr]);
				char *pStringBuffer = ((char *)&m_Buffer[m_BufferPtr + pHeader->FormattedSize]);
				while (*pStringBuffer)
				{
					//Fill strings with rand32om chars within the mask
					while (*pStringBuffer)
					{
						*pStringBuffer = RAND_STR_MASK[mtRandom.randInt() % RAND_STR_MASK_LEN];
						++pStringBuffer;
					}
					++pStringBuffer;
				}

				switch (pHeader->Type)
				{
				case BIOS_INFO_TYPE:
					StCallback_BiosInfo(pHeader);
					break;
				case SYSTEM_INFO_TYPE:
					StCallback_SysInfo(pHeader);
					break;
				case BASEBOARD_INFO_TYPE:
					StCallback_MBInfo(pHeader);
					break;
				case CHASSIS_INFO_TYPE:
					StCallback_ChassisInfo(pHeader);
					break;
				case PROCESSOR_INFO_TYPE:
					StCallback_CpuInfo(pHeader);
					break;
				case CACHE_INFO_TYPE:
					StCallback_CacheInfo(pHeader);
					break;
				case PORTS_INFO_TYPE:
					StCallback_PortsInfo(pHeader);
					break;
				case SYSTEMSLOTS_INFO_TYPE:
					StCallback_SystemSlotsInfo(pHeader);
					break;
				case ONBOARDDEVS_INFO_TYPE:
					StCallback_OnBoardDevsInfo(pHeader);
					break;
				case OEMSTRING_INFO_TYPE:
					StCallback_OemStringsInfo(pHeader);
					break;
				case SYSTEMCONFIG_INFO_TYPE:
					StCallback_SysConfigInfo(pHeader);
					break;
				case BIOSLANG_INFO_TYPE:
					StCallback_BiosLangInfo(pHeader);
					break;
				case GROUPASSOCS_INFO_TYPE:
					StCallback_GroupAssocsInfo(pHeader);
					break;
				case SYSLOG_INFO_TYPE:
					StCallback_SysLogInfo(pHeader);
					break;
				case PHYSMEM_INFO_TYPE:
					StCallback_PhysMemInfo(pHeader);
					break;
				case MEMDEV_INFO_TYPE:
					StCallback_MemDevInfo(pHeader);
					break;
				case MEMERROR32_INFO_TYPE:
					StCallback_MemError32Info(pHeader);
					break;
				case MEMARRAYMAPPED_INFO_TYPE:
					StCallback_MemArrayMappedInfo(pHeader);
					break;
				case MEMDEVMAPPED_INFO_TYPE:
					StCallback_MemDevMappedInfo(pHeader);
					break;
				case BUILTINPTRDEV_INFO_TYPE:
					StCallback_BuiltInPtrDevInfo(pHeader);
					break;
				case BATTERY_INFO_TYPE:
					StCallback_BatteryInfo(pHeader);
					break;
				case SYSRESET_INFO_TYPE:
					StCallback_SysResetInfo(pHeader);
					break;
				case HARDSEC_INFO_TYPE:
					StCallback_HardwareSecurityInfo(pHeader);
					break;
				case SYSPOWER_INFO_TYPE:
					StCallback_SysPowerInfo(pHeader);
					break;
				case VOLTPROBE_INFO_TYPE:
					StCallback_VoltageProbeInfo(pHeader);
					break;
				case COOLINGDEV_INFO_TYPE:
					StCallback_CoolingDevInfo(pHeader);
					break;
				case TEMPPROBE_INFO_TYPE:
					StCallback_TempProbeInfo(pHeader);
					break;
				case ELECPROBE_INFO_TYPE:
					StCallback_ElectricalProbeInfo(pHeader);
					break;
				case OOBRA_INFO_TYPE:
					StCallback_OobRemoteAccessInfo(pHeader);
					break;
				case SYSBOOT_INFO_TYPE:
					StCallback_SysBootInfo(pHeader);
					break;
				case MEMERROR64_INFO_TYPE:
					StCallback_MemError64Info(pHeader);
					break;
				case MNGDEV_INFO_TYPE:
					StCallback_ManageDevInfo(pHeader);
					break;
				case MNGDEVCOMP_INFO_TYPE:
					StCallback_ManageDevCompInfo(pHeader);
					break;
				case MNGDEVTHRES_INFO_TYPE:
					StCallback_ManageDevThresholdInfo(pHeader);
					break;
				case MEMCHAN_INFO_TYPE:
					StCallback_MemChannelInfo(pHeader);
					break;
				case IPMIDEV_INFO_TYPE:
					StCallback_IpmiDevInfo(pHeader);
					break;
				case POWERSUPPLY_INFO_TYPE:
					StCallback_PowerSupplyInfo(pHeader);
					break;
				case ADDITIONAL_INFO_TYPE:
					StCallback_AdditionalInfo(pHeader);
					break;
				case ONBOARDDEVSEX_INFO_TYPE:
					StCallback_OnBoardDevExInfo(pHeader);
					break;
				case MNGCTRLHOSTIF_INFO_TYPE:
					StCallback_ManageControlHostInterfaceInfo(pHeader);
					break;
				}

				m_BufferPtr += currentStLen;
			}


		}

	private:
		int32 GetStructureLen(void)
		{
			uint16 Offset = m_BufferPtr;
			uint16 BufferLen = m_BufferSize;
			StructureHeader* pHeader = ((StructureHeader *)&m_Buffer[m_BufferPtr]);

			Offset += pHeader->FormattedSize;

			while (Offset < BufferLen)
				if (!memcmp(&m_Buffer[Offset], SMBIOS_STRUCTURE_SEPARATOR, SMBIOS_STRUCTURE_SEPARATOR_SIZE))
					return Offset - m_BufferPtr + SMBIOS_STRUCTURE_SEPARATOR_SIZE;
				else ++Offset;

			return -1;
		}

		//BIOS_INFO_TYPE:
		void StCallback_BiosInfo(StructureHeader *Header)
		{
			uint8 rTo = Header->FormattedSize - sizeof(uint32);
			*((uint16 *)(Header->Data + 0x02)) = mtRandom.randInt() & 0xFFFF;
			Header->Data[0x05] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0A)) = mtRandom.randInt();
			for (uint8 i = 0x0E; i < rTo; ++i)
				Header->Data[i] = mtRandom.randInt() & 0xFF;
		}

		//SYSTEM_INFO_TYPE:
		void StCallback_SysInfo(StructureHeader *Header)
		{
			if (Header->FormattedSize < 0x19)
				return;

			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x10)) = mtRandom.randInt();
			Header->Data[0x14] = mtRandom.randInt() & 0xFF;
		}

		//BASEBOARD_INFO_TYPE:
		void StCallback_MBInfo(StructureHeader *Header)
		{
			uint8 rTo = Header->FormattedSize - sizeof(uint32);
			Header->Data[0x05] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x07)) = mtRandom.randInt() & 0xFFFF;
			Header->Data[0x09] = mtRandom.randInt() & 0xFF;
			Header->Data[0x0A] = mtRandom.randInt() & 0xFF;
			for (uint8 i = 0x0B; i < rTo; ++i)
				Header->Data[i] = mtRandom.randInt() & 0xFF;
		}

		//CHASSIS_INFO_TYPE:
		void StCallback_ChassisInfo(StructureHeader *Header)
		{
			uint8 rTo = Header->FormattedSize - sizeof(uint8) - sizeof(uint32);
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x05)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x09)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0D)) = mtRandom.randInt();
			for (uint8 i = 0x11; i < rTo; ++i)
				Header->Data[i] = mtRandom.randInt() & 0xFF;
		}

		//PROCESSOR_INFO_TYPE:
		void StCallback_CpuInfo(StructureHeader *Header)
		{
			*((uint16 *)(Header->Data + 0x01)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			Header->Data[0x0D] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x0E)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x12)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x16)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x1A)) = mtRandom.randInt() & 0xFFFF;
			Header->Data[0x1F] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x20)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x22)) = mtRandom.randInt();
		}

		//CACHE_INFO_TYPE:
		void StCallback_CacheInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x01)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x05)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x09)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x0D)) = mtRandom.randInt() & 0xFFFF;
		}

		//PORTS_INFO_TYPE:
		void StCallback_PortsInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x03)) = mtRandom.randInt() & 0xFFFF;
		}

		//SYSTEMSLOTS_INFO_TYPE:
		void StCallback_SystemSlotsInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x01)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x05)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x09)) = mtRandom.randInt();
		}

		//ONBOARDDEVS_INFO_TYPE:
		void StCallback_OnBoardDevsInfo(StructureHeader *Header)
		{
			uint8 devCount = (Header->FormattedSize - sizeof(uint32)) / sizeof(uint16);
			for (uint8 i = 0; i < devCount; ++i)
				Header->Data[2 * i] = mtRandom.randInt() & 0xFF;
		}

		//OEMSTRING_INFO_TYPE:
		void StCallback_OemStringsInfo(StructureHeader *Header)
		{//Nothing to do here
		}

		//SYSTEMCONFIG_INFO_TYPE:
		void StCallback_SysConfigInfo(StructureHeader *Header)
		{//Nothing to do here
		}

		//BIOSLANG_INFO_TYPE:
		void StCallback_BiosLangInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0x01;
		}

		//GROUPASSOCS_INFO_TYPE:
		void StCallback_GroupAssocsInfo(StructureHeader *Header)
		{
			uint8 rTo = Header->FormattedSize - sizeof(uint32);
			for (uint8 i = 0x01; i < rTo; ++i)
				Header->Data[i] = mtRandom.randInt() & 0xFF;
		}

		//SYSLOG_INFO_TYPE:
		void StCallback_SysLogInfo(StructureHeader *Header)
		{
			uint8 rTo = Header->FormattedSize - sizeof(uint32);
			*((uint16 *)(Header->Data + 0x06)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			Header->Data[0x10] = mtRandom.randInt() & 0xFF;
			for (uint8 i = 0x13; i < rTo; ++i)
				Header->Data[i] = mtRandom.randInt() & 0xFF;
		}

		//PHYSMEM_INFO_TYPE:
		void StCallback_PhysMemInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x01)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x03)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x09)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x0B)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
		}

		//MEMDEV_INFO_TYPE:
		void StCallback_MemDevInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			Header->Data[0x0E] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
			Header->Data[0x17] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x18)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x1C)) = mtRandom.randInt() & 0xFFFF;
		}

		//MEMERROR32_INFO_TYPE:
		void StCallback_MemError32Info(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x01)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x03)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x07)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0B)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
		}

		//MEMARRAYMAPPED_INFO_TYPE:
		void StCallback_MemArrayMappedInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x00)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			Header->Data[0x0A] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x0B)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x13)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x17)) = mtRandom.randInt();
		}

		//MEMDEVMAPPED_INFO_TYPE:
		void StCallback_MemDevMappedInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x00)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			Header->Data[0x0C] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x0D)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x13)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x17)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x1B)) = mtRandom.randInt();
		}

		//BUILTINPTRDEV_INFO_TYPE:
		void StCallback_BuiltInPtrDevInfo(StructureHeader *Header)
		{
			*((uint16 *)(Header->Data + 0x00)) = mtRandom.randInt() & 0xFFFF;
		}

		//BATTERY_INFO_TYPE:
		void StCallback_BatteryInfo(StructureHeader *Header)
		{
			Header->Data[0x05] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			Header->Data[0x0B] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			Header->Data[0x11] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x12)) = mtRandom.randInt();
		}

		//SYSRESET_INFO_TYPE:
		void StCallback_SysResetInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x01)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x05)) = mtRandom.randInt();
		}

		//HARDSEC_INFO_TYPE:
		void StCallback_HardwareSecurityInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
		}

		//SYSPOWER_INFO_TYPE:
		void StCallback_SysPowerInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x01)) = mtRandom.randInt();
		}

		//VOLTPROBE_INFO_TYPE:
		void StCallback_VoltageProbeInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x02)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0A)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0E)) = mtRandom.randInt();
		}

		//COOLINGDEV_INFO_TYPE:
		void StCallback_CoolingDevInfo(StructureHeader *Header)
		{
			*((uint16 *)(Header->Data + 0x02)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x08)) = mtRandom.randInt() & 0xFFFF;
		}

		//TEMPPROBE_INFO_TYPE:
		void StCallback_TempProbeInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x02)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x0A)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x10)) = mtRandom.randInt() & 0xFFFF;
		}

		//ELECPROBE_INFO_TYPE:
		void StCallback_ElectricalProbeInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x02)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x0A)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x10)) = mtRandom.randInt() & 0xFFFF;
		}

		//OOBRA_INFO_TYPE:
		void StCallback_OobRemoteAccessInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
		}

		//SYSBOOT_INFO_TYPE:
		void StCallback_SysBootInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x06)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0A)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x0A)) = mtRandom.randInt() & 0xFFFF;
		}

		//MEMERROR64_INFO_TYPE:
		void StCallback_MemError64Info(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint16 *)(Header->Data + 0x01)) = mtRandom.randInt() & 0xFFFF;
			*((uint32 *)(Header->Data + 0x03)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x07)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0B)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0F)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x13)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x17)) = mtRandom.randInt();
		}

		//MNGDEV_INFO_TYPE:
		void StCallback_ManageDevInfo(StructureHeader *Header)
		{
			Header->Data[0x01] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x02)) = mtRandom.randInt();
			Header->Data[0x06] = mtRandom.randInt() & 0xFF;
		}

		//MNGDEVCOMP_INFO_TYPE:
		void StCallback_ManageDevCompInfo(StructureHeader *Header)
		{//Nothing to do
		}

		//MNGDEVTHRES_INFO_TYPE:
		void StCallback_ManageDevThresholdInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x00)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
		}

		//MEMCHAN_INFO_TYPE:
		void StCallback_MemChannelInfo(StructureHeader *Header)
		{
			uint8 DevCount = (Header->FormattedSize - sizeof(uint32) - sizeof(uint16) - sizeof(uint8)) / (sizeof(uint16) + sizeof(uint8));
			*((uint16 *)(Header->Data + 0x00)) = mtRandom.randInt() & 0xFFFF;
			for (uint8 i = 0x00; i < DevCount; ++i)
				Header->Data[0x03 + 3 * i] = mtRandom.randInt() & 0xFF;
		}

		//IPMIDEV_INFO_TYPE:
		void StCallback_IpmiDevInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x00)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x04)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x0C)) = mtRandom.randInt() & 0xFFFF;
		}

		//POWERSUPPLY_INFO_TYPE:
		void StCallback_PowerSupplyInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
			*((uint32 *)(Header->Data + 0x08)) = mtRandom.randInt();
			*((uint32 *)(Header->Data + 0x0C)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x10)) = mtRandom.randInt() & 0xFFFF;
		}

		//ADDITIONAL_INFO_TYPE:
		void StCallback_AdditionalInfo(StructureHeader *Header)
		{//Fomart has not a fix std, so we don't modify it
		}

		//ONBOARDDEVSEX_INFO_TYPE:
		void StCallback_OnBoardDevExInfo(StructureHeader *Header)
		{
			*((uint32 *)(Header->Data + 0x01)) = mtRandom.randInt();
			*((uint16 *)(Header->Data + 0x05)) = mtRandom.randInt() & 0xFFFF;
		}

		//MNGCTRLHOSTIF_INFO_TYPE:
		void StCallback_ManageControlHostInterfaceInfo(StructureHeader *Header)
		{
			Header->Data[0x00] = mtRandom.randInt() & 0xFF;
		}

	protected:
		uint8 *m_Buffer;
		uint32 m_BufferPtr;
		uint32 m_BufferSize;
	};
};

//Unban specific functions
uint8 *g_XnaddrData = NULL;

void Unban_ChangeXuid(void)
{
	//Change XUID
	*LPDWORD(ADDRESS_XUID) = mtRandom.randInt();
}

void Unban_ChangeXnaddr(void)
{
	//Change XNADDR
	for (uint8 i = 0; i < XNADDR_LEN;)
		g_XnaddrData[i++] = mtRandom.randInt() & 0xFF;
}

void Unban_ChangeSteamId(void)
{
	LPBYTE pPlayerInfo = LPBYTE(*LPDWORD(ADDRESS_PLAYER_INFO));
	if (pPlayerInfo)
	{
		LPDWORD pSteamId = LPDWORD(pPlayerInfo + 0x13A);
		pSteamId[0] = mtRandom.randInt();
	}
}

UINT GetSystemFirmwareTableReal
(
	DWORD FirmwareTableProviderSignature,
	DWORD FirmwareTableID,
	PVOID pFirmwareTableBuffer,
	DWORD BufferSize
)
{
	ULONG uReturnedLen = 0;
	LPBYTE pBuffer = LPBYTE(MALLOC(BufferSize + 0x10));
	*LPDWORD(&pBuffer[0x00]) = FirmwareTableProviderSignature;
	*LPDWORD(&pBuffer[0x04]) = 0x00000001;
	*LPDWORD(&pBuffer[0x08]) = FirmwareTableID;
	*LPDWORD(&pBuffer[0x0C]) = BufferSize;

	LONG fnRet = ZwQuerySystemInformation(SYSTEMINFO_CLASS_FIRM_TABLE, pBuffer, BufferSize + 0x10, NULL);

	uReturnedLen = *LPDWORD(&pBuffer[0x0C]);
	if (fnRet < 0)
	{
		if (fnRet != 0xC0000023)
			uReturnedLen = 0;
	}
	else
		memcpy(pFirmwareTableBuffer, &pBuffer[0x10], uReturnedLen);

	FREE(pBuffer);
	return uReturnedLen;
}

UINT Hook_GetSystemFirmwareTable
(
	DWORD FirmwareTableProviderSignature,
	DWORD FirmwareTableID,
	PVOID pFirmwareTableBuffer,
	DWORD BufferSize
)
{
	UINT fnReturn = GetSystemFirmwareTableReal(FirmwareTableProviderSignature, FirmwareTableID, pFirmwareTableBuffer, BufferSize);

	if (BufferSize && fnReturn)
	{		
		Unban_ChangeXuid();
		Unban_ChangeXnaddr();
		Unban_ChangeSteamId();
		//Change SMBIOS Info
		SMBIOS::AlterInfo *alterSmBios = new SMBIOS::AlterInfo((uint8 *)pFirmwareTableBuffer, fnReturn);
		alterSmBios->Process();
		delete alterSmBios;
	}

	return fnReturn;
}

void Bypass(void)
{
	DWORD dwProtection = PAGE_EXECUTE_READWRITE;
	LPBYTE pHookAddress = LPBYTE(GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetSystemFirmwareTable"));
	ZwQuerySystemInformation = pZwQuerySystemInformation(GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwQuerySystemInformation"));

	LPBYTE pTargetAddress = LPBYTE(Hook_GetSystemFirmwareTable);
	SafeVirtualProtect(pHookAddress, JMP_HOOK_SIZE, dwProtection, &dwProtection);
	*pHookAddress = 0xE9; *LPDWORD(pHookAddress + 1) = pTargetAddress - pHookAddress - JMP_HOOK_SIZE;
	SafeVirtualProtect(pHookAddress, JMP_HOOK_SIZE, dwProtection, &dwProtection);

	//Zero fill unnamed player name string
	dwProtection = PAGE_EXECUTE_READWRITE;
	LPBYTE pUnnamedPlayerName = LPBYTE(ADDRESS_UNNAMEDPLAYER_NAME);
	SafeVirtualProtect(pUnnamedPlayerName, sizeof(uint8), dwProtection, &dwProtection);
	*pUnnamedPlayerName = 0;
	SafeVirtualProtect(pUnnamedPlayerName, sizeof(uint8), dwProtection, &dwProtection);

	//Change XNADDR Buffer
	if (g_XnaddrData == NULL)
		g_XnaddrData = ((uint8 *)MALLOC(XNADDR_LEN));

	LPDWORD pXnAddrBuffer = LPDWORD(ADDRESS_XNADDRESS_BUFFER);
	SafeVirtualProtect(pXnAddrBuffer, sizeof(uint32), dwProtection, &dwProtection);
	*pXnAddrBuffer = DWORD(g_XnaddrData);
	SafeVirtualProtect(pXnAddrBuffer, sizeof(uint32), dwProtection, &dwProtection);

	Unban_ChangeXuid();
	Unban_ChangeXnaddr();
	Unban_ChangeSteamId();
}