//=====================================================================================

#pragma once

#include "Thunk.hpp"
#include "VEHHook.hpp"

//=====================================================================================

#define MAX_CLIENTS 18
#define MAX_ENTITIES 2048
#define MASK_KILLSTREAK 0x2803001
#define MASK_NONKILLSTREAK 0x280E831
#define EF_CROUCH 0x4
#define EF_PRONE 0x8
#define EF_MANTLE 0x20000
#define EF_ZOOM 0x80000
#define TF_NONE 0x0
#define TF_SHADOW 0x3
#define BUTTON_FIRELEFT 0x1
#define BUTTON_FIRERIGHT 0x80000
#define BUTTON_CROUCH 0x200
#define BUTTON_ZOOM 0x800
#define BIND_ATTACK1 0x1
#define BIND_ATTACK0 0x2
#define BIND_ATTACK_AKIMBO_ACCESSIBLE1 0x4B
#define BIND_ATTACK_AKIMBO_ACCESSIBLE0 0x4C
#define WEAPON_DISABLE 0xC3
#define WEAPON_ENABLE 0x48
#define M_METERS 55.0f
#define M_PI_DOUBLE ((float)M_PI*2.0f)

#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) ((float)((a)*(360.0f/65536)))
#define AngleNormalize(a) (ShortToAngle(AngleToShort((a))))
#define DotProduct(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define AngleCompare180(a) (((a)<90.0f&&(a)>-90.0f)||((a)>270.0f||(a)<-270.0f))
#define VectorCopy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorSubtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define	VectorScale(a,b,c) ((c)[0]=(a)[0]*(b),(c)[1]=(a)[1]*(b),(c)[2]=(a)[2]*(b))
#define	VectorMA(a,b,c,d) ((d)[0]=(a)[0]+(c)[0]*(b),(d)[1]=(a)[1]+(c)[1]*(b),(d)[2]=(a)[2]+(c)[2]*(b))
#define VectorAverage(a,b,c) ((c)[0]=((a)[0]+(b)[0])/2.0f,(c)[1]=((a)[1]+(b)[1])/2.0f,(c)[2]=((a)[2]+(b)[2])/2.0f)
#define GetDistance2D(a,b) (sqrtf(((a)[0]-(b)[0])*((a)[0]-(b)[0])+((a)[1]-(b)[1])*((a)[1]-(b)[1])))
#define GetDistance3D(a,b) (sqrtf(((a)[0]-(b)[0])*((a)[0]-(b)[0])+((a)[1]-(b)[1])*((a)[1]-(b)[1])+((a)[2]-(b)[2])*((a)[2]-(b)[2])))
#define GetSign(a) ((a)?((*(int*)&(a)>>31)|0x1):0)

#define OFF_CHATHEIGHT_DVAR 0x1419A9788
#define OFF_CHATHEIGHT_EXCEPTION 0x14025C709
#define OFF_SMOOTHINGTIME_DVAR 0x141762418
#define OFF_SMOOTHINGTIME_EXCEPTION 0x140283257
#define OFF_HUDSAYPOSITION_DVAR 0x1419A94C8
#define OFF_HUDSAYPOSITION_EXCEPTION 0x14025C40B
#define OFF_PACKETDUPLICATION_DVAR 0x141E04AA8
#define OFF_PACKETDUPLICATION_EXCEPTION 0x1402C1FB2
#define OFF_WINDOWHANDLE 0x147AD2640
#define OFF_SWAPCHAIN 0x1480B1D70
#define OFF_REFRESH 0x14025CC50
#define OFF_CREATENEWCOMMANDS 0x1402BEE80
#define OFF_WRITEPACKET 0x1402C1E70
#define OFF_BULLETFIREPENETRATE 0x1402ACBF0
#define OFF_OBITUARY 0x14026A010
#define OFF_ADDCMDDRAWTEXT 0x140601070
#define OFF_CLIENTFRAME 0x140382890
#define OFF_LOCALCLIENTISINGAME 0x1402CFE50
#define OFF_ISMAINTHREAD 0x140423950
#define OFF_ISRENDERTHREAD 0x1404239A0
#define OFF_ISSERVERTHREAD 0x1404239E0
#define OFF_ISDATABASETHREAD 0x140424990
#define OFF_ENABLECONSOLE 0x140503130
#define OFF_PRINTTOCONSOLE 0x140502A80
#define OFF_CBUFADDCALL 0x1403F6AD0
#define OFF_CBUFADDTEXT 0x1403F6B50
#define OFF_GAMESENDSERVERCOMMAND 0x1404758C0
#define OFF_FINDVARIABLE 0x1404ECB60
#define OFF_GETVARIABLEINDEX 0x1403E9BC0
#define OFF_SAY 0x140393010
#define OFF_SAYTO 0x1403931A0
#define OFF_GETPLAYERADDR 0x14048D040
#define OFF_GETCURRENTNAME 0x1404FDAA0
#define OFF_GETCURRENTXUID 0x1404FDAB0
#define OFF_GETSCREENMATRIX 0x1402F6D00
#define OFF_GETCURRENTWEAPON 0x140239200
#define OFF_GETWEAPONNAME 0x1402A9B80
#define OFF_WEAPONISAKIMBO 0x140247B70
#define OFF_WORLDTOSCREEN 0x140262190
#define OFF_ADDMESSAGEICON 0x1402BAAE0
#define OFF_REGISTERFONT 0x1402C9AA0
#define OFF_REGISTERSHADER 0x1405F0E20
#define OFF_REGISTERTAG 0x1403C38F0
#define OFF_DRAWSTRETCHPIC 0x140600BE0
#define OFF_DRAWROTATEDPIC 0x140261270
#define OFF_DRAWENGINETEXT 0x140601070
#define OFF_STRINGHEIGHT 0x1405DFAE0
#define OFF_STRINGWIDTH 0x1405DFDB0
#define OFF_GETENTITYDOBJ 0x140416490
#define OFF_GETTAGPOSITION 0x140263F50
#define OFF_TRACE 0x1402B6020
#define OFF_VECTORANGLES 0x1404E36A0
#define OFF_ANGLEVECTORS 0x1404E39E0
#define OFF_VECTORNORMALIZE 0x140147FE0
#define OFF_EXECUTEKEY 0x1402BF0E0
#define OFF_SETZOOMSTATE 0x1402B9D70
#define OFF_BULLETTRACE 0x1402A6EB0
#define OFF_ADVANCETRACE 0x14023B010
#define OFF_GETSURFACEPENETRATIONDEPTH 0x140238FD0
#define OFF_GETHIPFIRESPREADFORWEAPON 0x1402409B0
#define OFF_GETZOOMSPREADFORWEAPON 0x14023A5A0
#define OFF_BULLETENDPOSITION 0x1402AC8B0
#define OFF_SEEDRANDOM 0x14021E420
#define OFF_RANDOM 0x14021E3F0
#define OFF_EVALUATETRAJECTORY 0x140218240
#define OFF_TEAMCHANGED 0x1403880D0
#define OFF_TAKEWEAPON 0x1402463F0
#define OFF_GIVEWEAPON 0x1403DA5E0
#define OFF_ADDAMMO 0x140399E50
#define OFF_KICKCLIENT 0x14046F730
#define OFF_PLAYERDIE 0x140396920
#define OFF_PLAYERKILL 0x1403CE260
#define OFF_SPREADMULTIPLIER 0x14187D43C
#define OFF_ZOOMMULTIPLIER 0x140B31D4C
#define OFF_WEAPONRECOIL 0x1402472A0
#define OFF_WEAPONSOUNDS 0x1402A8FA0
#define OFF_WEAPONRUMBLE 0x1402A90A0
#define OFF_FRAMEINTERPOLATION 0x1417E0488
#define OFF_FRAMETIME 0x1417E048C
#define OFF_NEWSERVERTIME 0x1417E0490
#define OFF_OLDSERVERTIME 0x1417E0494
#define OFF_PING 0x1419E5100
#define OFF_CG 0x14176EC00
#define OFF_REFDEF 0x1417E04D0
#define OFF_CHARACTERINFO 0x141869AB8
#define OFF_CENTITY 0x141887350
#define OFF_GENTITY 0x14427A0E0
#define OFF_PLAYERSTATE 0x14444CF10
#define OFF_CLIENTACTIVE 0x1419E5298
#define OFF_CLIENTINFO 0x141879028
#define OFF_WEAPONCOMPLETEDEF 0x141734DE0
#define OFF_WEAPONDEF 0x1417352E0
#define OFF_WINDOW 0x14780C394
#define OFF_VIEWMATRIX 0x1419E51E4
#define OFF_PUNCH 0x141823E5C
#define OFF_CHALLENGES 0x1445A2B40
#define OFF_SQUADPOINTS 0x1445A34A0
#define OFF_PRESTIGE 0x1445A3798
#define OFF_CURRENTNAME 0x147AC9DD4
#define OFF_SQUADMEMBERNAME 0x14459F835
#define OFF_SQUADMEMBEREXP 0x14459F857
#define OFF_SQUADMEMBERSIZE 0x564
#define OFF_STEAMOVERLAY 0x1613D0
#define OFF_STEAMAPI 0x147D4B048
#define OFF_STEAMNAME 0x1D2
#define OFF_ISCURRENTHOST 0x141734DD4
#define OFF_SERVERID 0x141D9BA3C
#define OFF_SERVERSESSION 0x1474F0060
#define OFF_MOUSEINPUT 0x147AC9D85

//=====================================================================================

namespace RhinoCheats
{
	typedef float Vector;
	typedef Vector Vector2[2];
	typedef Vector Vector3[3];
	typedef Vector Vector4[4];
	typedef Vector3 RGB;
	typedef Vector4 RGBA;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		SV_CMD_CAN_IGNORE,
		SV_CMD_RELIABLE,
		SV_CMD_MAX
	} eSvsCMDType;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		WEAPON_ICON_RATIO_1TO1,
		WEAPON_ICON_RATIO_2TO1,
		WEAPON_ICON_RATIO_4TO1,
		WEAPON_ICON_RATIO_MAX
	} eWeaponIconRatio;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		TEAM_FREE,
		TEAM_AXIS,
		TEAM_ALLIES,
		TEAM_SPECTATOR,
		TEAM_MAX
	} eTeam;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		TRACE_HITTYPE_NONE,
		TRACE_HITTYPE_ENTITY,
		TRACE_HITTYPE_DYNENT_MODEL,
		TRACE_HITTYPE_DYNENT_BRUSH,
		TRACE_HITTYPE_DYNENT_GLASS,
		TRACE_HITTYPE_MAX
	} eTraceHitType;
	/*
	//=====================================================================================
	*/
	typedef enum
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
		ET_AGENT,
		ET_AGENT_CORPSE,
		ET_EVENTS,
		ET_MAX
	} eEntityType;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		PERK_ACCURACY,
		PERK_FASTRELOAD,
		PERK_RATEOFFIRE,
		PERK_EXTRABREATH,
		PERK_LONGERSPRINT,
		PERK_DETECTEXPLOSIVE,
		PERK_EXPOSEENEMY,
		PERK_EXTRAAMMO,
		PERK_TWOPRIMARIES,
		PERK_ARMORVEST,
		PERK_FRAGGRENADE,
		PERK_SPECIALGRENADE,
		PERK_EXTENDEDMAGS,
		PERK_LIGHTWEIGHT,
		PERK_MARATHON,
		PERK_QUICKDRAW,
		PERK_IMPROVEDEXTRABREATH,
		PERK_FASTSPRINTRECOVERY,
		PERK_EXTENDEDMELEE,
		PERK_FASTOFFHAND,
		PERK_STALKER,
		PERK_HOLDBREATHWHILEADS,
		PERK_LONGERRANGE,
		PERK_FASTERMELEE,
		PERK_REDUCEDSWAY,
		PERK_JAMRADAR,
		PERK_EXTRABP,
		PERK_GRENADEDEATH,
		PERK_PISTOLDEATH,
		PERK_QUIETMOVE,
		PERK_PARABOLIC,
		PERK_BULLETDMG,
		PERK_SPRINTRELOAD,
		PERK_NOT_USED,
		PERK_EXPLOSIVEBULLETS,
		PERK_SCAVENGER,
		PERK_COLDBLOODED,
		PERK_BLINDEYE,
		PERK_NOPLAYERTARGET,
		PERK_HEARTBREAKER,
		PERK_SELECTIVEHEARING,
		PERK_FASTSNIPE,
		PERK_SPYGAME,
		PERK_AUTOMANTLE,
		PERK_QUICKSWAP,
		PERK_LOWPROFILE,
		PERK_HEARTBREAKER_PRO,
		PERK_THROWBACK,
		PERK_RADAR_ISREDBLIP,
		PERK_RADAR_ISREDARROW,
		PERK_RADAR_ISJUGGERNAUT,
		PERK_SILENTKILL,
		PERK_NOSCOPEOUTLINE,
		PREK_MAX
	} ePerk;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		BONE_HEAD,
		BONE_NECK,
		BONE_UPPER_SPINE,
		BONE_LOWER_SPINE,
		BONE_LEFT_SHOULDER,
		BONE_RIGHT_SHOULDER,
		BONE_LEFT_HIP,
		BONE_RIGHT_HIP,
		BONE_LEFT_ELBOW,
		BONE_RIGHT_ELBOW,
		BONE_LEFT_KNEE,
		BONE_RIGHT_KNEE,
		BONE_LEFT_WRIST,
		BONE_RIGHT_WRIST,
		BONE_LEFT_ANKLE,
		BONE_RIGHT_ANKLE,
		BONE_MAX
	} eBone;
	/*
	//=====================================================================================
	*/
	typedef enum
	{
		WEAPON_C4 = 15,
		WEAPON_AK_12 = 38,
		WEAPON_AMELI,
		WEAPON_ARX_160,
		WEAPON_SA_805,
		WEAPON_CBJ_MS,
		WEAPON_FAD,
		WEAPON_FP6,
		WEAPON_MR_28,
		WEAPON_LYNX,
		WEAPON_HONEY_BADGER = 48,
		WEAPON_IA_2,
		WEAPON_K7,
		WEAPON_CHAIN_SAW,
		WEAPON_COMBAT_KNIFE,
		WEAPON_VECTOR_CRB = 54,
		WEAPON_L115,
		WEAPON_LSAT,
		WEAPON_M27_IAR,
		WEAPON_M9A1,
		WEAPON_44_MAGNUM = 62,
		WEAPON_BULLDOG = 65,
		WEAPON_MTAR_X,
		WEAPON_MINIGUN,
		WEAPON_MK14_EBR = 69,
		WEAPON_MK32,
		WEAPON_MP_443_GRACH = 72,
		WEAPON_MSBS,
		WEAPON_MTS_255,
		WEAPON_P226,
		WEAPON_PANZERFAUST = 77,
		WEAPON_PDW,
		WEAPON_BIZON,
		WEAPON_REMINGTON_R5,
		WEAPON_KASTET,
		WEAPON_RIOT_SHIELD,
		WEAPON_SC_2010 = 84,
		WEAPON_SVU,
		WEAPON_USR,
		WEAPON_TAC_12,
		WEAPON_VEPR,
		WEAPON_VKS,
		WEAPON_HELO_PILOT = 95,
		WEAPON_LOKI = 102,
		WEAPON_ODIN = 103,
		WEAPON_GRYPHON = 111,
		WEAPON_IED = 122,
		WEAPON_MAVERICK = 136,
		WEAPON_RIPPER,
		WEAPON_MAVERICK_A2,
		WEAPON_GOLD_KNIFE,
		WEAPON_GOLD_PDW,
		WEAPON_MAX = 46
	} eWeapon;
	/*
	//=====================================================================================
	*/
	static std::pair<std::vector<LPCSTR>, std::vector<LPCSTR>> vBones = std::make_pair(
		std::vector<LPCSTR>({
		"Head", "Neck", "Upper Spine", "Lower Spine",
		"Left Shoulder", "Right Shoulder", "Left Hip", "Right Hip",
		"Left Elbow", "Right Elbow", "Left Knee", "Right Knee",
		"Left Wrist", "Right Wrist", "Left Ankle", "Right Ankle" }),
		std::vector<LPCSTR>({
		"j_head", "j_neck", "j_spineupper", "j_spinelower",
		"j_shoulder_le", "j_shoulder_ri", "j_hip_le", "j_hip_ri",
		"j_elbow_le", "j_elbow_ri", "j_knee_le", "j_knee_ri",
		"j_wrist_le", "j_wrist_ri", "j_ankle_le", "j_ankle_ri" }));
	/*
	//=====================================================================================
	*/
	static std::pair<std::vector<LPCSTR>, std::vector<eWeapon>> vWeapons = std::make_pair(
		std::vector<LPCSTR>({
		"AK-12", "Ameli", "ARX-160", "SA-805", "CBJ-MS",
		"FAD", "FP6", "MR-28", "Lynx", "Honey Badger",
		"IA-2", "K7", "Chain SAW", "Combat Knife", "Vector CRB",
		"L115", "LSAT", "M27-IAR", "M9A1", ".44 Magnum",
		"Bulldog", "MTAR-X", "Minigun", "MK14 EBR", "MK32",
		"MP-443 Grach", "MSBS", "MTS-255", "P226", "Panzerfaust",
		"PDW", "Bizon", "Remington R5", "Kastet", "Riot Shield",
		"SC-2010", "SVU", "USR", "Tac 12", "Vepr",
		"VKS", "Maverick", "Ripper", "Maverick-A2",
		"Gold Knife", "Gold PDW" }), 
		std::vector<eWeapon>({
		WEAPON_AK_12, WEAPON_AMELI, WEAPON_ARX_160, WEAPON_SA_805, WEAPON_CBJ_MS,
		WEAPON_FAD, WEAPON_FP6, WEAPON_MR_28, WEAPON_LYNX, WEAPON_HONEY_BADGER,
		WEAPON_IA_2, WEAPON_K7, WEAPON_CHAIN_SAW, WEAPON_COMBAT_KNIFE, WEAPON_VECTOR_CRB,
		WEAPON_L115, WEAPON_LSAT, WEAPON_M27_IAR, WEAPON_M9A1, WEAPON_44_MAGNUM,
		WEAPON_BULLDOG, WEAPON_MTAR_X, WEAPON_MINIGUN, WEAPON_MK14_EBR, WEAPON_MK32,
		WEAPON_MP_443_GRACH, WEAPON_MSBS, WEAPON_MTS_255, WEAPON_P226, WEAPON_PANZERFAUST,
		WEAPON_PDW, WEAPON_BIZON, WEAPON_REMINGTON_R5, WEAPON_KASTET, WEAPON_RIOT_SHIELD,
		WEAPON_SC_2010, WEAPON_SVU, WEAPON_USR, WEAPON_TAC_12, WEAPON_VEPR,
		WEAPON_VKS, WEAPON_MAVERICK, WEAPON_RIPPER, WEAPON_MAVERICK_A2,
		WEAPON_GOLD_KNIFE, WEAPON_GOLD_PDW }));
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		LPSTR szName;
		char _0x8[0x8];

		union uDvarValue
		{
			bool bValue;
			int iValue;
			DWORD dwValue;
			float flValue;
			LPSTR szValue;
			ImVec4 cValue;
		} Current, Latched, Reset;

		char _0x40[0x20];
	} sDvar;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		char _0x0[0x3C78];
		int iPing;
		int iServerTime;
	} sSnapShot;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iServerTime;
		char _0x4[0xC];
		int iIsInGame;
		char _0x14[0x8];
		Vector3 vOrigin;
		char _0x28[0x3338];
		int iClientNum;
		char _0x3364[0x24];
		sSnapShot* OldSnapShot;
		sSnapShot* NewSnapShot;
		char _0x3398[0xA66F8];
		Vector3 vRefDefViewAngles;
		Vector3 vWeaponAngles;
		char _0xA9AA8[0x24];
		Vector3 vThirdPersonViewAngles;
	} sCG;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iWidth;
		int iHeight;
		float flFovX;
		float flFovY;
		Vector3 vViewOrg;
		Vector3 vViewAxis[3];
	} sRefDef;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iEntityNum;
		int iInfoValid;
		int iNextValid;
		eTeam iTeam;
		eTeam iOldTeam;
		int iPerks[2];
		char _0x1C[0x444];
		Vector3 vAngles;
		char _0x46C[0x128];
		int iDualWielding;
		int iIsFemale;
		char _0x59C[0x3C];
	} sCharacterInfo;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iType;
		int iTime;
		int iDuration;
		Vector3 vBase;
		Vector3 vDelta;
	} sTrajectory;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iEntityFlags;
		sTrajectory PositionTrajectory;
		sTrajectory AngleTrajectory;
		char _0x4C[0x10];
		int iPrimaryWeapon;
		int iSecondaryWeapon;
		char _0x64[0x4];
	} sLerpEntityState;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iEventType;
		int iEventParam;
	} sEntityEvent;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iEntityNum;
		int iEntityType;
		sLerpEntityState LerpEntityState;
		char _0x70[0x8];
		int iOtherEntityNum;
		int iAttackerEntityNum;
		int iGroundEntityNum;
		char _0x84[0xC];
		int iClientNum;
		char _0x94[0x4];
		int iSolid;
		int iEventParam;
		int iEventSequence;
		sEntityEvent EntityEvent[4];
		int iWeapon;
		int iInAltWeaponMode;
		char _0xCC[0x8];
		int iEventParam2;
		char _0xD8[0x30];
	} sEntityState;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iClientIndex;
		eTeam iTeam;
		char _0x8[0x4];
		int iDualWielding;
		char _0x10[0x30];
		char szName[32];
		int iRank;
		int iPrestige;
		char _0x68[0x8];
		int iPerks[2];
		char _0x78[0x78];
	} sClientState;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iCommandTime;
		char _0x4[0xC];
		int iOtherFlags;
		int iLinkFlags;
		char _0x18[0x4];
		Vector3 vOrigin;
		Vector3 vVelocity;
		char _0x34[0x28];
		int iGravity;
		int iSpeed;
		char _0x64[0xC0];
		int iEntityFlags;
		char _0x128[0x54];
		int iClientNum;
		char _0x180[0x4];
		Vector3 vAngles;
		char _0x190[0x38];
		int iStats[4];
		char _0x1D8[0x360];
		int iLethalAmmo1;
		int iLethalAmmo2;
		char _0x540[0x8];
		int iTacticalAmmo1;
		int iTacticalAmmo2;
		char _0x550[0x8];
		int iPrimaryAmmo1;
		int iPrimaryAmmo2;
		char _0x560[0x8];
		int iSecondaryAmmo1;
		int iSecondaryAmmo2;
		char _0x570[0x2E84];
		sClientState ClientState;
		char _0x34E4[0x584];
	} sPlayerState;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		char _0x0[0x2];
		short wValid;
		char _0x4[0x14];
		Vector3 vOrigin;
		Vector3 vAngles;
		char _0x30[0x60];
		sLerpEntityState LerpEntityState;
		sEntityState EntityState;
		int iIsAlive;
		char _0x204[0x34];
	} sCEntity;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		sEntityState EntityState;
		char _0x108[0x60];
		sPlayerState* PlayerState;
		char _0x170[0x50];
		int iFlags;
		char _0x1C4[0x18];
		int iHealth;
		int iMaxHealth;
		char _0x1E4[0x10C];
	} sGEntity;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iServerTime;
		int iButtons;
		int iViewAngles[3];
		char _0x14[0x8];
		char szForwardMove;
		char szRightMove;
		char _0x1E[0x1E];
	} sUserCMD;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		sUserCMD UserCMD[128];
		int iCurrentCMD;

		sUserCMD *GetUserCMD(int number)
		{
			return &UserCMD[number & 0x7F];
		}
	} sClientActive;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iClientNum;
		char szName[32];
		int iRankMP;
		int iPrestigeMP;
		int iRankEXT;
		int iPrestigeEXT;
		char szClan[8];
		char _0x3C[4];
		int iHealth;
		char _0x44[0x2C];
	} sClientInfo;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		float flFraction;
		Vector3 vNormal;
		char _0x10[0x8];
		eTraceHitType TraceHitType;
		short wHitID;
		char _0x1E[0x6];
		short wPartGroup;
		bool bAllSolid;
		bool bStartSolid;
		char _0x28[0x4];
	} sTrace;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iMaxEntNum;
		int iEntityNum;
		float flPower;
		int iBulletType;
		char _0x10[0x4];
		Vector3 vViewOrigin;
		Vector3 vStart;
		Vector3 vEnd;
		Vector3 vDir;
	} sBulletFireParams;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		sTrace Trace;
		char _0x2C[0x4];
		sGEntity* pHitEnt;
		Vector3 vHitPos;
		int iIgnoreHitEnt;
		int iDepthSurfaceType;
		int iHitClientNum;
	} sBulletTraceResults;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iType;
		int iIP;
		int iPort;
		int iLocalNetID;
	} sNetAdr;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		char _0x0[0x370];
		LPVOID pHUDIcon;
		eWeaponIconRatio iHUDIconRatio;
		char _0x37C[0x4];
		LPVOID pPickupIcon;
		eWeaponIconRatio iPickupIconRatio;
		char _0x38C[0x4];
		LPVOID pAmmoCounterIcon;
		eWeaponIconRatio iAmmoCounterIconRatio;
		char _0x39C[0x320];
		eWeaponIconRatio iKillIconRatio;
		char _0x6C0[0x3A5];
		bool bFlipKillIcon;
		char _0xA66[0x32];
	} sWeaponDef;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		sWeaponDef* WeaponDef[160];
	} sWeapons;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		LPSTR szInternalName;
		sWeaponDef* WeaponDef;
		LPSTR szDisplayName;
		char _0x18[0x94];
		eWeaponIconRatio iDPadIconRatio;
		char _0xB0[0x28];
		LPVOID pKillIcon;
		LPVOID pDPadIcon;
		char _0xE8[0x40];
	} sWeaponCompleteDef;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		sWeaponCompleteDef* WeaponCompleteDef[160];
	} sCompleteWeapons;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		int iWidth;
		int iHeight;
	} sWindow;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		Vector3 vRecoilAngles;
		Vector3 vOrigin;
		char _0xC[0x84];
		Vector3 vViewAngles;
	} sViewMatrix;
	/*
	//=====================================================================================
	*/
	typedef struct
	{
		Vector2 vPunchAngles;
		float flVelocity;
		Vector3 vWeaponPunchAngles[3];
	} sPunch;
	/*
	//=====================================================================================
	*/
	static sCG* CG = (sCG*)OFF_CG;
	static sRefDef* RefDef = (sRefDef*)OFF_REFDEF;
	static sCharacterInfo* CharacterInfo = (sCharacterInfo*)OFF_CHARACTERINFO;
	static sCEntity* CEntity = (sCEntity*)OFF_CENTITY;
	static sGEntity* GEntity = (sGEntity*)OFF_GENTITY;
	static sPlayerState* PlayerState = (sPlayerState*)OFF_PLAYERSTATE;
	static sClientActive* ClientActive = (sClientActive*)OFF_CLIENTACTIVE;
	static sClientInfo* ClientInfo = (sClientInfo*)OFF_CLIENTINFO;
	static sWeapons* Weapons = (sWeapons*)OFF_WEAPONDEF;
	static sCompleteWeapons* CompleteWeapons = (sCompleteWeapons*)OFF_WEAPONCOMPLETEDEF;
	static sWindow* Window = (sWindow*)OFF_WINDOW;
	static sViewMatrix* ViewMatrix = (sViewMatrix*)OFF_VIEWMATRIX;
	static sPunch* Punch = (sPunch*)OFF_PUNCH;
	/*
	//=====================================================================================
	*/
	template<typename Return, typename... Parameters>
	inline Return VariadicCall(POINTER address, Parameters... params)
	{
		Return(*Function)(Parameters...) = (Return(*)(Parameters...))address;
		return Function(params...);
	}
	/*
	//=====================================================================================
	*/
	template<typename... Parameters>
	inline std::string VariadicText(LPCSTR format, Parameters... params)
	{
		char szBuffer[4096] = { NULL };
		sprintf_s(szBuffer, format, params...);
		return szBuffer;
	}
	/*
	//=====================================================================================
	*/
	typedef void(__fastcall* tFunction)(int localnum);
	inline void Cbuf_AddCall(tFunction function)
	{
		return VariadicCall<void>(OFF_CBUFADDCALL, 0, function);
	}
	/*
	//=====================================================================================
	*/
	inline void Cbuf_AddText(std::string command)
	{
		return VariadicCall<void>(OFF_CBUFADDTEXT, 0, command.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline void GameSendServerCommand(int clientnum, eSvsCMDType type, std::string command)
	{
		return VariadicCall<void>(OFF_GAMESENDSERVERCOMMAND, clientnum, type, command.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline BOOL LocalClientIsInGame()
	{
		return VariadicCall<BOOL>(OFF_LOCALCLIENTISINGAME, 0);
	}
	/*
	//=====================================================================================
	*/
	inline bool IsMainThread()
	{
		return VariadicCall<bool>(OFF_ISMAINTHREAD);
	}
	/*
	//=====================================================================================
	*/
	inline bool IsRenderThread()
	{
		return VariadicCall<bool>(OFF_ISRENDERTHREAD);
	}
	/*
	//=====================================================================================
	*/
	inline bool IsServerThread()
	{
		return VariadicCall<bool>(OFF_ISSERVERTHREAD);
	}
	/*
	//=====================================================================================
	*/
	inline bool IsDatabaseThread()
	{
		return VariadicCall<bool>(OFF_ISDATABASETHREAD);
	}
	/*
	//=====================================================================================
	*/
	inline LRESULT EnableConsole()
	{
		return VariadicCall<LRESULT>(OFF_ENABLECONSOLE);
	}
	/*
	//=====================================================================================
	*/
	inline void PrintToConsole(std::string message)
	{
		return VariadicCall<void>(OFF_PRINTTOCONSOLE, message.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline sDvar* FindVariable(std::string name)
	{
		return VariadicCall<sDvar*>(OFF_FINDVARIABLE, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline bool GetVariableIndex(sDvar* dvar, DWORD* index)
	{
		return VariadicCall<bool>(OFF_GETVARIABLEINDEX, dvar, index);
	}
	/*
	//=====================================================================================
	*/
	inline void Say(sGEntity* from, sGEntity* to, int mode, std::string message)
	{
		return VariadicCall<void>(OFF_SAY, from, to, mode, message.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline void SayTo(sGEntity* from, sGEntity* to, int mode, int color, std::string team, std::string name, std::string message)
	{
		return VariadicCall<void>(OFF_SAYTO, from, to, mode, color, team.c_str(), name.c_str(), message.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline sNetAdr* GetPlayerAddr(sNetAdr* netadr, int clientnum)
	{
		return VariadicCall<sNetAdr*>(OFF_GETPLAYERADDR, netadr, (LPVOID)OFF_SERVERSESSION, clientnum);
	}
	/*
	//=====================================================================================
	*/
	inline LPCSTR GetCurrentXUID()
	{
		return VariadicCall<LPCSTR>(OFF_GETCURRENTXUID, 0);
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID GetScreenMatrix()
	{
		return VariadicCall<LPVOID>(OFF_GETSCREENMATRIX);
	}
	/*
	//=====================================================================================
	*/
	inline int GetCurrentWeapon(sCG* cg)
	{
		return VariadicCall<int>(OFF_GETCURRENTWEAPON, cg);
	}
	/*
	//=====================================================================================
	*/
	inline LPCSTR GetWeaponName(int weapon, bool alternate, LPSTR name, int length)
	{
		return VariadicCall<LPCSTR>(OFF_GETWEAPONNAME, weapon, alternate, name, length);
	}
	/*
	//=====================================================================================
	*/
	inline bool WeaponIsAkimbo(int weapon)
	{
		return VariadicCall<bool>(OFF_WEAPONISAKIMBO, weapon);
	}
	/*
	//=====================================================================================
	*/
	inline bool WorldToScreen(LPVOID screenmatrix, const Vector3 world, Vector2 screen)
	{
		return VariadicCall<bool>(OFF_WORLDTOSCREEN, 0, screenmatrix, world, screen);
	}
	/*
	//=====================================================================================
	*/
	inline int AddMessageIcon(LPSTR icon, int length, LPVOID shader, float width, float height, bool flip)
	{
		return VariadicCall<int>(OFF_ADDMESSAGEICON, icon, length, 1024, shader, width, height, flip);
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID RegisterFont(std::string name)
	{
		return VariadicCall<LPVOID>(OFF_REGISTERFONT, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID RegisterShader(std::string name)
	{
		return VariadicCall<LPVOID>(OFF_REGISTERSHADER, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline int RegisterTag(std::string name)
	{
		return VariadicCall<int>(OFF_REGISTERTAG, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline void DrawStretchPic(float x, float y, float w, float h, RGBA color, LPVOID shader)
	{
		return VariadicCall<void>(OFF_DRAWSTRETCHPIC, x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f, color, shader);
	}
	/*
	//=====================================================================================
	*/
	inline void DrawRotatedPic(LPVOID screenmatrix, float x, float y, float w, float h, float angle, RGBA color, LPVOID shader)
	{
		return VariadicCall<void>(OFF_DRAWROTATEDPIC, screenmatrix, x, y, w, h, angle, color, shader);
	}
	/*
	//=====================================================================================
	*/
	inline void DrawEngineText(std::string text, LPVOID font, float x, float y, float w, float h, RGBA color, int flags)
	{
		return VariadicCall<void>(OFF_DRAWENGINETEXT, text.c_str(), text.length(), font, x, y, w, h, 0.0f, color, flags);
	}
	/*
	//=====================================================================================
	*/
	inline int StringHeight(LPVOID font)
	{
		return VariadicCall<int>(OFF_STRINGHEIGHT, font);
	}
	/*
	//=====================================================================================
	*/
	inline int StringWidth(std::string text, LPVOID font)
	{
		return VariadicCall<int>(OFF_STRINGWIDTH, text.c_str(), text.length(), font);
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID GetEntityDObj(int entitynum)
	{
		return VariadicCall<LPVOID>(OFF_GETENTITYDOBJ, entitynum);
	}
	/*
	//=====================================================================================
	*/
	inline int GetTagPosition(sCEntity* entity, LPVOID entitydobj, int tag, Vector3 position)
	{
		return VariadicCall<int>(OFF_GETTAGPOSITION, entity, entitydobj, tag, position);
	}
	/*
	//=====================================================================================
	*/
	inline int Trace(sTrace* trace, const Vector3 from, const Vector3 to, int skip, DWORD mask)
	{
		return VariadicCall<int>(OFF_TRACE, trace, from, to, skip, mask);
	}
	/*
	//=====================================================================================
	*/
	inline void VectorAngles(const Vector3 direction, Vector3 angles)
	{
		return VariadicCall<void>(OFF_VECTORANGLES, direction, angles);
	}
	/*
	//=====================================================================================
	*/
	inline void AngleVectors(const Vector3 angles, Vector3 forward, Vector3 right, Vector3 up)
	{
		return VariadicCall<void>(OFF_ANGLEVECTORS, angles, forward, right, up);
	}
	/*
	//=====================================================================================
	*/
	inline void VectorNormalize(Vector3 angles)
	{
		return VariadicCall<void>(OFF_VECTORNORMALIZE, angles);
	}
	/*
	//=====================================================================================
	*/
	inline void ExecuteKey(DWORD keyindex)
	{
		return VariadicCall<void>(OFF_EXECUTEKEY, 0, keyindex, 1, 0);
	}
	/*
	//=====================================================================================
	*/
	inline void SetZoomState(bool enable)
	{
		return VariadicCall<void>(OFF_SETZOOMSTATE, 0, enable);
	}
	/*
	//=====================================================================================
	*/
	inline bool BulletTrace(sBulletFireParams* bp, sCEntity* entity, sBulletTraceResults* br, int surfacetype)
	{
		return VariadicCall<bool>(OFF_BULLETTRACE, 0, bp, 0, entity, br, surfacetype);
	}
	/*
	//=====================================================================================
	*/
	inline bool AdvanceTrace(sBulletFireParams* bp, sBulletTraceResults* br, float distance)
	{
		return VariadicCall<bool>(OFF_ADVANCETRACE, bp, br, distance);
	}
	/*
	//=====================================================================================
	*/
	inline float GetSurfacePenetrationDepth(int weapon, bool alternate, int surfacetype)
	{
		return VariadicCall<float>(OFF_GETSURFACEPENETRATIONDEPTH, weapon, alternate, surfacetype);
	}
	/*
	//=====================================================================================
	*/
	inline float GetHipfireSpreadForWeapon(sCG* cg, int weapon, float* minimum, float* maximum)
	{
		return VariadicCall<float>(OFF_GETHIPFIRESPREADFORWEAPON, cg, weapon, minimum, maximum);
	}
	/*
	//=====================================================================================
	*/
	inline float GetZoomSpreadForWeapon(sCG* cg, int weapon, bool alternate)
	{
		return VariadicCall<float>(OFF_GETZOOMSPREADFORWEAPON, cg, weapon, alternate);
	}
	/*
	//=====================================================================================
	*/
	inline int BulletEndPosition(int* seed, float spread, const Vector3 vieworg, Vector3 end, Vector3 direction, const Vector3 forward, const Vector3 right, const Vector3 up)
	{
		return VariadicCall<int>(OFF_BULLETENDPOSITION, seed, 0.0f, spread, vieworg, end, direction, 0.0f, 360.0f, forward, right, up, 8192.0f);
	}
	/*
	//=====================================================================================
	*/
	inline int SeedRandom(int* servertime)
	{
		return VariadicCall<int>(OFF_SEEDRANDOM, servertime);
	}
	/*
	//=====================================================================================
	*/
	inline float RandomFloat(int* seed)
	{
		return VariadicCall<float>(OFF_RANDOM, seed);
	}
	/*
	//=====================================================================================
	*/
	inline void EvaluateTrajectory(sTrajectory* trajectory, int time, Vector3 result)
	{
		return VariadicCall<void>(OFF_EVALUATETRAJECTORY, trajectory, time, result);
	}
	/*
	//=====================================================================================
	*/
	inline void TeamChanged(int clientnum)
	{
		return VariadicCall<void>(OFF_TEAMCHANGED, clientnum);
	}
	/*
	//=====================================================================================
	*/
	inline long long TakeWeapon(sPlayerState* playerstate, int weapon)
	{
		return VariadicCall<long long>(OFF_TAKEWEAPON, playerstate, weapon);
	}
	/*
	//=====================================================================================
	*/
	inline long long GiveWeapon(sPlayerState* playerstate, int weapon, bool akimbo, bool alternate, bool previous)
	{
		return VariadicCall<long long>(OFF_GIVEWEAPON, playerstate, weapon, akimbo, alternate, previous);
	}
	/*
	//=====================================================================================
	*/
	inline long long AddAmmo(sPlayerState* playerstate, int weapon, bool alternate, int count, bool fill)
	{
		return VariadicCall<long long>(OFF_ADDAMMO, playerstate, weapon, alternate, count, fill);
	}
	/*
	//=====================================================================================
	*/
	inline void KickClient(int clientnum, std::string reason)
	{
		return VariadicCall<void>(OFF_KICKCLIENT, clientnum, reason.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline void PlayerDie(sGEntity* target, sGEntity* inflictor, sGEntity* attacker, int method, int weapon)
	{
		return VariadicCall<void>(OFF_PLAYERDIE, target, inflictor, attacker, 100000, method, weapon, 0, 0, 0, 0);
	}
	/*
	//=====================================================================================
	*/
	inline void PlayerKill(sGEntity* target, sGEntity* inflictor, sGEntity* attacker, int method, int weapon)
	{
		return VariadicCall<void>(OFF_PLAYERKILL, target, inflictor, attacker, 100000, method, weapon, 0, 0, 0, 0, 0);
	}
	/*
	//=====================================================================================
	*/
	inline bool UsingAkimbo(int weapon)
	{
		return (((BYTE)weapon == WEAPON_M9A1 || (BYTE)weapon == WEAPON_44_MAGNUM || (BYTE)weapon == WEAPON_MP_443_GRACH || (BYTE)weapon == WEAPON_P226) && weapon & 0x20000);
	}
	/*
	//=====================================================================================
	*/
	inline bool UsingKillstreak(int weapon)
	{
		return ((BYTE)weapon == WEAPON_HELO_PILOT || (BYTE)weapon == WEAPON_LOKI || (BYTE)weapon == WEAPON_ODIN || (BYTE)weapon == WEAPON_GRYPHON);
	}
	/*
	//=====================================================================================
	*/
	inline bool HasPerk(int clientnum, ePerk perk)
	{
		return (*(POINTER*)&CharacterInfo[clientnum].iPerks >> perk) & 0x1;
	}
	/*
	//=====================================================================================
	*/
	inline void EnablePerk(int clientnum, ePerk perk)
	{
		*(DWORD*)((POINTER)&PlayerState[clientnum] + 0x4 * (perk >> 0x5) + 0xE14) |= 0x1 << (perk & 0x1F);
	}
	/*
	//=====================================================================================
	*/
	inline void DisablePerk(int clientnum, ePerk perk)
	{
		*(DWORD*)((POINTER)&PlayerState[clientnum] + 0x4 * (perk >> 0x5) + 0xE14) &= ~(0x1 << (perk & 0x1F));
	}
}

//=====================================================================================