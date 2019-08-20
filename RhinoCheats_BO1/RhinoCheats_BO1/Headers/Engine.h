#pragma once

typedef void(*R_AddCmdDrawText_t)(const char *text, int maxChars, qfont_t font, float x, float y, float xScale, float yScale, float rotation, float* color, int style);
extern R_AddCmdDrawText_t R_AddCmdDrawText;

typedef void(*R_AddDrawStretchPic_t)(float x, float y, float w, float h, float s1, float t1, float s2, float t2, float* color, qhandle_t hShader);
extern R_AddDrawStretchPic_t  R_AddDrawStretchPic;

typedef int(__cdecl* CG_DrawRotatedPicPhysical_t) (float x, float y, float w, float h, float a5, float a6, float a7, float a8, float angle, float* color, int shader);
extern CG_DrawRotatedPicPhysical_t DrawRotatedPic;

typedef unsigned int(*RegisterTag_t)(const char* name);
extern RegisterTag_t Rtag;

typedef WeaponDef_t* (*BG_GetWeapondef_t)(__int16 weaponid);
extern BG_GetWeapondef_t BG_GetWeapondef;

typedef weapon_t* (*GetWeapon_t)(__int16 id);
extern GetWeapon_t GetWeapon;


//========================================================================

class Engine_t {

public:		

	void           DamageFeedBack_Removal();
	
	unsigned int   RegisterTag(const char * szBone);
	int            Getvectorfromtag(centity_t * pEnt, WORD wBone, vec3_t vOrigin);

	bool           WorldToScreen(vec3_t vWorldLocation, float Screen[2]);		
	
};

extern Engine_t Engine;








