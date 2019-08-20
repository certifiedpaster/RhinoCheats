#include "stdafx.h"

Entity_t Entity;

//========================================================================

bool Entity_t::IsValid(int i)
{
	if ((cg_entities[i].IsAlive & 2) && (cg->clientNum != i))
	{		
		return true;
	}

	return false;
}

bool Entity_t::IsEnemy(int i)
{
	if (ci[i].team != ci[cg->clientNum].team
		|| ci[i].team == TEAM_FREE)
		return true;

	return false;
}

void Entity_t::DrawEsp()
{	
	weapon_t *pWeapon = nullptr;

	/************************************************************************/
	/* Players                                                              */
	/************************************************************************/
	for (int i = 0; i < 1024; ++i)
	{	
		if (!IsValid(i))
			continue;

		switch (cg_entities[i].Type)
		{
		case ET_PLAYER:			
				
			if (!IsEnemy(i))
				continue;

			ImVec4Copy(ImVec4(1, 1, 1, 1), colVisible);
			if (IsBoneVisible(&cg_entities[i], j_helmet))
				ImVec4Copy(ImVec4(0, 1, 0, 1), colVisible);

			Radar.fRadarAngle = rdViewAngles->angles[1] - cg_entities[i].lerpAngles[1];
			Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin, colVisible, *(int *)0xD52318 /*CompassPlayerIcon*/, Radar.fRadarAngle);
				
			if (!Engine.Getvectorfromtag(&cg_entities[i], Entity.bones[j_helmet], vHead))
				continue;

			if (Engine.WorldToScreen(vHead, screen) && Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{
				//required for box
				dY = base[1] - screen[1];
				if (dY < 10)
					dX = dY * 3;
				else
					dX = dY / 2;

				Drawing.DrawString(screen[0], screen[1] - 10, Fonts->smallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 0, true, ci[i].name);
				
				{
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_helmet], Entity.bones[j_neck], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_neck], Entity.bones[j_shoulder_le], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_neck], Entity.bones[j_shoulder_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_shoulder_le], Entity.bones[j_elbow_le], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_shoulder_ri], Entity.bones[j_elbow_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_elbow_le], Entity.bones[j_wrist_le], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_elbow_ri], Entity.bones[j_wrist_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_neck], Entity.bones[j_mainroot], colVisible);

					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_mainroot], Entity.bones[j_hip_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_mainroot], Entity.bones[j_hip_le], colVisible);

					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_hip_ri], Entity.bones[j_knee_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_hip_le], Entity.bones[j_knee_le], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_knee_ri], Entity.bones[j_ankle_ri], colVisible);
					Drawing.DrawLineBone(&cg_entities[i], Entity.bones[j_knee_le], Entity.bones[j_ankle_le], colVisible);

				}
								
				Drawing.DrawRect(base[0] - dX / 2, screen[1], dX, dY, 1.0f, colVisible, true);

				pWeapon = GetWeapon(cg_entities[i].WeaponID);
				if (pWeapon && pWeapon->WeaponShader)
				{
					float vColor[] = { colVisible.x, colVisible.y, colVisible.z, colVisible.w };
					R_AddDrawStretchPic(base[0] - 24, base[1] + 3, 45, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
				}
			}

			break;

		case ET_ITEM:

			//Cyan
			ImVec4Copy(ImVec4(0.0f, 0.7f, 0.7f, 0.5f), misc_color);

			pWeapon = GetWeapon(cg_entities[i].PickupId);
			if (pWeapon && pWeapon->WeaponShader)
			{
				Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
					misc_color, pWeapon->WeaponShader, 0);

				if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
				{
					/*
					Doesn't work, because R_AddDrawStretchPic :  x,y,z,w, 0, 0, 0.0f, 0.0f
					
					Drawing.DrawShader(
						base[0] - 25, base[1] - 25, // x, y
						40.0f,
						40.0f,
						misc_color,
						pWeapon->WeaponShader);*/
					float vColor[] = { misc_color.x, misc_color.y, misc_color.z, 1.0f };
					R_AddDrawStretchPic(base[0] - 25, base[1] - 25, 40.0f, 40.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
				}
			}

			break;

		case ET_MISSILE:

			ImVec4Copy(ImVec4(1, 1, 1, 1), misc_color);

			if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{
				pWeapon = GetWeapon(cg_entities[i].WeaponID);
				if (pWeapon && pWeapon->WeaponShader) {
					//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

					float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
					R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
				}
				else
				{
					WeaponDef_t* weapondef = BG_GetWeapondef(cg_entities[i].WeaponID);
					if (weapondef && weapondef->UIName)
					{
						Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "%s", weapondef->UIName);
					}
					else
					{
						Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "Dangerous");
					}
				}
			}

			break;

		case ET_PLANE:

			ImVec4Copy(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), misc_color);

			pWeapon = GetWeapon(cg_entities[i].WeaponID);
			if (pWeapon && pWeapon->WeaponShader)			
				Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
					misc_color, pWeapon->WeaponShader, 0);

			if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
			{				
				if (pWeapon && pWeapon->WeaponShader) {
					//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

					float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
					R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
				}
				else
				{
					Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "Plane");
				}				
			}

			break;


		case ET_HELICOPTER:

			ImVec4Copy(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), misc_color);

			for (int heli_index = 0; heli_index < 8; heli_index++)
			{
				dogs_t* pHeli = (dogs_t*)(0xC98AF8 + (heli_index * 0x1C));
				if (!pHeli)
					continue;

				if (pHeli->EntityIndex == i)
				{
					if (pHeli->Team != ci[cg->clientNum].team)
					{
						pWeapon = GetWeapon(cg_entities[i].WeaponID);
						if (pWeapon && pWeapon->WeaponShader)
							Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
								misc_color, pWeapon->WeaponShader, 0);

						if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
						{
							if (pWeapon && pWeapon->WeaponShader) {
								//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

								float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
								R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
							}
							else
							{
								Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "Helicopter");
							}
						}						
					}
					
					break;
				}
			}

			break;


		case ET_ACTOR:

			ImVec4Copy(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), misc_color);

			for (int dog_index = 0; dog_index < 8; dog_index++)
			{
				dogs_t* pDog = (dogs_t*)(0xC98078 + (dog_index * 0x20));
				if (!pDog)
					continue;

				if (pDog->EntityIndex == i)
				{
					if (pDog->Team != ci[cg->clientNum].team)
					{
						pWeapon = GetWeapon(cg_entities[i].WeaponID);
						if (pWeapon && pWeapon->WeaponShader)
							Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
								misc_color, pWeapon->WeaponShader, 0);

						if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
						{							
							if (pWeapon && pWeapon->WeaponShader) {
								//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

								float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
								R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
							}
							else
							{
								Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "Dog");
							}
						}						
					}
					
					break;
				}
			}

			break;



		case ET_VEHICLE:

			ImVec4Copy(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), misc_color);

			for (int rcxd_index = 0; rcxd_index < 8; rcxd_index++)
			{
				dogs_t* pRCXD = (dogs_t*)(0xC97EB8 + (rcxd_index * 0x1C));
				if (!pRCXD)
					continue;

				if (pRCXD->EntityIndex == i)
				{
					if (pRCXD->Team != ci[cg->clientNum].team)
					{
						pWeapon = GetWeapon(cg_entities[i].WeaponID);
						if (pWeapon && pWeapon->WeaponShader)
							Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
								misc_color, pWeapon->WeaponShader, 0);

						if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
						{							
							if (pWeapon && pWeapon->WeaponShader) {
								//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

								float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
								R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
							}
							else
							{
								Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "RC-XD");
							}
						}						
					}
					
					break;
				}
			}

			break;



		case ET_MG42:

			ImVec4Copy(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), misc_color);

			for (int sentry_index = 0; sentry_index < 8; sentry_index++)
			{
				dogs_t* pSentry = (dogs_t*)(0xC97A38 + (sentry_index * 0x24));
				if (!pSentry)
					continue;

				if (pSentry->EntityIndex == i)
				{
					if (pSentry->Team != ci[cg->clientNum].team)
					{
						pWeapon = GetWeapon(cg_entities[i].WeaponID);
						if (pWeapon && pWeapon->WeaponShader)
							Drawing.DrawCompassIcon(refdef->width / 2.f - 250.0f, 108.0f, 500.f, 500.f, refdef->vieworg, cg_entities[i].lerpOrigin,
								misc_color, pWeapon->WeaponShader, 0);

						if (Engine.WorldToScreen(cg_entities[i].lerpOrigin, base))
						{							
							if (pWeapon && pWeapon->WeaponShader) {
								//Drawing.DrawShader(base[0] - 10, base[1] - 10, 20.0f, 20.0f, misc_color, pWeapon->WeaponShader);

								float vColor[] = { misc_color.x, misc_color.y, misc_color.z, misc_color.w };
								R_AddDrawStretchPic(base[0] - 10, base[1] - 10, 20.0f, 20.0f, 0, 0, 1.0f, 1.0f, vColor, pWeapon->WeaponShader);
							}
							else
							{
								Drawing.DrawString(base[0], base[1], Fonts->smallfont, 0.7, misc_color, 0, 1, "Sentry");
							}
						}						
					}

					
					break;
				}
			}

			break;

		}
		
	}

}


bool Entity_t::IsBoneVisible(centity_t* pEntity, int iBone)
{
	bool result = 0;

	WORD usBone = Entity.bones[iBone];
	if (!usBone)
		return result;

	static DWORD pCvisiblebone = 0x7A84D0;
	__asm
	{
		push 0
		xor eax, eax
		movzx eax, usBone
		mov ecx, pEntity
		call pCvisiblebone
		add esp, 4
		mov result, al
	}

	return result;
}

