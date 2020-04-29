#pragma once

class Bots_t {
public:

	void Scr_Notify(int entnum, short stringValue, unsigned int paramcount);
	void Scr_AddString(const char* value);
	void bots_assignteam();
	void bots_assignclass();
	void addbots();

private:

	struct gclient_t {
		char pad_0x0000[0x31DC];  // 0
		unsigned char Team;    // 31DC 
		char pad_0x31DD[0x227];   // 31DD
	};         // Size = 0x3404

	struct gentity_t {
		unsigned int clientnumber;  // 0
		unsigned int eType;    // 4
		char pad_0x0[0x150];   // 8
		gclient_t* gclient;    // 158
		char pad_0x15C[0x118];   // 15C
	};         // Size = 0x274

	std::vector<gentity_t*> SpawnedBots;
};

extern Bots_t Bots;