//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cTargetList _targetList;

	void cTargetList::GetInformation()
	{
		sTargetInfo TargetInfo;
		std::vector<sTargetInfo> vTargetInfo;

		_aimBot.AimState.iTargetNum = -1;

		bool bW2SSuccess = false;
		static int iCounter = 0;
		int iBonescanNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			EntityList[i].bIsValid = false;
			EntityList[i].bAimFeet = false;

			if (!EntityIsValid(i))
				continue;

			if (CEntity[i].EntityState.iEntityType == ET_PLAYER || CEntity[i].EntityState.iEntityType == ET_AGENT)
			{
				LPVOID pDObj = GetEntityDObj(CEntity[i].EntityState.iEntityNum);

				if (!pDObj)
					continue;

				for (int j = BONE_HEAD; j < BONE_MAX; j++)
					GetTagPosition(&CEntity[i], pDObj, RegisterTag(vBones.second[j]), EntityList[i].vBones3D[j]);
			}

			char szWeapon[1024] = { NULL };

			GetWeaponName(CEntity[i].EntityState.iWeapon, CEntity[i].EntityState.iInAltWeaponMode, szWeapon, sizeof(char[1024]));
			EntityList[i].szWeapon = szWeapon;

			size_t iPosition;

			while ((iPosition = EntityList[i].szWeapon.find("^")) != std::string::npos)
				EntityList[i].szWeapon.erase(iPosition, 2);

			EntityList[i].bIsValid = true;

			if (CEntity[i].EntityState.iEntityType == ET_PLAYER)
			{
				Vector3 vViewOrigin;
				VectorCopy(CEntity[i].vOrigin, vViewOrigin);
				vViewOrigin[2] += M_METERS;

				EntityList[i].bW2SSuccess = _drawing.CalculateESP(EntityList[i].vBones3D, EntityList[i].vBones2D, EntityList[i].vPosition, EntityList[i].vDimentions) &&
					WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower) && WorldToScreen(GetScreenMatrix(), vViewOrigin, EntityList[i].vUpper);

				_mathematics.WorldToRadar(CEntity[i].vOrigin, _drawing.Radar.vRadarPosition, _drawing.Radar.flScale, _drawing.Radar.flRadarSize, _drawing.Radar.flBlipSize, _drawing.Radar.vBlipPosition[i]);

				EntityList[i].vCenter[0] = EntityList[i].vPosition[0] + EntityList[i].vDimentions[0] / 2.0f;
				EntityList[i].vCenter[1] = EntityList[i].vPosition[1] + EntityList[i].vDimentions[1] / 2.0f;

				if (!EntityIsEnemy(i))
				{
					EntityList[i].cColor = IsVisible(EntityList[i].vBones3D[BONE_HEAD], CEntity[i].EntityState.iEntityNum, false, NULL) ?
						_profiler.gColorAlliesVisible.Custom.cValue : _profiler.gColorAlliesInvisible.Custom.cValue;

					continue;
				}

				EntityList[i].cColor = IsVisible(EntityList[i].vBones3D[BONE_HEAD], CEntity[i].EntityState.iEntityNum, false, NULL) ?
					_profiler.gColorAxisVisible.Custom.cValue : _profiler.gColorAxisInvisible.Custom.cValue;
			}

			else if (CEntity[i].EntityState.iEntityType == ET_ITEM)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower);

				EntityList[i].cColor = IsVisible(CEntity[i].vOrigin, CEntity[i].EntityState.iEntityNum, false, NULL) ?
					_profiler.gColorItemVisible.Custom.cValue : _profiler.gColorItemInvisible.Custom.cValue;

				continue;
			}

			else if (CEntity[i].EntityState.iEntityType == ET_MISSILE)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower);

				EntityList[i].cColor = IsVisible(CEntity[i].vOrigin, CEntity[i].EntityState.iEntityNum, false, NULL) ?
					_profiler.gColorEntityVisible.Custom.cValue : _profiler.gColorEntityInvisible.Custom.cValue;

				if (!EntityIsEnemy(i))
					continue;
			}

			else if (CEntity[i].EntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower) && WorldToScreen(GetScreenMatrix(), EntityList[i].vBones3D[BONE_HEAD], EntityList[i].vUpper);

				EntityList[i].cColor = IsVisible(EntityList[i].vBones3D[BONE_HEAD], CEntity[i].EntityState.iEntityNum, false, NULL) ?
					_profiler.gColorEntityVisible.Custom.cValue : _profiler.gColorEntityInvisible.Custom.cValue;

				if (!EntityIsEnemy(i))
					continue;
			}

			if (!(CEntity[i].EntityState.iEntityType == ET_PLAYER ||
				(_profiler.gTargetMissiles.Custom.bValue && CEntity[i].EntityState.iEntityType == ET_MISSILE &&
				(CEntity[i].EntityState.iWeapon == WEAPON_C4 || CEntity[i].EntityState.iWeapon == WEAPON_IED)) ||
					(_profiler.gTargetAgents.Custom.bValue && CEntity[i].EntityState.iEntityType == ET_AGENT)))
				continue;

			if (UsingKillstreak(CEntity[i].EntityState.iWeapon))
				continue;

			Vector3 vDirection, vAngles, vDelta;

			VectorSubtract(CEntity[i].vOrigin, CG->vOrigin, vDirection);

			VectorNormalize(vDirection);
			VectorAngles(vDirection, vAngles);
			_mathematics.NormalizeAngles(vAngles);

			VectorSubtract(vAngles, CEntity[i].vAngles, vDelta);

			if (((BYTE)CEntity[i].EntityState.iWeapon == WEAPON_RIOT_SHIELD && !AngleCompare180(vDelta[1])) ||
				((BYTE)CEntity[i].EntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD && AngleCompare180(vDelta[1])))
			{
				if (_profiler.gRiotshield.Custom.iValue == cProfiler::RIOTSHIELD_IGNOREPLAYER)
					continue;

				else if (_profiler.gRiotshield.Custom.iValue == cProfiler::RIOTSHIELD_TARGETFEET)
					EntityList[i].bAimFeet = true;
			}

			if (EntityList[i].bAimFeet)
			{
				bool bIsLeftAnkleVisible = IsVisible(EntityList[i].vBones3D[BONE_LEFT_ANKLE], CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL),
					bIsRightAnkleVisible = IsVisible(EntityList[i].vBones3D[BONE_RIGHT_ANKLE], CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL);

				if (bIsLeftAnkleVisible && bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = EntityList[i].vBones3D[BONE_LEFT_ANKLE][2] < EntityList[i].vBones3D[BONE_RIGHT_ANKLE][2] ? BONE_LEFT_ANKLE : BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsLeftAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_LEFT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					EntityList[i].bIsVisible = true;
				}

				else
					EntityList[i].bIsVisible = false;
			}

			else if (CEntity[i].EntityState.iEntityType == ET_PLAYER)
			{
				if (_profiler.gBonescan.Custom.iValue == cProfiler::BONESCAN_ONTIMER)
				{
					if (iBonescanNum == i)
					{
						EntityList[i].bIsVisible = Bonescan(EntityList[i].vBones3D, CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, &EntityList[i].iBoneIndex);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					}

					else
					{
						EntityList[i].bIsVisible = IsVisible(EntityList[i].vBones3D[EntityList[i].iBoneIndex], CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
					}
				}

				else if (_profiler.gBonescan.Custom.iValue == cProfiler::BONESCAN_IMMEDIATE)
				{
					EntityList[i].bIsVisible = Bonescan(EntityList[i].vBones3D, CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
				}

				else
				{
					EntityList[i].iBoneIndex = (eBone)_profiler.gAimbone.Custom.iValue;
					EntityList[i].bIsVisible = IsVisible(EntityList[i].vBones3D[EntityList[i].iBoneIndex], CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
				}
			}

			else if (CEntity[i].EntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].iBoneIndex = BONE_HEAD;
				EntityList[i].bIsVisible = IsVisible(EntityList[i].vBones3D[EntityList[i].iBoneIndex], CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL);
				VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vAimbotPosition);
			}

			else
			{
				EntityList[i].bIsVisible = IsVisible(CEntity[i].vOrigin, CEntity[i].EntityState.iEntityNum, _profiler.gAutowall.Custom.bValue, NULL);
				VectorCopy(CEntity[i].vOrigin, EntityList[i].vAimbotPosition);
			}

			if (_mathematics.CalculateFOV(EntityList[i].vAimbotPosition) > _profiler.gAimAngle.Custom.flValue)
				EntityList[i].bIsVisible = false;

			if (i < FindVariable("sv_maxclients")->Current.iValue && *(int*)OFF_ISCURRENTHOST)
				if (GEntity[i].iHealth < 1)
					continue;

			if (std::find(vIsTarget.begin(), vIsTarget.end(), TRUE) != vIsTarget.end())
			{
				if (i < FindVariable("sv_maxclients")->Current.iValue)
				{
					if (!vIsTarget[CEntity[i].EntityState.iEntityNum])
						continue;
				}

				else if (!vIsTarget[CEntity[i].EntityState.iOtherEntityNum])
					continue;
			}

			if (EntityList[i].bIsVisible)
			{
				ImVec2 vTarget, vCenter(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);

				if (WorldToScreen(GetScreenMatrix(), EntityList[i].vAimbotPosition, vTarget))
				{
					bW2SSuccess = true;

					TargetInfo.iIndex = i;
					TargetInfo.flDistance2D = GetDistance2D(vTarget, vCenter);

					vTargetInfo.push_back(TargetInfo);
				}

				if (!bW2SSuccess)
				{
					TargetInfo.iIndex = i;
					TargetInfo.flDistance3D = GetDistance3D(CEntity[i].vOrigin, CG->vOrigin);

					vTargetInfo.push_back(TargetInfo);
				}
			}
		}

		if (!vTargetInfo.empty())
		{
			if (bW2SSuccess)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance2D < b.flDistance2D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance3D < b.flDistance3D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			vTargetInfo.clear();
		}

		if (_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_AUTOMATIC ||
			(_profiler.gAimmode.Custom.iValue == cProfiler::AIMMODE_MANUAL &&
				CEntity[CG->iClientNum].EntityState.LerpEntityState.iEntityFlags & EF_ZOOM))
		{
			if (_aimBot.AimState.iCurrentAimDelay == _profiler.gAutoaimDelay.Custom.iValue)
				_aimBot.AimState.iCurrentAimTime += clock() - _aimBot.AimState.iDeltaTMR;

			_aimBot.AimState.iCurrentAimDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentZoomDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentFireDelay += clock() - _aimBot.AimState.iDeltaTMR;
		}

		_aimBot.AimState.iDeltaTMR = clock();

		if (_aimBot.AimState.iLastTarget != _aimBot.AimState.iTargetNum)
		{
			_aimBot.AimState.iLastTarget = _aimBot.AimState.iTargetNum;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (EntityList[_aimBot.AimState.iTargetNum].iLastBone != EntityList[_aimBot.AimState.iTargetNum].iBoneIndex)
		{
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

		if (_aimBot.AimState.iTargetNum > -1)
		{
			Vector3 vOldPosition, vNewPosition, vVelocity;

			EvaluateTrajectory(&CEntity[_aimBot.AimState.iTargetNum].LerpEntityState.PositionTrajectory, CG->OldSnapShot->iServerTime, vOldPosition);
			EvaluateTrajectory(&CEntity[_aimBot.AimState.iTargetNum].EntityState.LerpEntityState.PositionTrajectory, CG->NewSnapShot->iServerTime, vNewPosition);

			VectorSubtract(vNewPosition, vOldPosition, vVelocity);

			VectorMA(EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition, *(int*)OFF_FRAMETIME / 1000.0f, vVelocity, EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition);
			VectorMA(EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition, *(int*)OFF_PING / 1000.0f, vVelocity, EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition);

			_mathematics.CalculateAngles(EntityList[_aimBot.AimState.iTargetNum].vAimbotPosition, _aimBot.AimState.vAimbotAngles);
		}

		iCounter++;
		_aimBot.AimState.iFireTMR++;

		if (WeaponIsAkimbo(GetCurrentWeapon(CG)))
		{
			if (!(_aimBot.AimState.iFireTMR % ((BYTE)GetCurrentWeapon(CG) == WEAPON_44_MAGNUM ? 12 : 6)))
				_aimBot.AimState.bAkimbo = !_aimBot.AimState.bAkimbo;
		}

		else
			_aimBot.AimState.bAkimbo = false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsValid(int index)
	{
		return (index != CG->iClientNum && CEntity[index].wValid && CEntity[index].iIsAlive & 1);
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsEnemy(int index)
	{
		if (CEntity[index].EntityState.iEntityType == ET_PLAYER)
		{
			if (CharacterInfo[index].iTeam == TEAM_FREE ||
				CharacterInfo[index].iTeam != CharacterInfo[CG->iClientNum].iTeam)
				return true;
		}

		else
		{
			if (CharacterInfo[CEntity[index].EntityState.iOtherEntityNum].iTeam == TEAM_FREE ||
				CharacterInfo[CEntity[index].EntityState.iOtherEntityNum].iTeam != CharacterInfo[CG->iClientNum].iTeam)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisible(Vector3 position, int entitynum, bool autowall, float* damage)
	{
		if (autowall && !UsingKillstreak(GetCurrentWeapon(CG)))
		{
			float flDamage = _autoWall.Autowall(RefDef->vViewOrg, position);

			if (damage)
				*damage = flDamage;

			if (flDamage > 0.0f)
				return true;
		}

		else
		{
			sTrace Tr;

			Trace(&Tr, RefDef->vViewOrg, position, CG->iClientNum, UsingKillstreak(GetCurrentWeapon(CG)) ? MASK_KILLSTREAK : MASK_NONKILLSTREAK);

			if (Tr.wHitID == entitynum || Tr.flFraction == 1.0f)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::Bonescan(Vector3 bones3d[BONE_MAX], int entitynum, bool autowall, eBone* index)
	{
		bool bReturn = false;

		sDamageInfo DamageInfo;
		std::vector<sDamageInfo> vDamageInfo;

		for (int i = BONE_HEAD; i < BONE_MAX; i++)
		{
			if (autowall)
			{
				if (IsVisible(bones3d[i], entitynum, true, &DamageInfo.flDamage))
				{
					DamageInfo.iBoneIndex = (eBone)i;
					vDamageInfo.push_back(DamageInfo);

					bReturn = true;
				}
			}

			else
			{
				if (IsVisible(bones3d[i], entitynum, false, NULL))
				{
					*index = (eBone)i;
					return true;
				}
			}
		}

		if (!vDamageInfo.empty())
		{
			std::sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const sDamageInfo& a, const sDamageInfo& b) { return a.flDamage > b.flDamage; });
			*index = vDamageInfo.front().iBoneIndex;
			vDamageInfo.clear();
		}

		return bReturn;
	}
}

//=====================================================================================