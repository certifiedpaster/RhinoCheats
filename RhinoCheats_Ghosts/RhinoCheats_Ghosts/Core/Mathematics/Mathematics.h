//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cMathematics
	{
	public:

		void CalculateAngles(Vector3 position, Vector3 angles);
		void NormalizeAngles(Vector3 angles);
		float CalculateFOV(Vector3 position);
		void MakeVector(Vector3 angles, Vector3 out);
		void MovementFix(sUserCMD* usercmd, float yaw);
		void WorldToCompass(Vector3 world, ImVec2 compasspos, float compasssize, ImVec2& screen);
		void WorldToRadar(Vector3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen);
	} extern _mathematics;
}

//=====================================================================================
