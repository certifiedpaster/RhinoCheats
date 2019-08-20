#pragma once

//Defines
#define	CMD_BACKUP 0x7F
#define	SNAPFLAG_RATE_DELAYED	1
#define	SNAPFLAG_NOT_ACTIVE		2	// snapshot used during connection and for zombies
#define SNAPFLAG_SERVERCOUNT	4	// toggled every map_restart so transitions can be detected

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
	ET_MG42,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_ACTOR,
	ET_ACTOR_SPAWNER,
	ET_ACTOR_CORPSE,
	ET_STREAMER_HINT

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
	int servertime; // 0x0
	int buttons; // 0x4
	char Unknown001[0x4]; // 0x8
	int viewangles[3]; // 0xC
	char Unknown002[0x1C]; // 0x18
}usercmd_t;

typedef struct
{
	usercmd_t usercmds[128];
	int currentCmdNum; 

	usercmd_t *GetUserCmd(int cmdNum)
	{
		if ((cmdNum & CMD_BACKUP) > 127)
			return nullptr;

		return &usercmds[cmdNum & CMD_BACKUP];
	}
}input_t;

typedef struct 
{
	vec3_t            HitPos;
	int               UNKNOW1;
	float	          fraction;
	float             ViewX;
	float             ViewY;
	bool              Solid;
	int               entityNum;
	float             UNKNOW2;
	char              z_crap2[32];
	vec3_t            HitPos2;

}trace_t;

typedef struct
{
	BYTE Padding0[0x4]; //0x0000	
	BYTE RequiredforTags; //0x0004 
	char pad_0x0005[0x3]; //0x0005
	DWORD SpectatorType; //0x0008 
	BYTE Padding2[0x24]; //0x000C		
	vec3_t lerpOrigin;			// 0x30
	vec3_t lerpAngles;			// 0x3C
	char	pad3[320];		// 0x48
	vec3_t NewOrigin;		// 0x188
	char	pad4[24];		// 0x194
	vec3_t NewAngles;		// 0x1AC
	char	pad5[48];		// 0x1B8
	int		ClientNum;		// 0x1E8
	int		Pose;			// 0x1EC
	char	pad6[16];		// 0x1F0
	vec3_t OldOrigin;		// 0x200
	char	pad7[24];		// 0x20C
	vec3_t OldAngles;		// 0x224
	char	pad8[20];		// 0x230
	int		Flags;			// 0x244
	
	char pad_0x0248[0x38]; //0x0248
	__int16 PickupId; //0x0280 
	char pad_0x0282[0x24]; //0x0282
	__int16 Type; //0x02A6 
	char pad_0x02A8[0xA]; //0x02A8
	__int16 WeaponID; //0x02B2 	

	char	pad11[112];		// 0x2B4
	BYTE IsAlive; // 0x324
	char pad_0x0325[0x3]; //0x0325*/
}centity_t;

typedef struct
{
	BYTE Padding0[0x10]; //0x0000 
	DWORD valid; //0x0010 
	DWORD Padding1; //0x0014 Seems to always be 1
	DWORD clientNum; //0x0018 
	char name[32]; //0x001C 
	DWORD team; //0x003C 
	DWORD teamBackup; //0x0040 
	BYTE Padding2[0x2C]; //0x0044 
	DWORD currentFPS; //0x0070 
	DWORD alive; //0x0074 ||Dead: 0||Alive: 1||
	BYTE Padding3[0x10]; //0x0078 
	DWORD deathCount; //0x0088 
	BYTE Padding4[0x8]; //0x008C 
	DWORD deathCountBackup; //0x0094 
	BYTE Padding5[0x18]; //0x0098 
	char modelOne[32]; //0x00B0 
	BYTE Padding6[0x20]; //0x00D0 
	char modelTwo[32]; //0x00F0 
	BYTE Padding7[0x20]; //0x0110 
	char modelThree[32]; //0x0130 
	BYTE Padding8[0x160]; //0x0150 
	char modelFour[32]; //0x02B0 
	BYTE Padding9[0x188]; //0x02D0 
	float prediction; //0x0458 
	BYTE Padding10[0x4]; //0x045C 
	float viewAngleX; //0x0460 
	float viewAngleY; //0x0464 
	BYTE Padding11[0x78]; //0x0468 
	DWORD moveState; //0x04E0 
	BYTE Padding12[0x30]; //0x04E4 
	DWORD crouchState; //0x0514 ||Standing: 0||Crouching: 1||Proning: 2||
	DWORD weaponUnknown; //0x0518 Something to do with weapon (switches 1/0)
	DWORD shooting; //0x051C ||Shooting: 1||Not Shooting: 0||
	DWORD zooming; //0x0520 ||Zooming: 1||No Zooming: 0||
	BYTE Padding13[0xAC]; //0x0524 

}clientInfo_t;

typedef struct
{
	int x;
	int y;
	int		width;
	int		height;
	vec2_t fov;
	float	totalFov;
	char	pad1[4];
	vec3_t vieworg;
	char	pad2[8];
	vec3_t viewaxis[3];

}refdef_t;

typedef struct
{
	vec2_t angles;
}rdViewAngles_t;

typedef struct
{
	__int32             snapFlags;        //0x0000  
	__int32             Ping;            //0x0004  
	__int32             ServerTime;        //0x0008  
	char            z_crap1[16];        //0x000C
	__int32             isReloading;    //0x001C 80 When Reloading 
	char            z_crap2[20];        //0x0020
	vec3_t              viewOrg;        //0x0034 
	float               DeltaX;            //0x0040  
	float               DeltaY;            //0x0044  
	char            z_crap3[168];        //0x0048
	__int32             Stance;            //0x00F0 0:Standing; 4:Crouching; 8:Prone; 
	char            z_crap4[264];        //0x00F4
}snapshot_t;

typedef struct
{	
	__int32 clientNum; //0x0000 
	char pad_0x0004[0x20]; //0x0004
	__int32 serverTime; //0x0024 
	snapshot_t* snap; //0x0028 
	snapshot_t* nextsnap; //0x002C 
	char pad_0x0030[0x54]; //0x0030
	__int32 ping; //0x0084 
	char pad_0x0088[0x14C]; //0x0088
	__int32 weaponid; //0x01D4 
	char pad_0x01D8[0x10]; //0x01D8
	__int32 state; //0x01E8 
	char pad_0x01EC[0xC]; //0x01EC
	float isZoomed; //0x01F8 
	char pad_0x01FC[0x58]; //0x01FC
	__int32 health; //0x0254 

}cg_t;

typedef struct
{
	qfont_t bigfont;
	qfont_t smallfont;
	qfont_t consolefont;
	qfont_t boldfont;
	qfont_t normalfont;
	qfont_t extrabigfont;
}Fonts_t;

typedef struct {
	qfont_t smallDevFont;
	qfont_t bigDevFont;
}Fonts_Dev_t;

typedef struct
{
	char* Name;						// 0x0
}Material_t;

typedef struct
{
	char	pad1[9];				// 0x0
	int		SurfacePenetrateType;	// 0x9
	char	pad2[787];				// 0xD
	Material_t* Material;			// 0x320
	char	pad3[553];				// 0x324
	bool	LargePenetrate;			// 0x54D
	char	pad4[378];				// 0x54E
	float	spreadmultiplier;		// 0x6C8
}WeaponInfo_t;

//Nospread
typedef struct {
	char*	CoreName;				// 0x0
	DWORD	VariantCount;			// 0x4
	WeaponInfo_t* WeaponInfo;		// 0x8
	char*	UIName;					// 0xC
}WeaponDef_t; //Size=0x0600

//Esp
typedef struct 
{
	char        *aimAssistRangeScale;
	char        z_crap[40];
	float       Unknow;
	char       z_crap2[752];
	qhandle_t WeaponShader;
}weapon_t;

typedef struct
{
	__int32 EntityIndex; //0000
	__int32 Snapshot;//0004
	vec3_t vOrigin; //0008	
	__int32 Team; //0014
	__int32 Owner; //0018 	
}dogs_t;

//========================================================================

//Externals
extern centity_t *cg_entities;
extern clientInfo_t *ci;
extern cg_t	*cg;

extern refdef_t  *refdef;
extern rdViewAngles_t* rdViewAngles;

extern Fonts_t *Fonts;
extern Fonts_Dev_t *Fonts_Dev;







