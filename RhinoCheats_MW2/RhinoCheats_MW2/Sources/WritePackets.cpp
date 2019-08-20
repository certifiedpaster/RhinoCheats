#include "stdafx.h"
#include "WritePackets.h"


void DoCurCmd(usercmd_t *curCmd, int seed)
{
	if (Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire])
		curCmd->buttons |= BUTTON_FIRE;

	Nospread.ApplyNoSpread(curCmd, seed);

	if (Settings[silent_aim].Value.bValue && Aim.isReady[Aim_t::isReadyforFire] && !Settings[third_person].Value.bValue)
	{
		//-= doesn't work
		curCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1]);
		curCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0]);	
	}	
}



/************************************************************************/
/* WritePacket                                                          */
/************************************************************************/

void WritePacket()
{
	if (cg)
	{
#pragma region usercmd	
		usercmd_t *curCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		pinput->currentCmdNum += 1;
		usercmd_t *nextCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		*nextCmd = *curCmd;
		curCmd->servertime -= 1;
#pragma endregion curCmd for silent spread/aim, nextCmd for anti aim		


		if (!Aim.isVehicle &&
			cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1))
			DoCurCmd(curCmd, curCmd->servertime);		

		Aim.Autoshoot();
	}
}
