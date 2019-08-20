#pragma once

class KillSpam_t
{
	typedef struct {
		char name[16];
		int deaths;
	}victim_t;

	victim_t enemies[17];

	std::string pStrBuffer;

	int     MyKills;
	int     MyHeadshots;

	int     FinalKill;
	int     FinalHeadshot;

public:

	void Init();
	void KillsUpdate();

};

extern KillSpam_t KillSpam;