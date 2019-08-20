//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cTargetList _targetList;

	void cTargetList::GetInformation() {

		targetInfo_t TargetInfo;
		std::vector<targetInfo_t> vTargetInfo;

		_aimBot.AimState.iTargetNum = -1;

		bool bW2SSuccess = false;
		static int iCounter = 0;
		int iBonescanNum = iCounter % MAX_CLIENTS;

		for (int i = 0; i < MAX_CLIENTS; i++) {
			EntityList[i].bIsValid = false;
			EntityList[i].bAimFeet = false;

			if (!EntityIsValid(i))
				continue;

			LPVOID pDObj = GetDObj(&CG->CEntity[i]);

			if (!pDObj)
				continue;

			for (int j = BONE_HEAD; j < BONE_MAX; j++)
				GetTagPosition(&CG->CEntity[i], RegisterTag(vBones.second[j]), pDObj, EntityList[i].vBones3D[j]);

			char szWeapon[1024];

			GetWeaponName((BYTE)CG->CEntity[i].iWeaponID, szWeapon, sizeof(char[1024]));
			EntityList[i].szWeapon = szWeapon;

			size_t iPosition;

			while ((iPosition = EntityList[i].szWeapon.find("_mp")) != std::string::npos)
				EntityList[i].szWeapon.erase(iPosition, 3);

			EntityList[i].bIsValid = true;

			Vector3 vHeight;
			VectorCopy(CG->CEntity[i].vOrigin, vHeight);
			vHeight[2] += M_METERS;

			EntityList[i].bW2SSuccess = _drawing.CalculateESP(EntityList[i].vBones3D, EntityList[i].vBones2D, EntityList[i].vPosition, EntityList[i].vDimentions) &&
				_mathematics.WorldToScreen(CG->CEntity[i].vOrigin, EntityList[i].vLower) && _mathematics.WorldToScreen(vHeight, EntityList[i].vUpper);

			_mathematics.WorldToRadar(CG->CEntity[i].vOrigin, _drawing.Radar.vRadarPosition, _drawing.Radar.flScale, _drawing.Radar.flRadarSize, _drawing.Radar.flBlipSize, _drawing.Radar.vBlipPosition[i]);

			EntityList[i].vCenter[0] = EntityList[i].vPosition[0] + EntityList[i].vDimentions[0] / 2.0f;
			EntityList[i].vCenter[1] = EntityList[i].vPosition[1] + EntityList[i].vDimentions[1] / 2.0f;

			if (!EntityIsEnemy(i)) {
				EntityList[i].cColor = IsVisible(EntityList[i].vBones3D[BONE_HEAD], CG->CEntity[i].iClientNum, false, NULL) ? _profiler.gColorAlliesVisible.Custom.cValue : _profiler.gColorAlliesInvisible.Custom.cValue;
				continue;
			}

			else {
				EntityList[i].cColor = IsVisible(EntityList[i].vBones3D[BONE_HEAD], CG->CEntity[i].iClientNum, false, NULL) ? _profiler.gColorAxisVisible.Custom.cValue : _profiler.gColorAxisInvisible.Custom.cValue;
			}

			if (!_profiler.gAutowall.Custom.bValue) {
				Vector3 vDirection, vAngles, vDelta;

				VectorSubtract(CG->CEntity[i].vOrigin, CG->vOrigin, vDirection);

				_mathematics.VectorNormalize(vDirection);
				_mathematics.VectorAngles(vDirection, vAngles);
				_mathematics.NormalizeAngles(vAngles);

				VectorSubtract(vAngles, CG->Client[i].vViewAngles, vDelta);

				if (((BYTE)CG->CEntity[i].iPlayerPrimaryWeaponID == ID_ASSAULTSHIELD && !AngleCompare180(vDelta[1])) ||
					((BYTE)CG->CEntity[i].iPlayerSecondaryWeaponID == ID_ASSAULTSHIELD && AngleCompare180(vDelta[1]))) {
					if (_profiler.gRiotshield.Custom.iValue == cProfiler::RIOTSHIELD_IGNOREPLAYER)
						continue;

					else if (_profiler.gRiotshield.Custom.iValue == cProfiler::RIOTSHIELD_TARGETFEET)
						EntityList[i].bAimFeet = true;
				}
			}

			if (EntityList[i].bAimFeet) {
				bool bIsLeftAnkleVisible = IsVisible(EntityList[i].vBones3D[BONE_LEFT_ANKLE], CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, NULL),
					bIsRightAnkleVisible = IsVisible(EntityList[i].vBones3D[BONE_RIGHT_ANKLE], CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, NULL);

				if (bIsLeftAnkleVisible && bIsRightAnkleVisible) {
					EntityList[i].iBoneIndex = EntityList[i].vBones3D[BONE_LEFT_ANKLE][2] < EntityList[i].vBones3D[BONE_RIGHT_ANKLE][2] ? BONE_LEFT_ANKLE : BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsLeftAnkleVisible) {
					EntityList[i].iBoneIndex = BONE_LEFT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsRightAnkleVisible) {
					EntityList[i].iBoneIndex = BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else
					EntityList[i].bIsVisible = false;
			}

			else {
				if (_profiler.gBonescan.Custom.iValue == cProfiler::BONESCAN_ONTIMER) {
					if (iBonescanNum == i) {
						EntityList[i].bIsVisible = Bonescan(EntityList[i].vBones3D, CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, &EntityList[i].iBoneIndex);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					}

					else {
						EntityList[i].bIsVisible = IsVisible(EntityList[i].vBones3D[EntityList[i].iBoneIndex], CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, NULL);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					}
				}

				else if (_profiler.gBonescan.Custom.iValue == cProfiler::BONESCAN_IMMEDIATE) {
					EntityList[i].bIsVisible = Bonescan(EntityList[i].vBones3D, CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
				}

				else {
					EntityList[i].iBoneIndex = _profiler.gAimbone.Custom.iValue;
					EntityList[i].bIsVisible = IsVisible(EntityList[i].vBones3D[EntityList[i].iBoneIndex], CG->CEntity[i].iClientNum, _profiler.gAutowall.Custom.bValue, NULL);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
				}
			}

			if (_mathematics.CalculateFOV(EntityList[i].vAimbotPosition) > _profiler.gAimAngle.Custom.flValue)
				EntityList[i].bIsVisible = false;

			if (EntityList[i].bIsVisible) {
				ImVec2 vTarget, vCenter = { Window->iWidth / 2.0f, Window->iHeight / 2.0f };

				if (_mathematics.WorldToScreen(EntityList[i].vAimbotPosition, vTarget)) {
					bW2SSuccess = true;

					TargetInfo.iIndex = i;
					TargetInfo.flDistance2D = GetDistance2D(vTarget, vCenter);

					vTargetInfo.push_back(TargetInfo);
				}

				if (!bW2SSuccess) {
					TargetInfo.iIndex = i;
					TargetInfo.flDistance3D = GetDistance3D(CG->CEntity[i].vOrigin, CG->vOrigin);

					vTargetInfo.push_back(TargetInfo);
				}
			}
		}

		if (!vTargetInfo.empty()) {
			if (bW2SSuccess) {
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const targetInfo_t& a, const targetInfo_t& b) { return a.flDistance2D < b.flDistance2D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else {
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const targetInfo_t& a, const targetInfo_t& b) { return a.flDistance3D < b.flDistance3D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			vTargetInfo.clear();
		}

		if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC || (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL && CG->CEntity[CG->iClientNum].iFlags & EF_ZOOM)) {
			if (_aimBot.AimState.iCurrentAimDelay == _profiler.gAutoaimDelay.Custom.iValue)
				_aimBot.AimState.iCurrentAimTime += clock() - _aimBot.AimState.iDeltaTMR;

			_aimBot.AimState.iCurrentAimDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentZoomDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentFireDelay += clock() - _aimBot.AimState.iDeltaTMR;
		}

		_aimBot.AimState.iDeltaTMR = clock();

		if (_aimBot.AimState.iLastTarget != _aimBot.AimState.iTargetNum) {
			_aimBot.AimState.iLastTarget = _aimBot.AimState.iTargetNum;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (EntityList[_aimBot.AimState.iTargetNum].iLastBone != EntityList[_aimBot.AimState.iTargetNum].iBoneIndex) {
			EntityList[_aimBot.AimState.iTargetNum].iLastBone = EntityList[_aimBot.AimState.iTargetNum].iBoneIndex;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (_aimBot.AimState.iTargetNum == -1)
			_aimBot.AimState.iCurrentAimDelay = _aimBot.AimState.iCurrentZoomDelay = _aimBot.AimState.iCurrentFireDelay = 0;

		if (_aimBot.AimState.iCurrentAimTime > _profiler.gAutoaimTime.Custom.iValue)
			_aimBot.AimState.iCurrentAimTime = _profiler.gAutoaimTime.Custom.iValue;

		if (_aimBot.AimState.iCurrentAimDelay > _profiler.gAutoaimDelay.Custom.iValue)
			_aimBot.AimState.iCurrentAimDelay = _profiler.gAutoaimDelay.Custom.iValue;

		if (_aimBot.AimState.iCurrentZoomDelay > _profiler.gAutozoomDelay.Custom.iValue)
			_aimBot.AimState.iCurrentZoomDelay = _profiler.gAutozoomDelay.Custom.iValue;

		if (_aimBot.AimState.iCurrentFireDelay > _profiler.gAutofireDelay.Custom.iValue)
			_aimBot.AimState.iCurrentFireDelay = _profiler.gAutofireDelay.Custom.iValue;

		if (_aimBot.AimState.iTargetNum > -1) {
			ApplyPrediction(EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition, &CG->CEntity[_aimBot.AimState.iTargetNum]);
			_mathematics.CalculateAngles(EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition, _aimBot.AimState.vAimbotAngles);
		}

		iCounter++;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsValid(
		int index
	) {
		return (!(!CG->Client[index].iInfoValid || !(CG->CEntity[index].bAlive & 2) || &CG->CEntity[index] == &CG->CEntity[CG->iClientNum]));
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsEnemy(
		int index
	) {
		return (!(((CG->Client[index].iTeam == 1) || (CG->Client[index].iTeam == 2)) && (CG->Client[index].iTeam == CG->Client[CG->iClientNum].iTeam))) ^ false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisible(
		Vector3 position, 
		int entitynum, 
		bool autowall, 
		float* damage
	) {
		if (autowall) {
			float flDamage = _autoWall.BulletFirePenetrate(CG->RefDef.vViewOrg, position, entitynum);

			if (damage)
				*damage = flDamage;

			if (flDamage > 0.0f)
				return true;
		}

		else {
			bool bTraceHit = _autoWall.Trace(CG->RefDef.vViewOrg, position, entitynum);

			if (bTraceHit)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::Bonescan(
		Vector3 bones3d[BONE_MAX], 
		int entitynum,
		bool autowall, 
		int* index
	) {
		bool bReturn = false;

		damageInfo_t DamageInfo;
		std::vector<damageInfo_t> vDamageInfo;

		for (int i = BONE_HEAD; i < BONE_MAX; i++) {
			if (autowall) {
				if (IsVisible(bones3d[i], entitynum, true, &DamageInfo.flDamage)) {
					DamageInfo.iBoneIndex = i;
					vDamageInfo.push_back(DamageInfo);

					bReturn = true;
				}
			}

			else {
				if (IsVisible(bones3d[i], entitynum, false, NULL)) {
					*index = i;
					return true;
				}
			}
		}

		if (!vDamageInfo.empty()) {
			std::sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const damageInfo_t& a, const damageInfo_t& b) { return a.flDamage > b.flDamage; });
			*index = vDamageInfo.front().iBoneIndex;
			vDamageInfo.clear();
		}

		return bReturn;
	}
	/*
	//=====================================================================================
	*/
	void cTargetList::ApplyPrediction(
		Vector3 position,
		centity_t* entity
	) {
		float flMultiplier = 0.3f;
		Vector3 vVelocity;
		
		VectorSubtract(entity->vNewOrigin, entity->vOldOrigin, vVelocity);
		
		vVelocity[0] *= flMultiplier;
		vVelocity[1] *= flMultiplier;
		vVelocity[2] *= flMultiplier;
		
		VectorAdd(position, vVelocity, position);
	}
}

//=====================================================================================