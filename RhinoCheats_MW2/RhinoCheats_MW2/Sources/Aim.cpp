#include "stdafx.h"

Aim_t Aim;

//========================================================================

void Aim_t::ApplyPrediction(int i, vec3_t vData)
{
	vec3_t trDelta;
	VectorSubtract(cg_entities[i].lerpOrigin3, cg_entities[i].lerpOrigin2, trDelta);
	VectorMA(vData, *(INT *)Offsets::ping * 0.001f, trDelta, vData);
}

bool Aim_t::MyVisibilityMethod(int i, int autowall, float *damage, bool isVehicle)
{
	if (Settings[head_only].Value.bValue)
	{
		std::vector<int> AIMBOT_bones;
		AIMBOT_bones.push_back(Entity.bones[j_head]);
		AIMBOT_bones.push_back(Entity.bones[j_helmet]);

		for (int b = 0; b < AIMBOT_bones.size(); ++b)
		{
			if (Engine.Getvectorfromtag(&cg_entities[i], AIMBOT_bones[b], bestBone))
			{
				if (Settings[aim_prediction].Value.bValue)
				{
					ApplyPrediction(i, bestBone);
				}

				float flDamage = 0.0f;

				switch (autowall)
				{
				case 0:
					if (Autowall.NormalVisible(bestBone))
					{
						VectorCopy(bestBone, vAimLocation);
						return true;
					}
					break;

				default:
					if (isVehicle)
					{
						if (Autowall.NormalVisible(bestBone))
						{
							if (damage)
								*damage = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
							VectorCopy(bestBone, vAimLocation);
							return true;
						}
					}
					else
					{
						flDamage = (autowall != 2 ?
							Autowall.WallTraceSimulated(bestBone, &cg_entities[i]) :
							Autowall.WallTrace(bestBone, &cg_entities[i]));
						if (flDamage > 0.0f)
						{
							if (damage)
								*damage = flDamage;
							VectorCopy(bestBone, vAimLocation);
							return true;
						}
					}
				}
			}
		}
		
	}
	else
	{
		//Thanks IUF for the idea :)

		DamageInfo_t damageInfo;
		std::vector<DamageInfo_t> vDamageInfo;

		//Special case
		std::vector<int> AIMBOT_bones;
		AIMBOT_bones.push_back(Entity.bones[j_head]);
		AIMBOT_bones.push_back(Entity.bones[j_helmet]);
		AIMBOT_bones.push_back(Entity.bones[j_elbow_le]);
		AIMBOT_bones.push_back(Entity.bones[j_elbow_ri]);
		AIMBOT_bones.push_back(Entity.bones[j_spine4]);
		AIMBOT_bones.push_back(Entity.bones[j_ankle_le]);
		AIMBOT_bones.push_back(Entity.bones[j_ankle_ri]);

		for (int b = 0; b < AIMBOT_bones.size(); ++b)
		{
			if (Engine.Getvectorfromtag(&cg_entities[i], AIMBOT_bones[b], bestBone))
			{
				if (Settings[aim_prediction].Value.bValue)
				{
					ApplyPrediction(i, bestBone);
				}

				switch (autowall)
				{
				case 0:
					if (Autowall.NormalVisible(bestBone))
					{
						VectorCopy(bestBone, vAimLocation);
						return true;
					}
					break;

				default:
					
					if (isVehicle)
					{
						if (Autowall.NormalVisible(bestBone))
						{
							if (damage)
								*damage = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
							VectorCopy(bestBone, vAimLocation);
							return true;
						}
					}
					else
					{
						damageInfo.damage = (autowall != 2 ?
							Autowall.WallTraceSimulated(bestBone, &cg_entities[i]) :
							Autowall.WallTrace(bestBone, &cg_entities[i]));
						if (damageInfo.damage > 0.0f)
						{
							VectorCopy(bestBone, damageInfo.bestBone);
							vDamageInfo.push_back(damageInfo);
						}
					}
				}
			}
		}

		if (!vDamageInfo.empty())
		{
			std::sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const DamageInfo_t& a, const DamageInfo_t& b) 
			{ return a.damage > b.damage; });

			if (damage)
				*damage = vDamageInfo.front().damage;

			VectorCopy(vDamageInfo.front().bestBone, vAimLocation);

			return true;
		}
	}

	return false;
}

bool Aim_t::MyRiotShieldMethod(int i, int autowall, float *damage, bool isVehicle)
{	
	float flDamage = 0.0f;	

	switch (autowall)
	{
	case 0:
		if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_ankle_ri], bestBone) && Autowall.NormalVisible(bestBone))
		{
			VectorCopy(bestBone, vAimLocation);
			return true;
		}
		if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_ankle_le], bestBone) && Autowall.NormalVisible(bestBone))
		{
			VectorCopy(bestBone, vAimLocation);
			return true;
		}
		break;

	default:
		if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_ankle_ri], bestBone))
		{			
			if (isVehicle)
			{
				if (Autowall.NormalVisible(bestBone))
				{
					if (damage)
						*damage = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
					VectorCopy(bestBone, vAimLocation);
					return true;
				}
			}
			else
			{
				flDamage = (autowall != 2 ?
					Autowall.WallTraceSimulated(bestBone, &cg_entities[i]) :
					Autowall.WallTrace(bestBone, &cg_entities[i]));
				if (flDamage > 0.0f)
				{
					if (damage)
						*damage = flDamage;
					VectorCopy(bestBone, vAimLocation);
					return true;
				}
			}
			
		}
		if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_ankle_le], bestBone))
		{			
			if (isVehicle)
			{
				if (Autowall.NormalVisible(bestBone))
				{
					if (damage)
						*damage = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
					VectorCopy(bestBone, vAimLocation);
					return true;
				}
			}
			else
			{
				flDamage = (autowall != 2 ?
					Autowall.WallTraceSimulated(bestBone, &cg_entities[i]) :
					Autowall.WallTrace(bestBone, &cg_entities[i]));
				if (flDamage > 0.0f)
				{
					if (damage)
						*damage = flDamage;
					VectorCopy(bestBone, vAimLocation);
					return true;
				}
			}			
		}
	}

	return false;
}

bool Aim_t::GetVisibility(int i, int autowall, float *damage, bool isVehicle)
{		

	if (!Entity.IsUsingRiot(i))	
	{
		return MyVisibilityMethod(i, autowall, damage, isVehicle);
	}	
	else if (Settings[enemies_with_riotshield].Value.bValue) 		
	{
		return MyRiotShieldMethod(i, autowall, damage, isVehicle);
	}

	return false;

}

bool Aim_t::IsAbletoFire(int state) {
	switch (state) {
		//uav
	case 1:
	case 3:
		//reload
	case 8:
		//granata (null_knife)
	case 16:
	case 17:
	case 18:
	case 19:
	case 21:
		return false;
	}

	return true;

}

int  Aim_t::GetClosestClientByDistance(bool isVehicle)
{
	clientNum = -1;
	isReady[isReadyforKnife] = false;
	auto autoWallValue = Settings[auto_wall].Value.iValue;

	TargetInfo_t targetInfo;
	std::vector<TargetInfo_t> vTargetInfo;

	for (int i = 0; i < cgs->maxClients; ++i)
	{
		if (Entity.IsValid(i) && Entity.IsEnemy(i))
		{		
			if (autoWallValue != 0)
			{				
				if (GetVisibility(i, autoWallValue, &targetInfo.distance, isVehicle))
				{
					targetInfo.distanceToKnife = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin) / 36.0f;
					VectorCopy(vAimLocation, targetInfo.vAimLocation);
					targetInfo.clientNum = i;	

					vTargetInfo.push_back(targetInfo);
				}					
			}
			else
			{
				if (GetVisibility(i, autoWallValue, nullptr, isVehicle))
				{
					targetInfo.distance = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
					targetInfo.distanceToKnife = targetInfo.distance / 36.0f;
					VectorCopy(vAimLocation, targetInfo.vAimLocation);
					targetInfo.clientNum = i;

					vTargetInfo.push_back(targetInfo);
				}
			}
		}
	}

	if (!vTargetInfo.empty())
	{			
		std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const TargetInfo_t& a, const TargetInfo_t& b)
		{
			if (autoWallValue != 0)
			{
				return isVehicle ?
					(a.distance < b.distance) : //damage: more power   = closer
					(a.distance > b.distance);	//normal: min distance = closer	 	
			}
			else
			{
				return (a.distance < b.distance);
			}		
		});	
		
		if (IsAbletoFire(cg->state))
			isReady[isReadyforFire] = true;

		if (Settings[auto_knife].Value.bValue && vTargetInfo.front().distanceToKnife < 3.5f)
			isReady[isReadyforKnife] = true;

		VectorCopy(vTargetInfo.front().vAimLocation, vAimLocation);

		clientNum = vTargetInfo.front().clientNum;
			
	}
	
	return clientNum;		
}



int  Aim_t::GetClosestClientByFOV()
{	
	isReady[isReadyforKnife] = false;
	clientNum = -1;	

	for (int i = 0; i < cgs->maxClients; ++i)
	{
		if (Entity.IsValid(i) && Entity.IsEnemy(i))
		{
			//non riot.
			if (cg_entities[i].weaponID != 2 && cg_entities[i].weaponID != 3)
			{				
				if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_spine4], bestBone) && Autowall.NormalVisible(bestBone))
				{
					if (Math.GetFov(refdef->rdViewAngle, bestBone, refdef->vieworg) <= Settings[fov].Value.fValue)
					{
						VectorCopy(bestBone, vAimLocation);

						if (IsAbletoFire(cg->state))
							isReady[isReadyforFire] = true;

						if (Settings[auto_knife].Value.bValue &&
							(Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin) / 36.0f < 3.5f))
							isReady[isReadyforKnife] = true;
						
						clientNum = i;
					}					
				}
			}			
		}
	}

	return clientNum;
}

void Aim_t::AutoKnife()
{
	if (!Settings[auto_knife].Value.bValue)
		return;

	if (isVehicle)
		return;

	isReady[isReadyforKnife] = 0;
	
	clientDistance = FLT_MAX;

	for (int i = 0; i < cgs->maxClients; ++i)
	{
		if (Entity.IsValid(i) && Entity.IsEnemy(i))
		{
			tmpDist = Math.GetDistance(refdef->vieworg, cg_entities[i].lerpOrigin);
			if (tmpDist < clientDistance)
			{				
				if (Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_spine4], bestBone))
				{
					if (Autowall.NormalVisible(bestBone))
					{
						if (tmpDist / 36.0f < 3.5f)
						{
							VectorCopy(bestBone, vAimLocation);
							isReady[isReadyforKnife] = true;							
							clientDistance = tmpDist;							
						}
					}
				}
			}
		}
	}

	static int knife_state = false;
	if (isReady[isReadyforKnife] && !knife_state)
	{
		VectorSubtract(vAimLocation, refdef->vieworg, vDelta);
		VectorNormalize(vDelta);
		pViewMatrix->viewAngleX += Math.Degrees(asinf(DotProduct(refdef->viewaxis[1], vDelta)) * 1.57f);
		pViewMatrix->viewAngleY += Math.Degrees(-asinf(DotProduct(refdef->viewaxis[2], vDelta)) * 1.57f);

		key_input->melee_.wasPressed = 1;

		knife_state = true;
	}
	else if (knife_state)
	{
		key_input->melee_.wasPressed = 0;
		knife_state = false;
	}	
}

void Aim_t::PerformAimbot()
{
	
	bool predator = false;

	weapon_t *pMYWEAPON = Engine.GetCombatWeapon(cg_entities[cg->clientNum].weaponID);
	if (pMYWEAPON && pMYWEAPON->szName)
	{
		if (strstr(pMYWEAPON->szName, "predator"))
		{
			isVehicle = predator = true;
		}
		else if (strstr(pMYWEAPON->szName, "remote") || strstr(pMYWEAPON->szName, "ac130"))
		{
			isVehicle = true;
		}
	}

	switch (Settings[aim_mode].Value.iValue)
	{		
	case 0:
		closestPlayer = GetClosestClientByFOV();		
		break;

	case 1:
		closestPlayer = GetClosestClientByDistance(isVehicle);
	}

	if (closestPlayer != -1)
	{
		VectorSubtract(vAimLocation, refdef->vieworg, vDelta);	

		if (Settings[silent_aim].Value.bValue)
		{
			if (isVehicle || isReady[isReadyforKnife] || Settings[third_person].Value.bValue)
			{
				//Special case with Predator
				if (predator)
				{
					float vScreen[2];
					if (Engine.WorldToScreen(vAimLocation, vScreen))
						PredatorAim(vScreen[0], vScreen[1]);
				}
				else
				{
					VectorNormalize(vDelta);
					pViewMatrix->viewAngleX += Math.Degrees(asinf(DotProduct(refdef->viewaxis[1], vDelta)));
					pViewMatrix->viewAngleY += Math.Degrees(-asinf(DotProduct(refdef->viewaxis[2], vDelta)));
				}
			}
			else
			{
				if (isReady[isReadyforFire])
				{
					Math.VecToAngles(vDelta, vAimAngles);
					vAimAngles[1] -= punch->weaponViewAngle_1;
					vAimAngles[0] -= punch->weaponViewAngle_0;
				}
			}
		}
		else
		{
			//Special case with Predator
			if (predator)
			{
				float vScreen[2];
				if (Engine.WorldToScreen(vAimLocation, vScreen))
					PredatorAim(vScreen[0], vScreen[1]);
			}
			else
			{
				if (isReady[isReadyforFire])
				{
					VectorNormalize(vDelta);
					pViewMatrix->viewAngleX += Math.Degrees(asinf(DotProduct(refdef->viewaxis[1], vDelta)));
					pViewMatrix->viewAngleY += Math.Degrees(-asinf(DotProduct(refdef->viewaxis[2], vDelta)));
				}
			}
		}


		static bool bMelee = false;
		if (!bMelee && isReady[isReadyforKnife])
		{
			key_input->melee_.wasPressed = 1;
			bMelee = true;
		}
		else if (bMelee)
		{
			key_input->melee_.wasPressed = 0;
			bMelee = false;
		}

		if (Settings[names_stealer].Value.bValue && strcmp(ci[cg->clientNum].name, ci[closestPlayer].name) != 0)
		{
			if (timeChanged == 0)
			{
				/*strcpy((char*)Offsets::namegame, ci[closestPlayer].name);
				Engine.SendToConsole("Name Let's go!");*/


				std::string toSend = "userinfo \"";
				toSend += "\\name\\"; toSend += ci[closestPlayer].name;
				toSend += "\"";
				Engine.SendToConsole((char*)(toSend.c_str()));


			}
			++timeChanged;
			if (timeChanged >= 17)
				timeChanged = 0;
		}
	}
	

}

//========================================================================


void Aim_t::Run()
{
	isReady[isReadyforFire] = 0;

	isVehicle = false;

	if (!Settings[aim_enabled].Value.bValue)		
		return;	

	switch (Settings[aim_key].Value.iValue)
	{
	case 1:
		if (g_bAimbotKey[g_bAimbotL])
			PerformAimbot(); //Everything with knife 1
		else
			AutoKnife(); //Only knife
		break;
	case 2:
		if (g_bAimbotKey[g_bAimbotR])
			PerformAimbot(); //Everything with knife 2
		else
			AutoKnife(); //Only knife
		break;
	case 3:
		if (g_bAimbotKey[g_bAimbotM])
			PerformAimbot(); //Everything with knife 3
		else
			AutoKnife(); //Only knife
		break;
	case 4:
		if (g_bAimbotKey[g_bAimbotCtrl]) //Ctrl Key
			PerformAimbot();
		else
			AutoKnife();
		break;
	case 5:
		if (g_bAimbotKey[g_bAimbotShift]) //Shift Key
			PerformAimbot();
		else
			AutoKnife();
		break;

	default:
		PerformAimbot();
	}
}


void Aim_t::Autoshoot()
{
	/*static int fire_state = 0;

	if (isVehicle && !fire_state && Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire]) {
		Engine.SendToConsole("+attack\n");
		fire_state = 1;
	}
	else if (fire_state) {
		Engine.SendToConsole("-attack\n");
		fire_state = 0;
	}	
	
	doesn't work
	*/	

	static bool bShooting = false;

	if (!bShooting && Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire]
		)//&& isVehicle)
	{
		key_input->attack.wasPressed = 1;
		bShooting = true;
	}
	else if (bShooting)
	{
		key_input->attack.wasPressed = 0;
		bShooting = false;
	}
}

void Aim_t::PredatorAim(float x, float y)
{
	float ScreenCenterX = (refdef->width / 2);
	float ScreenCenterY = (refdef->height / 2);
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
			TargetX = -(ScreenCenterX - x);

		if (x < ScreenCenterX)
			TargetX = x - ScreenCenterX;
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
			TargetY = -(ScreenCenterY - y);


		if (y < ScreenCenterY)
			TargetY = y - ScreenCenterY;
	}

	mouse_event(0x0001, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
}



