//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cAimbot
	{
	public:

		struct sAimState
		{
			bool bAkimbo;
			int iFireTMR, iCurrentAimTime, iCurrentAimDelay, iCurrentZoomDelay, iCurrentFireDelay, iDeltaTMR, iTargetNum, iLastTarget;
			Vector3 vAimbotAngles;
		} AimState;

		void StandardAim();
		void SilentAim(sUserCMD* usercmd);
		void AutoFire(sUserCMD* usercmd);
	} extern _aimBot;
}

//=====================================================================================
