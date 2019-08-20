//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cMathematics _mathematics;

	void cMathematics::CalculateAngles(Vector3 position, Vector3 angles)
	{
		Vector3 vDirection;
		VectorSubtract(position, RefDef->vViewOrg, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);
		NormalizeAngles(angles);

		angles[0] -= UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vRefDefViewAngles[0] : CG->vWeaponAngles[0];
		angles[1] -= UsingKillstreak(GetCurrentWeapon(CG)) ? CG->vRefDefViewAngles[1] : CG->vWeaponAngles[1];

		NormalizeAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::NormalizeAngles(Vector3 angles)
	{
		while (angles[0] < -180.0f) angles[0] += 360.0f;
		while (angles[0] > 180.0f) angles[0] -= 360.0f;

		while (angles[1] < -180.0f) angles[1] += 360.0f;
		while (angles[1] > 180.0f) angles[1] -= 360.0f;

		if (angles[2] != 0.0f) angles[2] = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateFOV(Vector3 position)
	{
		Vector3 vDirection, vAngles, vAimAngles;
		VectorSubtract(position, RefDef->vViewOrg, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		MakeVector(CG->vRefDefViewAngles, vAimAngles);
		MakeVector(vAngles, vAngles);

		float flMag = sqrtf((vAimAngles[0] * vAimAngles[0]) + (vAimAngles[1] * vAimAngles[1]) + (vAimAngles[2] * vAimAngles[2])),
			flDot = (vAimAngles[0] * vAngles[0]) + (vAimAngles[1] * vAngles[1]) + (vAimAngles[2] * vAngles[2]);

		return RadiansToDegrees(acosf(flDot / powf(flMag, 2.0f)));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MakeVector(Vector3 angles, Vector3 out)
	{
		float flPitch = DegreesToRadians(angles[0]), flYaw = DegreesToRadians(angles[1]);

		out[0] = -cosf(flPitch) * -cosf(flYaw);
		out[1] = sinf(flYaw) * cosf(flPitch);
		out[2] = -sinf(flPitch);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MovementFix(sUserCMD* usercmd, float yaw)
	{
		if (usercmd->szForwardMove || usercmd->szRightMove)
		{
			float flMove = AngleNormalize(RadiansToDegrees(atan2(-usercmd->szRightMove / 127.0f, usercmd->szForwardMove / 127.0f))),
				flDelta = AngleNormalize(yaw),
				flDestination = AngleNormalize(flMove - flDelta),
				flForwardRatio = cos(DegreesToRadians(flDestination)),
				flRightRatio = -sin(DegreesToRadians(flDestination));

			if (abs(flForwardRatio) < abs(flRightRatio))
			{
				flForwardRatio *= 1.0f / abs(flRightRatio);
				flRightRatio = flRightRatio > 0.0f ? 1.0f : -1.0f;
			}

			else if (abs(flForwardRatio) > abs(flRightRatio))
			{
				flRightRatio *= 1.0f / abs(flForwardRatio);
				flForwardRatio = flForwardRatio > 0.0f ? 1.0f : -1.0f;
			}

			else
			{
				flForwardRatio = 1.0f;
				flRightRatio = 1.0f;
			}

			usercmd->szForwardMove = (char)(flForwardRatio * 127.0f);
			usercmd->szRightMove = (char)(flRightRatio * 127.0f);
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToCompass(Vector3 world, ImVec2 compasspos, float compasssize, ImVec2& screen)
	{
		float flAngle;

		Vector3 vDirection, vAngles;

		VectorSubtract(RefDef->vViewOrg, world, vDirection);
		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		VectorSubtract(CG->vRefDefViewAngles, vAngles, vAngles);
		_mathematics.NormalizeAngles(vAngles);

		flAngle = ((vAngles[1] + 180.f) / 360.f - 0.25f) * M_PI_DOUBLE;

		compasssize /= 2.0f;

		screen[0] = compasspos[0] + (cosf(flAngle) * compasssize);
		screen[1] = compasspos[1] + (sinf(flAngle) * compasssize);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToRadar(Vector3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen)
	{
		float flCosYaw = cosf(DegreesToRadians(CG->vRefDefViewAngles[1])),
			flSinYaw = sinf(DegreesToRadians(CG->vRefDefViewAngles[1])),
			flDeltaX = world[0] - RefDef->vViewOrg[0],
			flDeltaY = world[1] - RefDef->vViewOrg[1],
			flLocationX = (flDeltaY * flCosYaw - flDeltaX * flSinYaw) / scale,
			flLocationY = (flDeltaX * flCosYaw + flDeltaY * flSinYaw) / scale;

		if (flLocationX < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationX = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (flLocationX > (radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationX = (radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		if (flLocationY < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			flLocationY = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (flLocationY > (radarsize / 2.0f - blipsize / 2.0f))
			flLocationY = (radarsize / 2.0f - blipsize / 2.0f);

		screen[0] = -flLocationX + radarpos[0];
		screen[1] = -flLocationY + radarpos[1];
	}
}