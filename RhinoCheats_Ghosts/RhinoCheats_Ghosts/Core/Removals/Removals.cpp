//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cRemovals _removals;

	void cRemovals::NoRecoil()
	{
		if (_profiler.gNoRecoil.Custom.bValue)
		{
			ZeroMemory(Punch->vPunchAngles, sizeof(Vector2));
			ZeroMemory(ViewMatrix->vRecoilAngles, sizeof(Vector3));
			ZeroMemory(Punch->vWeaponPunchAngles, sizeof(Vector3[3]));
		}
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::NoSpreadStandardAim(sUserCMD* usercmd, bool akimbo)
	{
		if (_profiler.gNoSpread.Custom.bValue)
		{
			FirstBulletFix();

			Vector3 vAngles;

			GetSpreadAngles(akimbo, usercmd->iServerTime, GetWeaponSpread(), vAngles);

			usercmd->iViewAngles[0] += AngleToShort((UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vRefDefViewAngles[0] : CG->vWeaponAngles[0]) - vAngles[0]);
			usercmd->iViewAngles[1] += AngleToShort((UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vRefDefViewAngles[1] : CG->vWeaponAngles[1]) - vAngles[1]);
		}
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::NoSpreadSilentAim(sUserCMD* usercmd, bool akimbo)
	{
		if (_profiler.gNoSpread.Custom.bValue)
		{
			FirstBulletFix();

			int iSeed = TransformSeed(akimbo, usercmd->iServerTime);
			float flSpread = GetWeaponSpread(), flSpreadX, flSpreadY;

			GetRandomFloats(&iSeed, &flSpreadX, &flSpreadY);

			flSpreadX *= flSpread;
			flSpreadY *= flSpread;

			usercmd->iViewAngles[0] += AngleToShort(flSpreadY);
			usercmd->iViewAngles[1] += AngleToShort(flSpreadX);
		}
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::GetSpreadAngles(bool akimbo, int servertime, float spread, Vector3 angles)
	{
		Vector3 vForward, vRight, vUp, vEnd, vDir;
		int iSeed = TransformSeed(akimbo, servertime);

		AngleVectors(UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vRefDefViewAngles : CG->vWeaponAngles, vForward, vRight, vUp);
		BulletEndPosition(&iSeed, spread, RefDef->vViewOrg, vEnd, vDir, vForward, vRight, vUp);

		VectorAngles(vDir, angles);
		_mathematics.NormalizeAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::GetRandomFloats(int* seed, float* spreadx, float* spready)
	{
		float flRandom1 = RandomFloat(seed), 
			flRandom2 = RandomFloat(seed);

		flRandom1 *= M_PI_DOUBLE;

		*spreadx = cosf(flRandom1) * flRandom2;
		*spready = sinf(flRandom1) * flRandom2;
	}
	/*
	//=====================================================================================
	*/
	int cRemovals::TransformSeed(bool akimbo, int servertime)
	{
		int iServerTime = servertime;

		if (akimbo)
			iServerTime += 10;

		return SeedRandom(&iServerTime);
	}
	/*
	//=====================================================================================
	*/
	float cRemovals::GetWeaponSpread()
	{
		float flSpreadMultiplier = *(float*)OFF_SPREADMULTIPLIER / 255.0f, 
			flZoomSpread = GetZoomSpreadForWeapon(CG, CEntity[CG->iClientNum].EntityState.iWeapon, CEntity[CG->iClientNum].EntityState.iInAltWeaponMode),
			flMinSpread,
			flMaxSpread,
			flSpread;

		GetHipfireSpreadForWeapon(CG, GetCurrentWeapon(CG), &flMinSpread, &flMaxSpread);

		if (CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM)
		{
			if (*(float*)OFF_ZOOMMULTIPLIER == 1.0f)
				flSpreadMultiplier = 0.0f;

			flSpread = ((((flMaxSpread - flMinSpread) * flSpreadMultiplier) + flMinSpread) * (1.0f - *(float*)OFF_ZOOMMULTIPLIER))
				+ ((((flMaxSpread - flZoomSpread) * flSpreadMultiplier) + flZoomSpread) * *(float*)OFF_ZOOMMULTIPLIER);
		}

		else
			flSpread = ((flMaxSpread - flMinSpread) * flSpreadMultiplier) + flMinSpread;

		return flSpread;
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::FirstBulletFix()
	{
		float v1, v2, v3, v4;

		v1 = *(float*)0x141820718;

		if (*(float*)0x141823F20 != 0.0)
			v1 = *(float*)0x141820718 * *(float*)0x141823F20;

		*(float*)0x1419E51D8 = v1;

		v2 = *(float*)0x141823E80 + *(float*)0x141823E74;
		v3 = *(float*)0x141823E84 + *(float*)0x141823E78;
		v4 = *(float*)0x141823E88 + *(float*)0x141823E7C;

		*(float*)0x1419E51E4 = v2;
		*(float*)0x1419E51E8 = v3;
		*(float*)0x1419E51EC = v4;

		*(int*)0x1419E51CC = *(int*)0x141821368;
		*(int*)0x1419E51D0 = *(int*)0x141823B0C;
		*(int*)0x1419E51D4 = *(int*)0x14182136C != 0;

		*(int*)0x1419E5278 |= *(int*)0x14187D42C;
		*(int*)0x14187D42C = 0;
	}
}

//=====================================================================================