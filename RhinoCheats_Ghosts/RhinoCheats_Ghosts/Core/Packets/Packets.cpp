//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cPackets _packets;

	void cPackets::ModifyCommands()
	{
		sUserCMD* pOldCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD - 1);
		sUserCMD* pNewCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD);

		*pOldCMD = *pNewCMD;
		--pOldCMD->iServerTime;

		OldCommand(pOldCMD);
		NewCommand(pNewCMD);
	}
	/*
	//=====================================================================================
	*/
	void cPackets::OldCommand(sUserCMD* usercmd)
	{
		static int iTime = clock();
		static float flAngle = 0.0f;

		std::random_device Device;
		std::uniform_real_distribution<float> RandomYaw(0.0f, 360.0f);

		if (clock() - iTime > 1000)
		{
			flAngle = RandomYaw(Device);
			iTime = clock();
		}

		if (_profiler.gAFKMode.Custom.bValue)
		{
			usercmd->szForwardMove = 0x7F;
			_mathematics.MovementFix(usercmd, flAngle);
		}

		_aimBot.SilentAim(usercmd);

		if (!UsingKillstreak(GetCurrentWeapon(CG)))
			_aimBot.AutoFire(usercmd);

		if (_profiler.gSilentAim.Custom.bValue && !UsingKillstreak(GetCurrentWeapon(CG)))
			_removals.NoSpreadSilentAim(usercmd, WeaponIsAkimbo(GetCurrentWeapon(CG)) && usercmd->iButtons & BUTTON_FIRELEFT);

		else
			_removals.NoSpreadStandardAim(usercmd, WeaponIsAkimbo(GetCurrentWeapon(CG)) && usercmd->iButtons & BUTTON_FIRELEFT);
	}
	/*
	//=====================================================================================
	*/
	void cPackets::NewCommand(sUserCMD* usercmd)
	{
		_antiAim.AntiAim(usercmd);

		if (UsingKillstreak(GetCurrentWeapon(CG)))
			_aimBot.AutoFire(usercmd);
	}
}

//=====================================================================================