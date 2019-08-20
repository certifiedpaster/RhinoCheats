//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cDrawing _drawing;

	void cDrawing::DrawBones(ImVec2 bones2d[BONE_MAX], bool skeleton, ImVec4 color)
	{
		if (skeleton)
		{
			DrawLine(bones2d[BONE_NECK][0], bones2d[BONE_NECK][1],
				bones2d[BONE_LEFT_SHOULDER][0], bones2d[BONE_LEFT_SHOULDER][1],
				color);

			DrawLine(bones2d[BONE_LEFT_SHOULDER][0], bones2d[BONE_LEFT_SHOULDER][1],
				bones2d[BONE_LEFT_ELBOW][0], bones2d[BONE_LEFT_ELBOW][1],
				color);

			DrawLine(bones2d[BONE_LEFT_ELBOW][0], bones2d[BONE_LEFT_ELBOW][1],
				bones2d[BONE_LEFT_WRIST][0], bones2d[BONE_LEFT_WRIST][1],
				color);

			DrawLine(bones2d[BONE_NECK][0], bones2d[BONE_NECK][1],
				bones2d[BONE_RIGHT_SHOULDER][0], bones2d[BONE_RIGHT_SHOULDER][1],
				color);

			DrawLine(bones2d[BONE_RIGHT_SHOULDER][0], bones2d[BONE_RIGHT_SHOULDER][1],
				bones2d[BONE_RIGHT_ELBOW][0], bones2d[BONE_RIGHT_ELBOW][1],
				color);

			DrawLine(bones2d[BONE_RIGHT_ELBOW][0], bones2d[BONE_RIGHT_ELBOW][1],
				bones2d[BONE_RIGHT_WRIST][0], bones2d[BONE_RIGHT_WRIST][1],
				color);

			DrawLine(bones2d[BONE_NECK][0], bones2d[BONE_NECK][1],
				bones2d[BONE_UPPER_SPINE][0], bones2d[BONE_UPPER_SPINE][1],
				color);

			DrawLine(bones2d[BONE_UPPER_SPINE][0], bones2d[BONE_UPPER_SPINE][1],
				bones2d[BONE_LOWER_SPINE][0], bones2d[BONE_LOWER_SPINE][1],
				color);

			DrawLine(bones2d[BONE_LOWER_SPINE][0], bones2d[BONE_LOWER_SPINE][1],
				bones2d[BONE_LEFT_HIP][0], bones2d[BONE_LEFT_HIP][1],
				color);

			DrawLine(bones2d[BONE_LEFT_HIP][0], bones2d[BONE_LEFT_HIP][1],
				bones2d[BONE_LEFT_KNEE][0], bones2d[BONE_LEFT_KNEE][1],
				color);

			DrawLine(bones2d[BONE_LEFT_KNEE][0], bones2d[BONE_LEFT_KNEE][1],
				bones2d[BONE_LEFT_ANKLE][0], bones2d[BONE_LEFT_ANKLE][1],
				color);

			DrawLine(bones2d[BONE_LOWER_SPINE][0], bones2d[BONE_LOWER_SPINE][1],
				bones2d[BONE_RIGHT_HIP][0], bones2d[BONE_RIGHT_HIP][1],
				color);

			DrawLine(bones2d[BONE_RIGHT_HIP][0], bones2d[BONE_RIGHT_HIP][1],
				bones2d[BONE_RIGHT_KNEE][0], bones2d[BONE_RIGHT_KNEE][1],
				color);

			DrawLine(bones2d[BONE_RIGHT_KNEE][0], bones2d[BONE_RIGHT_KNEE][1],
				bones2d[BONE_RIGHT_ANKLE][0], bones2d[BONE_RIGHT_ANKLE][1],
				color);
		}

		else
		{
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_NECK][0], bones2d[BONE_NECK][1]), 
				ImVec2(bones2d[BONE_NECK][0] + 1.0f, bones2d[BONE_NECK][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_UPPER_SPINE][0], bones2d[BONE_UPPER_SPINE][1]),
				ImVec2(bones2d[BONE_UPPER_SPINE][0] + 1.0f, bones2d[BONE_UPPER_SPINE][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LOWER_SPINE][0], bones2d[BONE_LOWER_SPINE][1]), 
				ImVec2(bones2d[BONE_LOWER_SPINE][0] + 1.0f, bones2d[BONE_LOWER_SPINE][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_SHOULDER][0], bones2d[BONE_LEFT_SHOULDER][1]), 
				ImVec2(bones2d[BONE_LEFT_SHOULDER][0] + 1.0f, bones2d[BONE_LEFT_SHOULDER][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_SHOULDER][0], bones2d[BONE_RIGHT_SHOULDER][1]), 
				ImVec2(bones2d[BONE_RIGHT_SHOULDER][0] + 1.0f, bones2d[BONE_RIGHT_SHOULDER][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_HIP][0], bones2d[BONE_LEFT_HIP][1]), 
				ImVec2(bones2d[BONE_LEFT_HIP][0] + 1.0f, bones2d[BONE_LEFT_HIP][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_HIP][0], bones2d[BONE_RIGHT_HIP][1]), 
				ImVec2(bones2d[BONE_RIGHT_HIP][0] + 1.0f, bones2d[BONE_RIGHT_HIP][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_ELBOW][0], bones2d[BONE_LEFT_ELBOW][1]), 
				ImVec2(bones2d[BONE_LEFT_ELBOW][0] + 1.0f, bones2d[BONE_LEFT_ELBOW][1] + 1.0f),
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_ELBOW][0], bones2d[BONE_RIGHT_ELBOW][1]), 
				ImVec2(bones2d[BONE_RIGHT_ELBOW][0] + 1.0f, bones2d[BONE_RIGHT_ELBOW][1] + 1.0f),
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_KNEE][0], bones2d[BONE_LEFT_KNEE][1]), 
				ImVec2(bones2d[BONE_LEFT_KNEE][0] + 1.0f, bones2d[BONE_LEFT_KNEE][1] + 1.0f),
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_KNEE][0], bones2d[BONE_RIGHT_KNEE][1]),
				ImVec2(bones2d[BONE_RIGHT_KNEE][0] + 1.0f, bones2d[BONE_RIGHT_KNEE][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_WRIST][0], bones2d[BONE_LEFT_WRIST][1]), 
				ImVec2(bones2d[BONE_LEFT_WRIST][0] + 1.0f, bones2d[BONE_LEFT_WRIST][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_WRIST][0], bones2d[BONE_RIGHT_WRIST][1]), 
				ImVec2(bones2d[BONE_RIGHT_WRIST][0] + 1.0f, bones2d[BONE_RIGHT_WRIST][1] + 1.0f),
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_LEFT_ANKLE][0], bones2d[BONE_LEFT_ANKLE][1]), 
				ImVec2(bones2d[BONE_LEFT_ANKLE][0] + 1.0f, bones2d[BONE_LEFT_ANKLE][1] + 1.0f), 
				ImGui::GetColorU32(color));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bones2d[BONE_RIGHT_ANKLE][0], bones2d[BONE_RIGHT_ANKLE][1]), 
				ImVec2(bones2d[BONE_RIGHT_ANKLE][0] + 1.0f, bones2d[BONE_RIGHT_ANKLE][1] + 1.0f),
				ImGui::GetColorU32(color));
		}
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawString(std::string text, float x, float y, float size, bool shadow, ImVec4 color)
	{
		if (shadow)
		{
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x - 1.0f, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());

			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x + 1.0f, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x + 1.0f, y), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());

			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x + 1.0f, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());

			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x - 1.0f, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());
			ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x - 1.0f, y), ImGui::GetColorU32(ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue)), text.c_str());
		}

		ImGui::GetWindowDrawList()->AddText(ImGui::GetIO().FontDefault, size, ImVec2(x, y), ImGui::GetColorU32(color), text.c_str());
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawCorners(float x, float y, float w, float h, float hlength, float vlength, bool shadow, ImVec4 color)
	{
		if (shadow)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x - 1.0f, y - 1.0f), ImVec2(x + hlength + 2.0f, y + 2.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x - 1.0f, y - 1.0f), ImVec2(x + 2.0f, y + vlength + 2.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - hlength - 1.0f, y - 1.0f), ImVec2(x + w + 1.0f, y + 2.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - 2.0f, y - 1.0f), ImVec2(x + w + 1.0f, y + vlength + 2.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x - 1.0f, y + h - 2.0f), ImVec2(x + hlength + 2.0f, y + h + 1.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x - 1.0f, y + h - vlength - 1.0f), ImVec2(x + 2.0f, y + h + 1.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - hlength - 1.0f, y + h - 2.0f), ImVec2(x + w + 1.0f, y + h + 1.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - 2.0f, y + h - vlength - 1.0f), ImVec2(x + w + 1.0f, y + h + 1.0f), ImGui::GetColorU32(_profiler.gColorShadow.Custom.cValue));
		}

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + hlength + 1.0f, y + 1.0f), ImGui::GetColorU32(color));
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + 1.0f, y + vlength + 1.0f), ImGui::GetColorU32(color));

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - hlength, y), ImVec2(x + w, y + 1.0f), ImGui::GetColorU32(color));
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - 1.0f, y), ImVec2(x + w, y + vlength + 1.0f), ImGui::GetColorU32(color));

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y + h - 1.0f), ImVec2(x + hlength + 1.0f, y + h), ImGui::GetColorU32(color));
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y + h - vlength), ImVec2(x + 1.0f, y + h), ImGui::GetColorU32(color));

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - hlength, y + h - 1.0f), ImVec2(x + w, y + h), ImGui::GetColorU32(color));
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x + w - 1.0f, y + h - vlength), ImVec2(x + w, y + h), ImGui::GetColorU32(color));
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawBox(float x, float y, float w, float h, bool border, ImVec4 color)
	{
		if (border) ImGui::GetWindowDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(color));

		else ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(color));
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawLine(float x1, float y1, float x2, float y2, ImVec4 color)
	{
		ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(color));
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawPlayer(sCEntity* entity, ImVec2 bones2d[BONE_MAX], ImVec2 center, float scale, float distance, std::string name, ImVec4 color)
	{
		float flWidth, flHeight, flPadding = 3.0f;

		if (entity->EntityState.LerpEntityState.iEntityFlags & EF_PRONE)
		{
			flWidth = scale / 1.5f;
			flHeight = scale / 2.0f;
		}

		else if (entity->EntityState.LerpEntityState.iEntityFlags & EF_CROUCH)
		{
			flWidth = scale / 2.0f;
			flHeight = scale / 2.0f;
		}

		else
		{
			flWidth = scale / 2.0f;
			flHeight = scale / 1.5f;
		}

		flWidth *= 1.75f;
		flHeight *= 1.75f;

		if (_profiler.gPlayerBoxes.Custom.iValue == cProfiler::PLAYER_BOXES_BORDER)
			DrawBox(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight, true, color);

		else if (_profiler.gPlayerBoxes.Custom.iValue == cProfiler::PLAYER_BOXES_CORNER)
			DrawCorners(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight,
				flWidth / 3.0f, flHeight / 3.0f, false, color);
		
		else if (_profiler.gPlayerBoxes.Custom.iValue == cProfiler::PLAYER_BOXES_BORDER_FILLED)
		{
			DrawBox(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight, false, ImVec4(color[0], color[1], color[2], color[3] / 4.0f));
			DrawBox(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight, true, color);
		}

		else if (_profiler.gPlayerBoxes.Custom.iValue == cProfiler::PLAYER_BOXES_CORNER_FILLED)
		{
			DrawBox(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight, false, ImVec4(color[0], color[1], color[2], color[3] / 4.0f));
			DrawCorners(center[0] - flWidth / 2.0f, center[1] - flHeight / 2.0f, flWidth, flHeight,
				flWidth / 3.0f, flHeight / 3.0f, false, color);
		}

		if (_profiler.gPlayerBones.Custom.iValue == cProfiler::PLAYER_BONES_DOTS)
			DrawBones(bones2d, false, color);

		else if (_profiler.gPlayerBones.Custom.iValue == cProfiler::PLAYER_BONES_LINES)
			DrawBones(bones2d, true, color);

		if (_profiler.gPlayerSnaplines.Custom.bValue)
			DrawLine(center[0], center[1] + flHeight / 2.0f, ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y, color);

		if (_profiler.gPlayerInfo.Custom.bValue)
		{
			std::string szInformation(VariadicText("[%im] %s", (int)(distance / M_METERS), name.c_str()));
			ImVec2 vStringSize = ImGui::GetIO().FontDefault->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, szInformation.c_str());

			DrawString(szInformation, center[0] - vStringSize.x / 2.0f, center[1] - flHeight / 2.0f - flPadding - vStringSize.y, 14.0f, false, color);
		}

		if (_profiler.gPlayerWeapons.Custom.bValue)
		{
			ImVec2 vStringSize = ImGui::GetIO().FontDefault->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, _targetList.EntityList[entity->EntityState.iEntityNum].szWeapon.c_str());
			DrawString(_targetList.EntityList[entity->EntityState.iEntityNum].szWeapon, center[0] - vStringSize.x / 2.0f, center[1] + flHeight / 2.0f + flPadding, 14.0f, false, color);
		}
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawItem(std::string name, ImVec2 center, float distance, ImVec4 color)
	{
		std::string szInformation(VariadicText("[%im] %s", (int)(distance / M_METERS), name.c_str()));

		ImVec2 vStringSize = ImGui::GetIO().FontDefault->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, szInformation.c_str());
		DrawString(szInformation, center[0] - vStringSize.x / 2.0f, center[1] - vStringSize.y / 2.0f, 14.0f, false, color);
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawMissile(std::string name, ImVec2 center, float distance, ImVec4 color)
	{
		std::string szInformation(VariadicText("[%im] %s", (int)(distance / M_METERS), name.c_str()));

		ImVec2 vStringSize = ImGui::GetIO().FontDefault->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, szInformation.c_str());
		DrawString(szInformation, center[0] - vStringSize.x / 2.0f, center[1] - vStringSize.y / 2.0f, 14.0f, false, color);
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::DrawAgent(sCEntity* entity, ImVec2 center, float distance, ImVec4 color)
	{
		_targetList.EntityList[entity->EntityState.iEntityNum].szWeapon = (BYTE)entity->EntityState.iWeapon ?
			((BYTE)entity->EntityState.LerpEntityState.iPrimaryWeapon == WEAPON_RIOT_SHIELD ||
			(BYTE)entity->EntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD) ?
			"Squadmate" : "Juggernaut" :
			entity->EntityState.iOtherEntityNum >= FindVariable("sv_maxclients")->Current.iValue ?
			"Alien" : "Guard Dog";

		std::string szInformation(VariadicText("[%im] %s", (int)(distance / M_METERS), _targetList.EntityList[entity->EntityState.iEntityNum].szWeapon.c_str()));

		ImVec2 vStringSize = ImGui::GetIO().FontDefault->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, szInformation.c_str());
		DrawString(szInformation, center[0] - vStringSize.x / 2.0f, center[1] - vStringSize.y / 2.0f, 14.0f, false, color);
	}
	/*
	//=====================================================================================
	*/
	bool cDrawing::CalculateESP(Vector3 bones3d[BONE_MAX], ImVec2 bones2d[BONE_MAX], ImVec2& position, ImVec2& dimentions)
	{
		ImVec2 vTemp, vMaxTemp = { -FLT_MAX, -FLT_MAX }, vMinTemp = { FLT_MAX, FLT_MAX };

		for (int i = BONE_NECK; i < BONE_MAX; i++)
		{
			if (!WorldToScreen(GetScreenMatrix(), bones3d[i], vTemp))
				return false;

			if (vTemp[0] > vMaxTemp[0])
				vMaxTemp[0] = vTemp[0];

			if (vTemp[0] < vMinTemp[0])
				vMinTemp[0] = vTemp[0];

			if (vTemp[1] > vMaxTemp[1])
				vMaxTemp[1] = vTemp[1];

			if (vTemp[1] < vMinTemp[1])
				vMinTemp[1] = vTemp[1];

			bones2d[i][0] = vTemp[0];
			bones2d[i][1] = vTemp[1];
		}

		dimentions[0] = vMaxTemp[0] - vMinTemp[0];
		dimentions[1] = vMaxTemp[1] - vMinTemp[1];

		position[0] = vMinTemp[0];
		position[1] = vMinTemp[1];

		return true;
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::ESP()
	{
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			switch (CEntity[i].EntityState.iEntityType)
			{
			case ET_PLAYER:
			{
				if (_targetList.EntityList[i].bW2SSuccess && _targetList.EntityList[i].bIsValid)
					DrawPlayer(&CEntity[i], _targetList.EntityList[i].vBones2D, _targetList.EntityList[i].vCenter, _targetList.EntityList[i].vLower[1] - _targetList.EntityList[i].vUpper[1],
						GetDistance3D(CEntity[i].vOrigin, CG->vOrigin), ClientInfo[i].szName, _targetList.EntityList[i].cColor);
			} break;

			case ET_ITEM:
			{
				if (_profiler.gItems.Custom.bValue)
					if (_targetList.EntityList[i].bW2SSuccess && _targetList.EntityList[i].bIsValid)
						DrawItem(_targetList.EntityList[i].szWeapon, _targetList.EntityList[i].vLower, GetDistance3D(CEntity[i].vOrigin, CG->vOrigin), _targetList.EntityList[i].cColor);
			} break;

			case ET_MISSILE:
			{
				if (_profiler.gMissiles.Custom.bValue)
					if (_targetList.EntityList[i].bW2SSuccess && _targetList.EntityList[i].bIsValid && _targetList.EntityIsEnemy(i) &&
						CEntity[i].EntityState.iOtherEntityNum < FindVariable("sv_maxclients")->Current.iValue)
						DrawMissile(_targetList.EntityList[i].szWeapon, _targetList.EntityList[i].vLower, GetDistance3D(CEntity[i].vOrigin, CG->vOrigin), _targetList.EntityList[i].cColor);
			} break;

			case ET_AGENT:
			{
				if (_profiler.gAgents.Custom.bValue)
					if (_targetList.EntityList[i].bW2SSuccess && _targetList.EntityList[i].bIsValid && _targetList.EntityIsEnemy(i))
						DrawAgent(&CEntity[i], _targetList.EntityList[i].vUpper, GetDistance3D(CEntity[i].vOrigin, CG->vOrigin), _targetList.EntityList[i].cColor);
			} break;
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cDrawing::Crosshair()
	{
		if (_profiler.gCrosshair.Custom.bValue)
		{
			if (!(CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM))
			{
				ImGui::GetWindowDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 12.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 1.0f), ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 4.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 1.0f), ImGui::GetColorU32(_profiler.gColorCrosshair.Custom.cValue));
				ImGui::GetWindowDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f + 3.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 1.0f), ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f + 11.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 1.0f), ImGui::GetColorU32(_profiler.gColorCrosshair.Custom.cValue));

				ImGui::GetWindowDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 1.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 12.0f), ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 1.0f, ImGui::GetIO().DisplaySize.y / 2.0f - 4.0f), ImGui::GetColorU32(_profiler.gColorCrosshair.Custom.cValue));
				ImGui::GetWindowDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 1.0f, ImGui::GetIO().DisplaySize.y / 2.0f + 3.0f), ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - 1.0f, ImGui::GetIO().DisplaySize.y / 2.0f + 11.0f), ImGui::GetColorU32(_profiler.gColorCrosshair.Custom.cValue));
			}
		}
	}
}

//=====================================================================================