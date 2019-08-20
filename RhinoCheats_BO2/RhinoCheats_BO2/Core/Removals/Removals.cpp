//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cRemovals _removals;
	
	void cRemovals::NoRecoil() {
		if (_profiler.gNoRecoil.Custom.bValue) {
			RecoilVec->vAngles[0] = 0.0f;
			RecoilVec->vAngles[1] = 0.0f;
			RecoilVec->vAngles[2] = 0.0f;
		}
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::NoSpread(
		usercmd_s* usercmd,
		int servertime
	) {
		if (_profiler.gNoSpread.Custom.bValue) {
			int iSeed = servertime, WeaponInfo = GetWeapon(CG->CEntity[CG->iClientNum].iWeaponID);

			float flSpreadMultiplier = CG->flSpread / 255.0f,
				flMinSpread, flMaxSpread, 
				flSpreadX, flSpreadY, flSpread,
				flRandom1, flRandom2;

			GetSpreadForWeapon(CG->CEntity[CG->iClientNum].iWeaponID, &flMinSpread, &flMaxSpread);

			if (CG->PlayerState.flZoomProgress == 1.0f) {
				flSpread = *(float*)(WeaponInfo + 0x7E0) + ((flMaxSpread - *(float*)(WeaponInfo + 0x7E0)) * flSpreadMultiplier);
			}

			else {
				flSpread = flMinSpread + ((flMaxSpread - flMinSpread) * flSpreadMultiplier);
			}

			HashSeed(&iSeed);
			flRandom1 = RandomFloat(&iSeed);
			SeedRandom(&iSeed);
			flRandom2 = RandomFloat(&iSeed);
			flRandom1 *= M_PI_DOUBLE;

			flSpreadX = cosf(flRandom1) * flRandom2 * flSpread;
			flSpreadY = sinf(flRandom1) * flRandom2 * flSpread;

			usercmd->iViewAngles[0] += AngleToShort(flSpreadY);
			usercmd->iViewAngles[1] += AngleToShort(flSpreadX);
		}
	}
}

//=====================================================================================