//=====================================================================================

#include "StdAfx.hpp"

using namespace RhinoCheats;

//=====================================================================================

#define Hook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourAttach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())
#define UnHook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourDetach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
typedef HRESULT(WINAPI* tPresent)(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
tPresent oPresent;

//=====================================================================================

HRESULT WINAPI hPresent(
	_In_ IDXGISwapChain* swapchain, 
	_In_ UINT syncinterval, 
	_In_ UINT flags
) {
	_mainGui.Present(swapchain, syncinterval, flags);

	return oPresent(swapchain, syncinterval, flags);
}

//=====================================================================================

void Initialize() {
	_hooks.PatchAntiCheat();

	SetUnhandledExceptionFilter(NULL);
	_hooks.pVectoredExceptionHandler = AddVectoredExceptionHandler(TRUE, _hooks._thunkVectoredExceptionHandler.GetThunk());

	_hooks.dwDrawBigFPS = *(DWORD_PTR*)OFF_DRAWBIGFPS_DVAR;
	*(DWORD_PTR*)OFF_DRAWBIGFPS_DVAR = cHooks::VEH_INDEX_DRAWBIGFPS;

	_hooks.dwMouseAccel = *(DWORD_PTR*)OFF_MOUSEACCEL_DVAR;
	*(DWORD_PTR*)OFF_MOUSEACCEL_DVAR = cHooks::VEH_INDEX_MOUSEACCEL;

	_hooks.dwNoDelta = *(DWORD_PTR*)OFF_NODELTA_DVAR;
	*(DWORD_PTR*)OFF_NODELTA_DVAR = cHooks::VEH_INDEX_NODELTA;

	HMODULE hModule = GetModuleHandle("GameOverlayRenderer.dll");

	if (hModule) oPresent = *(tPresent*)((DWORD_PTR)hModule + OFF_GAMEOVERLAYPRESENT);
	else oPresent = *(tPresent*)(**(DWORD_PTR**)OFF_SWAPCHAIN + (sizeof(DWORD_PTR) * 0x8));

	Hook(oPresent, hPresent);
}

//=====================================================================================

void Deallocate() {
	*(DWORD_PTR*)OFF_DRAWBIGFPS_DVAR = _hooks.dwDrawBigFPS;
	*(DWORD_PTR*)OFF_MOUSEACCEL_DVAR = _hooks.dwMouseAccel;
	*(DWORD_PTR*)OFF_NODELTA_DVAR = _hooks.dwNoDelta;

	RemoveVectoredExceptionHandler(_hooks.pVectoredExceptionHandler);

	UnHook(oPresent, hPresent);

	_mainGui.pDevice->Release();
	_mainGui.pDeviceContext->Release();

	SetWindowLongPtr(_mainGui.hWindow, GWLP_WNDPROC, (LONG_PTR)_mainGui.oWindowProcess);

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

//=====================================================================================

BOOL APIENTRY DllMain(
	_In_ HINSTANCE hinstDLL, 
	_In_ DWORD fdwReason, 
	_In_ LPVOID lpvReserved
) {
	DisableThreadLibraryCalls(hinstDLL);

	switch (fdwReason) {
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