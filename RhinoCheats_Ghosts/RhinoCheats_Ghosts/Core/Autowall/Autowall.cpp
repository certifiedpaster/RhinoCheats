//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cAutowall _autoWall;

	float cAutowall::Autowall(Vector3 start, Vector3 end)
	{
		sBulletFireParams BP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&BP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));
		
		VectorSubtract(end, start, BP_Enter.vDir);
		VectorNormalize(BP_Enter.vDir);

		BP_Enter.iMaxEntNum = 2046;
		BP_Enter.iEntityNum = CG->iClientNum;
		BP_Enter.flPower = 1.0f;
		BP_Enter.iBulletType = 1;

		VectorCopy(start, BP_Enter.vViewOrigin);
		VectorCopy(start, BP_Enter.vStart);
		VectorCopy(end, BP_Enter.vEnd);

		bool bEnterHit = BulletTrace(&BP_Enter, &CEntity[CG->iClientNum], &TR_Enter, TRACE_HITTYPE_NONE);

		if (bEnterHit)
		{
			int iSurfaceCount = 0;
			float flEnterDepth = 0.0f, flExitDepth = 0.0f, flSurfaceDepth = 0.0f;

			sBulletFireParams BP_Exit;
			sBulletTraceResults TR_Exit;

			Vector3 vHitPos = { 0.0f };

			while (TRUE)
			{
				flEnterDepth = GetSurfacePenetrationDepth(CEntity[CG->iClientNum].EntityState.iWeapon, CEntity[CG->iClientNum].EntityState.iInAltWeaponMode, TR_Enter.iDepthSurfaceType);

				if (HasPerk(CG->iClientNum, PERK_EXTRABP))
					flEnterDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

				if (flEnterDepth <= 0.0f)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);

				if (!AdvanceTrace(&BP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = BulletTrace(&BP_Enter, &CEntity[CG->iClientNum], &TR_Enter, TR_Enter.iDepthSurfaceType);

				CopyMemory(&BP_Exit, &BP_Enter, sizeof(sBulletFireParams));
				VectorScale(BP_Enter.vDir, -1.0f, BP_Exit.vDir);
				
				VectorCopy(BP_Enter.vEnd, BP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, BP_Exit.vDir, BP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(sBulletTraceResults));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&BP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = BulletTrace(&BP_Exit, &CEntity[CG->iClientNum], &TR_Exit, TR_Exit.iDepthSurfaceType);
				bool bSolid = (bExitHit && TR_Exit.Trace.bAllSolid) || (TR_Enter.Trace.bStartSolid && TR_Exit.Trace.bStartSolid);

				if (bExitHit || bSolid)
				{
					if (bSolid)
						flSurfaceDepth = GetDistance3D(BP_Exit.vEnd, BP_Exit.vStart);

					else
						flSurfaceDepth = GetDistance3D(vHitPos, TR_Exit.vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit)
					{
						flExitDepth = GetSurfacePenetrationDepth(CEntity[CG->iClientNum].EntityState.iWeapon, CEntity[CG->iClientNum].EntityState.iInAltWeaponMode, TR_Exit.iDepthSurfaceType);

						if (HasPerk(CG->iClientNum, PERK_EXTRABP))
							flExitDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

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

				if (bEnterHit)
				{
					if (++iSurfaceCount < 5)
						continue;
				}

				return 0.0f;
			}
		}

		return BP_Enter.flPower;
	}
}

//=====================================================================================