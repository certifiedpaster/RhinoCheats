#pragma once

typedef struct v3{
	float x, y, z;
	v3()
	{
		x = y = z = 0.0f;
	}
	v3(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	
	float Distanceto(v3 b) {
		float
			A = this->x - b.x,
			B = this->y - b.y,
			C = this->z - b.z;
		return sqrtf((float)(A * A) + (B * B) + (C * C));
	}

} Vector3;

template<typename R, typename... Arguments>
R Call(long long function, Arguments... args)
{
	R(*temp)(Arguments...) = (R(*)(Arguments...))function;
	return temp(args...);
}

typedef int(__cdecl *VM_Notify_t)(int a1, unsigned int a2, unsigned int *a3);
extern VM_Notify_t VM_Notify;

typedef void(__cdecl* SV_GameSendServerCommand_t)(int clientNum, int reliable, char* command);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

typedef void(__cdecl *player_die_t)(DWORD* self, DWORD* inflictor, DWORD* attacker, int damage, int meansOfDeath, int Weapon, const float *Direction, int hitLocation, int TimeOffset, int unk);
extern player_die_t player_die;

namespace Host 
{
	//Called in Imgui's console
	void Derank(LPVOID HackPath);
	void Rerank(LPVOID HackPath);

	void Process();	
	
	void VM_Notify(unsigned int notifyListOwnerId, unsigned int stringValue, void *top);
}