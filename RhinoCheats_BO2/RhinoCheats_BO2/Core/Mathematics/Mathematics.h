//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cMathematics {
	public:

		float GetDistance(Vector3 a, Vector3 b);
		void NormalizeAngles(Vector3 angles);
		void VectorNormalize(Vector3 direction);
		void AngleVectors(Vector3 angles, Vector3 forward, Vector3 right, Vector3 up);
		void VectorAngles(Vector3 direction, Vector3 angles);
		float CalculateFOV(Vector3 position);
		void CalculateAngles(Vector3 position, Vector3 angles);
		bool WorldToScreen(Vector3 world, Vector2 screen);
		void WorldToRadar(Vector3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen);
		void MovementFix(usercmd_s* usercmd, float yaw);
		void MakeVector(Vector3 angles, Vector3 out);
	} extern _mathematics;
}

//=====================================================================================
