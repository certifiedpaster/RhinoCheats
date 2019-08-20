#include "stdafx.h"

dvar_s *perk_bulletPenetrationMultiplier = nullptr;
dvar_s *com_maxfps = nullptr;
dvar_s *cg_fov = nullptr;
dvar_s *cg_thirdPerson = nullptr;

dvar_s *cg_gun_x = nullptr;
dvar_s *cg_gun_y = nullptr;
dvar_s *cg_gun_z = nullptr;

//========================================================================

stunflash_t*          concGrenade = nullptr;
stunflash_t*         flashGrenade = nullptr;

centity_t            *cg_entities = nullptr;

cg_t	                      *cg = nullptr;
cgs_t                        *cgs = nullptr;
refdef_t                  *refdef = nullptr;
viewmatrix_t         *pViewMatrix = nullptr;
Fonts_t                    *Fonts = nullptr;
Fonts_Dev_t            *fonts_Dev = nullptr;
int                    **ServerID = nullptr;
input_t                   *pinput = nullptr;
stats_t*                   pStats = nullptr;
ServerSet_t            *ServerSet = nullptr;
entityInfo_s    *ServerEntityInfo = nullptr;
Health_s            *ServerHealth = nullptr;
KInput_t               *key_input = nullptr;
Punch_t                    *punch = nullptr;

//========================================================================

R_TextWidth_t StrWidth = nullptr;
R_TextHeight_t StrHeight = nullptr;
CL_DrawTextPhysicalWithEffects_t DrawGlowText = nullptr;
Material_RegisterHandle_t RegisterShader = nullptr;
G_TraceCapsule_t           Trace = nullptr;
VectorNormalize_t VectorNormalize = nullptr;
CG_DrawRotatedPicPhysical_t DrawRotatedPic = nullptr;
ScrPlace_GetActivePlacement_t GSM = nullptr;
CG_WorldPosToScreenPosReal_t WorldToScreenE = nullptr;
BG_GetSpreadForWeapon_t R_WeaponSpread = nullptr;
Scr_AddString_t pScr_AddString = nullptr;
CL_KeyEvent_t CL_KeyEvent = nullptr;
t_CL_DrawStretchPic CL_DrawStretchPic = nullptr;

Cbuf_AddText_t Cbuf_AddText = nullptr;

//Autowall special functions...
BG_WeaponBulletFire_GetMethodOfDeath_t pWeaponImpacttype = nullptr;
BG_GetSurfacePenetrationDepth_t pfnGetBulletPenetration = nullptr;
Com_Memcpy_t pfnCopyTrace = nullptr;
BG_AdvanceTrace_t pfnStepForward = nullptr;
tsub_46B370 psub_46B370 = nullptr;
tsub_52A190 psub_52A190 = nullptr;
tsub_4F0CF0 psub_4F0CF0 = nullptr;
tsub_4F0CF0 psub_489F70 = nullptr;

//========================================================================

qhandle_t HeadShotIcon = 0;
qhandle_t DiedIcon = 0;
qhandle_t ScavengerIcon = 0;
qhandle_t FlashbangIcon = 0;
qhandle_t CompassPlayerIcon = 0;

//========================================================================
//Force Classes

char DoCmd[30];
void ForceJug() {
	//ForceJuggg	
	sprintf_s(DoCmd, "mr %i 9 axis\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass1()
{
	sprintf_s(DoCmd, "mr %i 9 custom1\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass2()
{
	sprintf_s(DoCmd, "mr %i 9 custom2\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass3()
{
	sprintf_s(DoCmd, "mr %i 9 custom3\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass4()
{
	sprintf_s(DoCmd, "mr %i 9 custom4\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass5()
{
	sprintf_s(DoCmd, "mr %i 9 custom5\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass6()
{
	sprintf_s(DoCmd, "mr %i 9 custom6\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass7()
{
	sprintf_s(DoCmd, "mr %i 9 custom7\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass8()
{
	sprintf_s(DoCmd, "mr %i 9 custom8\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass9()
{
	sprintf_s(DoCmd, "mr %i 9 custom9\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass10()
{
	sprintf_s(DoCmd, "mr %i 9 custom10\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass11()
{
	sprintf_s(DoCmd, "mr %i 9 custom11\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass12()
{
	sprintf_s(DoCmd, "mr %i 9 custom12\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass13()
{
	sprintf_s(DoCmd, "mr %i 9 custom13\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass14()
{
	sprintf_s(DoCmd, "mr %i 9 custom14\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceHackedClass15()
{
	sprintf_s(DoCmd, "mr %i 9 custom15\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void GodMode()
{
	sprintf_s(DoCmd, "mr %i 9 gamemode\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ForceClass() {
	sprintf_s(DoCmd, "mr %i 8 changeclass_opfor\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void ChangeTeam()
{
	sprintf_s(DoCmd, "mr %i 2 changeteam\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
	SafeSleep(200);
}
void TeamSpectator()
{
	sprintf_s(DoCmd, "mr %i 2 spectator\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
}
void EndRound()
{
	sprintf_s(DoCmd, "mr %i -1 endround\n", reinterpret_cast<int>(ServerID[0]));
	Engine.SendToConsole(DoCmd);
}

//========================================================================

Engine_t Engine;

bool g_bF1 = false;
bool g_bF2 = false;
bool g_bF3 = false;
bool g_bF4 = false;
bool g_bF5 = false;
bool g_bF6 = false;
bool g_bF7 = false;
bool g_bF8 = false;
bool g_bF9 = false;
bool g_bF10 = false;
bool g_bEND = false;

bool g_bNumPad0 = false;
bool g_bNumPad1 = false;
bool g_bNumPad2 = false;
bool g_bNumPad3 = false;
bool g_bNumPad4 = false;
bool g_bNumPad5 = false;
bool g_bNumPad6 = false;
bool g_bNumPad7 = false;
bool g_bNumPad8 = false;
bool g_bNumPad9 = false;

void Engine_t::SwitchTeamProcess() {
	if (g_bF1)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceClass, nullptr, 0, nullptr);
	if (g_bF2)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ChangeTeam, nullptr, 0, nullptr);
	if (g_bF3)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceJug, nullptr, 0, nullptr);
	if (g_bF4)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)TeamSpectator, nullptr, 0, nullptr);
	if (g_bF5)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)GodMode, nullptr, 0, nullptr);
	if (g_bF6)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass11, nullptr, 0, nullptr);
	if (g_bF7)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass12, nullptr, 0, nullptr);
	if (g_bF8)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass13, nullptr, 0, nullptr);
	if (g_bF9)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass14, nullptr, 0, nullptr);
	if (g_bF10)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass15, nullptr, 0, nullptr);
	if (g_bEND && !isTekno)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)EndRound, nullptr, 0, nullptr);

	//========================================================================
	if (g_bNumPad1)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass1, nullptr, 0, nullptr);
	if (g_bNumPad2)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass2, nullptr, 0, nullptr);
	if (g_bNumPad3)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass3, nullptr, 0, nullptr);
	if (g_bNumPad4)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass4, nullptr, 0, nullptr);
	if (g_bNumPad5)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass5, nullptr, 0, nullptr);
	if (g_bNumPad6)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass6, nullptr, 0, nullptr);
	if (g_bNumPad7)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass7, nullptr, 0, nullptr);
	if (g_bNumPad8)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass8, nullptr, 0, nullptr);
	if (g_bNumPad9)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass9, nullptr, 0, nullptr);
	if (g_bNumPad0)
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ForceHackedClass10, nullptr, 0, nullptr);

}

bool Engine_t::Getvectorfromtag(centity_t * pEnt, WORD wBone, vec3_t vOrigin)
{
	//004107E0   8B8E D0000000    MOV ECX,DWORD PTR DS:[ESI+0xD0]
	/*DWORD dwCall = Offsets::gettag; return: void*

	__asm {
		MOV ESI, pEnt;
		MOVZX EDI, wBone;
		PUSH vOrigin;
		CALL[dwCall];
		ADD ESP, 0x4
	}*/

	//"cg_TeamColor", "cg_ColorBlind"
	typedef char *(__cdecl *sub_556940_t)(int a1);
	char *v3 = ((sub_556940_t)(/*0x556940*/ Offsets::gtag_1))(pEnt->nextState.number);
	if (v3)
	{
		typedef int *(__cdecl *sub_44D910_t)(centity_t * a1, char * a2, WORD a3, vec3_t a4);
		if (!((sub_44D910_t)/*0x44D910*/ Offsets::gtag_2)(pEnt, v3, wBone, vOrigin))
			return false;
	}

	return true;

}

WORD Engine_t::RegisterTag(const char* szBone)
{
	DWORD dwCall = Offsets::regtag;
	__asm {
		push 1
		push szBone
		call[dwCall]
		add esp, 0x8
	}
}

void Engine_t::SendToConsole(char* Cmd)
{
	Cbuf_AddText(0, Cmd);
}

weapon_t* Engine_t::GetCombatWeapon(int Index)
{
	return (weapon_t*)(*(DWORD*)(Offsets::getweapon + 0x04 * (Index & 0xFF)));
}

bool Engine_t::WorldToScreen(vec3_t vWorldLocation, float Screen[2])
{
	return WorldToScreenE(0, GSM(), vWorldLocation, Screen);
}

void Engine_t::DamageFeedBack_Removal() {	

	if (Settings[no_recoil].Value.bValue)
	{
		punch->punchAngles[0] = 0;
		punch->punchAngles[1] = 0;


		pViewMatrix->recoil[2] = 0;
		pViewMatrix->recoil[0] = 0;
		pViewMatrix->recoil[1] = 0;		
	}
}

void Engine_t::FuckStunFlashShit(stunflash_t* sfData)
{
	sfData->lookControl = FALSE;
	sfData->lookControlFadeTime = 0;
	sfData->lcMaxPitchSpeed = 0.0f;
	sfData->lcMaxYawSpeed = 0.0f;
	sfData->lcMouseSensitivityScale = 0.0f;
	sfData->data[0].screenType = 0;
	sfData->data[0].blurBlendFadeTime = 0;
	sfData->data[0].blurBlendTime = 0;
	sfData->data[0].viewKickPeriod = 0.0f;
	sfData->data[0].viewKickRadius = 0.0f;
	sfData->data[0].shockSound = 0; //:)

}

dvar_s* Engine_t::FindVar(char * name)
{
	for (int i = 0; i < *(int*)Offsets::dvarsize; ++i)
	{
		dvar_s * pVar = (dvar_s*)(Offsets::dvarbase + 0x4C * i);
		if (pVar && !strcmp(pVar->name, name))
			return pVar;
	}
	return nullptr;
}

void Engine_t::ApplyDvars()
{
	if (!perk_bulletPenetrationMultiplier)
		perk_bulletPenetrationMultiplier = FindVar("perk_bulletPenetrationMultiplier");
	XASSERT(perk_bulletPenetrationMultiplier);

	if (!com_maxfps)
		com_maxfps = Engine.FindVar("com_maxfps");
	XASSERT(com_maxfps);

	if (!cg_fov)
		cg_fov = FindVar("cg_fov");
	XASSERT(cg_fov);

	if (!cg_thirdPerson)
		cg_thirdPerson = FindVar("camera_thirdPerson");
	XASSERT(cg_thirdPerson);

	//========================================================================
	//ThirdPerson Fix

	dvar_s *pSpecial = nullptr;
	
	//Tested, camera_thirdPersonOffset not needed, but lets see:
	pSpecial = FindVar("camera_thirdPersonOffset");
	XASSERT(pSpecial);
	pSpecial->u6.fval = -140;

	pSpecial = FindVar("camera_thirdPersonCrosshairOffset");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	//========================================================================

	if (!cg_gun_x)
		cg_gun_x = FindVar("cg_gun_x");
	XASSERT(cg_gun_x);

	if (!cg_gun_y)
		cg_gun_y = FindVar("cg_gun_y");
	XASSERT(cg_gun_y);

	if (!cg_gun_z)
		cg_gun_z = FindVar("cg_gun_z");
	XASSERT(cg_gun_z);

	//========================================================================

	pSpecial = FindVar("bg_viewBobAmplitudeBase");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeSprinting");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeStanding");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeStandingAds");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeDucked");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeDuckedAds");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeProne");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobAmplitudeRoll");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobMax");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_viewBobLag");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;




	pSpecial = FindVar("bg_weaponBobAmplitudeBase");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobAmplitudeSprinting");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobAmplitudeStanding");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobAmplitudeDucked");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobAmplitudeProne");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobAmplitudeRoll");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobMax");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("bg_weaponBobLag");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("player_sprintCameraBob");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;

	pSpecial = FindVar("perk_lightWeightViewBobScale");
	XASSERT(pSpecial);
	pSpecial->u6.fval = 0;


}

void Engine_t::Misc_Process()
{
	//Obituary	
	KillSpam.KillsUpdate();

	//Show killSpree icons
	KillSpree.Update();

	//Sounds
	sounds.killsound();

	//<<<<<<

	static bool bFullBright = false;
	if (Settings[fullbright].Value.bValue && !bFullBright)
	{
		*(BYTE*)Offsets::fullbright = 4; //FullBright on
		bFullBright = true;
				
	}
	else if (bFullBright && !Settings[fullbright].Value.bValue)
	{		
		*(BYTE*)Offsets::fullbright = 9; //FullBright off
		bFullBright = false;				
	}				

	cg_fov->u6.fval = Settings[force_fov].Value.fValue;
	
	cg_thirdPerson->u6.ival = Settings[third_person].Value.bValue ? 1 : 0;

	cg_gun_x->u6.fval = Settings[gun_x].Value.fValue;

	cg_gun_y->u6.fval = Settings[gun_y].Value.fValue;

	cg_gun_z->u6.fval = Settings[gun_z].Value.fValue;

	//========================================================================	

	if (!Aim.isVehicle)
	{
		weapon_t *pMYWEAPON = Engine.GetCombatWeapon(cg_entities[cg->clientNum].nextState.weaponID);
		if (pMYWEAPON)
		{
			//static bool bDrunken = false;

			if (/*!bDrunken &&*/ Settings[drunken_style].Value.bValue && !Settings[third_person].Value.bValue)
			{
				pMYWEAPON->info->swayMultiplier = 1080.0f;
				pMYWEAPON->info->swayMultiplierADS = 1080.0f;
				//bDrunken = true;
			}
			else //if (bDrunken)
			{
				pMYWEAPON->info->swayMultiplier = 0;
				pMYWEAPON->info->swayMultiplierADS = 0;
				//bDrunken = false;
			}

			//With the flags: ghost drunken effect
		}
	}
	
}

void Engine_t::DrawLaser()
{
	if (!Settings[laser].Value.bValue)
		return;
	if (Settings[third_person].Value.bValue)
		return;
	if (Aim.isVehicle)
		return;

	/*if (*(int*)Offsets::killcam == 1)
		return;
	*/	
	if (*(DWORD*)/*0x973688*/Offsets::laser_1) //0x203800 as sequence of bytes, Ctrl-B Ida until this: if ( !(*(_DWORD *)(a4 + 216) & 0x203800) ),   inside: void __cdecl sub_47CB00(int a1, float *a2, int a3, int a4, int a5)
		return;

	centity_t *pLocalEnt = &cg_entities[cg->clientNum];
	if (pLocalEnt == nullptr)
		return;

	if (pLocalEnt->valid && (pLocalEnt->IsAlive & 1) /*&& (*PDWORD(Offsets::isplaying) == 4096 || *PDWORD(Offsets::isplaying) == 4352)*/)
	{	

		/*
		004E5CCE   68 8007A000      PUSH iw5mp.00A00780 <<<<
		004E5CD3   56               PUSH ESI
		004E5CD4   50               PUSH EAX
		004E5CD5   E8 56C81800      CALL iw5mp.00672530
		004E5CDA   83C4 0C          ADD ESP,0xC
		004E5CDD   5E               POP ESI
		004E5CDE   83C4 10          ADD ESP,0x10
		004E5CE1   C3               RETN

		iw5mp Base 400000 Size 68a5000 \x68\x00\x00\x00\x00\x56\x50\xE8\x00\x00\x00\x00\x83\xC4\x0C\x5E\x83\xC4\x10\xC3 x????xxx????xxxxxxxx
		*/

		laserInfo_t *Info = (laserInfo_t*)/*0xA00780*/ Offsets::laser_2;
		if (Info == nullptr)
			return;

		/*
		004821CB   8D3CBD 4426B000  LEA EDI,DWORD PTR DS:[EDI*4+0xB02644] <<<<<<
		004821D2   75 3F            JNZ SHORT iw5mp.00482213
		004821D4   B8 B0237F00      MOV EAX,iw5mp.007F23B0                   ; ASCII "right"
		004821D9   85F6             TEST ESI,ESI
		004821DB   74 05            JE SHORT iw5mp.004821E2
		004821DD   B8 A8237F00      MOV EAX,iw5mp.007F23A8                   ; ASCII "left"
		004821E2   8B4C24 14        MOV ECX,DWORD PTR SS:[ESP+0x14]
		004821E6   50               PUSH EAX
		004821E7   68 00040000      PUSH 0x400
		004821EC   8D4424 2C        LEA EAX,DWORD PTR SS:[ESP+0x2C]
		004821F0   50               PUSH EAX
		004821F1   51               PUSH ECX
		004821F2   E8 39D9FAFF      CALL iw5mp.0042FB30
		004821F7   83C4 0C          ADD ESP,0xC
		004821FA   50               PUSH EAX
		004821FB   55               PUSH EBP
		004821FC   68 40237F00      PUSH iw5mp.007F2340                      ; ASCII "no viewmodel set for player %d for '%s' in '%s' hand. Set viewmodel hands in script or weapon settings"

		iw5mp Base 400000 Size 68a5000 \x8D\x3C\xBD\x00\x00\x00\x00\x75\x3F\xB8\x00\x00\x00\x00\x85\xF6\x74\x05\xB8\x00\x00\x00\x00\x8B\x4C\x24\x14 xxx????xxx????xxxxx????xxxx
		*/
		//\x8D\x3C\xBD\x00\x00\x00\x00\x75 xxx????x //lea     edi, ds:0B06644h[edi*4]
		
		void* Laser = (void*)(*(DWORD*)/*0xB02644*/ Offsets::laser_3);
		if (Laser == nullptr)
			return;

		/*
		00454CE0   83EC 0C          SUB ESP,0xC
		00454CE3   D905 FC819600    FLD DWORD PTR DS:[0x9681FC] <<<
		00454CE9   53               PUSH EBX
		00454CEA   D95C24 04        FSTP DWORD PTR SS:[ESP+0x4]
		00454CEE   33DB             XOR EBX,EBX
		00454CF0   391D E0328F00    CMP DWORD PTR DS:[0x8F32E0],EBX
		00454CF6   D905 00829600    FLD DWORD PTR DS:[0x968200]

		iw5mp Base 400000 Size 68a5000 \x83\xEC\x0C\xD9\x05\x00\x00\x00\x00\x53\xD9\x5C\x24\x04\x33\xDB\x39\x1D\x00\x00\x00\x00\xD9\x05\x00\x00\x00\x00 xxxxx????xxxxxxxxx????xx????
		
		                               \x83\xEC\x0C\xD9\x05\x00\x00\x00\x00\x53 xxxxx????x
		now: sub_454DA0
		
		*/

		float *vOrg = (float*)/*0x9681FC*/ Offsets::laser_4;
		if (vOrg == nullptr)
			return;			

		/*
		00454BD0   83EC 34          SUB ESP,0x34 <<<<<<<
		00454BD3   0FB70D 5AA0C801  MOVZX ECX,WORD PTR DS:[0x1C8A05A]
		00454BDA   53               PUSH EBX
		00454BDB   55               PUSH EBP
		00454BDC   8B6C24 44        MOV EBP,DWORD PTR SS:[ESP+0x44]
		00454BE0   56               PUSH ESI
		00454BE1   8B7424 50        MOV ESI,DWORD PTR SS:[ESP+0x50]
		00454BE5   57               PUSH EDI
		00454BE6   8B7C24 50        MOV EDI,DWORD PTR SS:[ESP+0x50]
		00454BEA   8D4424 13        LEA EAX,DWORD PTR SS:[ESP+0x13]
		00454BEE   50               PUSH EAX
		00454BEF   51               PUSH ECX
		00454BF0   81C7 D0000000    ADD EDI,0xD0
		00454BF6   57               PUSH EDI
		00454BF7   55               PUSH EBP
		00454BF8   B3 FE            MOV BL,0xFE
		00454BFA   56               PUSH ESI
		00454BFB   885C24 27        MOV BYTE PTR SS:[ESP+0x27],BL
		00454BFF   E8 9CFEFFFF      CALL iw5mp.00454AA0
		00454C04   83C4 14          ADD ESP,0x14

		iw5mp Base 400000 Size 68a5000 \x83\xEC\x34\x0F\xB7\x0D\x5A\xA0\xC8\x01\x53\x55\x8B\x6C\x24\x44\x56\x8B\x74\x24\x50\x57 xxxxxxxxxxxxxxxxxxxxxx
		
		
		\x83\xEC\x34\x0F xxxx

		now: sub_454C90
		
		
		*/

		DWORD dwCall = /*0x454BD0*/Offsets::laser_5;

		__asm {
			push 1
			push 1
			push vOrg
			push Info
			push Laser
			push pLocalEnt
			push cg
			push 0x0
			call[dwCall]
			add esp, 0x20
		}
	}

}

int Engine_t::AddIconToText(int startLen, int shader, char* text, float iconW, float iconH, char flipIconHorizontal) {


	//\x56\x53\x8B\xF0\xE8\x00\x00\x00\x00\x83\xC4\x04\x8D xxxxx????xxxx
	DWORD dwFunc = Offsets::addicontotext; //didn't changed lol.
	__asm {
		push flipIconHorizontal
		push iconH
		push iconW
		mov edi, text
		mov ebx, shader
		mov eax, startLen
		call dwFunc
		add esp, 0xC
	}
}

char* Engine_t::GetIconText(int shader, float iconW, float iconH, BOOL flipIconHorizontal) {
	static char Buf[64];
	memset(Buf, 0, 64);

	int end = AddIconToText(0, shader, Buf, iconW, iconH, flipIconHorizontal);
	Buf[end] = 0;
	return Buf;
}

bool Engine_t::pVisible(centity_t *cent)
{
	DWORD DW_NAMETAGSV = Offsets::visible;
	__asm
	{
		push    0x2807823//MASK
		push    cent
		push    0
		call    DW_NAMETAGSV
		add     esp, 0x0C
	}
}

bool Engine_t::R_BulletPenetrationCheck(BulletFireParams *MapTrace)
{
	DWORD dwAddr = Offsets::bulletpenetrationcheck;
	__asm
	{
		mov eax, MapTrace
		call dwAddr
		movzx eax, al
	}
}

void Engine_t::R_BulletPenetration(int unk1, BulletFireParams* bulletTrace, int WeaponNum, int unk2, centity_t * cent, int unk3, vec3_t vieworg, int unk4)
{
	DWORD dwCallAddr = Offsets::bulletpenetration;
	__asm
	{
		push unk4
		push vieworg
		push unk3
		push cent
		push unk2
		push WeaponNum
		push bulletTrace
		push unk1
		call dwCallAddr
		add  esp, 0x20
	}
}

void Engine_t::AFK()
{
	static bool bAfk = false;
	if (Settings[afk_mode].Value.bValue && !bAfk)
	{
		key_input->forward.wasPressed = 1;
		key_input->sprint.wasPressed = 1;
		bAfk = true;
	}
	else if (bAfk && !Settings[afk_mode].Value.bValue)
	{
		key_input->forward.wasPressed = 0;
		key_input->sprint.wasPressed = 0;
		bAfk = false;
	}

	/*if (Settings[afk_mode].Value.bValue)
		key_input->forward.wasPressed |= 1; Won't work*/

}

clientInfo_t* Engine_t::GetClient(int i)
{
	return (clientInfo_t*)(Offsets::client + (isTekno ? 0x560 : 0x564) * i);
}
