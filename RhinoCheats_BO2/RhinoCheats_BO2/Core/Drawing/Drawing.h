//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cDrawing {
	public:

		struct radar_s {
			float flScale = 10.0f, flRadarSize = 200.0f, flBlipSize = 8.0f;
			ImVec2 vRadarPosition, vBlipPosition[MAX_CLIENTS];
		} Radar;

		void DrawBones(ImVec2 bones2d[BONE_MAX], bool skeleton, ImVec4 color);
		void DrawString(std::string text, float x, float y, float size, bool shadow, ImVec4 color);
		void DrawCorners(float x, float y, float w, float h, float hlength, float vlength, bool shadow, ImVec4 color);
		void DrawBox(float x, float y, float w, float h, bool border, ImVec4 color);
		void DrawLine(float x1, float y1, float x2, float y2, ImVec4 color);
		void DrawPlayer(centity_t* entity, ImVec2 bones2d[BONE_MAX], ImVec2 center, float scale, float distance, std::string name, ImVec4 color);
		bool CalculateESP(Vector3 bones3d[BONE_MAX], ImVec2 bones2d[BONE_MAX], ImVec2& position, ImVec2& dimentions);
		void ESP();
	} extern _drawing;
}

//=====================================================================================