#pragma once

class Aim_t
{
public:

	typedef struct
	{
		float damage;
		vec3_t bestBone;
	}DamageInfo_t;

	typedef struct {
		float distance;
		float distanceToKnife;
		vec3_t vAimLocation;
		int clientNum;
	}TargetInfo_t;

	enum AimKeys
	{
		g_bAimbotL,
		g_bAimbotR,
		g_bAimbotM,
		g_bAimbotCtrl,
		g_bAimbotShift,
		g_bAimbotKey_
	};

	enum AimReady {
		isReadyforFire,
		isReadyforKnife,
		isReady_
	};

	bool g_bAimbotKey[g_bAimbotKey_];
	bool isReady[isReady_];	

	vec2_t vAimAngles;
	vec3_t vAimLocation;
	bool isVehicle;

	Aim_t()
	{
		g_bAimbotKey[g_bAimbotKey_] = { false };
		isReady[isReady_] = { false };

		vAimAngles[2] = { 0.0f };
		vAimLocation[3] = { 0.0f };
		isVehicle = false;
	}

	void   SetTime4NameChange(unsigned int i) { timeChanged = i; }	
	void   Run();	

	void   Autoshoot(); // vehicles fix

private:
	void  ApplyPrediction(int i, vec3_t vData);
	bool  MyVisibilityMethod(int i, int autowall, float *damage, bool isVehicle);
	bool  MyRiotShieldMethod(int i, int autowall, float *damage, bool isVehicle);
	bool       GetVisibility(int i, int autowall, float *damage, bool isVehicle);
	bool  IsAbletoFire(int state);
	int   GetClosestClientByDistance(bool isVehicle);
	int   GetClosestClientByFOV();	
	void  AutoKnife();
	void  PerformAimbot();	
	void  PredatorAim(float x, float y);
	

	unsigned int timeChanged;
	int    closestPlayer, clientNum;	
	float  clientDistance, tmpDist;
	vec3_t bestBone, vDelta;	

	

};

extern Aim_t Aim;

