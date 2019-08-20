//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cHooks _hooks;

	void cHooks::Draw2D()
	{
		if (LocalClientIsInGame())
		{
			_targetList.GetInformation();
			_aimBot.StandardAim();
			_removals.NoRecoil();
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket()
	{
		if (LocalClientIsInGame())
			_packets.ModifyCommands();
	}
	/*
	//=====================================================================================
	*/
	void cHooks::BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime)
	{
		if (LocalClientIsInGame())
		{
			if (bp->iEntityNum == CG->iClientNum)
			{
				int iSeed = _removals.TransformSeed(WeaponIsAkimbo(GetCurrentWeapon(CG)) && 
					ClientActive->GetUserCMD(ClientActive->iCurrentCMD - !UsingKillstreak(GetCurrentWeapon(CG)))->iButtons & BUTTON_FIRELEFT, 
					CG->iServerTime);

				Vector3 vAngles, vForward, vRight, vUp;
				VectorCopy(_aimBot.AimState.vAimbotAngles, vAngles);

				vAngles[0] += FindVariable("camera_thirdPerson")->Current.bValue && !UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vThirdPersonViewAngles[0] : CG->vRefDefViewAngles[0];
				vAngles[1] += FindVariable("camera_thirdPerson")->Current.bValue && !UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vThirdPersonViewAngles[1] : CG->vRefDefViewAngles[1];

				AngleVectors(_profiler.gSilentAim.Custom.bValue && _aimBot.AimState.iTargetNum > -1 && (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
					(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM)) ?
					vAngles : FindVariable("camera_thirdPerson")->Current.bValue && !UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vThirdPersonViewAngles : CG->vRefDefViewAngles, vForward, vRight, vUp);

				BulletEndPosition(&iSeed, _profiler.gNoSpread.Custom.bValue ? 0.0f : _removals.GetWeaponSpread(), RefDef->vViewOrg,
					bp->vEnd, bp->vDir, vForward, vRight, vUp);
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::Obituary(int localnum, sEntityState* entitystate, int weapon)
	{
		if (LocalClientIsInGame())
		{
			if (entitystate->iAttackerEntityNum == CG->iClientNum)
			{
				if (_profiler.gKillspam.Custom.bValue)
				{
					int iWeapon = 0;

					if ((BYTE)weapon >= 160)
						iWeapon = (BYTE)weapon - 160;

					char szIcon[64] = { NULL };
					int iLength = AddMessageIcon(szIcon, 0, iWeapon == 9 ? RegisterShader("killiconheadshot") : RegisterShader("killicondied"), 1.4f, 1.4f, false);

					szIcon[iLength] = NULL;

					Cbuf_AddText(VariadicText("say \"^5%s ^7- Get ^6%s ^7courtesy of ^6Rhino^0Cheats^7.com\"\n",
						ClientInfo[entitystate->iOtherEntityNum].szName, szIcon));
				}

				if (_profiler.gNameStealer.Custom.bValue)
				{
					strcpy_s((LPSTR)(*(POINTER*)OFF_STEAMAPI + OFF_STEAMNAME), 
						sizeof(ClientInfo[entitystate->iOtherEntityNum].szName), 
						ClientInfo[entitystate->iOtherEntityNum].szName);
					
					Cbuf_AddText(VariadicText("name %s\n", ClientInfo[entitystate->iOtherEntityNum].szName));
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags)
	{
		LPSTR szInvalidText;

		if (szInvalidText = strstr(text, "\x5E\x01\x3D\x3D\xFF"))
			strcpy_s(szInvalidText, sizeof("crash"), "crash");
	}
	/*
	//=====================================================================================
	*/
	void cHooks::ClientFrame(sGEntity* entity)
	{
		if (LocalClientIsInGame())
			_host.MassKill();
	}
}

//=====================================================================================