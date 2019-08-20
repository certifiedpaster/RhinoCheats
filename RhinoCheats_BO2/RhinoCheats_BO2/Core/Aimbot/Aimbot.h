//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cAimbot {
	public:

		struct aimState_s {
			int iCurrentAimTime, iCurrentAimDelay, iCurrentZoomDelay, iCurrentFireDelay, iDeltaTMR, iTargetNum, iLastTarget;
			Vector3 vAimbotAngles;
		} AimState;

		void StandardAim();
		void SilentAim(usercmd_s* usercmd);
		void AutoFire(usercmd_s* usercmd);
	} extern _aimBot;
}

//=====================================================================================
