#include "stdafx.h"
#include "WritePackets.h"

void AntiAim(usercmd_t* pCmd)
{
	/*if (Settings[aim_enabled].Value.bValue || Settings[aim_key].Value.iValue)
		return;
	if (!Settings[aim_mode].Value.iValue)
		return;*/


		/*if (
			Settings[aim_enabled].Value.bValue &&
			Settings[aim_mode].Value.iValue
			)*/
	{

		if (cg_entities[cg->clientNum].eFlags & 8)
			return;

		//compiler doesn't like it, if i put it inside 'case 1'
		static float spinAngle = 0.0f;

		switch (Settings[anti_aim].Value.iValue)
		{
		case 1:

			if (spinAngle > 360.0f)
				spinAngle -= 360.0f;
			pCmd->viewangles[1] = ANGLE2SHORT(spinAngle);
			spinAngle += 40.0f;
			pCmd->viewangles[0] = ANGLE2SHORT(ANGLE_DOWN);
			//pCmd->buttons |= BUTTON_CROUCHED; //up & down effect
			break;

		case 2:

			if (!Entity.IsUsingRiot(cg->clientNum))
			{
				if (Aim.isReady[Aim_t::isReadyforFire])
				{
					pCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1] - 180);
					pCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0] - 5.7f);
				}
				else
				{
					pCmd->viewangles[1] = ANGLE2SHORT(pViewMatrix->viewAngleX - 180);
					pCmd->viewangles[0] = ANGLE2SHORT(pViewMatrix->viewAngleY - 5.7f);
				}
			}
		}
	}
}

void InverseTroller(usercmd_t* pCmd)
{
	if (/*Settings[aim_enabled].Value.bValue &&
		Settings[aim_mode].Value.iValue &&*/
		Settings[inverse_troller].Value.bValue)
	{
		static float frot = 360.0f;
		if (frot < 0.0f)
			frot += 360.0f;
		pCmd->viewangles[2] = ANGLE2SHORT(frot);
		frot -= 40.0f;
	}
	else
	{
		pCmd->viewangles[2] = 0.0f;
	}
}

void DoCurCmd(usercmd_t *curCmd, int seed)
{
	if (Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire])
		curCmd->buttons |= BUTTON_FIRE;

	Nospread.ApplyNoSpread(curCmd, seed);

	if (Settings[silent_aim].Value.bValue && Aim.isReady[Aim_t::isReadyforFire] && !Settings[third_person].Value.bValue)
	{
		//-= doesn't work
		float flOldYaw = SHORT2ANGLE(curCmd->viewangles[1]);

		curCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1]);
		curCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0]);
	}	
}

void DoNextCmd(usercmd_t* nextCmd)
{
	AntiAim(nextCmd);
	InverseTroller(nextCmd);
}



/************************************************************************/
/* WritePacket                                                          */
/************************************************************************/

void WritePacket()
{
	if (cg)
	{
#pragma region usercmd	
		/*usercmd_t *curCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		pinput->currentCmdNum += 1;
		usercmd_t *nextCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		*nextCmd = *curCmd;
		curCmd->servertime -= 1;*/

		usercmd_t* curCmd = pinput->GetUserCmd(pinput->currentCmdNum - 1);
		usercmd_t* nextCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		*curCmd = *nextCmd;
		--curCmd->servertime;
#pragma endregion curCmd for silent spread/aim, nextCmd for anti aim		


		if (!Aim.isVehicle &&
			cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1))
		{
			DoCurCmd(curCmd, curCmd->servertime);
			DoNextCmd(nextCmd);
		}

		Aim.Autoshoot();
	}
}
