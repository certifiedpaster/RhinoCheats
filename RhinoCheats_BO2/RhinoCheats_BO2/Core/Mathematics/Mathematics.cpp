//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cMathematics _mathematics;

	float cMathematics::GetDistance(
		Vector3 a,
		Vector3 b
	) {
		float flX = b[0] - a[0],
			flY = b[1] - a[1],
			flZ = b[2] - a[2];

		return sqrtf((float)((float)(flY * flY) + (float)(flX * flX)) + (float)(flZ * flZ));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::NormalizeAngles(
		Vector3 angles
	) {
		while (angles[0] > 180.0f) {
			angles[0] -= 360.0f;
		}

		while (angles[0] < -180.0f) {
			angles[0] += 360.0f;
		}

		while (angles[1] > 180.0f) {
			angles[1] -= 360.0f;
		}

		while (angles[1] < -180.0f) {
			angles[1] += 360.0f;
		}

		if (angles[2] != 0.0f) {
			angles[2] = 0.0f;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorNormalize(
		Vector3 direction
	) {
		float flLen = sqrtf(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

		if (flLen == 0.0f) {
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 1.0f;
		}

		else {
			flLen = 1.0f / flLen;

			direction[0] *= flLen;
			direction[1] *= flLen;
			direction[2] *= flLen;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::AngleVectors(
		Vector3 angles,
		Vector3 forward,
		Vector3 right,
		Vector3 up
	) {
		float flAngle, flSinRoll, flSinPitch, flSinYaw, flCosRoll, flCosPitch, flCosYaw;

		flAngle = DegreesToRadians(angles[1]);
		flSinYaw = sinf(flAngle);
		flCosYaw = cosf(flAngle);
		flAngle = DegreesToRadians(angles[0]);
		flSinPitch = sinf(flAngle);
		flCosPitch = cosf(flAngle);
		flAngle = DegreesToRadians(angles[2]);
		flSinRoll = sinf(flAngle);
		flCosRoll = cosf(flAngle);

		if (forward) {
			forward[0] = flCosPitch * flCosYaw;
			forward[1] = flCosPitch * flSinYaw;
			forward[2] = -flSinPitch;
		}

		if (right) {
			right[0] = (-1 * flSinRoll * flSinPitch * flCosYaw + -1 * flCosRoll * -flSinYaw);
			right[1] = (-1 * flSinRoll * flSinPitch * flSinYaw + -1 * flCosRoll * flCosYaw);
			right[2] = -1 * flSinRoll * flCosPitch;
		}

		if (up) {
			up[0] = (flCosRoll * flSinPitch * flCosYaw + -flSinRoll * -flSinYaw);
			up[1] = (flCosRoll * flSinPitch * flSinYaw + -flSinRoll * flCosYaw);
			up[2] = flCosRoll * flCosPitch;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorAngles(
		Vector3 direction,
		Vector3 angles
	) {
		float flTemp, flYaw, flPitch;

		if (direction[0] == 0.0f && direction[1] == 0.0f) {
			flYaw = 0.0f;

			if (direction[2] > 0.0f)
				flPitch = 90.0f;

			else
				flPitch = 270.0f;
		}

		else {
			flYaw = RadiansToDegrees(atan2f(direction[1], direction[0]));

			if (flYaw < 0.0f)
				flYaw += 360.0f;

			flTemp = sqrtf(direction[0] * direction[0] + direction[1] * direction[1]);
			flPitch = RadiansToDegrees(atan2f(direction[2], flTemp));

			if (flPitch < 0.0f)
				flPitch += 360.0f;
		}

		angles[0] = -flPitch;
		angles[1] = flYaw;
		angles[2] = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MakeVector(
		Vector3 angles,
		Vector3 out
	) {
		float flPitch = DegreesToRadians(angles[0]), flYaw = DegreesToRadians(angles[1]);

		out[0] = -cosf(flPitch) * -cosf(flYaw);
		out[1] = sinf(flYaw) * cosf(flPitch);
		out[2] = -sinf(flPitch);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateFOV(
		Vector3 position
	) {
		Vector3 vDirection, vAngles, vAimAngles;
		VectorSubtract(position, CG->RefDef.vViewOrg, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		MakeVector(CG->PlayerState.vViewAngles, vAimAngles);
		MakeVector(vAngles, vAngles);

		float flMag = sqrtf((vAimAngles[0] * vAimAngles[0]) + (vAimAngles[1] * vAimAngles[1]) + (vAimAngles[2] * vAimAngles[2])),
			flDot = (vAimAngles[0] * vAngles[0]) + (vAimAngles[1] * vAngles[1]) + (vAimAngles[2] * vAngles[2]);

		return RadiansToDegrees(acosf(flDot / powf(flMag, 2.0f)));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::CalculateAngles(
		Vector3 position,
		Vector3 angles
	) {
		Vector3 vDirection;
		VectorSubtract(position, CG->RefDef.vViewOrg, vDirection);

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);
		NormalizeAngles(angles);

		angles[0] -= CG->PlayerState.vViewAngles[0];
		angles[1] -= CG->PlayerState.vViewAngles[1];

		NormalizeAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	bool cMathematics::WorldToScreen(
		Vector3 world,
		Vector2 screen
	) {
		int iCenterX = CG->RefDef.iWidth / 2,
			iCenterY = CG->RefDef.iHeight / 2;

		Vector3 vLocal,
			vTransForm;

		VectorSubtract(world, CG->RefDef.vViewOrg, vLocal);

		vTransForm[0] = DotProduct(vLocal, CG->RefDef.vViewAxis[1]);
		vTransForm[1] = DotProduct(vLocal, CG->RefDef.vViewAxis[2]);
		vTransForm[2] = DotProduct(vLocal, CG->RefDef.vViewAxis[0]);

		if (vTransForm[2] < 0.01f)
			return false;

		screen[0] = iCenterX * (1 - (vTransForm[0] / CG->RefDef.flFOVX / vTransForm[2]));
		screen[1] = iCenterY * (1 - (vTransForm[1] / CG->RefDef.flFOVY / vTransForm[2]));

		return true;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToRadar(
		Vector3 world,
		ImVec2 radarpos,
		float scale,
		float radarsize,
		float blipsize,
		ImVec2& screen
	) {
		float flCosYaw = cosf(DegreesToRadians(CG->PlayerState.vViewAngles[1])),
			flSinYaw = sinf(DegreesToRadians(CG->PlayerState.vViewAngles[1])),
			flDeltaX = world[0] - CG->RefDef.vViewOrg[0],
			flDeltaY = world[1] - CG->RefDef.vViewOrg[1],
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
	/*
	//=====================================================================================
	*/
	void cMathematics::MovementFix(
		usercmd_s* usercmd,
		float yaw
	) {
		if (usercmd->szForwardMove || usercmd->szRightMove) {
			float flMove = AngleNormalize(RadiansToDegrees(atan2(-usercmd->szRightMove / 127.0f, usercmd->szForwardMove / 127.0f))),
				flDelta = AngleNormalize(yaw),
				flDestination = AngleNormalize(flMove - flDelta),
				flForwardRatio = cos(DegreesToRadians(flDestination)),
				flRightRatio = -sin(DegreesToRadians(flDestination));

			if (abs(flForwardRatio) < abs(flRightRatio)) {
				flForwardRatio *= 1.0f / abs(flRightRatio);
				flRightRatio = flRightRatio > 0.0f ? 1.0f : -1.0f;
			}

			else if (abs(flForwardRatio) > abs(flRightRatio)) {
				flRightRatio *= 1.0f / abs(flForwardRatio);
				flForwardRatio = flForwardRatio > 0.0f ? 1.0f : -1.0f;
			}

			else {
				flForwardRatio = 1.0f;
				flRightRatio = 1.0f;
			}

			usercmd->szForwardMove = (char)(flForwardRatio * 127.0f);
			usercmd->szRightMove = (char)(flRightRatio * 127.0f);
		}
	}
}