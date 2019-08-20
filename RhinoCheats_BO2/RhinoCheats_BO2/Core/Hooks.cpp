//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cHooks _hooks;

	LONG cHooks::VectoredExceptionHandler(
		LPEXCEPTION_POINTERS ExceptionInfo
	) {
		*(DWORD*)OFF_TACSSHANDLE = 0x1;
		
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE) {
			if (ExceptionInfo->ContextRecord->Eip == OFF_PREDICTPLAYERSTATE) {
				PredictPlayerState();
			}

			return EXCEPTION_CONTINUE_EXECUTION;
		}
		
		else {
			DWORD dwProtection = 0;

			switch (ExceptionInfo->ContextRecord->Eax) {
			case VEH_INDEX_DRAWBIGFPS:
				ExceptionInfo->ContextRecord->Eax = dwDrawBigFPS;

				if (ExceptionInfo->ContextRecord->Eip == OFF_DRAWBIGFPS_EXCEPTION)
					DrawBigFPS();

				return EXCEPTION_CONTINUE_EXECUTION;

			case VEH_INDEX_MOUSEACCEL:
				ExceptionInfo->ContextRecord->Eax = dwMouseAccel;

				if (ExceptionInfo->ContextRecord->Eip == OFF_MOUSEACCEL_EXCEPTION)
					VirtualProtect((LPVOID)OFF_PREDICTPLAYERSTATE, sizeof(BYTE), PAGE_EXECUTE | PAGE_GUARD, &dwProtection);

				return EXCEPTION_CONTINUE_EXECUTION;

			case VEH_INDEX_NODELTA:
				ExceptionInfo->ContextRecord->Eax = dwNoDelta;

				if (ExceptionInfo->ContextRecord->Eip == OFF_NODELTA_EXCEPTION)
					WritePacket();

				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
	/*
	//=====================================================================================
	*/
	void cHooks::DrawBigFPS() {
		if (LocalClientIsInGame()) {
			_targetList.GetInformation();
			_aimBot.StandardAim();
			_removals.NoRecoil();
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::PredictPlayerState() {
		if (LocalClientIsInGame()) {
			static int iAngleBackup[3];

			usercmd_s* pOldCmd = Input->GetUserCMD(Input->iCurrentCMD - 1);
			usercmd_s* pCurrentCmd = Input->GetUserCMD(Input->iCurrentCMD);
			usercmd_s* pNewCmd = Input->GetUserCMD(Input->iCurrentCMD + 1);

			CopyMemory(pNewCmd, pCurrentCmd, sizeof(usercmd_s));
			++Input->iCurrentCMD;

			pOldCmd->iViewAngles[0] = iAngleBackup[0];
			pOldCmd->iViewAngles[1] = iAngleBackup[1];
			pOldCmd->iViewAngles[2] = iAngleBackup[2];

			iAngleBackup[0] = pCurrentCmd->iViewAngles[0];
			iAngleBackup[1] = pCurrentCmd->iViewAngles[1];
			iAngleBackup[2] = pCurrentCmd->iViewAngles[2];

			++pOldCmd->iServerTime;
			--pCurrentCmd->iServerTime;

			_aimBot.AutoFire(pCurrentCmd);
			_aimBot.SilentAim(pOldCmd);
			_removals.NoSpread(pOldCmd, pCurrentCmd->iServerTime);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket() {
		if (LocalClientIsInGame()) {
			usercmd_s* pUserCmd = Input->GetUserCMD(Input->iCurrentCMD);

			_aimBot.AutoFire(pUserCmd);
			_antiAim.AntiAim(pUserCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	BOOL cHooks::PatchAntiCheat() {
		BOOL bResult;
		DWORD dwProtection;

		char szPatchStub[7] = {
			-117,
			-1,
			85,
			-117,
			-20,
			-117,
			-55
		};

		if (*(DWORD_PTR*)OFF_TACSSCHECK) {
			VirtualProtect((LPVOID)OFF_TACSSPATCH, 0x8, PAGE_EXECUTE_READWRITE, &dwProtection);

			for (int i = 0; i < 7; ++i) {
				*((BYTE*)OFF_TACSSPATCH + sizeof(DWORD_PTR) * i) = szPatchStub[i];
			}

			bResult = VirtualProtect((LPVOID)OFF_TACSSPATCH, 0x8, dwProtection, &dwProtection);
			*(DWORD_PTR*)OFF_TACSSCHECK = NULL;
		}

		return bResult;
	}
}

//=====================================================================================