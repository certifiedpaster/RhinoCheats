#include "stdafx.h"
#include "EndFrame.h"
#include "WritePackets.h"

#include "Host.h"

/*mw3:
		push    offset aCgHudsaypositi ; "cg_hudSayPosition"
   		call    sub_5BEAE0
		fld     ds:flt_7E98B8
		add     esp, 1Ch
		push    offset aPositionOfTheH_2 ; "Position of the HUD vote box"
		push    1 ; int
		sub     esp, 10h
		fstp    [esp+18h+var_C] ; float
		mov     dword_8FA640, eax <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 8FA640

*/

#define OFF_HUDSAYPOSITION_DVAR 0x88CBD0	

/*mw3:
		.text:00448301                 mov     esi, dword_8FA640
		.text:00448307                 fld     dword ptr [esi+10h]
*/

#define OFF_HUDSAYPOSITION_EXCEPTION 0x488307

/*mw3:
		.text:00492A33                 push    offset aClPacketdup ; "cl_packetdup"
		.text:00492A38                 mov     dword_1062574, eax
		.text:00492A3D                 call    sub_5BEA40
		.text:00492A42                 fld     ds:flt_7F3218
		.text:00492A48                 add     esp, 48h
		.text:00492A4B                 push    offset aMouseSensitivi ; "Mouse sensitivity"
		.text:00492A50                 push    1               ; int
		.text:00492A52                 sub     esp, 0Ch
		.text:00492A55                 fstp    [esp+14h+var_C] ; float
		.text:00492A59                 mov     dword_106256C, eax <<<<<<<<<<<<<<<<<<<<<<

*/

#define OFF_PACKETDUPLICATION_DVAR 0xBC385C	

/*
		.text:004C0B48                 mov     ecx, dword_BC385C
		.text:004C0B4E                 sub     eax, [ecx+0Ch]*/

#define OFF_PACKETDUPLICATION_EXCEPTION 0x004C0B4E

/************************************************************************/
/* Threads                                                              */
/************************************************************************/

HANDLE Handle_GetAKSThread = nullptr; //GetAsyncKeyStateThread
HANDLE SetNullPtrThreadHandle = nullptr;

void GAKSThread()
{
	while (true)
	{
		Engine.SwitchTeamProcess();

		SafeSleep(100);
	}
}

DWORD WINAPI SetNullPtrThread(void *unused)
{
	while (2019)	
	{	
		*(DWORD *)OFF_PACKETDUPLICATION_DVAR = 0; //"cl_packetdup"	
		*(DWORD *)OFF_HUDSAYPOSITION_DVAR = 0; //"cg_hudSayPosition"			

		SafeSleep(5017);
	}

	return 0;
}


/************************************************************************/
/* Offsets - Pointers                                                   */
/************************************************************************/

void GetOffsets() {

	HMODULE iw5mp_module = NULL;
	do
	{
		iw5mp_module = GetModuleHandleA("iw4mp.exe");
		SafeSleep(/*117*/118);
	} while (iw5mp_module == NULL);

	DWORD iw5mp_size = Hook.GetModuleInfo(iw5mp_module).SizeOfImage;
	XASSERT(iw5mp_size);
	
	Offsets::stats = 0x01B8B7E8;

	//Thanks CoMPMStR:
	//To find new offsets you can search for the string "flashbang_tinnitus_loop" after you join a server, then subtract 0x21 and that will be your offsets.
	
	Offsets::stunflash_congrenade = 0x86A8F8;
	Offsets::stunflash_flashgrenade = 0x86AB60;

	//Scanning it live :)
	/*
	Search your victim's name in memory.	
	*/	
	Offsets::victim = 0x00887f6f;

	/*
	Well basically when it says "match begins in" search for 4 value and when you gonna be able to move search for 0. If you gonna get the right one put 2 instead of 0.
Btw you can freeze players with it.


Yeah, pause the game, and search for 4, when you move, search for 0, click next, next, next until you get similar offset to 0x1C2.......
After that, test the offset with 1,

Once you have it, calculate delta: new offset - old offset of noclip

01C30A0C new
-
01C2CA0C old 
___________
0x4000 = delta

delta + old_server_entityinfo = new_server_entityinfo

	*/
	
	Offsets::server_entityinfo = 0; //can't get pattern	


	/*
	004966CD   C705 B81A0601 E7>MOV DWORD PTR DS:[0x1061AB8],0x3E7 <<
	004966D7   85FF             TEST EDI,EDI
	004966D9   7E 4D            JLE SHORT iw5mp.00496728
	004966DB   8B0D 50F9B700    MOV ECX,DWORD PTR DS:[0xB7F950]


	iw5mp Base 400000 Size 68a5000 \xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x85\xFF\x7E\x4D\x8B\x0D\x00\x00\x00\x00 xx????????xxxxxx????
	*/
	Offsets::ping = /*0x1061AB8*/ 0xBC7650;

	/*
	iw5mp Base 400000 Size 68a5000 \x83\xEC\x5C\x57\x8B\xF8\xA1\x00\x00\x00\x00\x80\x78\x0C\x00\x75\x07 xxxxxxx????xxxxxx	
	*/
	Offsets::bulletpenetrationcheck = /*0x47C4C0*/ 0x4B47A0;

	/*
	iw5mp Base 400000 Size 68a5000 \x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x55\x8B\xAC\x24\x00\x00\x00\x00\x56\x57\x83\xFB\xFF\x74\x29\x69\xDB\x00\x00\x00\x00 xx????xxxx????xxxx????xxxxxxxxx????	
	
	For Ida sigmaker...
	\x81\xEC\x00\x00\x00\x00\x53\x8B\x9C\x24\x00\x00\x00\x00\x55\x8B\xAC\x24\x00\x00\x00\x00\x56 xx????xxxx????xxxx????x
	
	*/
	Offsets::bulletpenetration = /*0x480D40*/ 0x4B8A40; // The old pattern didn't work, too long for Ida sigmaker, but the addy didn't changed lol.

	/*
	0044AE30   83EC 20          SUB ESP,0x20
	0044AE33   53               PUSH EBX
	0044AE34   8B5C24 2C        MOV EBX,DWORD PTR SS:[ESP+0x2C]
	0044AE38   55               PUSH EBP
	0044AE39   8B6C24 2C        MOV EBP,DWORD PTR SS:[ESP+0x2C]
	0044AE3D   56               PUSH ESI
	0044AE3E   57               PUSH EDI
	0044AE3F   8B3D E0029000    MOV EDI,DWORD PTR DS:[0x9002E0] <<<<<<<<<<<<
	0044AE45   8BF3             MOV ESI,EBX
	0044AE47   69FF 64050000    IMUL EDI,EDI,0x564
	0044AE4D   69F6 64050000    IMUL ESI,ESI,0x564
	0044AE53   55               PUSH EBP
	0044AE54   81C7 78A69F00    ADD EDI,iw5mp.009FA678
	0044AE5A   81C6 78A69F00    ADD ESI,iw5mp.009FA678
	0044AE60   E8 5B020800      CALL iw5mp.004CB0C0
	0044AE65   50               PUSH EAX
	0044AE66   E8 B5180500      CALL iw5mp.0049C720
	0044AE6B   83C4 08          ADD ESP,0x8
	0044AE6E   84C0             TEST AL,AL
	0044AE70   8B4424 3C        MOV EAX,DWORD PTR SS:[ESP+0x3C]
	0044AE74   74 0A            JE SHORT iw5mp.0044AE80
	0044AE76   8B4C24 40        MOV ECX,DWORD PTR SS:[ESP+0x40]
	0044AE7A   894C24 38        MOV DWORD PTR SS:[ESP+0x38],ECX
	0044AE7E   EB 04            JMP SHORT iw5mp.0044AE84
	0044AE80   894424 38        MOV DWORD PTR SS:[ESP+0x38],EAX
	0044AE84   8B76 1C          MOV ESI,DWORD PTR DS:[ESI+0x1C]
	0044AE87   83FE 03          CMP ESI,0x3
	0044AE8A   75 08            JNZ SHORT iw5mp.0044AE94
	0044AE8C   50               PUSH EAX
	0044AE8D   68 64897E00      PUSH iw5mp.007E8964                      ; ASCII "%s_Spectator"
	0044AE92   EB 51            JMP SHORT iw5mp.0044AEE5
	0044AE94   3B1D E0029000    CMP EBX,DWORD PTR DS:[0x9002E0]
	0044AE9A   74 3F            JE SHORT iw5mp.0044AEDB
	0044AE9C   8B7F 1C          MOV EDI,DWORD PTR DS:[EDI+0x1C]
	0044AE9F   85FF             TEST EDI,EDI
	0044AEA1   74 04            JE SHORT iw5mp.0044AEA7
	0044AEA3   3BFE             CMP EDI,ESI
	0044AEA5   74 13            JE SHORT iw5mp.0044AEBA
	0044AEA7   8B4424 38        MOV EAX,DWORD PTR SS:[ESP+0x38]
	0044AEAB   50               PUSH EAX
	0044AEAC   68 8C897E00      PUSH iw5mp.007E898C                      ; ASCII "%s_EnemyTeam"
	0044AEB1   6A 20            PUSH 0x20
	0044AEB3   8D4C24 1C        LEA ECX,DWORD PTR SS:[ESP+0x1C]
	0044AEB7   51               PUSH ECX
	0044AEB8   EB 32            JMP SHORT iw5mp.0044AEEC
	0044AEBA   53               PUSH EBX
	0044AEBB   55               PUSH EBP
	0044AEBC   E8 CF680100      CALL iw5mp.00461790
	0044AEC1   83C4 08          ADD ESP,0x8
	0044AEC4   85C0             TEST EAX,EAX
	0044AEC6   74 13            JE SHORT iw5mp.0044AEDB
	0044AEC8   8B5424 38        MOV EDX,DWORD PTR SS:[ESP+0x38]
	0044AECC   52               PUSH EDX
	0044AECD   68 80897E00      PUSH iw5mp.007E8980                      ; ASCII "%s_MyParty"

	iw5mp Base 400000 Size 68a5000 \x8B\x3D\x00\x00\x00\x00\x8B\xF3\x69\xFF\x00\x00\x00\x00\x69\xF6\x00\x00\x00\x00 xx????xxxx????xx????
	*/

	Offsets::bullettrail = /*0x9002E0*/ 0x8A4220;

	/*
	005C91A9   803D 1C0EA905 00 CMP BYTE PTR DS:[0x5A90E1C],0x0 <<<<<<<<<<<<<<<
	005C91B0   74 25            JE SHORT iw5mp.005C91D7
	005C91B2   56               PUSH ESI
	005C91B3   8B35 7CF37D00    MOV ESI,DWORD PTR DS:[<&USER32.ShowCurso>; USER32.ShowCursor
	005C91B9   6A 01            PUSH 0x1
	005C91BB   C605 1C0EA905 00 MOV BYTE PTR DS:[0x5A90E1C],0x0
	005C91C2   FFD6             CALL ESI
	005C91C4   85C0             TEST EAX,EAX
	005C91C6   74 0E            JE SHORT iw5mp.005C91D6
	
	iw5mp Base 400000 Size 68a5000 \x80\x3D\x1C\x0E\xA9\x05\x00\x74\x25\x56\x8B\x35\x00\x00\x00\x00 xxxxxxxxxxxx????

	
	\x80\x3D\x00\x00\x00\x00\x00\x74\x2E xx?????xx

	*/
	Offsets::mouse = /*0x5A90E1C*/ 0x6427DBC;

	/*
	00464CDA   D91D C818A000    FSTP DWORD PTR DS:[0xA018C8] <<<

	iw5mp Base 400000 Size 68a5000 \xD9\x1D\x00\x00\x00\x00\x8B\x5C\x24\x2C\x55\x56\x57\x6A\x00\x8B\xC3\xE8\x00\x00\x00\x00 xx????xxxxxxxxxxxx????
	*/
	Offsets::nospread_numerator = /*0xA018C8*/ 0x99E1D4;


	/*iw5mp Base 400000 Size 68a5000 \xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x56\x8B\xB4\x24\x00\x00\x00\x00\x8B\xC6\x57\x8D\x50\x01\x8A\x08\x40\x84\xC9\x75\xF9\x2B\xC2\x3D\x00\x00\x00\x00\x76\x1A\x8B\xC6\x8D\x50\x01 x????x????xxxx????xxxxxxxxxxxxxxxx????xxxxxxx
		
	\xE8\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\x50\xE8 x????xx????xx...... yeah, call [our offset]
	*/
	Offsets::printtoconsole = /*0x5CF5D0*/ 0;

	/*iw5mp Base 400000 Size 68a5000 \x56\x8B\x74\x24\x08\x56\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x75\x1B\x8B\x44\x24\x10 xxxxxxx????xxxxxxxxxxx
	
	
	\x56\x8B\x74\x24\x08\x56\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x75\x1B xxxxxxx????xxxxxxx

	*/
	Offsets::addcommand = /*0x545D00*/ 0;


	/*
	0058B1C9   6A 00            PUSH 0x0
	0058B1CB   68 E4EA8000      PUSH iw5mp.0080EAE4                      ; ASCII "ui_cursor"
	0058B1D0   A3 6CE58E05      MOV DWORD PTR DS:[0x58EE56C],EAX
	0058B1D5   E8 46950E00      CALL iw5mp.00674720
	0058B1DA   6A 03            PUSH 0x3
	0058B1DC   68 D0EA8000      PUSH iw5mp.0080EAD0                      ; ASCII "scrollbar_arrow_up"
	0058B1E1   A3 54E58E05      MOV DWORD PTR DS:[0x58EE554],EAX <<<<<<<<<<<<<<<<<<<	
	
	iw5mp Base 400000 Size 68a5000 \xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x03\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x03\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x03\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x40\x6A\x00\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00 x????x????xxx????x????x????xxx????x????x????xxx????x????x????xxxxxx????x????x????
	


	\x6A\x00\x68\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x6A\x03 xxx????x????x????xx
	*/
	Offsets::uicursor = /*0x58EE554*/ 0;


	/*
	
	0043BC10   833D F4419700 00 CMP DWORD PTR DS:[0x9741F4],0x0 <<<<
	0043BC17   53               PUSH EBX
	0043BC18   75 30            JNZ SHORT iw5mp.0043BC4A
	0043BC1A   F705 B0CF8F00 00>TEST DWORD PTR DS:[0x8FCFB0],0x800
	
	iw5mp Base 400000 Size 68a5000 \x83\x3D\x00\x00\x00\x00\x00\x53\x75\x30\xF7\x05\x00\x00\x00\x00\x00\x00\x00\x00 xx?????xxxxx????????
	*/
	Offsets::killcam = /*0x9741F4*/ 0;

	/*
	iw5mp Base 400000 Size 68a5000 \x81\xEC\x00\x00\x00\x00\x83\xBC\x24\xB4\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x53\x55 xx????xxxxxxxxxx????xx
	*/
	Offsets::weaponsound = /*0x44F0B0*/ 0;

	/*
	iw5mp Base 400000 Size 68a5000 \x8B\x44\x24\x04\x8B\x4C\x24\x08\x83\xF8\xFF\x75\x16 xxxxxxxxxxxxx
	*/
	Offsets::svgamesndconcmd = /*0x573100*/ 0;


	/*
	
	00531900   81EC 18010000    SUB ESP,0x118
	00531906   833D 38C38D00 00 CMP DWORD PTR DS:[0x8DC338],0x0
	0053190D   0F85 A0000000    JNZ iw5mp.005319B3
	00531913   53               PUSH EBX
	00531914   55               PUSH EBP
	00531915   6A 01            PUSH 0x1
	00531917   E8 74330200      CALL iw5mp.00554C90
	0053191C   E8 2FDEEFFF      CALL iw5mp.0042F750
	00531921   68 A09E7F00      PUSH iw5mp.007F9EA0                      ; ASCII "defaultweapon_mp"
	00531926   E8 65F6FFFF      CALL iw5mp.00530F90
	0053192B   6A 46            PUSH 0x46
	0053192D   8D4C24 14        LEA ECX,DWORD PTR SS:[ESP+0x14]
	00531931   0FB6C0           MOVZX EAX,AL
	00531934   51               PUSH ECX
	00531935   6A 1C            PUSH 0x1C
	00531937   A3 84BD8D00      MOV DWORD PTR DS:[0x8DBD84],EAX <<<<<<<

	iw5mp Base 400000 Size 68a5000 \xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8B\xE8\x6A\x04 x????x????x????xxxx
	
	*/
	
	//dword_8DBD84 = (unsigned __int8)sub_530F90("defaultweapon_mp");
	Offsets::host = /*0x008DBD84*/ 0x86E408;

	//iw5mp Base 400000 Size 68a5000 \x8B\x54\x24\x04\x83\xEC\x08\x53\x8B\x5C\x24\x14\x56\x84\xDB\x74\x2E\x8B\xC2\x25\x00\x00\x00\x00\x8B\x0C\x85\x00\x00\x00\x00\x8B\x01\x80\x38\x69\x75\x12\x80\x78\x01\x77\x75\x0C\x80\x78\x02\x35\x75\x06\x80\x78\x03\x5F\x74\x07\x8B\x41\x78\x84\xC0\x75\x02\x8B\xC2\x25\x00\x00\x00\x00\x8B\x0C\x85\x00\x00\x00\x00\x8B\xB1\x00\x00\x00\x00\x8D\x44\x24\x0C\x50\x8D\x4C\x24\x0C\x51\x8D\x44\x24\x20\x50\x53\x52\xE8\x00\x00\x00\x00\x8B\x44\x24\x2C\x83\xC4\x14\x85\xC0\x74\x12\x83\x78\x38\x00\x74\x0C\x8B\x48\x38\x8B\x41\x0C xxxxxxxxxxxxxxxxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xx????xxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx
	
	//\xE8\x00\x00\x00\x00\xD9\x84\x24\x00\x00\x00\x00\x83\xC4\x08 x????xxx????xxx... that call	[our offset] <<<< sig pattern taken from ida.
	
	Offsets::awallcheck_1 = /*0x432570*/ 0;

	//iw5mp Base 400000 Size 68a5000 \x56\x8B\x74\x24\x10\x57\x8B\x7C\x24\x10\x56\x57\xE8\x00\x00\x00\x00\x83\xC4\x08\x83\xF8\x04\x74\x05 xxxxxxxxxxxxx????xxxxxxxx
	
	//\xE8\x00\x00\x00\x00\x83\xC4\x38\x84\xC0\x74\x1C x????xxxxxxx... again	
	
	Offsets::awallcheck_2 = /*0x438F40*/ 0;

	//iw5mp Base 400000 Size 68a5000 \x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x03\x32\xC0\xC3\xA1\x00\x00\x00\x00\x85\xC0\x74\xF4\x83\x3D\x00\x00\x00\x00\x00\x75\xEB\x33\xC9\x83\xB8\x0C\x10\x5F\x00\x01 xxxxxx????xxxxxxxxxxx????xxxxxx?????xxxxxxxxxxx
	
	//\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x1F x????xxxxxxx      again

	Offsets::awallcheck_3 = /*0x6A72D0*/ 0;

	//iw5mp Base 400000 Size 68a5000 \x8B\x44\x24\x04\x50\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x03\x32\xC0\xC3\xA1\x00\x00\x00\x00\x85\xC0\x74\xF4\x83\x3D\x00\x00\x00\x00\x00\x75\xEB\x33\xC9\x83\xB8\x0C\x10\x5F\x00\x02 xxxxxx????xxxxxxxxxxx????xxxxxx?????xxxxxxxxxxx
	
	//\xE8\x00\x00\x00\x00\x83\xC4\x04\x84\xC0\x75\x12\x8D x????xxxxxxxx  again
	
	Offsets::awallcheck_4 = /*0x6A7310*/ 0;
	

	/*	
	004FA25C   8990 C42FA601    MOV DWORD PTR DS:[EAX+0x1A62FC4],EDX <<<<<
	004FA262   8B81 A8330000    MOV EAX,DWORD PTR DS:[ECX+0x33A8]
	004FA268   8981 A4010000    MOV DWORD PTR DS:[ECX+0x1A4],EAX
	004FA26E   C3               RETN

	iw5mp Base 400000 Size 68a5000 \x89\x90\x00\x00\x00\x00\x8B\x81\x00\x00\x00\x00\x89\x81\x00\x00\x00\x00\xC3 xx????xx????xx????x
	*/
	Offsets::server_health = /*0x01A62FC4*/ 0;

	//iw5mp Base 400000 Size 68a5000 \x6A\x1F\xE8\x00\x00\x00\x00\x8B\x44\x24\x0C\x8A\x08\x83\xC4\x04 xxx????xxxxxxxxx
	Offsets::cbufaddtext = /*0x545590*/ 0x563D10;

	//iw5mp Base 400000 Size 68a5000 \x56\x8B\x74\x24\x08\x8B\x86\x00\x00\x00\x00\x8B\x88\x00\x00\x00\x00\x51 xxxxxxx????xx????x
	Offsets::playerdie = /*0x503370*/ 0;

	/*	
	004896CC   81C6 B082B300    ADD ESI,iw5mp.00B382B0
	004896D2   F705 A4E70501 00>TEST DWORD PTR DS:[0x105E7A4],0x800 
	004896DC   0F85 44010000    JNZ iw5mp.00489826
	004896E2   80BE C4000000 00 CMP BYTE PTR DS:[ESI+0xC4],0x0
	004896E9   A1 E00EBA00      MOV EAX,DWORD PTR DS:[0xBA0EE0] <<<<<
	
	iw5mp Base 400000 Size 68a5000 \x81\xC6\x00\x00\x00\x00\xF7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x80\xBE\xC4\x00\x00\x00\x00\xA1\x00\x00\x00\x00 xx????xx????????xx????xxxxxxxx????
	
	
.text:004896C0 sub_4896C0 
.text:004896C0
.text:004896C0 var_14          = qword ptr -14h
.text:004896C0 var_C           = dword ptr -0Ch
.text:004896C0 var_8           = qword ptr -8
.text:004896C0
.text:004896C0                 sub     esp, 14h
.text:004896C3                 push    esi
.text:004896C4                 mov     esi, eax
.text:004896C6                 imul    esi, 258h
.text:004896CC                 add     esi, offset unk_B3C2B0 <<<<<<<<<<<<<
.text:004896D2                 test    dword_10627A4, 800h
.text:004896DC                 jnz     loc_489826


	\x81\xC6\x00\x00\x00\x00\xF7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x0F xx????xx????????x
	
		
	
	*/
	Offsets::key_input = /*0xB382B0*/ 0xAB2B98;


	//iw5mp Base 400000 Size 68a5000 \x8B\x44\x24\x04\x8B\x40\x04\xC3 xxxxxxxx	                                 
	Offsets::strHeight = /*0x5FD440*/ 0x40F7C0;

	//========================================================================

	/*DWORD Base = reinterpret_cast<DWORD>(GetModuleHandleA("steam_api.dll")) + 0x0001824C;
	DWORD Runner = GET_INT(Base);
	Runner = GET_INT(Runner) + 0x4;
	Runner = GET_INT(Runner) + 0x8;
	Runner = GET_INT(Runner) + 0x10;
	Runner = GET_INT(Runner) + 0x7e;
	Offsets::namegame = Runner;
	XASSERT(Offsets::namegame);*/

	
	Offsets::normalize = 0x00412A80;

	
	Offsets::cgt = 0x8A0ED0;

	
	Offsets::cgs = 0x89D420;

	
	Offsets::entity = 0x9A4110;

	
	Offsets::client = 0x9978E0;

	
	Offsets::refdef = 0x90B648;

	
	Offsets::text = 0x4C56A0;

	
	Offsets::strWidth = 0x40F710;

	
	Offsets::white = 0xB175C0;


	Offsets::fonts_t = 0x627CD68;

	
	Offsets::fonts_dev_t = 0x99FC4C;


	Offsets::shader = 0x40FD60;

	
	Offsets::regtag = 0x57A6E0;

	
	Offsets::gettag = 0x4B5C50;

	
	Offsets::cgtrace = 0x52FD80;


	Offsets::getweapon = 0x86CE20;

	
	Offsets::viewmatrix = 0xBC7710;
	
	Offsets::zoom = 0;
	
	Offsets::serverid = 0xB58150;

	Offsets::input = 0xBC775C;

	Offsets::weaponspread = 0x47A1A0;
	
	Offsets::randomspread = 0x004B4AC0;

	Offsets::isplaying = 0;


	Offsets::visible = 0;

	
	Offsets::dvarbase = 0x6381470;
	
	Offsets::dvarsize = 0x637C448;
	
	Offsets::fullbright = 0x6E66224;
	
	Offsets::screeematrix = 0x4DF860;
	
	Offsets::worldtoscreen = 0x48BF30;
	
	Offsets::drawrotatedpic = 0x48B580;
	
	Offsets::bounding = 0;
	
	Offsets::hud_dpad_arrow = 0x9A3CD8;
	
	Offsets::weaponimpacttype = 0x47AC30;	
	
	Offsets::newseed = 0x90B628;
	
	Offsets::serverset = 0;
	
	Offsets::m_addr_TraceBullet = 0x4B4520;

	
	Offsets::m_addr_GetPenetrationPower = 0x479430;
	
	Offsets::m_addr_CopyTrace = 0x5B8FD0;

	
	Offsets::m_addr_StepForward = 0x479A80;

	
	Offsets::punch = 0x914D7C;


}
void GetPointers()
{
	punch = (Punch_t *)Offsets::punch;
	concGrenade = (stunflash_t*)Offsets::stunflash_congrenade;
	flashGrenade = (stunflash_t*)Offsets::stunflash_flashgrenade;
	cg_entities = (centity_t *)Offsets::entity;
	ci = (clientInfo_t *)Offsets::client;
	cg = (cg_t*)Offsets::cgt;
	cgs = (cgs_t *)Offsets::cgs;
	refdef = (refdef_t*)Offsets::refdef;
	pViewMatrix = (viewmatrix_t *)Offsets::viewmatrix;	
	Fonts = (Fonts_t *)Offsets::fonts_t;
	fonts_Dev = (Fonts_Dev_t *)Offsets::fonts_dev_t;
	ServerID = (int **)Offsets::serverid;
	pinput = (input_t*)Offsets::input;	
	pStats = (stats_t*)Offsets::stats;	
	 
	/*ServerSet = (ServerSet_t *)Offsets::serverset;
	ServerEntityInfo = (entityInfo_s *)Offsets::server_entityinfo;
	ServerHealth = (Health_s *)Offsets::server_health;

	*/

	key_input = (KInput_t *)Offsets::key_input;

	//========================================================================

	Trace = (G_TraceCapsule_t)Offsets::cgtrace;
	VectorNormalize = (VectorNormalize_t)Offsets::normalize;
	R_WeaponSpread = (BG_GetSpreadForWeapon_t)Offsets::weaponspread;
	//CMD_AddCommand = (Cmd_AddCommandInternal_t)Offsets::addcommand;	
	Cbuf_AddText = (Cbuf_AddText_t)Offsets::cbufaddtext;
	SV_GameSendServerCommand = (SV_GameSendServerCommand_t)Offsets::svgamesndconcmd;
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

	/*psub_46B370 = (tsub_46B370)Offsets::awallcheck_1;
	psub_52A190 = (tsub_52A190)Offsets::awallcheck_2;
	psub_4F0CF0 = (tsub_4F0CF0)Offsets::awallcheck_3;
	psub_489F70 = (tsub_4F0CF0)Offsets::awallcheck_4; */
	
	
}

CVMTHookManager* pD3DHook = nullptr;

/*
.text:004113D7                 jz      locret_4114FE
.text:004113DD                 lea     edx, [eax+1] <<<< dwNext

.text:004114FE                 retn  << dwEnd
*/

DWORD dwEnd = 0x004114FE;
DWORD dwNext = 0x004113DD;
DWORD _eax = 0;
char *pData = nullptr;
void FixString(char *pChar)
{
	/*if (strstr(pChar, "^3You ^1have been^7"))
	{
		printf("Valor: %s\n", pChar);
		printf("VNumeric:");

		for (int i = 0; i < strlen(pChar); i++)
		{
			printf("[%i]", pChar[i]);
		}
		printf("\n");
	}*/

	//94 2 48 48 -72 53 72 1
	/*doesn't work... for (int i = 0; i < iSize; i++)
		if (pChar[i] < 32)
			if (!(pChar[i] == 2 && pChar[i + 1] == 48 && pChar[i + 2] == 48 && pChar[i + 3] == -72 && pChar[i + 4] == 53 && pChar[i + 5] == 72 && pChar[i + 6] == 1))
				pChar[i] = 32;*/

	LPSTR szInvalidText;

	if (szInvalidText = strstr(pChar, "\x5E\x02\xFF\xFF\xFF"))
		strcpy_s(szInvalidText, strlen("crash") + 1, "crash");

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



typedef void(__cdecl * Scr_AddString_t)(char* string);
Scr_AddString_t pScr_AddString = (Scr_AddString_t)0x581100;
/*
inside sub_525860

.text:00525913                 call    sub_581100
.text:00525918                 lea     edx, [esp+0C04h+Dest] <<<<
*/
DWORD dwContinue = 0x00525918;
char *pFixChar = nullptr;
DWORD _ecx = 0;
void FixEndRound(char *pChar)
{
	/*
	Doesn't work.
	if (!*(int*)Offsets::host_offset
	&& strstr(pChar, "endround"))
	strcpy(pChar, "lemonade");
	*/

	if (strstr(pChar, "endround") && *(int*)Offsets::host && Settings[host_antiendgame].Value.bValue)
		strcpy(pChar, "lemonade");
	/*else
	strcpy(pChar, "endround");*/

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


//Console Fix
typedef char(__cdecl *t_48D120)(int a1, int a2, int a3);
t_48D120 o_48D120 = (t_48D120)0x4C2980;
char __cdecl h_48D120(int a1, int a2, int a3)
{
	__asm pushad
	char retVal;

	if (D3D::g_bMainWindowOpen)
	{
		retVal = 1;
	}
	else
	{
		retVal = o_48D120(a1, a2, a3);
	}

	_asm popad;

	return retVal;

}

 /*#include <io.h>
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
 
 	//Won't work on 64bit with Visual Studio 2015 :(
 	//*stdout = *fp;
 	//setvbuf(stdout, NULL, _IONBF, 0);
 
 	//http://stackoverflow.com/questions/32185512/output-to-console-from-a-win32-gui-application-on-windows-10
 	freopen_s(&fp, "CONOUT$", "w", stdout);
 }*/


typedef void(__cdecl *t_CL_DrawStretchPic)(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, /*const*/ float *color, void *material);
t_CL_DrawStretchPic CL_DrawStretchPic = (t_CL_DrawStretchPic)0x04BC1B0;
void CL_DrawStretchPic_Hook(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, /*const*/ float *color, void *material) {

	if (!D3D::g_bWasInitialized)
		CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign, s1, t1, s2, t2, color, material);
	else
	{
		char *Shader = *reinterpret_cast<char**>(material);
		if (Shader)
		{
			//printf("Shader: %s\n", Shader);

			/*if (strstr(Shader, "mw2_")
				|| strstr(Shader, "menu_button_selection")
				|| strstr(Shader, "gradient")
				|| strstr(Shader, "mockup")
				|| strstr(Shader, "drop")
				)*/

				/*if (!strcmp(Shader, "mw2_main_background")
					|| !strcmp(Shader, "mw2_main_cloud_overlay")
					|| strstr(Shader, "gradient")
					|| strstr(Shader, "mockup")
					)*/

			if (Settings[style_rainbow].Value.bValue)
			{
				if (
					/*!strcmp(Shader, "mw2_main_background")
					|| !strcmp(Shader, "mw2_main_cloud_overlay")*/
					strstr(Shader, "mw2")
					|| strstr(Shader, "menu")
					|| strstr(Shader, "mockup")
					)
					color = reinterpret_cast<float*>(&ImVec4(D3D::colRainBow.x, D3D::colRainBow.y, D3D::colRainBow.z, color[3]));
			}
		}

		CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign, s1, t1, s2, t2, color, material);
	}
}



//========================================================================


/*VM_Notify_t VM_Notify = (VM_Notify_t)0x569720;
int __cdecl VM_Notify_Hook(int a1, unsigned int a2, unsigned int *a3) {
		
	Host::VM_Notify(a1, a2, a3);

	return VM_Notify(a1, a2, a3);
}*/


long __stdcall pVEH_Hook(_EXCEPTION_POINTERS *pInfo)
{
	switch (pInfo->ContextRecord->Eip)
	{
	case OFF_HUDSAYPOSITION_EXCEPTION:
		pInfo->ContextRecord->Esi = 0x063883C4;
		RenderAll();
		return EXCEPTION_CONTINUE_EXECUTION;
		break;	
		
	case OFF_PACKETDUPLICATION_EXCEPTION:
		pInfo->ContextRecord->Ecx = 0x6389464;
		WritePacket();
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
	//CreateConsole("Output");
	//printf("Dev console\n");
	//std::cout << "cout works\n\n";

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
	/*
		.text:004113D0                 mov     eax, [esp+Src]
		.text:004113D4                 cmp     byte ptr [eax], 0
		.text:004113D7                 jz      locret_4114FE <<< hook here
		*/
	MakeJMP((BYTE *)0x004113D7, (DWORD)h_client_crasher_fix, 0x6);

	/*
		.text:00525913                 call    sub_581100 <<< hook here
		.text:00525918                 lea     edx, [esp+0C04h+Dest]
		*/
	MakeJMP((BYTE *)0x00525913, (DWORD)h_endround_fix, 0x5);
	
	HookModule(GetCurrentThread(), o_48D120, h_48D120); //Console Fix		
	HookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);//Background Effect	
	//HookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook); 	

	// Level 3
 	if (!D3D::InitAndHookD3D())	
	{
		SafeMessageBox(HWND_DESKTOP, "Error! at Level 3", ALASKA17, MB_ICONERROR);		

		UnHookModule(GetCurrentThread(), o_48D120, h_48D120);
		UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);
		//UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);			

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

			// Level 5
			if (Handle_GetAKSThread)
			{
				SafeCreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hook.playSound), sounds.activated, 0, nullptr);
			}
			else
			{
				SafeMessageBox(HWND_DESKTOP, "Error! at Level 5", ALASKA17, MB_ICONERROR);
				
				FinishThread(SetNullPtrThreadHandle);
				*(DWORD *)OFF_PACKETDUPLICATION_DVAR = 0x6389464; //"cl_packetdup"	
				*(DWORD *)OFF_HUDSAYPOSITION_DVAR = 0x063883C4; //"cg_hudSayPosition"
				RemoveVectoredExceptionHandler(pVEH_Hook);

				UnHookModule(GetCurrentThread(), o_48D120, h_48D120);
				UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);
				//UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);					

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

			UnHookModule(GetCurrentThread(), o_48D120, h_48D120);
			UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);
			//UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);

			D3D::Restore_WndProc();

			if (pD3DHook)
			{
				pD3DHook->UnHook();
				pD3DHook = nullptr;
			}

			exit(-1);			
		}	
 	}
}

void Hook_t::ExecCleaningThread()
{	
	FinishThread(SetNullPtrThreadHandle);
	*(DWORD *)OFF_PACKETDUPLICATION_DVAR = 0x6389464; //"cl_packetdup"	
	*(DWORD *)OFF_HUDSAYPOSITION_DVAR = 0x063883C4; //"cg_hudSayPosition"
	RemoveVectoredExceptionHandler(pVEH_Hook);	

	UnHookModule(GetCurrentThread(), o_48D120, h_48D120);
	UnHookModule(GetCurrentThread(), CL_DrawStretchPic, CL_DrawStretchPic_Hook);
	//UnHookModule(GetCurrentThread(), VM_Notify, VM_Notify_Hook);	

	D3D::Restore_WndProc();

	if (pD3DHook)
	{
		pD3DHook->UnHook();
		pD3DHook = nullptr;
	}
		
	FinishThread(Handle_GetAKSThread);	
	
}