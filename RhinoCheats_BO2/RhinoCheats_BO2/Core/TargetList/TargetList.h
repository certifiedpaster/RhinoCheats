//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cTargetList {
	public:

		typedef struct {
			int iIndex;

			float flDistance2D = FLT_MAX;
			float flDistance3D = FLT_MAX;
		} targetInfo_t;

		typedef struct {
			float flDamage;
			int iBoneIndex;
		} damageInfo_t;

		struct entityList_s {
			bool bIsValid, bW2SSuccess, bAimFeet, bIsVisible;
			int iBoneIndex, iLastBone;
			std::string szWeapon;
			ImVec2 vBones2D[BONE_MAX], vPosition, vDimentions, vLower, vCenter, vUpper;
			Vector3 vBones3D[BONE_MAX], vAimbotPosition;
			ImVec4 cColor;
		} EntityList[MAX_ENTITIES];

		std::vector<BOOL> vIsTarget = std::vector<BOOL>(MAX_CLIENTS, FALSE);

		void GetInformation();
		bool EntityIsValid(int index);
		bool EntityIsEnemy(int index);
		bool IsVisible(Vector3 position, int entitynum, bool autowall, float* damage);
		bool Bonescan(Vector3 bones3d[BONE_MAX], int entitynum, bool autowall, int* index);
		void ApplyPrediction(Vector3 position, centity_t* entity);
	} extern _targetList;
}

//=====================================================================================
