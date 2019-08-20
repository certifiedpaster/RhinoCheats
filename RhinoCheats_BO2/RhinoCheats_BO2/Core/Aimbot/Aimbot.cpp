//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cAimbot _aimBot;

	void cAimbot::StandardAim() {
		if (!_profiler.gSilentAim.Custom.bValue && AimState.iTargetNum > -1) {
			AimState.vAimbotAngles[0] *= _profiler.gAimStrength.Custom.iValue / 100.0f;
			AimState.vAimbotAngles[1] *= _profiler.gAimStrength.Custom.iValue / 100.0f;

			if (_profiler.gAutoaimTime.Custom.iValue) {
				AimState.vAimbotAngles[0] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoaimTime.Custom.iValue;
				AimState.vAimbotAngles[1] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoaimTime.Custom.iValue;
			}

			if (AimState.iCurrentAimDelay == _profiler.gAutoaimDelay.Custom.iValue) {
				if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC || (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CG->CEntity[CG->iClientNum].iFlags & EF_ZOOM)) {
					ViewAngle->vAngles[0] += AimState.vAimbotAngles[0];
					ViewAngle->vAngles[1] += AimState.vAimbotAngles[1];
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
	void cAimbot::SilentAim(
		usercmd_s* usercmd
	) {
		if (_profiler.gSilentAim.Custom.bValue && AimState.iTargetNum > -1) {
			if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC || (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CG->CEntity[CG->iClientNum].iFlags & EF_ZOOM)) {
				usercmd->iViewAngles[0] += AngleToShort(AimState.vAimbotAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(AimState.vAimbotAngles[1]);

				//_mathematics.MovementFix(usercmd, AimState.vAimbotAngles[1]);
			}

			if (AimState.iCurrentZoomDelay == _profiler.gAutozoomDelay.Custom.iValue)
				if (_profiler.gAutozoom.Custom.bValue && _profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::AutoFire(
		usercmd_s* usercmd
	) {
		if (AimState.iCurrentFireDelay == _profiler.gAutofireDelay.Custom.iValue && AimState.iTargetNum > -1) {
			if (_profiler.gAutofire.Custom.bValue && (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC || (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CG->CEntity[CG->iClientNum].iFlags & EF_ZOOM))) {
				usercmd->iButtons[0] |= 0x80000000;
				usercmd->iButtons[1] |= 0x20000000;

				if (WeaponIsAkimbo(CG->CEntity[CG->iClientNum].iWeaponID))
					usercmd->iButtons[0] |= 0x100080;
			}
		}
	}
}

//=====================================================================================