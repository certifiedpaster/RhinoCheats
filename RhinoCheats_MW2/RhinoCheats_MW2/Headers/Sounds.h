#pragma once

class Sounds_t
{
public:
	bool bGetValuesOnce;

	//------
	char activated[MAX_PATH];
	char start[MAX_PATH];
	char menu[MAX_PATH];
	char menu_toggle[MAX_PATH];

	//kill sounds
	char laughtspy[MAX_PATH];
	char laughtscout[MAX_PATH];
	char laughtdemoman[MAX_PATH];
	char laughtmedic[MAX_PATH];
	char laughtsniper[MAX_PATH];

	char headshot[MAX_PATH];
	char firstblood[MAX_PATH];
	char doublekill[MAX_PATH];
	char tripplekill[MAX_PATH];
	char excellent[MAX_PATH];
	char impressive[MAX_PATH];
	char killingspree[MAX_PATH];
	char multikill[MAX_PATH];
	char ultrakill[MAX_PATH];
	char dominating[MAX_PATH];
	char unstoppable[MAX_PATH];
	char monsterkill[MAX_PATH];
	char wickedsick[MAX_PATH];
	char godlike[MAX_PATH];
	char ludicrouskill[MAX_PATH];
	char holyshit[MAX_PATH];
	char rampage[MAX_PATH];
	char humiliation[MAX_PATH];

	char assist[MAX_PATH];

	//========================================================================

	int HeadShotHolder;
	int KillsHolder;
	int DeathHolder;
	int AssistHolder;
	int KillTotal;

	bool Start_;
	bool bFirstBlood;
	bool bDoublekill;
	bool bTriplekill;
	bool bExcellent;
	bool bRampage;
	bool bKillingspree;
	bool bMultikill;
	bool bUltrakill;
	bool bDominating;
	bool bMonsterkill;
	bool bWickedSick;
	bool bUnstoppable;
	bool bGodlike;
	bool bLudicrouskill;
	bool bHolyshit;
	bool bImpressive;
	bool blaught_0;
	bool blaught_1;
	bool blaught_2;
	bool blaught_3;
	bool blaught_4;


	//========================================================================	

	Sounds_t();

	void killsound();
	void MapSoundDirectory(const char *soundsFolder);
};

extern Sounds_t sounds;


/***********************************************************************
  They're using public Twice's Autowall src posted on GD, Well,
  it's fair if we use their icon effect isn't it?.
  Yeah, they play like that, lets play like that too lol.
  Anyway, I never saw a P2C giving credits to REAL developers like KingOrgy or Twice, etc.
  So Yeah, Lets do it :)
/************************************************************************/

#define TIMER_VALUE 117
#define TEXTOJBJECT_SIZE 15

struct A17_MovingText
{
	bool InUse;
	bool OnceFix;
	int Counter;
	float Rotate;
	int KillIcon;
	float Size;
	float PosY;
	float PosX;

	A17_MovingText()
	{
		Rotate = 0;
		InUse = false;
	}
	void Update(int CurCount);
	void Setup(int Icon);
	void Reset()
	{
		InUse = false;
		PosY = refdef->height / 2 - 37.5f;
		PosX = refdef->width / 2 - 37.5f;
		Rotate = 0;
		OnceFix = true;
	}
};
struct A17_Sounds
{
	int Origin;
	int Destination;
	int Size;
	int NumOfSymbols;
	int Rotating;
	int Shader;
	int* CurKills;
	int Spree;
	int LastKills;
	int Timer;
	int KillCounter;
	int KillCounter2;
	int CurTextIndex;
	A17_MovingText TextObjects[TEXTOJBJECT_SIZE];

	void Init();
	int  GetKills() { return *CurKills; }
	void Update();

};

extern A17_Sounds KillSpree;