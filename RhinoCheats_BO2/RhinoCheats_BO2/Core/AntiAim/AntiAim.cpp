//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cAntiAim _antiAim;

	void cAntiAim::AntiAim(
		usercmd_s* usercmd
	) {
		if (CG->CEntity[CG->iClientNum].iWeaponID &&
			!(CG->CEntity[CG->iClientNum].iFlags & EF_PRONE)) {
			if (_profiler.gAntiAim.Custom.iValue == cProfiler::ANTIAIM_SPINBOT) {
				static float flAngle = 0.0f;

				if (flAngle > 360.0f)
					flAngle -= 360.0f;

				usercmd->iViewAngles[0] += AngleToShort(70.0f - CG->PlayerState.vViewAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(flAngle - CG->PlayerState.vViewAngles[1]);

				//_mathematics.MovementFix(usercmd, flAngle - CG->PlayerState.ViewAngles[1]);
				flAngle += 40.0f;
			}

			else if (_profiler.gAntiAim.Custom.iValue == cProfiler::ANTIAIM_JITTERBOT) {
				static int iMode = 1;

				switch (iMode) {
				case 1:
					usercmd->iViewAngles[0] += AngleToShort(-80.0f - CG->PlayerState.vViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(0.0f - CG->PlayerState.vViewAngles[1]);

					//_mathematics.MovementFix(usercmd, 0.0f - CG->PlayerState.ViewAngles[1]);
					iMode = 2;

					break;

				case 2:
					usercmd->iViewAngles[0] += AngleToShort(80.0f - CG->PlayerState.vViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(90.0f - CG->PlayerState.vViewAngles[1]);

					//_mathematics.MovementFix(usercmd, 90.0f - CG->PlayerState.ViewAngles[1]);
					iMode = 3;

					break;

				case 3:
					usercmd->iViewAngles[0] += AngleToShort(-80.0f - CG->PlayerState.vViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(180.0f - CG->PlayerState.vViewAngles[1]);

					//_mathematics.MovementFix(usercmd, 180.0f - CG->PlayerState.ViewAngles[1]);
					iMode = 4;

					break;

				case 4:
					usercmd->iViewAngles[0] += AngleToShort(80.0f - CG->PlayerState.vViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(270.0f - CG->PlayerState.vViewAngles[1]);

					//_mathematics.MovementFix(usercmd, 270.0f - CG->PlayerState.ViewAngles[1]);
					iMode = 1;

					break;
				}
			}

			else if (_profiler.gAntiAim.Custom.iValue == cProfiler::ANTIAIM_RANDOMIZED) {
				std::random_device Device;
				std::uniform_real_distribution<float> RandomPitch(-85.0f, 85.0f), RandomYaw(0.0f, 360.0f);

				usercmd->iViewAngles[0] += AngleToShort(RandomPitch(Device) - CG->PlayerState.vViewAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(RandomYaw(Device) - CG->PlayerState.vViewAngles[1]);

				//_mathematics.MovementFix(usercmd, RandomYaw(Device) - CG->PlayerState.ViewAngles[1]);
			}

			else if (_profiler.gAntiAim.Custom.iValue == cProfiler::ANTIAIM_REVERSED) {
				if (_aimBot.AimState.iTargetNum > -1 && (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
					(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CG->CEntity[CG->iClientNum].iFlags & EF_ZOOM))) {
					usercmd->iViewAngles[0] += AngleToShort(_aimBot.AimState.vAimbotAngles[0] - 5.7f);
					usercmd->iViewAngles[1] += AngleToShort(_aimBot.AimState.vAimbotAngles[1] - 180.0f);

					//_mathematics.MovementFix(usercmd, _aimBot.AimState.vAimbotAngles[1] - 180.0f);
				}

				else {
					usercmd->iViewAngles[0] += AngleToShort(-5.7f);
					usercmd->iViewAngles[1] += AngleToShort(-180.0f);

					//_mathematics.MovementFix(usercmd, -180.0f);
				}
			}
		}
	}
}

//=====================================================================================