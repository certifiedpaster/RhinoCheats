#include "stdafx.h"

template<typename... Arguments>
char* va(const char * fmt, Arguments... args) {
	return Call<char*>(0x005C2CF0, fmt, args...);
}

void iPrintlnBold(int Client, const char * message) {
	Call<void>(0x00573220, Client, 0, va("c \"%s\"", message));
}
void iPrintln(int Client, const char * message) {
	Call<void>(0x00573220, Client, 0, va("f \"%s\"", message));
}

const char * SL_ConvertToString(int string) {
	return Call<const char *>(0x00564270, string);
}

//G_
void(*G_SetModel)(int clientIndex, const char* Model) = (void(*)(int, const char*))0x0052DFD0;
int(*G_Spawn)() = (int(*)())0x0052E820;
void G_SetOrigin(int Ent, Vector3 Origin) {
	Call<void>(0x0052ECD0, Ent, (float*)&Origin);
}
void G_SetAngle(int Ent, Vector3 Angle) {
	Call<void>(0x0052ED20, Ent, (float*)&Angle);
}
void G_LocationalTrace(trace_t *results, Vector3 start, Vector3 end, int passEntityNum, int contentmask, char *priorityMap)
{
	Call<void>(0x0050CFB0, results, (float*)&start, (float*)&end, passEntityNum, contentmask, priorityMap);
}
int G_LocalizedStringIndex(const char * text)
{
	return Call<int>(0x52D900, text);
}
int G_MaterialIndex(const char * material)
{
	return Call<int>(0x52D9C0, material);
}


//Scr_
int Scr_GetLocalPlayer() {
	return *(int*)(0x009010F0);
}
void Scr_AddInt(int A) {
	Call<void>(0x0056AA20, A);
}
void Scr_AddFloat(float A)
{
	Call<void>(0x0056AA70, A);
}
void Scr_AddString(const char * A) {
	Call<void>(0x0056AC00, A);
}
void Scr_AddEntity(int ent)
{
	Call<void>(0x0052B060, ent);
}
void Scr_AddVector(Vector3 ssssssss)
{
	Call<void>(0x0056AD20, (float*)&ssssssss);
}
void Scr_SetNumParam(int A) {
	*(int*)(0x20B4A98) = A;
}
void Scr_ClearOutParams() {
	Call<void>(0x00569010);
}
int Scr_GetSelf(unsigned int threadId) {
	return Call<int>(0x00565F60, threadId);
}
int Scr_GetFunctionHandle(int A, int B) {
	return Call<int>(0x005618A0, A, B);
}
void Scr_AddAny(int A) {
	Call<void>(0x0056AA20, A);
}
void Scr_AddAny(const char * A) {
	Call<void>(0x0056AC00, A);
}

Vector3 Scr_BulletTrace(int Ent, Vector3 start, Vector3 end)
{
	trace_t trace;

	G_LocationalTrace(&trace, start, end, Ent, 0x2806831, 0);

	return Vector3(
		start.x + (end.x - start.x) * trace.fraction,
		(end.y - start.y) * trace.fraction + start.y,
		trace.fraction * (end.z - start.z) + start.z
	);
}

//GScr_
void GScr_SetDvar(const char * dvar, const char * value) {
	Scr_AddString(value);
	Scr_AddString(dvar);
	Scr_SetNumParam(2);
	Call<void>(0x00517AB0);
	Scr_ClearOutParams();
}
void GScr_SetDvar2(const char * dvar, const char * value) {
	Scr_AddString(value);
	Scr_AddString(dvar);
	Scr_SetNumParam(2);
	Call<void>(0x005256E0);
	Scr_ClearOutParams();
}
void GScr_MakeDvarServerInfo(const char * dvar, const char * value) {
	Scr_AddString(value);
	Scr_AddString(dvar);
	Scr_SetNumParam(2);
	Call<void>(0x00517B50);
	Scr_ClearOutParams();
}
void GScr_MagicBullet(const char * bullet, Vector3 start, Vector3 end, int ent)
{
	Scr_AddEntity(ent);
	Scr_AddVector(end);
	Scr_AddVector(start);
	Scr_AddString(bullet);
	Scr_SetNumParam(4);
	Call<void>(0x00521C60);
	Scr_ClearOutParams();
}
void GScr_SetExpFog(float startDist, float halfwayDist, float red, float green, float blue, float transitionTime)
{
	Scr_AddFloat(transitionTime);
	Scr_AddFloat(blue);
	Scr_AddFloat(green);
	Scr_AddFloat(red);
	Scr_AddFloat(halfwayDist);
	Scr_AddFloat(startDist);
	Scr_SetNumParam(6);
	Call<void>(0x00520170);
	Scr_ClearOutParams();
}

template <typename AA, typename BB>
void GScr_SetPlayerData(int client, AA stat, BB value) {
	Scr_AddAny(value);
	Scr_AddAny(stat);
	Scr_SetNumParam(2);
	Call<void>(0x00522920, client);
	Scr_ClearOutParams();
}

template <typename AA, typename BB, typename CC>
void GScr_SetPlayerData(int client, AA stat, BB value1, CC value2) {
	Scr_AddAny(value2);
	Scr_AddAny(value1);
	Scr_AddAny(stat);
	Scr_SetNumParam(2);
	Call<void>(0x00522920, client);
	Scr_ClearOutParams();
}

template <typename AA, typename BB, typename CC, typename DD>
void GScr_SetPlayerData(int client, AA stat, BB value1, CC value2, DD value3) {
	Scr_AddAny(value3);
	Scr_AddAny(value2);
	Scr_AddAny(value1);
	Scr_AddAny(stat);
	Scr_SetNumParam(2);
	Call<void>(0x00522920, client);
	Scr_ClearOutParams();
}

template <typename AA, typename BB, typename CC, typename DD, typename EE>
void GScr_SetPlayerData(int client, AA stat, BB value1, CC value2, DD value3, EE value4) {
	Scr_AddAny(value4);
	Scr_AddAny(value3);
	Scr_AddAny(value2);
	Scr_AddAny(value1);
	Scr_AddAny(stat);
	Scr_SetNumParam(2);
	Call<void>(0x00522920, client);
	Scr_ClearOutParams();
}

//========================================================================

void(*SV_SetBrushModel)(int Entity) = (void(*)(int))0x005733A0;
void(*SV_UnlinkEntity)(int Entity) = (void(*)(int))0x00000000;
void(*SV_linkEntity)(int Entity) = (void(*)(int))0x00529050;
void(*SP_Script_Model)(int Entity) = (void(*)(int))0x00529100;

int GetLocalClient() {
	return *(int*)(0x00900FA0 + 0x150);
}
int GetEntity(int num) {
	return (0x01A66E28 + (0x274 * num));
}

Vector3 GetOrigin(int Client)
{
	return Vector3(*(float*)(0x1A66E28 + (0x274 * Client) + 0x18), *(float*)(0x1A66E28 + (0x274 * Client) + 0x1C), *(float*)(0x1A66E28 + (0x274 * Client) + 0x20));
}
Vector3 GetAngle(int Client)
{
	return Vector3(*(float*)(0x1A66E28 + (0x274 * Client) + 0x3C), *(float*)(0x1A66E28 + (0x274 * Client) + 0x40), *(float*)(0x1A66E28 + (0x274 * Client) + 0x44));
}

int SpawnEntity(Vector3 Origin, Vector3 Angles, const char* Model = "com_plasticcase_friendly", int Index = 2)
{
	int dwEntity = (int)G_Spawn();
	*(Vector3*)(dwEntity + 0x138) = Origin;
	*(Vector3*)(dwEntity + 0x144) = Angles;
	G_SetModel(dwEntity, Model);
	SP_Script_Model(dwEntity);
	*(char*)(dwEntity + 0x101) = 4;
	*(int*)(dwEntity + 0x8C) = Index;
	SV_SetBrushModel(dwEntity);
	SV_linkEntity(dwEntity);
	return dwEntity;
}
int SpawnIn(Vector3 Origin, Vector3 Angles)
{
	int sEntity = (int)G_Spawn();
	*(Vector3*)(sEntity + 0x138) = Origin;
	*(Vector3*)(sEntity + 0x144) = Angles;
	G_SetModel(sEntity, "com_plasticcase_friendly");
	SP_Script_Model(sEntity);
	*(char*)(sEntity + 0x101) = 4;
	*(int*)(sEntity + 0x8C) = 4;
	SV_SetBrushModel(sEntity);
	SV_linkEntity(sEntity);
	return sEntity;
}
void SpawnHeli(int Client) {
	int MaxHeli = G_Spawn();
	Call<void>(0x00564C40, (MaxHeli + 370), *(short*)(0x01C8E0D6));
	Call<void>(0x00564C40, (MaxHeli + 372), *(short*)(0x01C8E0D6));
	G_SetOrigin(MaxHeli, GetOrigin(0));
	G_SetAngle(MaxHeli, GetAngle(0));
	Call<void>(0x00626100, MaxHeli, GetEntity(Client), "pavelow_mp", "vehicle_pavelow");
	Call<void>(0x0052B060, MaxHeli);
	//printf("Entity Spawned: %x\n", MaxHeli);
}

void Infect(const char * dvar, const char * value) {
	GScr_SetDvar(dvar, value);
	GScr_SetDvar2(dvar, value);
	GScr_MakeDvarServerInfo(dvar, value);
}

void ScriptEntCmd_MoveTo(int entNum, Vector3 Origin, float accelSpeed, float time, float deccelSpeed) {
	Scr_AddFloat(deccelSpeed);
	Scr_AddFloat(time);
	Scr_AddFloat(accelSpeed);
	Scr_AddVector(Origin);
	Scr_SetNumParam(4);
	Call<void>(0x00529260, entNum);
	Scr_ClearOutParams();
}

void Cmd_RegisterNotification(int Client, const char * command, const char * notify) {
	Call<void>(0x005461C0, Client, command, notify);
}

Vector3 PlayerAnglesToForward(int clientIndex, float Distance = 200.0f)
{
	Vector3 Angles = { *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x3C), *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x40), *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x44) };
	Vector3 Position = { *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x18), *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x1C), *(float*)(0x1A66E28 + (0x274 * clientIndex) + 0x20) + 50 };//GScr_GetTagOrigin(clientIndex, "tag_eye");
	float angle, sr, sp, sy, cr, cp, cy, PiDiv;
	PiDiv = ((float)3.14159265358979323846 / 180.0f);
	angle = Angles.y * PiDiv;
	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = Angles.x * PiDiv;
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = Angles.z * PiDiv;
	sr = (float)sin(angle);
	cr = (float)cos(angle);
	Vector3 Forward = { (cp * cy * Distance) + Position.x, (cp * sy * Distance) + Position.y, (-sp * Distance) + Position.z };
	return Forward;
}
bool isAlive(int Client)
{
	if (*(int*)(0x1A66E28 + (Client * 0x274) + 0x19C) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void SetOrigin(int num, Vector3 Origin) {
	*(float*)(0x01C2D45C + (num * 0x38EC) + 0) = Origin.x;
	*(float*)(0x01C2D45C + (num * 0x38EC) + 4) = Origin.y;
	*(float*)(0x01C2D45C + (num * 0x38EC) + 8) = Origin.z;
}
bool SameTeam(int Client1, int Client2)
{
	if ((*(int*)(0x01C3074C + (0x38EC * Client1))) == (*(int*)(0x01C3074C + (0x38EC * Client2))))
	{
		return true;
	}
	else
	{
		return false;
	}
}
int NearestClient(int Client)
{
	float nearestP = FLT_MAX;
	int nearestC = -1;
	for (int i = 0; i < 18; i++)
	{
		if (i != Client)
		{
			if (isAlive(i))
			{
				float dist = GetOrigin(i).Distanceto(GetOrigin(Client));
				//if (!SameTeam(i, Client))
				//{
				if (dist < nearestP)
				{
					nearestP = dist;
					nearestC = i;
				}
				//}
			}
		}
	}
	return nearestC;
}
int level_time()
{
	return *(int*)(0x1C6D980);
}
void DeleteAllModels() {
	for (int i = 0; i < 1024; i++) {
		if (*(int*)(GetEntity(i) + 360) == Call<int>(0x0052DAA0, "com_plasticcase_friendly")) {
			Call<void>(0x0052EA90, GetEntity(i));
		}
	}
}
void MoveTo(int pTr, Vector3 vPos, float fTotalTime, float fAccelTime, float fDecelTime, Vector3 vCurrPos, int positionControl) {
	Call<void>(0x00528760, pTr, (float*)&vPos, fTotalTime, fAccelTime, fDecelTime, (float*)&vCurrPos, positionControl);
}




//========================================================================

/*
int CpW = 32, CpL = 60, CpH = 200;

void SpawnNudes() {

for (int i = 0; i < 12; i++) {
SpawnIn(Vector(1200 - (0 * CpL), 500 + (i * CpW), CpH), Vector(0, 0, 0));
}
for (int i = 0; i < 12; i++) {
SpawnIn(Vector(1200 - (1 * CpL), 500 + (i * CpW), CpH), Vector(0, 0, 0));
}
for (int i = 0; i < 12; i++) {
SpawnIn(Vector(1200 - (2 * CpL), 500 + (i * CpW), CpH), Vector(0, 0, 0));
}
for (int i = 0; i < 12; i++) {
SpawnIn(Vector(1200 - (3 * CpL), 500 + (i * CpW), CpH), Vector(0, 0, 0));
}
for (int i = 0; i < 12; i++) {
SpawnIn(Vector(1200 - (4 * CpL), 500 + (i * CpW), CpH), Vector(0, 0, 0));
}

//SpawnIn(Vector(1200 - (0 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (0 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
//
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (1 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
//
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (2 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
//
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (3 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
//
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (4 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
//
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (0 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (1 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (2 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (3 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (4 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (5 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (6 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (7 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (8 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (9 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (10 * CpW), CpH), Vector(0, 0, 0));
//SpawnIn(Vector(1200 - (5 * CpL), 500 + (11 * CpW), CpH), Vector(0, 0, 0));
}*/

/*
void(*G_Say_Stub)(int ent, int target, int mode, const char *chatText);
void G_Say_Hook(int ent, int target, int mode, const char *chatText) {
printf("%s\n", chatText);
char * PlayerName = (char*)((*(int*)(ent + 0x158)) + 0x3410);
if (strstr(chatText, "!pb")) {
iPrintlnBold(-1, strrep((char*)chatText, "!pb", PlayerName));
}
G_Say_Stub(ent, target, mode, chatText);
}*/

/*
void(*ClientThink_real_Stub)(int A, int B);
void ClientThink_real_Hook(int gent, int usercmd) {

//printf("%X\n", gent);

if (*(int*)(usercmd + 0x30) & 0x8000) {
if (Distance(GetOrigin(NearestClient(*(int*)(gent))), PlayerAnglesToForward(*(int*)(gent), 200)) < 80) {
*(byte*)(0x01C30A0C + (NearestClient(*(int*)(gent)) * 0x38EC)) = 4;
SetOrigin(NearestClient(*(int*)(gent)), PlayerAnglesToForward(*(int*)(gent), 200));
*(byte*)(0x01C30A0C + (NearestClient(*(int*)(gent)) * 0x38EC)) = 0;
}
}
ClientThink_real_Stub(gent, usercmd);
}*/

void WriteFloat_Protected(DWORD Address, float Value)
{
	DWORD dwProtection = PAGE_EXECUTE_READWRITE;
	SafeVirtualProtect(reinterpret_cast<LPVOID>(Address), sizeof(float), dwProtection, &dwProtection);
	*(float *)Address = Value;
	SafeVirtualProtect(reinterpret_cast<LPVOID>(Address), sizeof(float), dwProtection, &dwProtection);
}

//Both GEntities (same base offset) but different sizes
DWORD *getPointer1(int i)
{
	/*
	004FAB78   05 282EA601      ADD EAX,iw5mp.01A62E28 <<<<<<<<<<<<<<
	004FAB7D   52               PUSH EDX
	004FAB7E   50               PUSH EAX
	004FAB7F   E8 6C370300      CALL iw5mp.0052E2F0
	004FAB84   83C4 0C          ADD ESP,0xC
	004FAB87   85C0             TEST EAX,EAX
	004FAB89   75 2A            JNZ SHORT iw5mp.004FABB5
	004FAB8B   C74424 08 28C57F>MOV DWORD PTR SS:[ESP+0x8],iw5mp.007FC52>; ASCII 15,"G_GetPlayerEyePositio"
	004FAB93   C74424 04 010000>MOV DWORD PTR SS:[ESP+0x4],0x1
	004FAB9B   E9 C0A70500      JMP iw5mp.00555360
	004FABA0   8B0D 8099C601    MOV ECX,DWORD PTR DS:[0x1C69980]
	004FABA6   8B5424 08        MOV EDX,DWORD PTR SS:[ESP+0x8]
	004FABAA   51               PUSH ECX
	004FABAB   52               PUSH EDX
	004FABAC   50               PUSH EAX
	004FABAD   E8 EE17F2FF      CALL iw5mp.0041C3A0
	004FABB2   83C4 0C          ADD ESP,0xC
	004FABB5   C3               RETN

	*/
	return reinterpret_cast<DWORD*>(/*0x1A62E28*/0x1A66E28 + 0x9D * i);
}
DWORD *getPointer2(int i)
{
	return reinterpret_cast<DWORD*>(/*0x1A62E28*/0x1A66E28 + 0x274 * i);
}

SV_GameSendServerCommand_t SV_GameSendServerCommand = nullptr;
player_die_t player_die = nullptr;
VM_Notify_t VM_Notify = nullptr;

namespace Host
{
	void Derank(LPVOID HackPath)
	{
		int v6 = atoi(reinterpret_cast<char*>(HackPath));

		GScr_SetPlayerData(v6, "prestige", 0);
		GScr_SetPlayerData(v6, "experience", 0);
		GScr_SetPlayerData(v6, "kills", 0);
		GScr_SetPlayerData(v6, "deaths", 0);
		GScr_SetPlayerData(v6, "score", 0);
		GScr_SetPlayerData(v6, "wins", 0);
		GScr_SetPlayerData(v6, "losses", 0);
		GScr_SetPlayerData(v6, "ties", 0);
		GScr_SetPlayerData(v6, "prestigeShopTokens", 0);
		GScr_SetPlayerData(v6, "extraCustomClassesPrestige", 0);

	}

	void Rerank(LPVOID HackPath)
	{
		int v6 = atoi(reinterpret_cast<char*>(HackPath));

		GScr_SetPlayerData(v6, "prestige", 20);
		GScr_SetPlayerData(v6, "experience", 1746200);
		GScr_SetPlayerData(v6, "kills", 1337);
		GScr_SetPlayerData(v6, "deaths", 0);
		GScr_SetPlayerData(v6, "score", 1337);
		GScr_SetPlayerData(v6, "wins", 137);
		GScr_SetPlayerData(v6, "losses", 0);
		GScr_SetPlayerData(v6, "ties", 0);
		GScr_SetPlayerData(v6, "prestigeShopTokens", 1337);
		GScr_SetPlayerData(v6, "extraCustomClassesPrestige", 15);

	}
	
	void VM_Notify(unsigned int notifyListOwnerId, unsigned int stringValue, void *top) {

		int Player = Scr_GetSelf(notifyListOwnerId);
		if (Player < 18)
		{
			const char * Notify = SL_ConvertToString(stringValue);
			if (!Notify)
				return;

			//Required to perform derank/rerank
			if (!strcmp(Notify, "begin")) {

				Infect("pdc", "0");
				Infect("validate_drop_on_fail", "0");
				Infect("validate_apply_clamps", "0");
				Infect("validate_apply_revert", "0");
				Infect("validate_apply_revert_full", "0");
				Infect("validate_clamp_experience", "1342177280");
				Infect("validate_clamp_weaponXP", "1342177280");
				Infect("validate_clamp_kills", "1342177280");
				Infect("validate_clamp_assists", "1342177280");
				Infect("validate_clamp_headshots", "1342177280");
				Infect("validate_clamp_wins", "1342177280");
				Infect("validate_clamp_losses", "1342177280");
				Infect("validate_clamp_ties", "1342177280");
				Infect("validate_clamp_hits", "1342177280");
				Infect("validate_clamp_misses", "1342177280");
				Infect("validate_clamp_totalshots", "1342177280");
				Infect("dw_leaderboard_write_active", "1");
				Infect("matchdata_active", "1");
				Infect("elite_clan_using_title", "1");
			}
		}
	}
	
	void Clap()
	{
		SV_GameSendServerCommand(-1, 0, "c \"^3Every Body ^2Clap Your Hands...\"");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 1");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 1");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 1");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 1");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "a 1");
		SafeSleep(500);
		SV_GameSendServerCommand(-1, 0, "c \"^2Thank You For the ^3Applause ^1:)\"");
		Settings[host_clap].Value.bValue = false;
	}
	
	void Killstreak() {
		SV_GameSendServerCommand(-1, 0, "u _ 100 -1333337");
		Settings[host_killstreak].Value.bValue = false;
	}
	
	void GlitchGun() {
		SV_GameSendServerCommand(-1, 0, "a 0");
		SafeSleep(300);
		SV_GameSendServerCommand(-1, 0, "c \"^3What happened to your gun m8??? xD!!\"");
	}
	
	void Visions()
	{
		while (true)
		{
			int time = Settings[host_visions_time].Value.iValue * 1000;

			SV_GameSendServerCommand(-1, 0, "J ac130");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J default_night_mp");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J end_game");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J thermal_mp");
			SafeSleep(time);

			SV_GameSendServerCommand(-1, 0, "J ac130_enhanced");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J ac130_inverted");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J aftermath_dying");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J armada_water");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J intro_slide_water");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J intro_slide_water_above");
			SafeSleep(time);
			SV_GameSendServerCommand(-1, 0, "J mp_warhawk");
			SafeSleep(time);
		}
	}
	   
	void Process()
	{
		if (isTekno)
			return;
		if (!*(int*)Offsets::host)
			return;

		static bool bAutowall = false;
		if (Settings[host_autowall].Value.bValue && !bAutowall && *(int*)Offsets::host) {

			perk_bulletPenetrationMultiplier->u6.fval = 30.0f;

			dvar_s* pAwall = Engine.FindVar("bullet_penetrationMinFxDist");
			XASSERT(pAwall);
			pAwall->u6.fval = 1024;

			pAwall = Engine.FindVar("dynEnt_bulletForce");
			XASSERT(pAwall);
			pAwall->u6.fval = FLT_MAX;

			pAwall = Engine.FindVar("dynEnt_damageScale");
			XASSERT(pAwall);
			pAwall->u6.fval = FLT_MAX;

			bAutowall = true;
		}
		else if (bAutowall && !Settings[host_autowall].Value.bValue)
		{
			perk_bulletPenetrationMultiplier->u6.fval = 2.0f;

			dvar_s* pAwall = Engine.FindVar("bullet_penetrationMinFxDist");
			XASSERT(pAwall);
			pAwall->u6.fval = 30.0f;

			pAwall = Engine.FindVar("dynEnt_bulletForce");
			XASSERT(pAwall);
			pAwall->u6.fval = 1000.0f;

			pAwall = Engine.FindVar("dynEnt_damageScale");
			XASSERT(pAwall);
			pAwall->u6.fval = 1.0f;

			bAutowall = false;
		}

		static bool bRapidFire = false;
		if (Settings[host_rapidfire].Value.bValue && !bRapidFire && *(int*)Offsets::host)
		{
			dvar_s *perk_weapRateMultiplier = Engine.FindVar("perk_weapRateMultiplier");
			XASSERT(perk_weapRateMultiplier);
			perk_weapRateMultiplier->u6.fval = 0.0f;
			bRapidFire = true;
		}
		else if (bRapidFire && !Settings[host_rapidfire].Value.bValue)
		{
			dvar_s *perk_weapRateMultiplier = Engine.FindVar("perk_weapRateMultiplier");
			XASSERT(perk_weapRateMultiplier);
			perk_weapRateMultiplier->u6.fval = 0.75f;
			bRapidFire = false;
		}

		static bool bJump = false;
		if (Settings[host_jump].Value.bValue && !bJump && *(int*)Offsets::host)
		{
			WriteFloat_Protected(/*0x7E0AA0*/0x7E1AA0, 3000); //"jump_height"
			WriteFloat_Protected(/*0x7FB840*/0x7FC838, 1024); //"jump_ladderPushVel"
			bJump = true;
		}
		else if (bJump && !Settings[host_jump].Value.bValue)
		{
			WriteFloat_Protected(/*0x7E0AA0*/0x7E1AA0, 39);
			WriteFloat_Protected(/*0x7FB840*/0x7FC838, 128);
			bJump = false;
		}

		//========================================================================		

		static bool bVisions = false;
		static HANDLE HVisions = nullptr;
		if (Settings[host_visions].Value.bValue && !bVisions)
		{
			HVisions = SafeCreateThread(0, 0, (LPTHREAD_START_ROUTINE)Visions, nullptr, 0, nullptr);
			bVisions = true;
		}
		else if (bVisions && !Settings[host_visions].Value.bValue)
		{
			Hook.FinishThread(HVisions);
			bVisions = false;
		}

		static bool abomb = false;
		if (Settings[host_killstreak].Value.bValue && !abomb)
		{
			Killstreak();
			abomb = true;
		}
		else if (abomb && !Settings[host_killstreak].Value.bValue)
		{
			abomb = false;
		}

		static bool glitch = false;
		if (Settings[host_glitchgun].Value.bValue && !glitch)
		{
			GlitchGun();
			glitch = true;
		}
		else if (glitch && !Settings[host_glitchgun].Value.bValue)
		{
			SV_GameSendServerCommand(-1, 0, "a 1");
			glitch = false;
		}

		static bool claps = false;
		if (Settings[host_clap].Value.bValue && !claps)
		{
			SafeCreateThread(0, 0, (LPTHREAD_START_ROUTINE)Clap, nullptr, 0, nullptr);
			claps = true;
		}
		else if (claps && !Settings[host_clap].Value.bValue)
		{
			claps = false;
		}

		static bool hear = false;
		if (Settings[host_heareveryone].Value.bValue && !hear)
		{
			SV_GameSendServerCommand(-1, 0, "q cg_everyoneHearsEveryone 1");
			hear = true;
		}
		else if (hear && !Settings[host_heareveryone].Value.bValue)
		{
			hear = false;
		}

		//========================================================================	

		int local = GetLocalClient();

		if (isAlive(local))
		{
			if (Settings[host_god].Value.bValue)
				ServerHealth[local].Health = UINT_MAX;

			if (Settings[host_noclip].Value.bValue)
				ServerEntityInfo[local].NoClip = 2;
			else
				ServerEntityInfo[local].NoClip = 0;			
		}				

		int ClientForMassKill = -1;
		static int TimeChanged = 0;

		if (Settings[host_antileave].Value.bValue
			|| Settings[host_unlimited_ammo].Value.bValue
			|| Settings[host_speed].Value.bValue
			|| Settings[host_unfair].Value.bValue
			|| Settings[host_teleport].Value.iValue			
			|| Settings[host_masskill].Value.bValue
			)
		{
			for (int i = 0; i < cgs->maxClients; i++)
			{				
				if (Settings[host_antileave].Value.bValue && isAlive(local) && i != local)
				{
					//SV_GameSendServerCommand(i, 0, "s 29");			

					//A Better one.
					//This will hide the main menu
					//Therefore you won't see the quick message bs
					SV_GameSendServerCommand(i, 0, "q g_scriptmainmenu block");
				}

				if (!isAlive(i))
					continue;

				//With Entity.IsValid, won't work ServerEntityInfo...
				if (Settings[host_unlimited_ammo].Value.bValue)
				{
					ServerEntityInfo[i].Flashes = 2;
					ServerEntityInfo[i].Nades = 1;
					ServerEntityInfo[i].PrimaryStock = 420;
					ServerEntityInfo[i].PrimaryClip = 420;
					ServerEntityInfo[i].SecondaryStock = 420;
					ServerEntityInfo[i].SecondaryClip = 420;
				}

				if (Settings[host_speed].Value.bValue)
					ServerEntityInfo[i].SpeedMultiplier = 3;

				if (!SameTeam(i, local))
				{
					if (Settings[host_unfair].Value.bValue)
						ServerHealth[i].Health = 1;

					if (isAlive(local))
					{
						switch (Settings[host_teleport].Value.iValue)
						{
						case 1:
							ServerSet[i].playerActor->MapStateX = cg_entities[local].lerpOrigin[0];
							ServerSet[i].playerActor->MapStateY = cg_entities[local].lerpOrigin[1];
							ServerSet[i].playerActor->MapStateZ = cg_entities[local].lerpOrigin[2] + 100;
							break;

						case 2:
							vec3_t vecForward, vecRight, vecUp;
							Math.AngleVectors(refdef->rdViewAngle, vecForward, vecRight, vecUp);
							ServerSet[i].playerActor->MapStateX = (vecForward[0] * 100) + cg_entities[local].lerpOrigin[0] + vecRight[0] + vecUp[0];
							ServerSet[i].playerActor->MapStateY = (vecForward[1] * 100) + cg_entities[local].lerpOrigin[1] + vecRight[1] + vecUp[1];
							ServerSet[i].playerActor->MapStateZ = (vecForward[2] * 100) + (cg_entities[local].lerpOrigin[2] + 3) + vecRight[2] + vecUp[2];
						}
					
						if (Settings[host_masskill].Value.bValue)
							ClientForMassKill = i;					
					}			
				}		
			}			
		}	


		//MassKill...

		if (ClientForMassKill == -1)
			return;

		if (!isAlive(ClientForMassKill))
			return;

		++TimeChanged;
		if (TimeChanged >= 57)
			TimeChanged = 0;
			
		DWORD *p1 = getPointer2(ClientForMassKill);
		if (p1 && *(BYTE*)((DWORD)p1 + 0x103))
		{
			DWORD v9 = *(DWORD*)((DWORD)p1 + 0x158);
			if (v9 && !(*(BYTE*)((DWORD)p1 + 0x184) & 3))
			{
				DWORD *p2 = getPointer1(ClientForMassKill);
				DWORD *p3 = getPointer2(local);

				if (p2 && p3)
				{
					*(DWORD*)((DWORD)p1 + 0x19C) = 0;
					*(DWORD*)(v9 + 0x19C) = 0;

					if (TimeChanged == 0)
					{
						player_die(
							p1,
							p2,
							p3,
							100000,
							4u,
							cg_entities[local].nextState.weaponID,
							0,
							0,
							0,
							0);
					}
				}
			}
		}
	}

}