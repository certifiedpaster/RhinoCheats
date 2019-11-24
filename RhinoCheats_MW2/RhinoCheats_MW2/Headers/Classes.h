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

//Typedefs
typedef int qhandle_t;
typedef void *qfont_t;

typedef struct  {
	int servertime; //0x0000 
	int buttons; //0x0004 
	int viewangles[3]; //0x0008 // below here is some usefull stuffs
	char weapon;
	char forwardmove;
	char rightmove;
	char upmove;
	char lol[16];
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

}trace_t;

typedef struct {
	int	worldEntNum;	         // 00 
	int	ignoreEntIndex;	         // 04 
	float damageMultiplier;	     // 08 
	int	methodOfDeath;	         // 12 

	vec3_t	origStart;		// 16
	vec3_t	start;			// 28
	vec3_t	end;			// 40
	vec3_t	dir;	        // 52    
}BulletFireParams;//size= 68 or 0x44

 

typedef struct
{
	short currentValid; //0x0000  
	short valid; //0x0002  
	char unknown4[20]; //0x0004
	vec3_t lerpOrigin;
	vec3_t lerpAngles;
	char unknown48[60]; //0x0030
	int eFlags; //0x006C  
	char unknown112[12]; //0x0070
	vec3_t lerpOrigin2;
	char unknown136[84]; //0x0088
	int clientnum; //0x00DC  
	int eType; //0x00E0  
	int PlayerPose; //0x00E4  
	char unknown232[12]; //0x00E8
	vec3_t lerpOrigin3;
	char unknown256[108]; //0x0100
	int PickupItemID; //0x016C
	int clientNum2; //0x0170
	char unknown372[52]; //0x0174
	short weaponID; //0x01A8  
	char unknown426[2]; //0x01AA
	short secondWeaponID; //0x01AC  
	char unknown430[46]; //0x01AE
	int IsAlive; //0x01DC  
	char unknown480[32]; //0x01E0
	int clientNum3; //0x0200
}centity_t;

typedef struct
{
	__int32 valid; //0x0000  
	__int32 infoValid2; //0x0004  
	__int32 clientNum; //0x0008  
	char name[16]; //0x000C  
	__int32 team; //0x001C  
	__int32 team2; //0x0020  
	__int32 rank; //0x0024  
	char unknown40[4]; //0x0028
	__int32 perk; //0x002C  
	char unknown48[976]; //0x0030
	float viewangle_y; //0x0400  
	float viewangle_x; //0x0404  
	char unknown1032[124]; //0x0408
	__int32 state; //0x0484  
	char unknown1160[12]; //0x0488
	__int32 attacking; //0x0494  
	char unknown1176[4]; //0x0498
	__int32 zooming; //0x049C  
	char unknown1184[68]; //0x04A0
	__int32 weaponID; //0x04E4  
	char unknown1256[68]; //0x04E8
}clientInfo_t;//Size=0x564

typedef struct
{
	int x;
	int y;
	int width; //0x0008  
	int height; //0x000C  
	float fov_x; //0x0010  
	float fov_y; //0x0014  
	vec3_t vieworg; //0x0018
	vec3_t viewaxis[3]; //0x0024  
	char szUnknown1[16200]; //0x0048  
	vec3_t rdViewAngle; //0x3F90

}refdef_t;

typedef struct
{
	__int32 serverTime; //0x0000 
	char _0x0004[12];
	__int32 isIngame; //0x0010 
	char _0x0014[156];
	__int32 pose; //0x00B0 
	char _0x00B4[80];
	__int32 clientNum; //0x0104 
	char _0x0108[72];
	__int32 health; //0x0150 
	char pad_0x0154[0x38]; //0x0154
	__int32 maxEntities; //0x018C 
	char pad_0x0190[0x68]; //0x0190
	__int32 state; //0x01F8 
	char _0x01FC[224];
	__int32 ammo2; //0x02DC 
	char _0x02E0[12];
	__int32 ammo1; //0x02EC 
	char _0x02F0[100];
	__int32 mag2; //0x0354 
	char _0x0358[8];
	__int32 grenade; //0x0360 
	char _0x0364[8];
	__int32 mag1; //0x036C 
	char _0x0370[8];
	__int32 nades; //0x0378 
}cg_t;

typedef struct
{
	char ID0455B210[8]; //0x0000  
	__int32 screenXScale; //0x0008  
	__int32 screenYScale; //0x000C  
	float screenXBias; //0x0010  
	__int32 serverCommandSequence; //0x0014  
	__int32 processedSnapshotNum; //0x0018  
	DWORD localServer; //0x001C  
	char gameType[4]; //0x0020  
	char ID026700D0[28]; //0x0024  
	char hostName[16]; //0x0040  
	char ID0455C600[244]; //0x0050  
	__int32 maxClients; //0x0144  
	char ID04559250[4]; //0x0148  
	char mapName[64]; //0x014C  
}cgs_t;

typedef struct
{
	vec3_t       recoil;		// 0x0000
	char         _p00[24];		// 0x000C
	float vaY; //0x0024  
	float vaX; //0x0028  
	char unknown44[20]; //0x002C
	float viewAngleY; //0x0040  
	float viewAngleX; //0x0044

}viewmatrix_t;

typedef struct {
	vec2_t punchAngles;
	char unknown8[4]; //0x0008
	vec3_t recoil[2];
	char unknown36[12]; //0x0024
	float weaponViewAngle_0; //0x0030  
	float weaponViewAngle_1; //0x0034 
	vec3_t Axis[4];
}Punch_t;

typedef struct
{
	char *name; //0x0000 
	char pad_0x0004[0x4]; //0x0004
	__int32 dvarFlags; //0x0008 
	__int32 value; //0x000C 
	char pad_0x0010[0x3C]; //0x0010

	bool isBool()
	{
		return (dvarFlags & 0xFF) == 0;
	}
	bool isFloat()
	{
		return (dvarFlags & 0xFF) == 1;
	}
	bool isInt()
	{
		return (dvarFlags & 0xFF) == 5;
	}
	bool isString()
	{
		return (dvarFlags & 0xFF) == 7;
	}
	bool hasBeenModified()
	{
		return ((dvarFlags >> 8) & 0xFF) != 0;
	}
	int getIntValue()
	{
		return *(int*)&value;
	}
	float getFloatValue()
	{
		return *(float*)&value;
	}
	bool getBoolValue()
	{
		if (value & 0xFF)
			return true;
		return false;
	}
	char *getStringValue()
	{
		return (char*)value;
	}
	void setBool(bool val)
	{
		*(bool*)&value = val;
	}
	void setInt(int val)
	{
		*(int*)&value = val;
	}
	void setFloat(float val)
	{
		*(float*)&value = val;
	}
	void setString(char *val)
	{
		int len = strlen(val) + 1;
		char *tmp = (char*)malloc(len);
		memcpy(tmp, val, len);
		value = (int)tmp;
	}
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

/*typedef struct
{for mw3:
	char _pad0[1116];
	float swayMultiplier;    //0x045C 
	float swayMultiplierADS; //0x0460 
}weaponInfo_t;
*/
//mw2, icons:
typedef struct {
	
	char* szName; //0x0000 
	void* Data; // 0x4
	char* szLocalizedName; // 0x8
	char _pad1[0x3C]; // 0xC
	int Icon; // 0x48

}weapon_t; //Size=0x0600

//mw2, nospread, autowall:
typedef struct
{
	char unknown0[64]; //0x0000
	char ModelName[32]; //0x0040  
}CWeaponInfo;//Size=0x0060(96)
typedef struct
{
	char unknown0[4]; //0x0000
	CWeaponInfo* WeaponInfo; //0x0004  
	char unknown8[44]; //0x0008
	DWORD typePenetration; //0x0034  
	char unknown56[488]; //0x0038
	__int32 iShots; //0x0220  
	char unknown548[276]; //0x0224
	float flSpreadVar1; //0x0338  
	float flSpreadVar2; //0x033C  
	float flSpreadVar3; //0x0340  
	float flSpreadVar4; //0x0344  
	float flSpreadVar5; //0x0348  
	float flSpreadVar6; //0x034C  
	char unknown848[596]; //0x0350
	float flDistance; //0x05A4  
	char unknown1448[4152]; //0x05A8
}CWeapon;//Size=0x15E0(5600)

typedef struct
{
	__int32 xp; //0x0000 
	char pad_0x0004[0x4]; //0x0004
	__int32 prestige; //0x0008 
	char pad_0x000C[0x4]; //0x000C
	__int32 score; //0x0010 
	__int32 kills; //0x0014 
	__int32 streaks; //0x0018 
	__int32 deaths; //0x001C 
	char pad_0x0020[0x4]; //0x0020
	__int32 assists; //0x0024 
	__int32 headshots; //0x0028 
	char pad_0x002C[0x1C]; //0x002C
	__int32 wins; //0x0048 
	__int32 loses; //0x004C 
	__int32 ties; //0x0050 
	__int32 killstreak; //0x0054 
	char N00000D23[1499]; //0x0058 
	char weaponChallenges[284]; //0x0633 
	char perkChallenges[16]; //0x074F 
	char miscChallenges[187]; //0x075F

}stats_t;

typedef struct
{
	__int32 key; //0x0000 
	char pad_0x0004[0xC]; //0x0004
	BYTE wasPressed; //0x0010 
	char pad_0x0011[3];
}kbutton;//Size=0x0014

typedef struct
{
	char pad_0x0000[0x28]; //0x0000
	kbutton forward; //0x0028 
	kbutton back; //0x003C 
	char pad_0x0050[0x28]; //0x0050
	kbutton moveleft; //0x0078 
	kbutton moveright; //0x008C 
	char pad_0x00A0[0x78]; //0x00A0
	kbutton attack; //0x0118 
	kbutton shift_1; //0x012C 
	char pad_0x0140[0x10]; //0x0140
	kbutton grenade; //0x0150 
	kbutton specialGrenade; //0x0164 
	char pad_0x0178[0x30]; //0x0178
	kbutton melee_; //0x01A8 
	char pad_0x01BC[0x38]; //0x01BC
	kbutton shift_2; //0x01F4 
	char pad_0x0208[0x18]; //0x0208
	kbutton ads; //0x0220 
	char pad_0x0234[0x2FC]; //0x0234

}KInput_t;                 // 230


//Host
/*typedef struct
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
*/

//Externals
extern Punch_t *punch;

extern dvar_t *perk_bulletPenetrationMultiplier;
extern dvar_t *com_maxfps;
extern dvar_t *cg_fov;
extern dvar_t *cg_thirdPerson;
extern dvar_t *cg_gun_x;
extern dvar_t *cg_gun_y;
extern dvar_t *cg_gun_z;

extern stunflash_t* concGrenade;
extern stunflash_t* flashGrenade;

extern centity_t *cg_entities;
extern clientInfo_t *ci;
extern refdef_t  *refdef;
extern cg_t	*cg;
extern cgs_t *cgs;
extern viewmatrix_t *pViewMatrix;
extern Fonts_t *Fonts;
extern Fonts_Dev_t *fonts_Dev;
extern input_t   *pinput;
extern int **ServerID;
extern stats_t* pStats;

/*extern ServerSet_t *ServerSet;
extern entityInfo_s *ServerEntityInfo;
extern Health_s *ServerHealth;
*/

extern KInput_t *key_input;

extern qhandle_t HeadShotIcon;
extern qhandle_t DiedIcon;
extern qhandle_t ScavengerIcon;
extern qhandle_t FlashbangIcon;





