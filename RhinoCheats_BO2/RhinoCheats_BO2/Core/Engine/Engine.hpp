//=====================================================================================

#pragma once

#include <vector>
#include "Thunk.hpp"

//=====================================================================================

#define MAX_CLIENTS 18
#define MAX_ENTITIES 1024
#define EF_CROUCH 0x4
#define EF_PRONE 0x8
#define EF_SPRINT 0x20000
#define EF_ZOOM 0x80000
#define P_ATTACK 1
#define M_ATTACK 2
#define M_METERS 55.0f
#define ID_ASSAULTSHIELD 89
#define M_PI_DOUBLE ((float)M_PI*2.0f)

#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) ((float)((a)*(360.0f/65536)))
#define AngleNormalize(a) (ShortToAngle(AngleToShort((a))))
#define DotProduct(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define AngleCompare180(a) (((a)<90.0f&&(a)>-90.0f)||((a)>270.0f||(a)<-270.0f))
#define VectorCopy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define VectorAdd(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorSubtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define	VectorScale(a,b,c) ((c)[0]=(a)[0]*(b),(c)[1]=(a)[1]*(b),(c)[2]=(a)[2]*(b))
#define	VectorMA(a,b,c,d) ((d)[0]=(a)[0]+(c)[0]*(b),(d)[1]=(a)[1]+(c)[1]*(b),(d)[2]=(a)[2]+(c)[2]*(b))
#define VectorAverage(a,b,c) ((c)[0]=((a)[0]+(b)[0])/2.0f,(c)[1]=((a)[1]+(b)[1])/2.0f,(c)[2]=((a)[2]+(b)[2])/2.0f)
#define GetDistance2D(a,b) (sqrtf(((a)[0]-(b)[0])*((a)[0]-(b)[0])+((a)[1]-(b)[1])*((a)[1]-(b)[1])))
#define GetDistance3D(a,b) (sqrtf(((a)[0]-(b)[0])*((a)[0]-(b)[0])+((a)[1]-(b)[1])*((a)[1]-(b)[1])+((a)[2]-(b)[2])*((a)[2]-(b)[2])))

#if defined CONFIGURATION_PLUTONIUMT6R
#define OFF_CHATHEIGHT_DVAR 0x2A1AC3C
#define OFF_CHATHEIGHT_EXCEPTION 0x44DFC9
#define OFF_DRAWFPS_DVAR 0x10AA218
#define OFF_DRAWFPS_EXCEPTION 0x44DFC9
#define OFF_DRAWBIGFPS_DVAR 0x1140A08
#define OFF_DRAWBIGFPS_EXCEPTION 0x44DFC9
#define OFF_DRAW2D_DVAR 0x10AC164
#define OFF_DRAW2D_EXCEPTION 0x6226FB
#define OFF_NODELTA_DVAR 0x11C3634
#define OFF_NODELTA_EXCEPTION 0x6226FB
#define OFF_SMOOTHINGTIME_DVAR 0x11409D0
#define OFF_SMOOTHINGTIME_EXCEPTION 0x4645DB
#define OFF_MOUSEACCEL_DVAR 0x11C7834
#define OFF_MOUSEACCEL_EXCEPTION 0x4645DB
#define OFF_PREDICTPLAYERSTATE 0x5B4F40

#define OFF_CG 0x3F0F8C80
#define OFF_INPUT 0x3D77C5D8
#define OFF_VIEWANGLE 0x3D73C5BC
#define OFF_RECOILVEC 0x3D73C530
#define OFF_WINDOW 0x11D073C
#define OFF_WINDOWHANDLE 0x2B6ED88
#define OFF_SERVERID 0x11D0ACC

#define OFF_LOCALCLIENTISINGAME 0x402F80
#define OFF_ADDRELIABLECOMMAND 0x6A1C40
#define OFF_GETSPREADFORWEAPON 0x5B0BE0
#define OFF_REGISTERTAG 0x5EE820
#define OFF_GETDOBJ 0x4DA190
#define OFF_GETTAGPOS 0x4A1210
#define OFF_BULLETTRACE 0x7E0A00
#define OFF_GETTRACEHITTYPE 0x6A32F0
#define OFF_ADVANCETRACE 0x703090
#define OFF_GETPENETRATETYPE 0x4AC220
#define OFF_GETSURFACEPENETRATIONDEPTH 0x622CE0
#define OFF_ENTITYISDEPLOYEDRIOTSHIELD 0x501160
#define OFF_HASPERK 0x530C10
#define OFF_WEAPONISAKIMBO 0x52E780
#define OFF_LOCATIONALTRACE 0x50B870
#define OFF_GETPLAYERVIEWORIGIN 0x640E80
#define OFF_HASHSEED 0x6AACC0
#define OFF_GETWEAPON 0x5225F0
#define OFF_GETWEAPONNAME 0x9502B0
#define OFF_RANDOMFLOAT 0x4947E0
#define OFF_SEEDRANDOM 0x556180
#define OFF_WEAPONFIRERECOIL 0x54C640
#define OFF_SETZOOMSTATE 0x583950
#define OFF_EXECUTEKEY 0x66C390

#define OFF_GAMEOVERLAYPRESENT 0x124B84
#define OFF_SWAPCHAIN 0x35E5F94
#define OFF_MOUSEINPUT 0x2B69969

#define OFF_TACSSCHECK 0x2B68838
#define OFF_TACSSPATCH 0x2B68824
#define OFF_TACSSHANDLE 0x3A248F4
#elif defined CONFIGURATION_REDACTED
#define OFF_CHATHEIGHT_DVAR 0x2A1AC3C
#define OFF_CHATHEIGHT_EXCEPTION 0x44DFC9
#define OFF_DRAWFPS_DVAR 0x10AA218
#define OFF_DRAWFPS_EXCEPTION 0x44DFC9
#define OFF_DRAWBIGFPS_DVAR 0x1140A08
#define OFF_DRAWBIGFPS_EXCEPTION 0x44DFC9
#define OFF_DRAW2D_DVAR 0x10AC164
#define OFF_DRAW2D_EXCEPTION 0x6226FB
#define OFF_NODELTA_DVAR 0x11C3634
#define OFF_NODELTA_EXCEPTION 0x6226FB
#define OFF_SMOOTHINGTIME_DVAR 0x11409D0
#define OFF_SMOOTHINGTIME_EXCEPTION 0x4645DB
#define OFF_MOUSEACCEL_DVAR 0x11C7834
#define OFF_MOUSEACCEL_EXCEPTION 0x4645DB
#define OFF_PREDICTPLAYERSTATE 0x5B4F40

#define OFF_CG 0x2E448C80
#define OFF_INPUT 0x2CACC5D8
#define OFF_VIEWANGLE 0x2CA8C5BC
#define OFF_RECOILVEC 0x2CA8C530
#define OFF_WINDOW 0x11D073C
#define OFF_WINDOWHANDLE 0x2B6ED88
#define OFF_SERVERID 0x11D0ACC

#define OFF_LOCALCLIENTISINGAME 0x402F80
#define OFF_ADDRELIABLECOMMAND 0x6A1C40
#define OFF_GETSPREADFORWEAPON 0x5B0BE0
#define OFF_REGISTERTAG 0x5EE820
#define OFF_GETDOBJ 0x4DA190
#define OFF_GETTAGPOS 0x4A1210
#define OFF_BULLETTRACE 0x7E0A00
#define OFF_GETTRACEHITTYPE 0x6A32F0
#define OFF_ADVANCETRACE 0x703090
#define OFF_GETPENETRATETYPE 0x4AC220
#define OFF_GETSURFACEPENETRATIONDEPTH 0x622CE0
#define OFF_ENTITYISDEPLOYEDRIOTSHIELD 0x501160
#define OFF_HASPERK 0x530C10
#define OFF_WEAPONISAKIMBO 0x52E780
#define OFF_LOCATIONALTRACE 0x50B870
#define OFF_GETPLAYERVIEWORIGIN 0x640E80
#define OFF_HASHSEED 0x6AACC0
#define OFF_GETWEAPON 0x5225F0
#define OFF_GETWEAPONNAME 0x9502B0
#define OFF_RANDOMFLOAT 0x4947E0
#define OFF_SEEDRANDOM 0x556180
#define OFF_WEAPONFIRERECOIL 0x54C640
#define OFF_SETZOOMSTATE 0x583950
#define OFF_EXECUTEKEY 0x66C390

#define OFF_GAMEOVERLAYPRESENT 0x124B84
#define OFF_SWAPCHAIN 0x35E5F94
#define OFF_MOUSEINPUT 0x2B69969

#define OFF_TACSSCHECK 0x2B68838
#define OFF_TACSSPATCH 0x2B68824
#define OFF_TACSSHANDLE 0x3A248F4
#elif defined CONFIGURATION_STEAM
#define OFF_CHATHEIGHT_DVAR 0x2A1AC3C
#define OFF_CHATHEIGHT_EXCEPTION 0x441069
#define OFF_DRAWFPS_DVAR 0x10AA218
#define OFF_DRAWFPS_EXCEPTION 0x441069
#define OFF_DRAWBIGFPS_DVAR 0x1140A08
#define OFF_DRAWBIGFPS_EXCEPTION 0x441069
#define OFF_DRAW2D_DVAR 0x10AC164
#define OFF_DRAW2D_EXCEPTION 0x47871B
#define OFF_NODELTA_DVAR 0x11C3634
#define OFF_NODELTA_EXCEPTION 0x47871B
#define OFF_SMOOTHINGTIME_DVAR 0x11409D0
#define OFF_SMOOTHINGTIME_EXCEPTION 0x5BE6FB
#define OFF_MOUSEACCEL_DVAR 0x11C7834
#define OFF_MOUSEACCEL_EXCEPTION 0x5BE6FB
#define OFF_PREDICTPLAYERSTATE 0x5B3C40

#define OFF_CG 0x2E448C80
#define OFF_INPUT 0x2CACC5D8
#define OFF_VIEWANGLE 0x2CA8C5BC
#define OFF_RECOILVEC 0x2CA8C530
#define OFF_WINDOW 0x11D073C
#define OFF_WINDOWHANDLE 0x2B6ED88
#define OFF_SERVERID 0x11D0ACC

#define OFF_LOCALCLIENTISINGAME 0x5922F0
#define OFF_ADDRELIABLECOMMAND 0x5E58E0
#define OFF_GETSPREADFORWEAPON 0x402A80
#define OFF_REGISTERTAG 0x479B40
#define OFF_GETDOBJ 0x5D2590
#define OFF_GETTAGPOS 0x664930
#define OFF_BULLETTRACE 0x7E0170
#define OFF_GETTRACEHITTYPE 0x4AC3D0
#define OFF_ADVANCETRACE 0x5B0F60
#define OFF_GETPENETRATETYPE 0x62E290
#define OFF_GETSURFACEPENETRATIONDEPTH 0x572480
#define OFF_ENTITYISDEPLOYEDRIOTSHIELD 0x45D4D0
#define OFF_HASPERK 0x42BD20
#define OFF_WEAPONISAKIMBO 0x6625B0
#define OFF_LOCATIONALTRACE 0x6C6C00
#define OFF_GETPLAYERVIEWORIGIN 0x580890
#define OFF_HASHSEED 0x543CE0
#define OFF_GETWEAPON 0x5846F0
#define OFF_GETWEAPONNAME 0x458190
#define OFF_RANDOMFLOAT 0x6A3490
#define OFF_SEEDRANDOM 0x4F38A0
#define OFF_WEAPONFIRERECOIL 0x407530
#define OFF_SETZOOMSTATE 0x42BD00
#define OFF_EXECUTEKEY 0x4A5A20

#define OFF_GAMEOVERLAYPRESENT 0x124B84
#define OFF_SWAPCHAIN 0x3606F94
#define OFF_MOUSEINPUT 0x2B69969

#define OFF_TACSSCHECK 0x2B68838
#define OFF_TACSSPATCH 0x2B68824
#define OFF_TACSSHANDLE 0x3A458F4
#endif

//=====================================================================================

namespace RhinoCheats {
	typedef float Vector;
	typedef Vector Vector2[2];
	typedef Vector Vector3[3];
	typedef Vector RGBA[4];
	/*
	//=====================================================================================
	*/
	typedef enum {
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
	} bone_e;
	/*
	//=====================================================================================
	*/
	typedef enum {
		PENETRATE_TYPE_NONE,
		PENETRATE_TYPE_SMALL,
		PENETRATE_TYPE_MEDIUM,
		PENETRATE_TYPE_LARGE,
		PENETRATE_TYPE_COUNT,
	} penetrateType_e;
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
	typedef struct {
		int iServerTime;
		char _0x4[20];
		int iGodMode;
		char _0x1C[12];
		Vector3 vOrigin;
		Vector3 vVelocity;
		char _0x40[12];
		int iRapidFire;
		char _0x50[60];
		int iGravity;
		char _0x90[4];
		int iSpeed;
		char _0x98[328];
		float flZoomProgress;
		char _0x1E4[4];
		float flSpreadMultiplier;
		int iNoSpread;
		char _0x1F0[8];
		Vector3 vViewAngles;
		int iPlayerHeight;
		Vector3 vPlayerHeight;
		char _0x214[44];
		int iHealth;
		char _0x244[4];
		int iMaxHealth;
		char _0x24C[116];
		int iSecondaryWeaponID;
		char _0x2C4[24];
		int iPrimaryWeaponID;
		char _0x2E0[292];
		int iSecondaryAmmoStock;
		int iPrimaryAmmoStock;
		char _0x40C[52];
		int iSecondaryAmmoClip;
		int iPrimaryAmmoClip;
		int iLethalAmmo;
		int iTacticalAmmo;
		char _0x450[256];
		int iPerkFlags;
	}  playerState_s;
	/*
	//=====================================================================================
	*/
	typedef struct {
		char _0x0[8];
		int iWidth;
		int iHeight;
		char _0x10[16];
		float flFOVX;
		float flFOVY;
		float flFOVZ;
		char _0x2C[4];
		float flFOV;
		Vector3 vViewOrg;
		char _0x40[4];
		Vector3 vViewAxis[3];
	} refdef_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int* szName;
		char _0x4[108];
	} shader_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		char _0x0[940];
		shader_t* pShader;
		char _0x3B0[12];
		shader_t* pShader2;
	} weapon_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int iInfoValid;
		int iNextValid;
		int iClientNum;
		char szName[32];
		int iTeam;
		int iTeam2;
		int iFFATeam;
		char _0x38[40];
		int iRank;
		char _0x64[48];
		int iScore;
		char _0x98[72];
		char szModel[64];
		char _0x120[932];
		Vector3 vViewAngles;
		char _0x4D0[244];
		int iWeaponID;
		char _0x5C8[448];
		weapon_t* pCurrentWeapon;
		char _0x78C[12];
		weapon_t* pPrimaryWeapon;
		char _0x79C[108];
	} clientInfo_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		char _0x0[48];
		char szGameType[4];
	} cgs_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		char _0x0[4];
		short iUsedForPlayerMesh;
		char _0x8[38];
		Vector3 vOrigin;
		char _0x38[296];
		int eFlags;
		char _0x164[16];
		Vector3 vOldOrigin;
		char _0x180[60];
		int iPlayerPrimaryWeaponID;
		int iPlayerSecondaryWeaponID;
		char _0x1C4[24];
		int iClientNum;
		int iFlags;
		char _0x1E4[16];
		Vector3 vNewOrigin;
		char _0x200[60];
		int iWeaponID2;
		char _0x240[116];
		short bEntityType;
		char _0x2B6[10];
		int iWeaponID;
		char _0x2C4[180];
		BYTE bAlive;
		char _0x379[7];
	} centity_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int iClientNum;
		char _0x4[20];
		int iIsInGame;
		char _0x1C[104];
		int iPing;
		int iNewTime;
		int iTime;
		int iOldTime;
		char _0x94[36];
		Vector3 vOrigin;
		Vector3 vVelocity;
		char _0xD0[140];
		int iMoveType;
		char _0x160[44];
		int iMoveFlag;
		char _0x190[184];
		int iWeaponID;
		char _0x24C[36];
		float flZoomProgress;
		char _0x274[84];
		int iHealth;
		char _0x2CC[294364];
		playerState_s PlayerState;
		char _0x485FC[21140];
		refdef_t RefDef;
		char _0x4D8F8[115048];
		clientInfo_t Client[MAX_CLIENTS];
		char _0x72AF0[57452];
		float flSpread;
		char _0x80B60[27552];
		cgs_t CGS;
		char _0x87734[121100];
		centity_t CEntity[MAX_ENTITIES];
	} cg_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int iServerTime;
		int iButtons[2];
		int iViewAngles[3];
		char _0x18[12];
		char szForwardMove;
		char szRightMove;
		char _0x26[22];
	} usercmd_s;
	/*
	//=====================================================================================
	*/
	typedef struct {
		usercmd_s UserCMD[128];
		int iCurrentCMD;

		usercmd_s* GetUserCMD(
			int number
		) {
			return &UserCMD[number & 0x7F];
		}
	} input_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		Vector3 vAngles;
	} viewAngle_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		Vector3 vAngles;
	} recoilVec_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int iWidth;
		int iHeight;
	} window_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		Vector3 vNormal;
		char _0xC[4];
		float flFraction;
		int iSFlags;
		char _0x18[8];
		short iHitID;
		char _0x22[6];
		short iPartGroup;
		char bAllSolid;
		char bStartSolid;
		char _0x2C[20];
	} trace_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		trace_t Trace;
		char _0x40[4];
		Vector3 vHitPos;
		char iIgnoreHitEnt;
		char _0x51[3];
		int iSurfaceType;
		char _0x58[8];
	} bulletTraceResults_t;
	/*
	//=====================================================================================
	*/
	typedef struct {
		int iMaxEntNum;
		int iEntityNum;
		float flPower;
		int iBulletType;
		Vector3 vViewOrigin;
		Vector3 vStart;
		Vector3 vEnd;
		Vector3 vDir;
	} bulletFireParams_t;
	/*
	//=====================================================================================
	*/
	static cg_t* CG = (cg_t*)OFF_CG;
	static input_t* Input = (input_t*)OFF_INPUT;
	static viewAngle_t* ViewAngle = (viewAngle_t*)OFF_VIEWANGLE;
	static recoilVec_t* RecoilVec = (recoilVec_t*)OFF_RECOILVEC;
	static window_t* Window = (window_t*)OFF_WINDOW;
	/*
	//=====================================================================================
	*/
	template<typename Return, typename... Parameters>
	inline Return VariadicCall(DWORD_PTR address, Parameters... params)
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
	inline BOOL LocalClientIsInGame() {
		return VariadicCall<BOOL>(OFF_LOCALCLIENTISINGAME, 0);
	}
	/*
	//=====================================================================================
	*/
	inline int AddReliableCommand(
		std::string command
	) {
		return VariadicCall<int>(OFF_ADDRELIABLECOMMAND, 0, command.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline WORD RegisterTag(
		std::string name
	) {
		return VariadicCall<WORD>(OFF_REGISTERTAG, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID GetDObj(
		centity_t* Ent
	) {
		return VariadicCall<LPVOID>(OFF_GETDOBJ, Ent->iClientNum, Ent->iUsedForPlayerMesh);
	}
	/*
	//=====================================================================================
	*/
	inline LPVOID GetTagPosition(
		centity_t* Ent,
		WORD Tag,
		LPVOID dobj,
		Vector3 Out
	) {
		return VariadicCall<LPVOID>(OFF_GETTAGPOS, Ent, dobj, Tag, Out);
	}
	/*
	//=====================================================================================
	*/
	inline bool GetPlayerViewOrigin(
		Vector3 Out
	) {
		return VariadicCall<bool>(OFF_GETPLAYERVIEWORIGIN, 0, &CG->PlayerState, Out);
	}
	/*
	//=====================================================================================
	*/
	inline bool HasPerk(
		int iIndex
	) {
		return VariadicCall<bool>(OFF_HASPERK, 0, CG->iClientNum, iIndex);
	}
	/*
	//=====================================================================================
	*/
	inline bool WeaponIsAkimbo(
		int weapon
	) {
		return VariadicCall<bool>(OFF_WEAPONISAKIMBO, weapon);
	}
	/*
	//=====================================================================================
	*/
	inline int GetWeapon(
		int weapon
	) {
		return VariadicCall<int>(OFF_GETWEAPON, weapon);
	}
	/*
	//=====================================================================================
	*/
	inline LPCSTR GetWeaponName(
		int weapon,
		LPSTR name,
		int length
	) {
		return VariadicCall<LPCSTR>(OFF_GETWEAPONNAME, weapon, name, length);
	}
	/*
	//=====================================================================================
	*/
	inline int LocationalTrace(
		trace_t* Trace,
		Vector3 ViewOrigin,
		Vector3 EndVec,
		int a4,
		int Num,
		int a6,
		int a7
	) {
		return VariadicCall<int>(OFF_LOCATIONALTRACE, Trace, ViewOrigin, EndVec, a4, Num, a6, a7);
	}
	/*
	//=====================================================================================
	*/
	inline penetrateType_e GetPenetrateType(
		int weapon
	) {
		return VariadicCall<penetrateType_e>(OFF_GETPENETRATETYPE, weapon);
	}
	/*
	//=====================================================================================
	*/
	inline float GetSurfacePenetrationDepth(
		penetrateType_e PenType,
		int SurfaceType
	) {
		return VariadicCall<float>(OFF_GETSURFACEPENETRATIONDEPTH, PenType, SurfaceType);
	}
	/*
	//=====================================================================================
	*/
	inline BOOL EntityIsDeployedRiotshield(
		centity_t* Ent
	) {
		return VariadicCall<BOOL>(OFF_ENTITYISDEPLOYEDRIOTSHIELD, Ent);
	}
	/*
	//=====================================================================================
	*/
	inline bool AdvanceTrace(
		bulletFireParams_t* bp,
		bulletTraceResults_t* br,
		float dist
	) {
		return VariadicCall<bool>(OFF_ADVANCETRACE, bp, br, dist);
	}
	/*
	//=====================================================================================
	*/
	inline int GetTraceHitType(
		bulletTraceResults_t* br
	) {
		return VariadicCall<int>(OFF_GETTRACEHITTYPE, br);
	}
	/*
	//=====================================================================================
	*/
	inline void GetSpreadForWeapon(
		int weapon,
		float* minSpread,
		float* maxSpread
	) {
		return VariadicCall<void>(OFF_GETSPREADFORWEAPON, &CG->PlayerState, weapon, minSpread, maxSpread);
	}
	/*
	//=====================================================================================
	*/
	inline void HashSeed(
		int* serverTimeSeed
	) {
		return VariadicCall<void>(OFF_HASHSEED, serverTimeSeed);
	}
	/*
	//=====================================================================================
	*/
	inline float RandomFloat(
		int* serverTimeSeed
	) {
		return VariadicCall<float>(OFF_RANDOMFLOAT, serverTimeSeed);
	}
	/*
	//=====================================================================================
	*/
	inline void SeedRandom(
		int* serverTimeSeed
	) {
		return VariadicCall<void>(OFF_SEEDRANDOM, serverTimeSeed);
	}
	/*
	//=====================================================================================
	*/
	inline int SetZoomState(
		bool enable
	) {
		return VariadicCall<int>(OFF_SETZOOMSTATE, 0, enable);
	}
	/*
	//=====================================================================================
	*/
	inline void ExecuteKey(DWORD keyindex)
	{
		return VariadicCall<void>(OFF_EXECUTEKEY, 0, keyindex, 1, 0);
	}
}

//=====================================================================================