#include "stdafx.h"

NoSpread_t Nospread;

//\x83\xEC\x10\xD9\x05\x00\x00\x00\x00\xD9\x1C xxxxx????xx
void NoSpread_t::FirstBulletFix()
{	
	float v3, v4, v5, v6;

	v3 = *(float*)Offsets::fb1;

	if (*(float*)Offsets::fb2 != 0.0)
		v3 = *(float*)Offsets::fb2 * *(float*)Offsets::fb1;

	*(float*)Offsets::fb3 = v3;

	v4 = *(float*)Offsets::fb4 + *(float*)Offsets::fb5;
	v5 = *(float*)Offsets::fb6 + *(float*)Offsets::fb7;
	v6 = *(float*)Offsets::fb8 + *(float*)Offsets::fb9;

	*(float*)Offsets::fb10 = v4;
	*(float*)Offsets::fb11 = v5;
	*(float*)Offsets::fb12 = v6;

	*(int*)Offsets::fb13 = *(int*)Offsets::fb14;
	*(int*)Offsets::fb15 = *(int*)Offsets::fb16;
	*(int*)Offsets::fb17 = *(int*)Offsets::fb18 != 0;

	*(int*)Offsets::fb19 |= *(int*)Offsets::fb20;
	*(int*)Offsets::fb20 = 0;
}

void NoSpread_t::GetWeaponSpread(float*Spread)
{
	R_WeaponSpread(cg, cg_entities[cg->clientNum].nextState.weaponID, &baseSpread, &maxSpread);
	*Spread = baseSpread + ((maxSpread - baseSpread) * (*reinterpret_cast<float*>(Offsets::nospread_numerator) / 255.0f));
}

void NoSpread_t::ApplyNoSpread(usercmd_t *cmd, int seed)
{	
	if (Settings[no_spread].Value.bValue &&  
		*(BYTE *)Offsets::zoom == NULL)
	{		
		FirstBulletFix();

		GetWeaponSpread(&weaponSpread);

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