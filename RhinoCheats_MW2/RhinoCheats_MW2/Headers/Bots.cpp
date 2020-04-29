#include "stdafx.h"

Bots_t Bots;

void Bots_t::Scr_Notify(int entnum, short stringValue, unsigned int paramcount)
{
	((void(__cdecl*) (int, int, short, unsigned int))0x580180) (entnum, 0, stringValue, paramcount);
}

void Bots_t::Scr_AddString(const char* value)
{
	((void(__cdecl*) (const char*))0x581100) (value);
}

void Bots_t::bots_assignteam() {
	for (auto& bot : SpawnedBots) {
		Scr_AddString("autoassign");
		Scr_AddString("team_marinesopfor");
		Scr_Notify(bot->clientnumber, *(short*)0x1B6AB60, 2);
	}
}

void Bots_t::bots_assignclass() {
	for (auto& bot : SpawnedBots) {
		Scr_AddString("class1");
		Scr_AddString("changeclass");
		Scr_Notify(bot->clientnumber, *(short*)0x1B6AB60, 2);
	}
}

void Bots_t::addbots() {
	if (cg->isIngame && *(int*)Offsets::host) {
		SpawnedBots.clear();
		for (int i = 0; i < 17; i++) {
			gentity_t* bot = ((gentity_t * (__cdecl*) ())0x588280)();
			SpawnedBots.push_back(bot);
		}
		Sleep(200);
		bots_assignteam();
		Sleep(200);
		bots_assignclass();
	}
}