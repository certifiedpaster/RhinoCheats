#include "stdafx.h"

Entity_t Entity;

//========================================================================

bool Entity_t::IsValid(int i)
{
	if (cg_entities[i].valid && (cg_entities[i].IsAlive & 1) && (cg->clientNum != i))
	{
		// ToDo: Need a faster way for this
		// disadvantage: you won't be able to stop them because they disappear	
		/*{
			weapon_t *pMyWeapon = Engine.GetCombatWeapon(cg_entities[i].weaponID);
			if (pMyWeapon != nullptr && pMyWeapon->szName != nullptr &&
				(strstr(pMyWeapon->szName, "remote")
					|| strstr(pMyWeapon->szName, "predator")
					|| strstr(pMyWeapon->szName, "ac130")))
				return false; //avoid users inside theses vehicles.

			return true;
		}*/

		return true;
	}

	return false;
}

bool Entity_t::IsEnemy(int i)
{
	if (Settings[team_killer].Value.bValue)
	{
		if (ci[i].team == ci[cg->clientNum].team)
			return true;

		return false;
	}

	if (ci[i].team != ci[cg->clientNum].team || ci[i].team == TEAM_FREE)
		return true;

	return false;
}

bool Entity_t::IsUsingRiot(int i)
{
	if (cg_entities[i].weaponID != 2 || cg_entities[i].weaponID != 3)
		return false;

	return true;
}

void Entity_t::DrawEsp()
{
	if (!Settings[esp_enabled].Value.bValue)
		return;

	/************************************************************************/
	/* Players                                                              */
	/************************************************************************/
	for (int i = 0; i < cgs->maxClients; ++i)
	{
		if (IsValid(i))
		{
			if (!Engine.Getvectorfromtag(&cg_entities[i], bones[j_helmet], vHead))
				continue;

			if (IsEnemy(i))
				isEnemy = true;
			else
				isEnemy = false;

			//Don't draw the enemy is esp_enemy is off
			if (isEnemy && !Settings[esp_enemy].Value.bValue)
				continue;
			//Don't draw the friendly is esp_friendly is off
			if (!isEnemy && !Settings[esp_friendly].Value.bValue)
				continue;

			if (isEnemy)
			{
				ImVec4Copy(
					(Settings[rainbow_mode].Value.bValue ? Drawing.colRainBow : Settings[esp_enemy_color].Value.v4Value),
					colVisible);

				if (Autowall.NormalVisible(vHead))
				{
					ImVec4Copy(Settings[esp_visible_color].Value.v4Value, colVisible);
				}
			}
			else
			{
				ImVec4Copy(Settings[esp_friendly_color].Value.v4Value, colVisible);
			}

			if (!Settings[esp_bright_colors].Value.bValue)
				colVisible.w = 0.5f;


			/************************************************************************/
			/* Radars                                                               */
			/************************************************************************/
			if (Settings[esp_compass_radar].Value.bValue || Settings[esp_radar].Value.bValue)
				Radar.fRadarAngle = refdef->rdViewAngle[1] - cg_entities[i].lerpAngles[1];

			//Square radar
			if (Settings[esp_radar].Value.bValue)
			{
				Radar.CalcRadarPoint(cg_entities[i].lerpOrigin, &Radar.screenX, &Radar.screenY);
				Radar.screenX -= 5;
				Radar.screenY -= 5;
				float vColor[] = { colVisible.x, colVisible.y, colVisible.z, colVisible.w };
				DrawRotatedPic(GSM(0), Radar.screenX, Radar.screenY, 15.0f, 15.0f, Radar.fRadarAngle, vColor, *(int *)Offsets::hud_dpad_arrow /*CompassPlayerIcon*/);
			}

			//compass radar	
			if (Settings[esp_compass_radar].Value.bValue)
				Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin, colVisible, *(int *)Offsets::hud_dpad_arrow /*CompassPlayerIcon*/, Radar.fRadarAngle);
			
			if (Engine.WorldToScreen(vHead, screen) && Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{

				if (
					(Settings[esp_box].Value.iValue && Settings[esp_name_boxed].Value.bValue) ||
					(Settings[esp_box].Value.iValue || Settings[esp_name_boxed].Value.bValue)
					)
				{
					dY = base[1] - screen[1];
					if (dY < 10)
						dX = dY * 3;
					else
						dX = dY / 2;
				}

				if (Settings[esp_name].Value.bValue)
				{
					if (Settings[esp_box].Value.iValue != 6) //All except with Lilli's boxes.
					{
						if (!Settings[esp_name_boxed].Value.bValue)
						{
							//White color text fix
							if (colVisible.x == 1.0f
								&& colVisible.y == 1.0f
								&& colVisible.z == 1.0f)
							{
								Drawing.DrawGlowString(screen[0], screen[1] - 10, Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.5f), 0, true, ci[i].name);
								if (IsUsingRiot(i))
									Drawing.DrawGlowString(base[0], base[1], Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.5f), 0, true, "RiotShield");

							}
							else
							{
								Drawing.DrawGlowString(screen[0], screen[1] - 10, Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), colVisible, 0, true, ci[i].name);
								if (IsUsingRiot(i))
									Drawing.DrawGlowString(base[0], base[1], Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), colVisible, 0, true, "RiotShield");
							}
						}
						else {
							Drawing.DrawStringBoxed(screen[0], screen[1] - 13, fonts_Dev->smallDevFont, 1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 0, true, colVisible, ImVec4(colVisible.x, colVisible.y, colVisible.z, 0.2f), base[0] - dX / 2, dX, "%s", ci[i].name);
						}
					}
				}

				if (Settings[esp_conected_bones].Value.bValue)
				{
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[0], Entity.bones[2], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[2], Entity.bones[8], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[2], Entity.bones[7], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[8], Entity.bones[18], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[7], Entity.bones[17], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[18], Entity.bones[20], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[17], Entity.bones[19], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[2], Entity.bones[10], colVisible);

					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[10], Entity.bones[11], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[10], Entity.bones[12], colVisible);

					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[11], Entity.bones[15], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[12], Entity.bones[16], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[15], Entity.bones[21], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[16], Entity.bones[22], colVisible);

				}

				if (Settings[esp_box].Value.iValue)
				{
					//float Health_Value, fRed, fGreen, fSub;
					//ImVec4 colHealth;

					switch (Settings[esp_box].Value.iValue)
					{
						//head:screen
						//base:origin	

					case 1:
						Drawing.DrawCorners(base[0] - dX / 2, screen[1], dX, dY, 5, ImVec4(colVisible.x, colVisible.y, colVisible.z, 1.0f));
						break;

					case 2:
						Drawing.DrawCornersFilled(base[0] - dX / 2, screen[1], dX, dY, 5, ImVec4(colVisible.x, colVisible.y, colVisible.z, 1.0f), ImVec4(colVisible.x, colVisible.y, colVisible.z, 0.2f));
						break;

					case 3:
						Drawing.DrawRect(base[0] - dX / 2, screen[1], dX, dY, 1.0f, colVisible, true);
						break;

					case 4:
						Drawing.DrawRectFilled(base[0] - dX / 2, screen[1], dX, dY, 1.0f, colVisible, ImVec4(colVisible.x, colVisible.y, colVisible.z, 0.2f), true);

						/*Health_Value = static_cast<float>(ServerHealth[i].Health);
						if (Health_Value > 100)
							Health_Value = 100;

						fRed = 0;
						fGreen = 255;
						fSub = 255;
						fSub -= Health_Value * 2.55f;
						fGreen -= fSub;
						fRed += fSub;
						fGreen /= 255;
						fRed /= 255;

						MAKERGBA(colHealth, fRed, fGreen, fGreen, 1.0f);


						Drawing.DrawShader(screen[0] - 8, screen[1] - 2, 104.f, 5, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
						Drawing.DrawShader(screen[0] - 6, screen[1], Health_Value * 0.12f, 5, colHealth, *(DWORD *)Offsets::white);*/

						break;

					case 5:
						vec3_t Min;
						vec3_t Max;
						if (cg_entities[i].eFlags & 8)
						{
							Min[0] = cg_entities[i].lerpOrigin[0] - 25;
							Min[1] = cg_entities[i].lerpOrigin[1] - 45;
							Min[2] = cg_entities[i].lerpOrigin[2];

							Max[0] = cg_entities[i].lerpOrigin[0] + 25;
							Max[1] = cg_entities[i].lerpOrigin[1] + 45;
							Max[2] = cg_entities[i].lerpOrigin[2] + 20;
						}
						else if (cg_entities[i].eFlags & 4)
						{
							Min[0] = cg_entities[i].lerpOrigin[0] - 20;
							Min[1] = cg_entities[i].lerpOrigin[1] - 20;
							Min[2] = cg_entities[i].lerpOrigin[2];

							Max[0] = cg_entities[i].lerpOrigin[0] + 20;
							Max[1] = cg_entities[i].lerpOrigin[1] + 20;
							Max[2] = cg_entities[i].lerpOrigin[2] + 42;
						}
						else
						{
							Min[0] = cg_entities[i].lerpOrigin[0] - 25;
							Min[1] = cg_entities[i].lerpOrigin[1] - 25;
							Min[2] = cg_entities[i].lerpOrigin[2];

							Max[0] = cg_entities[i].lerpOrigin[0] + 25;
							Max[1] = cg_entities[i].lerpOrigin[1] + 25;
							Max[2] = cg_entities[i].lerpOrigin[2] + 65;
						}
						Drawing.DrawCube(Min, Max, colVisible, x3, y3);
						break;


					case 6:
						//Lilli'x boxes.
						Drawing.DrawStringBoxed(screen[0], screen[1] - 13, fonts_Dev->smallDevFont, 1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 0, true, colVisible, ImVec4(colVisible.x, colVisible.y, colVisible.z, 0.2f), base[0] - dX / 2, dX, "%s", ci[i].name);
						Drawing.DrawRectFilled(base[0] - dX / 2, screen[1], dX, dY, 1.0f, colVisible, ImVec4(colVisible.x, colVisible.y, colVisible.z, 0.2f), true);

					}
				}

				if (Settings[esp_weapon].Value.bValue)
					Drawing.DrawShader(base[0] - 24, base[1] + 3, 45, 20, colVisible, Engine.GetCombatWeapon(cg_entities[i].weaponID)->Icon);

				switch (Settings[esp_snap_lines].Value.iValue)
				{
				case 1: //top center

					if (Settings[esp_box].Value.iValue != 5)
						Drawing.DrawLine(refdef->width / 2.0f, 0, base[0] - dX / 2, screen[1], colVisible, 1);
					else if (Settings[esp_box].Value.iValue == 5)
						Drawing.DrawLine(refdef->width / 2.0f, 0, x3, y3, colVisible, 1);
					else
						Drawing.DrawLine(refdef->width / 2.0f, 0, screen[0], screen[1], colVisible, 1);
					break;

				case 2: //crosshair			
					if (Settings[esp_box].Value.iValue != 5)
						Drawing.DrawLine(refdef->width / 2.0f, refdef->height / 2.0f, base[0] - dX / 2, screen[1], colVisible, 1);
					else if (Settings[esp_box].Value.iValue == 5)
						Drawing.DrawLine(refdef->width / 2.0f, refdef->height / 2.0f, x3, y3, colVisible, 1);
					else
						Drawing.DrawLine(refdef->width / 2.0f, refdef->height / 2.0f, screen[0], screen[1], colVisible, 1);
					break;

				case 3: //base	
					if (Settings[esp_box].Value.iValue != 5)
						Drawing.DrawLine(refdef->width / 2.0f, static_cast<float>(refdef->height), base[0] - dX / 2, screen[1], colVisible, 1);
					else if (Settings[esp_box].Value.iValue == 5)
						Drawing.DrawLine(refdef->width / 2.0f, static_cast<float>(refdef->height), x3, y3, colVisible, 1);
					else
						Drawing.DrawLine(refdef->width / 2.0f, static_cast<float>(refdef->height), screen[0], screen[1], colVisible, 1);
					break;
				}
			}
		}
	}

	/************************************************************************/
	/* Misc                                                                 */
	/************************************************************************/
	if (!Settings[esp_misc].Value.bValue)
		return;

	for (int i = cgs->maxClients; i <= cg->maxEntities; ++i)
	{
		if (cg_entities[i].eType != ET_ITEM &&
			cg_entities[i].eType != ET_MISSILE &&
			cg_entities[i].eType != ET_TURRET &&
			cg_entities[i].eType != ET_HELICOPTER &&
			cg_entities[i].eType != ET_PLANE &&
			cg_entities[i].eType != ET_VEHICLE &&
			cg_entities[i].eType != ET_VEHICLE_COLLMAP &&
			cg_entities[i].eType != ET_VEHICLE_CORPSE &&
			cg_entities[i].eType != ET_VEHICLE_SPAWNER
			)
			continue;

		if (!IsValid(i))
			continue;

		/************************************************************************/
		/* Square radar for Misc                                                */
		/************************************************************************/

		// With this, i get Radar.screenX, Radar.screenY	
		bIsInsideSquareRadar = false;
		if (Settings[esp_radar].Value.bValue)
		{
			switch (cg_entities[i].eType)
			{
			case ET_ITEM:
				if (Settings[esp_misc_items].Value.bValue)
				{
					Radar.CalcRadarPoint(cg_entities[i].lerpOrigin, &Radar.screenX, &Radar.screenY);
					Radar.screenX -= 5;
					Radar.screenY -= 5;
					bIsInsideSquareRadar = true;
				}
				break;

			case ET_TURRET:
			case ET_HELICOPTER:
			case ET_PLANE:
			case ET_VEHICLE:
			case ET_VEHICLE_COLLMAP:
			case ET_VEHICLE_CORPSE:
			case ET_VEHICLE_SPAWNER:
				if (Settings[esp_misc_devices].Value.bValue)
				{
					Radar.CalcRadarPoint(cg_entities[i].lerpOrigin, &Radar.screenX, &Radar.screenY);
					Radar.screenX -= 5;
					Radar.screenY -= 5;
					bIsInsideSquareRadar = true;
				}
			}
		}


		switch (cg_entities[i].eType)
		{
		case ET_ITEM:

			if (!Settings[esp_misc_items].Value.bValue)
				break;

			ImVec4Copy(Settings[esp_misc_items_color].Value.v4Value, misc_color);

			if (!Settings[esp_bright_colors].Value.bValue)
				misc_color.w = 0.5f;

			if (Engine.GetCombatWeapon(cg_entities[i].PickupItemID % 1400))
				icon = Engine.GetCombatWeapon(cg_entities[i].PickupItemID % 1400)->Icon;

			if (Settings[esp_compass_radar].Value.bValue)
				Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
					icon ? misc_color : ImVec4(1.0f, 1.0f, 1.0f, misc_color.w),
					icon ? icon : ScavengerIcon,
					0);

			if (bIsInsideSquareRadar)
				Drawing.DrawShader(Radar.screenX, Radar.screenY, 10, 10,
					icon ? misc_color : ImVec4(1.0f, 1.0f, 1.0f, misc_color.w),
					icon ? icon : ScavengerIcon);

			if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{
				Drawing.DrawShader(
					base[0] - 25, base[1] - 25, // x, y
					icon ? 40.0f : 20.0f,
					icon ? 40.0f : 10.0f,
					icon ? misc_color : ImVec4(1.0f, 1.0f, 1.0f, misc_color.w),
					icon ? icon : ScavengerIcon);
			}

			break;

		case ET_MISSILE:

			if (!Settings[esp_misc_explosives].Value.bValue)
				break;

			ImVec4Copy(Settings[esp_misc_explosives_color].Value.v4Value, misc_color);

			if (!Settings[esp_bright_colors].Value.bValue)
				misc_color.w = 0.5f;

			if (Engine.GetCombatWeapon(cg_entities[i].weaponID))
				icon = Engine.GetCombatWeapon(cg_entities[i].weaponID)->Icon;

			if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{
				if (icon) {
					Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, icon);
					/*printf("explosive: icon: [%i], [%f,%f], CWeapon: [0x%X], %s\n",
						icon,
						base[0], base[1],
						Engine.GetCombatWeapon(cg_entities[i].weaponID),
						Engine.GetCombatWeapon(cg_entities[i].weaponID)->szName);*/							
				}
				else if (Engine.GetCombatWeapon(cg_entities[i].weaponID))
				{
					char *pSzName = Engine.GetCombatWeapon(cg_entities[i].weaponID)->szName;
					if (strstr(pSzName, "flash"))
					{
						Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, FlashbangIcon);
					}
					/*else
					{
						Drawing.DrawGlowString(base[0], base[1], Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, misc_color.w), misc_color, 90.0f, true, pSzName);
					}*/
				}
			}

			break;

		case ET_TURRET:
		case ET_HELICOPTER:
		case ET_PLANE:
		case ET_VEHICLE:
		case ET_VEHICLE_COLLMAP:
		case ET_VEHICLE_CORPSE:
		case ET_VEHICLE_SPAWNER:

			if (!Settings[esp_misc_devices].Value.bValue)
				break;

			ImVec4Copy(Settings[esp_misc_devices_color].Value.v4Value, misc_color);

			if (!Settings[esp_bright_colors].Value.bValue)
				misc_color.w = 0.5f;

			if (Engine.GetCombatWeapon(cg_entities[i].weaponID))
				icon = Engine.GetCombatWeapon(cg_entities[i].weaponID)->Icon;

			if (icon)
			{
				if (Settings[esp_compass_radar].Value.bValue)
					Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin, misc_color, icon, 0);

				if (bIsInsideSquareRadar)
					Drawing.DrawShader(Radar.screenX, Radar.screenY, 10, 10, misc_color, icon);
			}

			if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{
				if (icon) {
					Drawing.DrawShader(base[0] - 50, base[1] - 50, 100.f, 100.f, misc_color, icon);
				}
				else
				{
					Drawing.DrawGlowString(base[0], base[1], Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, misc_color.w), misc_color, 0, true, "[Dangerous]");
				}
			}
		}
	}

}


