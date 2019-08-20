#include "stdafx.h"


centity_t            *cg_entities = nullptr;
clientInfo_t                  *ci = nullptr;
cg_t	                      *cg = nullptr;

refdef_t                  *refdef = nullptr;
rdViewAngles_t      *rdViewAngles = nullptr;

Fonts_t                    *Fonts = nullptr;
Fonts_Dev_t            *Fonts_Dev = nullptr;

//========================================================================

R_AddCmdDrawText_t R_AddCmdDrawText = nullptr;
R_AddDrawStretchPic_t  R_AddDrawStretchPic = nullptr;
CG_DrawRotatedPicPhysical_t DrawRotatedPic = nullptr;
RegisterTag_t Rtag = nullptr;
BG_GetWeapondef_t BG_GetWeapondef = nullptr;
GetWeapon_t GetWeapon = nullptr;

//========================================================================

Engine_t Engine;


void Engine_t::DamageFeedBack_Removal() {

	float *v2 = (float *)Offsets::cg;

	//Punch
	v2[0x17BD9] = 0.0f;
	v2[0x17BD8] = 0.0f;
}

unsigned int Engine_t::RegisterTag(const char* szBone)
{	
	return Rtag(szBone);
}

int Engine_t::Getvectorfromtag(centity_t * pEnt, WORD wBone, vec3_t vOrigin)
{
	if (!wBone)
		return false;

	typedef DWORD(__cdecl *BoneInfo_t)(DWORD unk1, DWORD unk2);
	DWORD unk = ((BoneInfo_t)(Offsets::Gettag1))(*(DWORD *)((DWORD)pEnt + 0x1E8), *(DWORD *)((DWORD)pEnt + 0x4));
	if (unk)
	{
		typedef int(__cdecl *GetTag_t)(centity_t *a1, DWORD a2, WORD a3, vec3_t a4);
		if (!((GetTag_t)(Offsets::Gettag2))(pEnt, unk, wBone, vOrigin))
			return false;
	}

	return true;
}

bool Engine_t::WorldToScreen(vec3_t vWorldLocation, float Screen[2])
{
	vec3_t vLocal, vTransForm;
	VectorSubtract(vWorldLocation, refdef->vieworg, vLocal);

	vTransForm[0] = DotProduct(vLocal, refdef->viewaxis[1]);
	vTransForm[1] = DotProduct(vLocal, refdef->viewaxis[2]);
	vTransForm[2] = DotProduct(vLocal, refdef->viewaxis[0]);

	if (vTransForm[2] < 0.01f) {
		return false;
	}

	Screen[0] = ((refdef->width / 2) * (1 - (vTransForm[0] / refdef->fov[0] / vTransForm[2])));
	Screen[1] = ((refdef->height / 2) * (1 - (vTransForm[1] / refdef->fov[1] / vTransForm[2])));

	return true;
}



