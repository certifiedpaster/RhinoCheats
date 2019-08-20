//=====================================================================================

#include "StdAfx.hpp"

using namespace RhinoCheats;

//=====================================================================================

#define HOOKCALL __fastcall

#define Hook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourAttach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())
#define UnHook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourDetach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
typedef HRESULT(WINAPI* tPresent)(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
tPresent oPresent;

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
typedef void(HOOKCALL* tBulletFirePenetrate)(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
tBulletFirePenetrate oBulletFirePenetrate = (tBulletFirePenetrate)OFF_BULLETFIREPENETRATE;

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon);
typedef void(HOOKCALL* tObituary)(int localnum, sEntityState* entitystate, int weapon);
tObituary oObituary = (tObituary)OFF_OBITUARY;

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
typedef void(HOOKCALL* tAddCmdDrawText)(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
tAddCmdDrawText oAddCmdDrawText = (tAddCmdDrawText)OFF_ADDCMDDRAWTEXT;

void HOOKCALL hClientFrame(sGEntity* entity);
typedef void(HOOKCALL* tClientFrame)(sGEntity* entity);
tClientFrame oClientFrame = (tClientFrame)OFF_CLIENTFRAME;

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* swapchain, _In_ UINT syncinterval, _In_ UINT flags)
{
	_mainGui.Present(swapchain, syncinterval, flags);

	return oPresent(swapchain, syncinterval, flags);
}

//=====================================================================================

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime)
{
	_hooks.BulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);

	oBulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);
}

//=====================================================================================

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon)
{
	_hooks.Obituary(localnum, entitystate, weapon);

	oObituary(localnum, entitystate, weapon);
}

//=====================================================================================

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags)
{
	_hooks.AddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);

	oAddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);
}

//=====================================================================================

void HOOKCALL hClientFrame(sGEntity* entity)
{
	_hooks.ClientFrame(entity);

	oClientFrame(entity);
}

//=====================================================================================

void Initialize()
{
	HMODULE hModule = GetModuleHandle("GameOverlayRenderer64.dll");

	if (hModule) oPresent = *(tPresent*)((POINTER)hModule + OFF_STEAMOVERLAY);
	else oPresent = *(tPresent*)(**(POINTER**)OFF_SWAPCHAIN + (sizeof(POINTER) * 0x8));

	VEH::AddHook(OFF_HUDSAYPOSITION_DVAR, OFF_HUDSAYPOSITION_EXCEPTION, _hooks._thunkDraw2D.GetThunk());
	VEH::AddHook(OFF_PACKETDUPLICATION_DVAR, OFF_PACKETDUPLICATION_EXCEPTION, _hooks._thunkWritePacket.GetThunk());

	VEH::ForceExceptions();

	Hook(oPresent, hPresent);
	Hook(oBulletFirePenetrate, hBulletFirePenetrate);
	Hook(oObituary, hObituary);
	Hook(oAddCmdDrawText, hAddCmdDrawText);
	Hook(oClientFrame, hClientFrame);
}

//=====================================================================================

void Deallocate()
{
	VEH::RemoveHooks();

	UnHook(oPresent, hPresent);
	UnHook(oBulletFirePenetrate, hBulletFirePenetrate);
	UnHook(oObituary, hObituary);
	UnHook(oAddCmdDrawText, hAddCmdDrawText);
	UnHook(oClientFrame, hClientFrame);

	_mainGui.pDevice->Release();
	_mainGui.pDeviceContext->Release();

	SetWindowLongPtr(_mainGui.hWindow, GWLP_WNDPROC, (LONG_PTR)_mainGui.oWindowProcess);

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

//=====================================================================================

BOOL APIENTRY DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	DisableThreadLibraryCalls(hinstDLL);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Initialize();
		return TRUE;

	case DLL_PROCESS_DETACH:
		Deallocate();
		return TRUE;
	}

	return FALSE;
}

//=====================================================================================