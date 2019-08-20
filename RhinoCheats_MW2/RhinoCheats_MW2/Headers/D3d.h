#pragma once

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "../Headers/ImGUI/imgui.h"
#include "../Headers/ImGUI/imgui_impl_dx9.h"
#include "../Headers/ImGUI/imgui_impl_win32.h"
#include "../Headers/ImGUI/imgui_internal.h"
#include "../Headers/ImGUI/imgui_stdlib.h"
#include "../Headers/ImGUI/addons/imgui_user.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace D3D
{
	class cSprite
	{	
		LPDIRECT3DTEXTURE9 pTex;
		LPD3DXSPRITE lpSprite;
		D3DCOLOR color;
		bool result;

	public:

		cSprite();
		cSprite(float x, float y);
		
		~cSprite();

		//Variable
		D3DXVECTOR3 pPos;

		//Functions	
		bool Init(IDirect3DDevice9*, std::string, float, float);
		bool IsInit();
		void Draw();
		void Release();	
	};

	//Main background texture
	extern ImVec4 colRainBow;	

	//Players textures
	extern LPDIRECT3DTEXTURE9 texAxis;
	extern LPDIRECT3DTEXTURE9 texAxisVisible;
	extern LPDIRECT3DTEXTURE9 texAllies;
	extern LPDIRECT3DTEXTURE9 texAlliesVisible;

	//Weapon texture
	extern LPDIRECT3DTEXTURE9 texWeapon;
	extern LPDIRECT3DTEXTURE9 texWeaponCustom;
	extern LPDIRECT3DTEXTURE9 texWeaponVisible;

	//Map texture
	extern LPDIRECT3DTEXTURE9 texMap;

	//Misc
	extern char texturesFolder[MAX_PATH];
	extern bool g_bWasInitialized;	
	extern bool g_bMainWindowOpen;	
	
	void Restore_WndProc();		
	
	//========================================================================
	//Hooks

#define INDEX_DIP 82
#define INDEX_ENDSCENE 42
#define INDEX_RESET 16

	typedef HRESULT(__stdcall* DrawIndexedPrimitive_t)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
	extern DrawIndexedPrimitive_t OrigDrawIndexedPrimitive;
	HRESULT __stdcall DrawIndexedPrimitive_Stub(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);

	typedef HRESULT(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
	extern EndScene_t OrigEndScene;
	HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice);

	typedef HRESULT(__stdcall* Reset_t)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern Reset_t OrigReset;
	HRESULT __stdcall ResetHook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	

	//========================================================================

	bool InitAndHookD3D();
}








