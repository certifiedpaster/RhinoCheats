#pragma once

//========================================================================
//Engine

typedef void(__cdecl *G_TraceCapsule_t)(trace_t *result, vec3_t Start, vec3_t End, vec3_t bounding, int skipNumber, int mask);
extern G_TraceCapsule_t Trace;

typedef double(__cdecl *VectorNormalize_t)(vec3_t a1);
extern VectorNormalize_t VectorNormalize;

typedef void(__cdecl* BG_GetSpreadForWeapon_t)(cg_t* cg, int weapon, float*BaseSpread, float*MaxSpread);
extern BG_GetSpreadForWeapon_t R_WeaponSpread;

typedef void(__cdecl* Cbuf_AddText_t)(int a1, char* text);
extern Cbuf_AddText_t Cbuf_AddText;

//========================================================================
//Drawing

typedef ScreenMatrix* (__cdecl *ScrPlace_GetActivePlacement_t)();
extern ScrPlace_GetActivePlacement_t GSM;

typedef bool(__cdecl *CG_WorldPosToScreenPosReal_t)(int, ScreenMatrix*, vec3_t, float*);
extern CG_WorldPosToScreenPosReal_t WorldToScreenE;

typedef int(__cdecl* CG_DrawRotatedPicPhysical_t) (ScreenMatrix* ptr, float x, float y, float w, float h, float angle, float* color, int shader);
extern CG_DrawRotatedPicPhysical_t DrawRotatedPic;

typedef int(*Material_RegisterHandle_t)(char * szShader);
extern Material_RegisterHandle_t RegisterShader;

typedef int(*CL_DrawTextPhysicalWithEffects_t)(char*, int, qfont_t, float, float, float, float, float*, float, float*, int, int, int, int, int, int);
extern CL_DrawTextPhysicalWithEffects_t DrawGlowText;

typedef int(*R_TextWidth_t)(char* a1, int a2, void *a3, int a4);
extern R_TextWidth_t StrWidth;

typedef int(*R_TextHeight_t)(void *font);
extern R_TextHeight_t StrHeight;

typedef void(__cdecl * Scr_AddString_t)(char* string);
extern Scr_AddString_t pScr_AddString;

typedef char(__cdecl *CL_KeyEvent_t)(int a1, int a2, int a3);
extern CL_KeyEvent_t CL_KeyEvent;

typedef void(__cdecl *t_CL_DrawStretchPic)(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float s1, float t1, float s2, float t2, /*const*/ float *color, void *material);
extern t_CL_DrawStretchPic CL_DrawStretchPic;

//========================================================================
//Autowall

typedef int(__cdecl *BG_WeaponBulletFire_GetMethodOfDeath_t)(void *a1, unsigned int a2, int a3);
extern BG_WeaponBulletFire_GetMethodOfDeath_t pWeaponImpacttype;

typedef float(__cdecl * BG_GetSurfacePenetrationDepth_t)(int iWeaponNum, int, int iMaterialType);
extern BG_GetSurfacePenetrationDepth_t pfnGetBulletPenetration;

typedef bool(__cdecl *Com_Memcpy_t)(void* a1, void* a2, int a3);
extern Com_Memcpy_t pfnCopyTrace;

typedef bool(__cdecl *BG_AdvanceTrace_t)(BulletFireParams* a1, trace_t* a2, float a3);
extern BG_AdvanceTrace_t pfnStepForward;

typedef int(__cdecl *tsub_46B370)(unsigned int a1, int a2);
extern tsub_46B370 psub_46B370;
typedef bool(__cdecl *tsub_52A190)(void *a1, unsigned int a2, int a3);
extern tsub_52A190 psub_52A190;
typedef bool(__cdecl *tsub_4F0CF0)();
extern tsub_4F0CF0 psub_4F0CF0;
extern tsub_4F0CF0 psub_489F70;

//========================================================================

class Engine_t {

public:		
	weapon_t *GetCombatWeapon(int Index);
	WORD      RegisterTag(const char * szBone);
	dvar_s   *FindVar(char * name);
	bool      WorldToScreen(vec3_t vWorldLocation, float Screen[2]);	
	bool      Getvectorfromtag(centity_t * pEnt, WORD wBone, vec3_t vOrigin);	
	void      FuckStunFlashShit(stunflash_t * sfData);	
	void      ApplyDvars();
	void      SendToConsole(char* Cmd);	
	void      DamageFeedBack_Removal();
	void      SwitchTeamProcess();		
	void      Misc_Process();
	void      DrawLaser();
	int       AddIconToText(int startLen, int shader, char * text, float iconW, float iconH, char flipIconHorizontal);
	char     *GetIconText(int shader, float iconW, float iconH, BOOL flipIconHorizontal);
	bool      pVisible(centity_t * cent);
	bool      R_BulletPenetrationCheck(BulletFireParams * MapTrace);
	void      R_BulletPenetration(int unk1, BulletFireParams * bulletTrace, int WeaponNum, int unk2, centity_t * cent, int unk3, vec3_t vieworg, int unk4);
	void      AFK();
	clientInfo_t* GetClient(int i);

};

extern Engine_t Engine;

extern bool g_bF1;
extern bool g_bF2;
extern bool g_bF3;
extern bool g_bF4;
extern bool g_bF5;
extern bool g_bF6;
extern bool g_bF7;
extern bool g_bF8;
extern bool g_bF9;
extern bool g_bF10;
extern bool g_bEND;

extern bool g_bNumPad0;
extern bool g_bNumPad1;
extern bool g_bNumPad2;
extern bool g_bNumPad3;
extern bool g_bNumPad4;
extern bool g_bNumPad5;
extern bool g_bNumPad6;
extern bool g_bNumPad7;
extern bool g_bNumPad8;
extern bool g_bNumPad9;







