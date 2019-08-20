//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cAutowall _autoWall;

	float cAutowall::BulletFirePenetrate(
		Vector3 start,
		Vector3 end,
		int entitynum
	) {
		int iIndex = CG->iClientNum;
		centity_t* pCEntity = &CG->CEntity[iIndex];
		int iWeaponID = pCEntity->iWeaponID;
		int iWeaponInfo = GetWeapon(iWeaponID);
		constexpr int iWeaponDef = 0;

		bulletFireParams_t BP_Enter;
		ZeroMemory(&BP_Enter, sizeof(bulletFireParams_t));

		bulletTraceResults_t TR_Enter;
		ZeroMemory(&TR_Enter, sizeof(bulletTraceResults_t));

		bulletFireParams_t BP_Exit;
		ZeroMemory(&BP_Exit, sizeof(bulletFireParams_t));

		bulletTraceResults_t TR_Exit;
		ZeroMemory(&TR_Exit, sizeof(bulletTraceResults_t));

		BP_Enter.iMaxEntNum = 1022;
		BP_Enter.iEntityNum = iIndex;
		BP_Enter.flPower = 1.0f;
		BP_Enter.iBulletType = (*(BYTE*)(iWeaponInfo + 0x631) != 0) + 1;

		VectorCopy(start, BP_Enter.vViewOrigin);
		VectorCopy(start, BP_Enter.vStart);
		VectorCopy(end, BP_Enter.vEnd);

		VectorSubtract(end, start, BP_Enter.vDir);
		_mathematics.VectorNormalize(BP_Enter.vDir);

		bool bEnterHit = BulletTrace(&TR_Enter, 0, &BP_Enter, 0, pCEntity, 0);

		if (HitRiotshield(&TR_Enter))
			return 0.0f;

		if (bEnterHit) {
			bool bHasFMJ = HasPerk(6);
			penetrateType_e iPenetrateType = GetPenetrateType(iWeaponID);

			int iSurfaceCount = 0;
			float flEnterDepth = 0.0f;
			float flExitDepth = 0.0f;
			float flSurfaceDepth = 0.0f;

			Vector3 vHitPos = { 0.0f };

			while (TRUE) {
				flEnterDepth = GetSurfacePenetrationDepth(iPenetrateType, TR_Enter.iSurfaceType);

				if (TR_Enter.Trace.iSFlags & 0x4)
					flEnterDepth = 100.0f;

				if (bHasFMJ)
					flEnterDepth *= 2.0f;

				if (flEnterDepth <= 0.0)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);

				if (!AdvanceTrace(&BP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = BulletTrace(&TR_Enter, 0, &BP_Enter, 0, pCEntity, TR_Enter.iSurfaceType);

				if (HitRiotshield(&TR_Enter))
					return 0.0f;

				CopyMemory(&BP_Exit, &BP_Enter, sizeof(bulletFireParams_t));
				VectorScale(BP_Enter.vDir, -1.0f, BP_Exit.vDir);

				VectorCopy(BP_Enter.vEnd, BP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, BP_Exit.vDir, BP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(bulletTraceResults_t));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&BP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = BulletTrace(&TR_Exit, 0, &BP_Exit, 0, pCEntity, TR_Exit.iSurfaceType);
				bool bSolid = bExitHit && TR_Exit.Trace.bAllSolid || TR_Exit.Trace.bStartSolid && TR_Enter.Trace.bStartSolid;

				if (HitRiotshield(&TR_Exit))
					return 0.0f;

				if (bExitHit || bSolid) {
					if (bSolid)
						flSurfaceDepth = _mathematics.GetDistance(BP_Exit.vStart, BP_Exit.vEnd);
					else
						flSurfaceDepth = _mathematics.GetDistance(TR_Exit.vHitPos, vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit) {
						flExitDepth = GetSurfacePenetrationDepth(iPenetrateType, TR_Exit.iSurfaceType);

						if (TR_Exit.Trace.iSFlags & 0x4)
							flExitDepth = 100.0f;

						if (bHasFMJ)
							flExitDepth *= 2.0f;

						flEnterDepth = min(flEnterDepth, flExitDepth);

						if (flEnterDepth <= 0.0f)
							return 0.0f;
					}

					BP_Enter.flPower -= flSurfaceDepth / flEnterDepth;

					if (BP_Enter.flPower <= 0.0f)
						return 0.0f;
				}

				else if (!bEnterHit)
					return BP_Enter.flPower;

				if (bEnterHit) {
					if (++iSurfaceCount < 5)
						continue;
				}

				return 0.0f;
			}
		}

		return BP_Enter.flPower;
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::BulletTrace(
		bulletTraceResults_t* br_,
		int localnum,
		bulletFireParams_t* bp_,
		int weapon,
		centity_t* attacker,
		int surfacetype
	) {
		bool bResult = false;
		DWORD_PTR dwAddress = OFF_BULLETTRACE;
		_declspec(align(16)) char szSave[512];
		_fxsave(szSave);
		_asm {
			push weapon
			push surfacetype
			push attacker
			push bp_
			push localnum
			mov  esi, br_
			call dwAddress
			mov	 bResult, al
			add  esp, 14h
		}
		_fxrstor(szSave);
		return bResult;
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::Trace(
		Vector3 start,
		Vector3 end,
		int entitynum
	) {
		trace_t Tr;

		LocationalTrace(&Tr, start, end, CG->iClientNum, 0x2803001, 0, 0);
		return bool(Tr.iHitID == entitynum || Tr.flFraction == 1.0f);
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::HitRiotshield(
		bulletTraceResults_t* br
	) {
		if (br->iIgnoreHitEnt)
			return false;

		if (br->Trace.iPartGroup == 20)
			return true;

		int iHitID = GetTraceHitType(br);

		if (iHitID != 1022) {
			if (EntityIsDeployedRiotshield(&CG->CEntity[iHitID])) {
				return true;
			}
		}

		return false;
	}
}

//=====================================================================================