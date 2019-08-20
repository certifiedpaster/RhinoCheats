#include "stdafx.h"

NoSpread_t Nospread;

//\x83\xEC\x10\xD9\x05\x00\x00\x00\x00\xD9\x1C xxxxx????xx
void NoSpread_t::FirstBulletFix()
{
	/*float v1, v2, v3, v4;

	v4 = *(float*)0x976D84;

	if ((*(float*)0x978178) != 0.f)
	{
		v4 = *(float*)0x978178 * *(float*)0x976D84;
	}
	
	*(float*)0x1065BE0 = v4;	

	*(float*)0x1065BEC = *(float*)0x9780D4;
	*(float*)0x1065BF0 = *(float*)0x9780E4;
	*(float*)0x1065BF4 = *(float*)0x9780E8;

	//========================================================================	
		
	*(DWORD*)0x977D6C = 0; 		

	v1 = *(float*)0x9780E0 + *(float*)0x1065BD4;
	v2 = *(float*)0x9780D8 + *(float*)0x1065BD8;
	v3 = *(float*)0x9780DC + *(float*)0x1065BDC;
	*(float*)0xFB7410 = v1;
	*(float*)0xFB7414 = v2;
	*(float*)0xFB7418 = v3;

	*(int*)0x1065C7C |= *(int*)0xA058B8;

	*(int*)0xA058B8 = 0;*/
		
	//crash: ((void(__cdecl*)(void *a1, int a2))0x448990)(0, 3);
}

void NoSpread_t::GetWeaponSpread(float*Spread)
{
	CWeapon *pWeapon = (CWeapon *)(Engine.GetCWeapon(cg_entities[cg->clientNum].weaponID));
	if (!pWeapon) {
		*Spread = 0.0f;
		return;
	}

	R_WeaponSpread(cg, pWeapon, &baseSpread, &maxSpread);
	*Spread = baseSpread + ((maxSpread - baseSpread) * (*reinterpret_cast<float*>(Offsets::nospread_numerator) / 255.0f));
}

//========================================================================

/*void GetRandomSpread(int iSeed, float* flSpreadRight, float* flSpreadUp)
{
	DWORD dwCall = Offsets::randomspread;
	int iSpreadSeed = 214013 * (214013 * (214013 * (214013 * iSeed + 2531011) + 2531011) + 2531011) + 2531011;
	__asm
	{
		lea eax, dword ptr ds : [flSpreadUp];
		push[eax];
		lea ecx, dword ptr ds : [flSpreadRight];
		push[ecx];
		lea esi, dword ptr ds : [iSpreadSeed];
		call dword ptr ds : [dwCall];
		add esp, 0x8
	}
}

void GetWeaponSpreadVector(vec3_t vWeaponStartPos, vec3_t endVec, int iSeed, float SpreadAngle, vec3_t vecForward, vec3_t vecRight, vec3_t vecUp)
{
	float vRight, vUp = 0.0f;
	GetRandomSpread(iSeed, &vRight, &vUp);

	float flSpreadAngle = tan(Math.Radians(SpreadAngle)) * WEAPON_DISTANCE;

	vRight *= flSpreadAngle;
	vUp *= flSpreadAngle;

	endVec[0] = vecForward[0] * WEAPON_DISTANCE + vWeaponStartPos[0] + (vecRight[0] * vRight) + (vecUp[0] * vUp);
	endVec[1] = vecForward[1] * WEAPON_DISTANCE + vWeaponStartPos[1] + (vecRight[1] * vRight) + (vecUp[1] * vUp);
	endVec[2] = vecForward[2] * WEAPON_DISTANCE + vWeaponStartPos[2] + (vecRight[2] * vRight) + (vecUp[2] * vUp);
}
*/
//========================================================================


void NoSpread_t::ApplyNoSpread(usercmd_t *cmd, int seed)
{	
	if (Settings[no_spread].Value.bValue && 
		!ci[cg->clientNum].zooming)
	{			
		// FirstBulletFix();

		GetWeaponSpread(&weaponSpread);		

		/*vec3_t WeaponViewAngle, ForwardVec, RightVec, UpVec, Spreaded, SpreadedAngles, Fix;
		WeaponViewAngle[0] = punch->weaponViewAngle_0;
		WeaponViewAngle[1] = punch->weaponViewAngle_1;
		WeaponViewAngle[2] = 0.0f;	

		Math.AngleVectors(WeaponViewAngle, ForwardVec, RightVec, UpVec);
		GetWeaponSpreadVector(refdef->vieworg, Spreaded, seed, weaponSpread, ForwardVec, RightVec, UpVec);

		VectorSubtract(Spreaded, refdef->vieworg, Spreaded);
		Math.VecToAngles(Spreaded, SpreadedAngles);
		VectorSubtract(WeaponViewAngle, SpreadedAngles, Fix);	


		if (Settings[silent_aim].Value.bValue &&
			Aim.isReady[Aim_t::isReadyforFire] &&			
			!Settings[third_person].Value.bValue)
		{
			cmd->viewangles[1] = ANGLE2SHORT(Fix[1] + pViewMatrix->viewAngleX + Aim.vAimAngles[1]);
			cmd->viewangles[0] = ANGLE2SHORT(Fix[0] + pViewMatrix->viewAngleY + Aim.vAimAngles[0]);
		}
		else  //Normal Spread Fix		
		{  
			cmd->viewangles[1] = ANGLE2SHORT(Fix[1] + pViewMatrix->viewAngleX);
			cmd->viewangles[0] = ANGLE2SHORT(Fix[0] + pViewMatrix->viewAngleY);			
		}*/
		
		//========================================================================
		//By OGCz, great job mate congratz :)

		int SeedTransform = 214013 * (214013 * (214013 * (214013 * (214013 * seed + 2531011) + 2531011) + 2531011) + 2531011) + 2531011;

		//double random1 = ((unsigned int)SeedTransform >> 17)*0.000030517578125*360.0*0.017453292384369; <<<

		double random1 = ((unsigned int)SeedTransform >> 17)*0.000030517578125*360.0*0.01745329238474369; //from the game...

	    //sub_421E40                                                                   
		double random2 = ((unsigned int)(214013 * SeedTransform + 2531011) >> 17)*0.000030517578125;

		spreadX = cos(random1)*random2*-weaponSpread;
		spreadY = sin(random1)*random2*-weaponSpread;

		cmd->viewangles[0] -= ANGLE2SHORT(spreadY);
		cmd->viewangles[1] -= ANGLE2SHORT(spreadX);	
	
	}

}