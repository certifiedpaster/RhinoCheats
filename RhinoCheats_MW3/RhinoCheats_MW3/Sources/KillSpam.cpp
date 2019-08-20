#include "stdafx.h"

KillSpam_t KillSpam;

void KillSpam_t::Init()
{
	for (int i = 0; i < 17; i++)
	{
		memset(enemies[i].name, 0, 16);
		enemies[i].deaths = 0;
	}

	MyKills = pStats->kills;
	MyHeadshots = pStats->headshots;

	FinalKill = MyKills;
	FinalHeadshot = MyHeadshots;
}

void KillSpam_t::KillsUpdate()
{
	if (!((cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1)) && Settings[kill_spam].Value.bValue))
		return;

	bool bHeadshot = false;

	int enemypos = -1;

	if (pStats->kills != FinalKill)
	{
		char *pvictim = reinterpret_cast<char*>(Offsets::victim);

		//search if the enemy is registered		
		for (int i = 0; i < 17; i++)
		{
			if (strstr(enemies[i].name, pvictim))
			{
				enemypos = i;
				break;
			}
		}

		//if the enemy isn't registered
		if (enemypos == -1)
		{
			//search for a free position of the array for registration
			for (int i = 0; i < 17; i++)
			{
				if (!enemies[i].deaths)
				{
					enemypos = i;
					break;
				}
			}
			//we have a free position ready
			if (enemypos != -1)
			{
				strcpy_s(enemies[enemypos].name, pvictim);
				enemies[enemypos].deaths += 1;
			}
		}
		else
		{
			//if the enemy is registered, update their deaths
			enemies[enemypos].deaths += 1;
		}


		if (pStats->headshots != FinalHeadshot)
		{
			bHeadshot = true;
			FinalHeadshot = pStats->headshots;
		}

		FinalKill = pStats->kills;
	}

	//now, let's spam the server
	if (enemypos != -1)
	{
		std::string sWeapon = " by my: ";
		weapon_t *pMYWEAPON = Engine.GetCombatWeapon(cg_entities[cg->clientNum].nextState.weaponID);
		if (pMYWEAPON && pMYWEAPON->szLocalizedName)
			sWeapon += Aim.isVehicle ? "KillStreak" : (pMYWEAPON->szLocalizedName + 7);
		else
			sWeapon = " by my skillz";


		pStrBuffer = STREAM(
			"say \"^3"
			<< enemies[enemypos].name
			<< std::string(bHeadshot ? " ^1have been^7 " + std::string(Engine.GetIconText(HeadShotIcon, 1.0f, 1.0f, TRUE)) : " ^7Died")
			<< sWeapon
			<< std::string(", Also Died ^3" + std::to_string(enemies[enemypos].deaths) + " ^7Times. ")
			<< Settings[kill_spam_text].Value.szValue << "\"");

		Engine.SendToConsole((char *)pStrBuffer.c_str());


	}
}