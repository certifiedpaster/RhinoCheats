#include "stdafx.h"

dvar_t *perk_bulletPenetrationMultiplier = nullptr;
dvar_t *com_maxfps = nullptr;
dvar_t *cg_fov = nullptr;
dvar_t *cg_thirdPerson = nullptr;

dvar_t *cg_gun_x = nullptr;
dvar_t *cg_gun_y = nullptr;
dvar_t *cg_gun_z = nullptr;

//========================================================================

stunflash_t*          concGrenade = nullptr;
stunflash_t*         flashGrenade = nullptr;

centity_t            *cg_entities = nullptr;
clientInfo_t                  *ci = nullptr;
cg_t	                      *cg = nullptr;
cgs_t                        *cgs = nullptr;
refdef_t                  *refdef = nullptr;
viewmatrix_t         *pViewMatrix = nullptr;
Fonts_t                    *Fonts = nullptr;
Fonts_Dev_t            *fonts_Dev = nullptr;
int                    **ServerID = nullptr;
input_t                   *pinput = nullptr;
stats_t*                   pStats = nullptr;
/*ServerSet_t            *ServerSet = nullptr;
entityInfo_s    *ServerEntityInfo = nullptr;
Health_s            *ServerHealth = nullptr;
*/
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
//Cmd_AddCommandInternal_t CMD_AddCommand = nullptr;

Cbuf_AddText_t Cbuf_AddText = nullptr;

//Autowall special functions...
BG_WeaponBulletFire_GetMethodOfDeath_t pWeaponImpacttype = nullptr;
BG_GetSurfacePenetrationDepth_t pfnGetBulletPenetration = nullptr;
Com_Memcpy_t pfnCopyTrace = nullptr;
BG_AdvanceTrace_t pfnStepForward = nullptr;

/*tsub_46B370 psub_46B370 = nullptr;
tsub_52A190 psub_52A190 = nullptr;
tsub_4F0CF0 psub_4F0CF0 = nullptr;
tsub_4F0CF0 psub_489F70 = nullptr;*/

//========================================================================

qhandle_t HeadShotIcon = 0;
qhandle_t DiedIcon = 0;
qhandle_t ScavengerIcon = 0;
qhandle_t FlashbangIcon = 0;
qhandle_t CompassPlayerIcon = 0;

//========================================================================
//Force Classes

char DoCmd[30];

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
		SafeCreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)TeamSpectator, nullptr, 0, nullptr);	
	if (g_bEND)
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
	/*DWORD dwCall = Offsets::gettag_offset; return: void*

	__asm {
		MOV ESI, pEnt;
		MOVZX EDI, wBone;
		PUSH vOrigin;
		CALL[dwCall];
		ADD ESP, 0x4
	}*/

	//"cg_TeamColor", "cg_ColorBlind"
	typedef char *(__cdecl *sub_556940_t)(int a1);
	char *v3 = ((sub_556940_t)(/*0x556940*/ 0x56B8E0))(pEnt->clientnum);
	if (v3)
	{
		typedef int *(__cdecl *sub_44D910_t)(centity_t * a1, char * a2, WORD a3, vec3_t a4);
		if (!((sub_44D910_t)/*0x44D910*/ 0x48E720)(pEnt, v3, wBone, vOrigin))
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
	/*int __cdecl sub_479520(int a1)
	{
		return (int)*(&off_86CE20 + a1);
	}*/	

	// Correct, only for esp, name check.
	typedef DWORD(__cdecl* t_gcw)(int i);
	t_gcw gwc = (t_gcw)0x479520;
	return (weapon_t*)gwc(Index);

}

CWeapon* Engine_t::GetCWeapon(int i)
{
	/*
	int __cdecl sub_479510(int a1)
	{
		return dword_86F9F0[a1];
	}*/

	typedef CWeapon*(__cdecl* dw_getWeapon)(int i);
	dw_getWeapon getWeapon = (dw_getWeapon)0x479510;	//<<< correct
	return getWeapon(i);
}


bool Engine_t::WorldToScreen(vec3_t vWorldLocation, float Screen[2])
{
	//return WorldToScreenE(0, GSM(0), vWorldLocation, Screen);

	vec3_t local, transformed;

	VectorSubtract(vWorldLocation, refdef->vieworg, local);

	transformed[0] = DotProduct(local, refdef->viewaxis[1]);
	transformed[1] = DotProduct(local, refdef->viewaxis[2]);
	transformed[2] = DotProduct(local, refdef->viewaxis[0]);

	if (transformed[2] < 0.01f) {
		return false;
	}

	Screen[0] = ((refdef->width / 2) * (1 - (transformed[0] / refdef->fov_x / transformed[2])));
	Screen[1] = ((refdef->height / 2) * (1 - (transformed[1] / refdef->fov_y / transformed[2])));
	return true;	 
	

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

dvar_t* Engine_t::FindVar(char * name)
{
	for (int i = 0; i < *(int*)Offsets::dvarsize; ++i)
	{
		dvar_t * pVar = (dvar_t*)(Offsets::dvarbase + 0x4C * i);
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

	dvar_t *pSpecial = nullptr;

	//Tested, camera_thirdPersonOffset not needed, but lets see:
	pSpecial = FindVar("camera_thirdPersonOffset");
	XASSERT(pSpecial);
	pSpecial->setFloat(-140);

	pSpecial = FindVar("camera_thirdPersonCrosshairOffset");
	XASSERT(pSpecial);
	pSpecial->setInt(0);

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

	

	pSpecial = FindVar("bg_viewBobAmplitudeBase");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeSprinting");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeStanding");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeStandingAds");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeDucked");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeDuckedAds");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeProne");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobAmplitudeRoll");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobMax");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_viewBobLag");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);


	pSpecial = FindVar("bg_weaponBobAmplitudeBase");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobAmplitudeSprinting");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobAmplitudeStanding");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobAmplitudeDucked");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobAmplitudeProne");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobAmplitudeRoll");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobMax");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("bg_weaponBobLag");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("player_sprintCameraBob");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);

	pSpecial = FindVar("perk_lightWeightViewBobScale");
	XASSERT(pSpecial);
	pSpecial->setFloat(0);


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

	if (cg_fov)
		cg_fov->setFloat(Settings[force_fov].Value.fValue);

	if (cg_thirdPerson && Settings[third_person].Value.bValue)
		cg_thirdPerson->setInt(1);
	else
		cg_thirdPerson->setInt(0);

	if (cg_gun_x)
		cg_gun_x->setFloat(Settings[gun_x].Value.fValue);

	if (cg_gun_y)
		cg_gun_y->setFloat(Settings[gun_y].Value.fValue);

	if (cg_gun_z)
		cg_gun_z->setFloat(Settings[gun_z].Value.fValue);

	//========================================================================	

	/*weapon_t *pMYWEAPON = Engine.GetCombatWeapon(cg_entities[cg->clientNum].weaponID);
	if (pMYWEAPON)
	{
		if (Settings[drunken_style].Value.bValue)
		{
			pMYWEAPON->info->swayMultiplier = 1080.0f;
			pMYWEAPON->info->swayMultiplierADS = 1080.0f;
		}
		else
		{
			pMYWEAPON->info->swayMultiplier = 0;
			pMYWEAPON->info->swayMultiplierADS = 0;
		}
	}*/
}

void Engine_t::DrawLaser()
{
	if (!Settings[laser].Value.bValue)
		return;
	if (Settings[third_person].Value.bValue)
		return;
	if (Aim.isVehicle)
		return;


	centity_t *pLocalEnt = &cg_entities[cg->clientNum];
	if (pLocalEnt == nullptr)
		return;

	if (pLocalEnt->valid && (pLocalEnt->IsAlive & 1))
	{
		void* Laser = (void*)(*(DWORD*)0xAA6188);
		if (Laser == nullptr)
			return;

		void *Info = (void*)0x99D5F8;
		if (Info == nullptr)
			return;

		float *vOrg = (float*)0x90B694;
		if (vOrg == nullptr)
			return;

		DWORD dwCall = 0x495370;

		__asm {
			push 1
			push 1
			push vOrg
			push Info
			push Laser
			push pLocalEnt
			push cg
			call[dwCall]
			add esp, 0x1C
		}
	}

}

int Engine_t::AddIconToText(int startLen, int shader, char* text, float iconW, float iconH, char flipIconHorizontal) {


	DWORD dwFunc = 0x4BE910;
	__asm {
		push flipIconHorizontal
		push iconH
		push iconW
		push shader
		mov ecx, text
		mov eax, startLen
		call dwFunc
		add esp, 0x10
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
	/*for mw3
	DWORD DW_NAMETAGSV = Offsets::visible;
	__asm
	{
		push    0x2807823//MASK
		push    cent
		push    0
		call    DW_NAMETAGSV
		add     esp, 0x0C
	}*/

	return false;
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

void Engine_t::R_BulletPenetration(int unk1, BulletFireParams* bulletTrace, int WeaponNum, centity_t * cent, int unk5, vec3_t vieworg, int unk7)
{
	DWORD dwCallAddr = Offsets::bulletpenetration;
	__asm
	{
		push unk7
		push vieworg
		push unk5
		push cent
		push WeaponNum
		push bulletTrace
		push unk1
		call dwCallAddr
		add  esp, 0x1C
	}
}

void Engine_t::AFK()
{

	static bool bAfk = false;
	if (Settings[afk_mode].Value.bValue && !bAfk)
	{
		key_input->forward.wasPressed = 1;
		key_input->shift_2.wasPressed = 1;
		bAfk = true;
	}
	else if (bAfk && !Settings[afk_mode].Value.bValue)
	{
		key_input->forward.wasPressed = 0;
		key_input->shift_2.wasPressed = 0;
		bAfk = false;
	}

}


