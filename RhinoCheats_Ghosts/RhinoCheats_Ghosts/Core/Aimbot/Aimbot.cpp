//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cAimbot _aimBot;

	void cAimbot::StandardAim()
	{
		if ((!_profiler.gSilentAim.Custom.bValue || UsingKillstreak(GetCurrentWeapon(CG))) && AimState.iTargetNum > -1)
		{
			AimState.vAimbotAngles[0] *= _profiler.gAimStrength.Custom.iValue / 100.0f;
			AimState.vAimbotAngles[1] *= _profiler.gAimStrength.Custom.iValue / 100.0f;

			if (_profiler.gAutoaimTime.Custom.iValue)
			{
				AimState.vAimbotAngles[0] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoaimTime.Custom.iValue;
				AimState.vAimbotAngles[1] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoaimTime.Custom.iValue;
			}

			if (AimState.iCurrentAimDelay == _profiler.gAutoaimDelay.Custom.iValue)
			{
				if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
					(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL &&
						CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM))
				{
					ViewMatrix->vViewAngles[0] += AimState.vAimbotAngles[0];
					ViewMatrix->vViewAngles[1] += AimState.vAimbotAngles[1];
				}
			}

			if (AimState.iCurrentZoomDelay == _profiler.gAutozoomDelay.Custom.iValue)
				if (_profiler.gAutozoom.Custom.bValue && _profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::SilentAim(sUserCMD* usercmd)
	{
		if (_profiler.gSilentAim.Custom.bValue && !UsingKillstreak(GetCurrentWeapon(CG)) && AimState.iTargetNum > -1)
		{
			if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
				(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL &&
					CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM))
			{
				usercmd->iViewAngles[0] += AngleToShort(AimState.vAimbotAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(AimState.vAimbotAngles[1]);

				_mathematics.MovementFix(usercmd, AimState.vAimbotAngles[1]);
			}

			if (AimState.iCurrentZoomDelay == _profiler.gAutozoomDelay.Custom.iValue)
				if (_profiler.gAutozoom.Custom.bValue && _profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::AutoFire(sUserCMD* usercmd)
	{
		if (AimState.iCurrentFireDelay == _profiler.gAutofireDelay.Custom.iValue && AimState.iTargetNum > -1)
		{
			if (_profiler.gAutofire.Custom.bValue && (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
				(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM)))
			{
				if (WeaponIsAkimbo(GetCurrentWeapon(CG)))
				{
					if (AimState.bAkimbo)
						usercmd->iButtons |= BUTTON_FIRELEFT;

					else
						usercmd->iButtons |= BUTTON_FIRERIGHT;
				}

				else
					usercmd->iButtons |= BUTTON_FIRELEFT;
			}
		}
	}
}

//=====================================================================================