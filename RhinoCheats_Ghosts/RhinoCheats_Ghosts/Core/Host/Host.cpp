//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cHost _host;

	void cHost::StartMatch()
	{
		if (!_mutex.try_lock())
			return;

		bool bTeamBased = FindVariable("party_teambased")->Current.bValue;
		PBYTE pResult = VariadicCall<PBYTE>(0x1402D6880);

		if (pResult[78])
		{
			LPVOID* v1 = VariadicCall<LPVOID*>(0x1402D6870);
			VariadicCall<PBYTE>(0x1402E9A60, v1);
		}

		if (!pResult[78] || !bTeamBased)
		{
			_mutex.unlock();
			return;
		}

		for (int i = clock(); !LocalClientIsInGame(); Sleep(100))
		{
			if (clock() - i > 60000)
			{
				_mutex.unlock();
				return;
			}
		}

		if (PlayerState[CG->iClientNum].ClientState.iTeam == TEAM_FREE)
		{
			PlayerState[CG->iClientNum].ClientState.iTeam = TEAM_ALLIES;
			TeamChanged(CG->iClientNum);
		}

		_mutex.unlock();
	}
	/*
	//=====================================================================================
	*/
	void cHost::MassKill()
	{
		if (!*(int*)OFF_ISCURRENTHOST)
			return;

		static int iCounter = 0;
		int iTargetNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		if (iTargetNum != CG->iClientNum && CEntity[iTargetNum].EntityState.iWeapon)
			if (CharacterInfo[iTargetNum].iInfoValid && CharacterInfo[iTargetNum].iNextValid)
				if (_profiler.gMasskill.Custom.iValue == cProfiler::MASSKILL_ALL ||
					(_profiler.gMasskill.Custom.iValue == cProfiler::MASSKILL_AXIS && _targetList.EntityIsEnemy(iTargetNum)) ||
					(_profiler.gMasskill.Custom.iValue == cProfiler::MASSKILL_ALLIES && !_targetList.EntityIsEnemy(iTargetNum)))
					PlayerKill(&GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? NULL : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? &GEntity[CG->iClientNum] : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? 0 : 14,
						_targetList.EntityIsEnemy(iTargetNum) ? GetCurrentWeapon(CG) : 0);

		iCounter++;
	}
}

//=====================================================================================