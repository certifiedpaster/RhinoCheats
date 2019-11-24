#include "stdafx.h"

NoSpread_t Nospread;

//\x83\xEC\x10\xD9\x05\x00\x00\x00\x00\xD9\x1C xxxxx????xx
void NoSpread_t::FirstBulletFix()
{
	float v3, v4, v5, v6;

	v3 = *(float*)0x91362C;

	if (*(float*)0x914E3C != 0.0)
		v3 = *(float*)0x914E3C * *(float*)0x91362C;

	*(float*)0xBC7704 = v3;

	v4 = *(float*)0x914DA0 + *(float*)0x914D94;
	v5 = *(float*)0x914DA4 + *(float*)0x914D98;
	v6 = *(float*)0x914DA8 + *(float*)0x914D9C;

	*(float*)0xBC7710 = v4;
	*(float*)0xBC7714 = v5;
	*(float*)0xBC7718 = v6;

	*(int*)0xBC76FC = *(int*)0x913F1C;
	*(int*)0xBC7700 = *(int*)0x914A24;

	*(int*)0xBC7748 |= *(int*)0x99E1C4;
	*(int*)0x99E1C4 = 0;
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
		FirstBulletFix();

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