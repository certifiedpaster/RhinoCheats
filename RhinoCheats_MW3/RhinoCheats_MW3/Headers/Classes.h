#pragma once

//Defines
#define	CMD_BACKUP 0x7F
#define BUTTON_FIRE ( 1 << 0 )
#define BUTTON_CROUCHED ( 1 << 9 )

//Enums
enum entityType_t
{
	ET_GENERAL,
	ET_PLAYER,
	ET_PLAYER_CORPSE,
	ET_ITEM,
	ET_MISSILE,
	ET_INVISIBLE,
	ET_SCRIPTMOVER,
	ET_SOUND_BLEND,
	ET_FX,
	ET_LOOP_FX,
	ET_PRIMARY_LIGHT,
	ET_TURRET,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_SPAWNER,
};

enum team_t
{
	TEAM_FREE,
	TEAM_AXIS,
	TEAM_ALLIES,
	TEAM_SPECTATOR
};

enum svscmd_type {
	SV_CMD_CAN_IGNORE = 0x0,
	SV_CMD_RELIABLE = 0x1,
};

//Typedefs
typedef int qhandle_t;
typedef void *qfont_t;

typedef struct {
	unsigned int weaponIdx : 8;
	unsigned int weaponVariation : 6;
	unsigned int weaponScopes : 3;
	unsigned int weaponUnderBarrels : 2;
	unsigned int weaponOthers : 7;
	unsigned int scopeVariation : 6;
}weaponx_t;

union Weapon {
	weaponx_t __s0;
	unsigned int data;
};

typedef struct  {
	int servertime; //0-4
	unsigned int buttons; //4-8
	int viewangles[3]; //8-14
	Weapon weapon; //14 - 18
	Weapon offHand; //18-1c
	char forwardmove; //1c
	char rightmove; //1d
	unsigned short airburstMarkDistance; //1E - 20
	unsigned short meleeChargeEnt; //20 - 22
	char meleeChargeDist; //22
	char selectedLoc[2]; //23 - 25
	char selectedLocAngle; //26
	unsigned short sightedClientsMask; //26 - 2A
	unsigned short spawnTraceEntIndex; //2A - 2C
}usercmd_t;

typedef struct
{
	usercmd_t usercmds[128];
	int currentCmdNum; // 0x16E8

	usercmd_t *GetUserCmd(int cmdNum)
	{
		if ((cmdNum & CMD_BACKUP) > 127)
			return nullptr;

		return &usercmds[cmdNum & CMD_BACKUP];
	}
}input_t;

typedef struct 
{
	/*
	float fraction;
	float normal[3];
	int surfaceFlags;
	int contents;
	const char *material;
	int hitType;
	unsigned __int16 hitId;
	unsigned __int16 modelIndex;
	unsigned __int16 partName;
	unsigned __int16 partGroup;
	bool allsolid;
	bool startsolid;
	bool walkable;
	*/

	/*float fraction; //0-4
	float normal[3]; //4-10
	int surfaceFlags; //10-14
	int contents; //14 - 18
	TraceHitType hitType; //18 - 1C
	unsigned short hitId; //1C - 1E
	unsigned short modelIndex; //1E - 20
	short partName; //20 - 22
	unsigned short partGroup; //22 - 24
	bool allsolid; //25
	bool startsolid; //26
	bool walkable; //28
	bool getPenetration; //29
	bool removePitchAndRollRotations; //2A*/


	float fraction;					//0x0000
	vec3_t surfaceDirection;		//0x0004
	int surfaceFlags;				//0x0010
	int contents;					//0x0014
	const char *material;			//0x0018
	int hitType;					//0x001c
	unsigned short entityNum;			//0x0020
	unsigned short modelIndex;		//0x0022
	unsigned short partName;		//0x0024
	unsigned short partGroup;		//0x0026
	bool allsolid;					//0x0028
	bool startsolid;				//0x0029
	bool walkable;					//0x002A
	char _0x002B[5];
	vec3_t endpos;					//0x0030 
	BYTE didHitEntity; //0x003C 
	char pad_0x003D[0x3]; //0x003D	
	__int32 materialType;		//0x0040


	char pad_0x0044[0x4]; //0x0044

}trace_t;

typedef struct {
	int	worldEntNum;	         // 00 v80
	int	ignoreEntIndex;	         // 04 v81
	float damageMultiplier;	     // 08 v82
	int	methodOfDeath;	         // 12 v83

	int pureCocaine;	// v84		

	vec3_t	origStart;	// v85
	vec3_t	start;		// v86
	vec3_t	end;		// v87
	vec3_t	dir;	    // v88
}BulletFireParams;//size= 68 or 0x44

typedef struct
{
	unsigned long trType; //0x0000 
	unsigned long trTime; //0x0004 
	unsigned long trDuration; //0x0008 
	vec3_t trBase; //0x000C 
	vec3_t trDelta; //0x0018 
}trajectory_t;//size 0x24 [ 36 ]

typedef struct
{
	int eFlags;                // 0
	trajectory_t pos;          // 4
	trajectory_t apos;         // 40
	unsigned char unknown[16]; // 76 
	BYTE secondWeaponID;       // 92
	unsigned char unknown1[11];// 93
}cLerpEntityState;//size 104 ( 0x68 )

typedef struct
{
	int					number;				// 0 //208
	int					eType;				// 4
	cLerpEntityState     Lerp;               // 8
	unsigned char unknown[88];              // 112
	BYTE                weaponID;           // 200 //Entity + 0x198 -> correcto
	unsigned char unknown2[55];// 201
}centityState_t;//size 256 ( 0x100 )			  

typedef struct
{
	char _0000[2]; //0x0000  
	WORD valid; //0x0002  
	char _0004[16]; //0x0004  
	vec3_t lerpOrigin; //0x0014  
	vec3_t lerpAngles; //0x0020  
	char _002C[60]; //0x002C  
	cLerpEntityState currentState;//0x0068 <- intro to lerpentstate
	centityState_t   nextState;//0x0D0 <- intro to entstate
	__int32 IsAlive; //0x01D0  
	char unknown468[36]; //0x01D4
}centity_t;

typedef struct
{
	char _0x0004[0xC];                                      //0x0 (0x9FCAF8)	
	char name[16];                                   //0xC (0x9FCB04)
	int team;                                        //0x1C (0x9FCB14)
	char _0x0020[0x4];                               //0x20
	int rank;                                        //0x24 (0x9FCB1C)
	char _0x0028[0x10];                              //0x28
	int perk;                                        //0x38 (0x9FCB30)
	char _0x003C[0x8];                               //0x3C
	int score;                                       //0x44 (0x9FCB3C)
	char _0x0048[0x458];                             //0x48
	int attacking;                                   //0x4A0 (0x9FCF98)
	char _0x04A4[0x4];                               //0x4A4
	int zooming;                                     //0x4A8 (0x9FCFA0)

}clientInfo_t;

typedef struct
{
	char unknown0[8]; //0x0000	
	__int32 width; //0008
	__int32 height; //000C
	float fov_x; //0010
	float fov_y; //0014
	vec3_t vieworg; //0018	
	vec3_t viewaxis[3]; //0024
	char _0x0048[4];
	vec3_t oldorigin; //0x004C 
	__int32 time; //0x0058 	
	__int32 rdflags; //0x005C 
	__int32 menu; //0x0060 

	char _0x0064[8];
	float isZoomed; //0x006C 
	char _0x0070[19132];

	/*float rdViewAngleY; //0x4B2C
	float rdViewAngleX; //0x4B30
	char _0x4B34[16];*/

	vec3_t rdViewAngle; //0x4B2C 
	char _0x4B38[12];

	vec3_t vOrigin1; //0x4B44 
	vec3_t vOrigin2; //0x4B50 
	char _0x4B5C[12];

	/*	After Menu,,,,char pad_0x0064[0x4AC8]; //0x0064
	vec3_t rdViewAngle; //0x4B2C
	char pad_0x4B38[0x73D4]; //0x4B38
	vec2_t PunchAngles; //0xBF0C  //0046268D   D91D BC409700    FSTP DWORD PTR DS:[0x9740BC] //0x9740BC <<<<
	float PlayerVelocity;
	vec3_t WeaponPunchAngles[3]; // Null to remove recoil
	float weaponViewAngle_0; //0x0030 //Y //0047CFF3   D91D EC409700    FSTP DWORD PTR DS:[0x9740EC], 0x9740EC <<<<
	float weaponViewAngle_1; //X*/

}refdef_t;

typedef struct
{
	int serverTime;
	char unknown4[12]; //0x0004
	int isIngame;
	char _0x0014[316];
	int clientNum;
	char unknown340[72]; //0x0154
	int health;
	char _0x01A0[28];
	int maxEntities;
	char unknown448[124]; //0x01C0
	BYTE state; //0x023C
	char pad_0x023D[0x15B]; //0x023D
	__int32 ammo2; //0x0398 
	char pad_0x039C[0xC]; //0x039C
	__int32 ammo1; //0x03A8 
	char pad_0x03AC[0x64]; //0x03AC
	__int32 mag2; //0x0410 
	char pad_0x0414[0x14]; //0x0414
	__int32 mag1; //0x0428 
	char pad_0x042C[0xA8]; //0x042C
	__int32 sometime; //0x04D4 	
}cg_t;

typedef struct
{
	char unknown0[28]; //0x0000
	__int32 serverTime; //0x001C  
	char unknown32[4]; //0x0020
	char gameType[4];                                //0x24 (0x9CE144)
	char unknown40[28]; //0x0028
	char hostName[64];                               //0x44 (0x9CE164)
	char _0x0084[0xC4];                              //0x84
	int maxClients;                                  //0x148 (0x9CE268)
	char _0x014C[0x4];                               //0x14C
	char mapName[64];                                //0x150 (0x9CE270)	
}cgs_t;

typedef struct
{
	vec3_t recoil;                                //0x0 (0x1140FEC)
	vec3_t origin;                                //0xC (0x1140FF8)
	char pad_0x0018[0xC]; //0x0018
	float vaY; //0x0024 
	float vaX; //0x0028 
	char pad_0x002C[0x6C]; //0x002C
	float viewAngleY; //0x0098 
	float viewAngleX; //0x009C 

}viewmatrix_t;

typedef struct {
	vec2_t punchAngles;
	float playerVelocity;
	vec3_t weaponPunchAngles[3]; // Null to remove recoil 
	float weaponViewAngle_0; //0x0030 //Y //0047CFF3   D91D EC409700    FSTP DWORD PTR DS:[0x9740EC], 0x9740EC <<<<
	float weaponViewAngle_1; //X	
}Punch_t;

typedef struct
{
	union
	{
		float fval;
		int   ival;
		char* sval;
	};
}type_s;

typedef struct dvar_s
{
	char*    name;    // 0
	int        u1;        // 4
	BYTE    typeId;    // 8
	BYTE    u3;        // 9
	BYTE    u4;        // 10
	BYTE    u5;        // 11

	type_s    u6;        // 12
	type_s    u7;        // 16
	type_s    u8;        // 20
	type_s    u9;        // 24
	type_s    u10;    // 28
	type_s    u11;    // 32
	type_s    u12;    // 36
	type_s    u13;    // 40
	type_s    u14;    // 44
	type_s    u15;    // 48
	type_s    u16;    // 52
	type_s    u17;    // 56

	int        u18;    // 60
	int        u19;    // 64
	int        u20;    // 68
	dvar_s*    next;    // 72
}dvar_t; // 0x4C 

typedef struct
{
	qfont_t bigfont;
	qfont_t smallfont;
	qfont_t consolefont;
	qfont_t boldfont;
	qfont_t normalfont;
	qfont_t extrabigfont;
	qfont_t objetivefont;
	qfont_t hudbigfont;
	qfont_t hudsmallfont;
}Fonts_t;

typedef struct {
	qfont_t smallDevFont;
	qfont_t bigDevFont;
}Fonts_Dev_t;

typedef struct
{
	int blurBlendFadeTime; //0000
	int blurBlendTime; //0004
	int whiteFadeTime; //0008
	int shotFadeTime; //000C
	int screenType; //0010
	int unknown_int3000; //0014
	float viewKickPeriod; //0018
	float viewKickRadius; //001C
	bool shockSound; //0020
	char sfString[31]; //0021
}stunflashdata_t;

typedef struct
{
	stunflashdata_t data[4]; //0000
	char unknown0[36];
	int soundFadeInTime; //0124
	int soundFadeOutTime; //0128
	float soundDryLevel; //012C
	float soundWetLevel; //0130
	char soundRoomType[16]; //0134
	char unknown1[256];
	int soundModEndDelay; //0244
	int soundLoopFadeTime; //0248
	int soundLoopEndDelay; //024C
	BOOL lookControl; //0250
	int lookControlFadeTime; //0254
	float lcMouseSensitivityScale; //0258
	float lcMaxPitchSpeed; //025C
	float lcMaxYawSpeed; //0260
	BOOL shockMovement; //0264
}stunflash_t;

typedef struct
{
	char _pad0[1116];
	float swayMultiplier;    //0x045C 
	float swayMultiplierADS; //0x0460 
}weaponInfo_t;

typedef struct {
	char *szName; //0x0000 
	weaponInfo_t* info; //0x0004 
	char *szLocalizedName; //0x0008 
	char _0x000C[60];
	float flZoomedFOV; //0x0048 
	char _0x004C[8];
	__int32 iMaxClipRounds; //0x0054 
	__int32 iBulletImpactEffectType; //0x0058 
	__int32 iWeaponFireDelay; //0x005C 
	__int32 iWeaponBulletType; //0x0060 
	char _0x0064[4];
	vec3_t vWeaponWeight; //0x0068 
	char _0x0074[16]; //124
	__int32 Icon; //0x0084 
	char _0x0088[424];
	int bulletType;  //0x0230 
}weapon_t; //Size=0x0600

typedef struct
{
	char unk[0xDC];
}ScreenMatrix;

typedef struct
{
	__int32 xp; //0x0000 

	char pad_0x0004[0x4]; //0x0004

						  //========================================================================

	__int32 usp_exp; //0x0000  << 0x8 
	__int32 mp412_exp; //0x0004 + 8 = 0xC
	__int32 magnum44_exp; //0x0008 
	__int32 deserteagle_exp; //0x000C 
	__int32 p99_exp; //0x0010 
	__int32 fiveseven_exp; //0x0014 
	__int32 acr_exp; //0x0018 
	__int32 type95_exp; //0x001C 
	__int32 m4a1_exp; //0x0020 
	__int32 ak47_exp; //0x0024 
	__int32 m16a4_exp; //0x0028 
	__int32 mk14_exp; //0x002C 
	__int32 g36c_exp; //0x0030 
	__int32 scar_exp; //0x0034 
	__int32 fad_exp; //0x0038 
	__int32 cm901_exp; //0x003C 
	__int32 mp5_exp; //0x0040 
	__int32 pm9_exp; //0x0044 
	__int32 p90_exp; //0x0048 
	__int32 pp90m1_exp; //0x004C 
	__int32 ump45_exp; //0x0050 
	__int32 mp7_exp; //0x0054 
	__int32 fmg9_exp; //0x0058 
	__int32 g18_exp; //0x005C 
	__int32 mp9_exp; //0x0060 
	__int32 scorpion_exp; //0x0064 
	char pad_0x0068[0x4]; //0x0068
	__int32 spas12_exp; //0x006C 
	__int32 aa12_exp; //0x0070 
	__int32 striker_exp; //0x0074 
	__int32 model1887_exp; //0x0078 
	__int32 usas12_exp; //0x007C 
	__int32 ksg12_exp; //0x0080 
	__int32 m60e4_exp; //0x0084 
	__int32 mk46_exp; //0x0088 
	__int32 pkppecheneg_exp; //0x008C 
	__int32 l86lsw_exp; //0x0090 
	__int32 mg36_exp; //0x0094 
	__int32 barrett50_exp; //0x0098 
	__int32 msr_exp; //0x009C 
	__int32 rsass_exp; //0x00A0 
	__int32 dragunov_exp; //0x00A4 
	__int32 as50_exp; //0x00A8 
	__int32 l118a_exp; //0x00AC 
	char pad_0x00B0[0x4]; //0x00B0
	__int32 rpg7_exp; //0x00B4 
	char pad_0x00B8[0x18]; //0x00B8
	__int32 javelin_exp; //0x00D0 
	__int32 stinger_exp; //0x00D4 
	__int32 smaw_exp; //0x00D8 
	__int32 m320_exp; //0x00DC 
	char pad_0x00E0[0x18]; //0x00E0
	__int32 riotshield_exp; //0x00F8 
	char pad_0x00FC[0x8]; //0x00FC
	__int32 xm25_exp; //0x0104 <<< + 8 = 0x10C

					  //========================================================================

	char pad_0x0110[0x100]; //0x0110

	__int32 prestige; //0x0210 
	char pad_0x0214[0x4]; //0x0214
	__int32 score; //0x0218 
	__int32 games; //0x021C 
	char pad_0x0220[0x20]; //0x0220
	__int32 kills; //0x0240 
	__int32 killstreak; //0x0244 
	__int32 deaths; //0x0248 
	char pad_0x024C[0x4]; //0x024C
	__int32 assists; //0x0250 
	__int32 headshots; //0x0254 
	char pad_0x0258[0x8]; //0x0258
	__int32 timeunderscore; //0x0260 
	char pad_0x0264[0x8]; //0x0264
	__int32 timeplayedinsecs; //0x026C 
	__int32 kdratio; //0x0270 
	__int32 wins; //0x0274 
	__int32 loses; //0x0278 
	__int32 ties; //0x027C 
	__int32 winstreak; //0x0280 
	char pad_0x0284[0x8]; //0x0284
	__int32 hits; //0x028C 
	__int32 misses; //0x0290 
	char pad_0x0294[0x4]; //0x0294
	__int32 accuracy; //0x0298 
	char pad_0x029C[0x2FC]; //0x029C
	WORD class1weapon1; //0x0598 
	WORD class1weapon1_attach1; //0x059A 
	WORD class1weapon1_attach2; //0x059C 
	WORD class1weapon1_camo; //0x059E 
	WORD class1weapon1_prof; //0x05A0 
	char pad_0x05A2[0x2]; //0x05A2
	WORD class1weapon2; //0x05A4 
	WORD class1weapon2_attach1; //0x05A6 
	WORD class1weapon2_attach2; //0x05A8 
	WORD class1weapon2_camo; //0x05AA 
	WORD class1weapon2_prof; //0x05AC 
	char pad_0x05AE[0x2]; //0x05AE
	WORD leathal; //0x05B0 
	WORD perk1; //0x05B2 
	WORD perk2; //0x05B4 
	WORD perk3; //0x05B6 	

				//char pad_0x05B8[0x1852]; //0x05B8

	char _0x05B8[5194];
	__int32 ExtremeConditioning; //0x1A02 
	__int32 SleightOfHand; //0x1A06 
	__int32 Scavenger; //0x1A0A 
	__int32 BlindEye; //0x1A0E 
	__int32 Recon; //0x1A12 
	__int32 Hardline; //0x1A16 
	__int32 Assassin; //0x1A1A 
	__int32 Quickdraw; //0x1A1E 
	__int32 Overkill; //0x1A22 
	__int32 BlastShield; //0x1A26 
	__int32 Marksman; //0x1A2A 
	__int32 Sitrep; //0x1A2E 
	__int32 SteadyAim; //0x1A32 
	__int32 DeadSilence; //0x1A36 
	__int32 Stalker; //0x1A3A 
	__int32 Parkour; //0x1A3E 
	__int32 HighNoon; //0x1A42 
	__int32 HoardingAmmo; //0x1A46 
	__int32 RestrictedAirspace; //0x1A4A 
	__int32 PaintballGun; //0x1A4E 
	__int32 AndOne; //0x1A52 
	__int32 Surprise; //0x1A56 
	__int32 BringingTheBoom; //0x1A5A 
	__int32 Overkiller; //0x1A5E 
	__int32 CantPhaseMe; //0x1A62 
	__int32 CQCNotForMe; //0x1A66 
	__int32 IHeardThat; //0x1A6A 
	__int32 RunnersHigh; //0x1A6E 
	__int32 Featherweight; //0x1A72 
	__int32 TripDelayed; //0x1A76 
	char _0x1A7A[912];

	WORD emblem; //0x1E0A 
	char pad_0x1E0C[0x2]; //0x1E0C
	WORD title; //0x1E0E 
	char pad_0x1E10[0x25F]; //0x1E10
	BYTE tokens; //0x206F 
	char pad_0x2070[0x7]; //0x2070
	WORD extracustomclassslots; //0x2077 
	char pad_0x2079[0x4C]; //0x2079
	__int32 doublexp; //0x20C5 
	char pad_0x20C9[0x4]; //0x20C9
	__int32 doubleweaponxp; //0x20CD 


}stats_t;//Size=0x029C

typedef struct
{
	char unknown0[20];
	vec3_t vVec1; //0014	
	vec3_t vVec2; //0020	
}laserInfo_t;

typedef struct
{
	__int32 key; //0x0000 
	char pad_0x0004[0xC]; //0x0004
	BYTE wasPressed; //0x0010 
	char pad_0x0011[3];
}kbutton;//Size=0x0014

typedef struct
{
	kbutton _pad0[2];
	kbutton forward; //0x0028 
	kbutton back; //0x003C 
	char pad_0x0050[0x28]; //0x0050
	kbutton moveleft; //0x0078 
	kbutton moveright; //0x008C 
	char pad_0x00A0[0x28]; //0x00A0
	kbutton jump; //0x00C8 
	char pad_0x00DC[0x3C]; //0x00DC
	kbutton attack; //0x0118 
	char pad_0x012C[0x28]; //0x012C
	kbutton grenade; //0x0154 
	char pad_0x0168[0x14]; //0x0168
	kbutton realmelee; //0x017C 
	char pad_0x0190[0x78]; //0x0190
	kbutton sprint; //0x0208 

}KInput_t;                 // 230


//Host
typedef struct
{
	vec3_t			lerpOrigin;
	vec3_t			Velocity;
	char				unk01[0x340];
	int				Overlay; //65 = Nightvision
	char				unk02[0x4];
	float				spreadMultiplier;
	char				unk03[0x18];
	int				SecondaryStock;
	char				unk04[0xC];
	int				PrimaryStock;
	char				unk05[0x64];
	int				SecondaryClip;
	char				unk06[0x8];
	int				Nades;
	char				unk07[0x8];
	int				PrimaryClip;
	char				unk08[0x8];
	int				Flashes;
	char				unk09[0x9C];
	int				ServerTime;
	char				unk10[0x13];
	BYTE				Perks;
	char				unk[0x2ECD];
	float				SpeedMultiplier;
	char				unk11[0x20C];
	int				NoClip;
	char				unk12[0x338];
}entityInfo_s;

typedef struct
{
	int				Health;
	char			unk01[0x270];
}Health_s;

typedef struct {
	__int32 ServerTime; //0x0000  
	char unknown4[8]; //0x0004
	__int32 eflag; //0x000C  
	char unknown16[12]; //0x0010
	float MapStateX; //0x001C  
	float MapStateY; //0x0020  
	float MapStateZ; //0x0024  
	float strafeX; //0x0028  
	float strafeY; //0x002C  
	char unknown48[8]; //0x0030
	__int32 EntitieCount; //0x0038  
	char unknown60[28]; //0x003C
	__int32 gGravity; //0x0058  
	__int32 gSpeed; //0x005C  
	char unknown96[12]; //0x0060
	__int32 EntitieCount2; //0x006C  
	char unknown112[32]; //0x0070
	__int32 fall; //0x0090 0 = fall
	char unknown148[196]; //0x0094
	float ViewAngleX; //0x0158  
	float ViewAngleY; //0x015C  
	float ViewAngleZ; //0x0160  
	__int32 PlayerHeightInt; //0x0164  
	float PlayerHeightFloat; //0x0168  
	char unknown364[40]; //0x016C
	__int32 Damage; //0x0194  
}playerActor_t;

typedef struct {
	char unknown0[8]; //0x0000
	__int32 State; //0x0008 4 = crouch 8 = prone 0 = stand
	char unknown12[12]; //0x000C
	float originX; //0x0018  
	float originY; //0x001C  
	float originZ; //0x0020  
	char unknown36[24]; //0x0024
	float ViewX; //0x003C  
	float ViewY; //0x0040  
	char unknown68[276]; //0x0044
	playerActor_t* playerActor; //0x0158  
	char unknown348[44]; //0x015C
	__int32 CommandCount; //0x0188  
	char unknown396[4]; //0x018C
	__int32 ServerTime; //0x0190  
	char unknown404[8]; //0x0194
	__int32 Health; //0x019C  
	char unknown416[124]; //0x01A0
	__int32 ID039B5CF0; //0x021C  
	char unknown544[84]; //0x0220
}ServerSet_t;


//Externals
extern Punch_t *punch;

extern dvar_s *perk_bulletPenetrationMultiplier;
extern dvar_s *com_maxfps;
extern dvar_s *cg_fov;
extern dvar_s *cg_thirdPerson;
extern dvar_s *cg_gun_x;
extern dvar_s *cg_gun_y;
extern dvar_s *cg_gun_z;

extern stunflash_t* concGrenade;
extern stunflash_t* flashGrenade;

extern centity_t *cg_entities;

extern refdef_t  *refdef;
extern cg_t	*cg;
extern cgs_t *cgs;
extern viewmatrix_t *pViewMatrix;
extern Fonts_t *Fonts;
extern Fonts_Dev_t *fonts_Dev;
extern input_t   *pinput;
extern int **ServerID;
extern stats_t* pStats;

extern ServerSet_t *ServerSet;
extern entityInfo_s *ServerEntityInfo;
extern Health_s *ServerHealth;

extern KInput_t *key_input;

extern qhandle_t HeadShotIcon;
extern qhandle_t DiedIcon;
extern qhandle_t ScavengerIcon;
extern qhandle_t FlashbangIcon;

//========================================================================

extern bool isTekno;





