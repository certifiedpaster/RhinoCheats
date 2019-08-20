//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

#define MAX_VIRTUALKEYS 0x100

#define DEFAULT_CFG "\\RhinoCheats.cfg"
#define DEFAULT_INI "\\RhinoCheats.ini"
#define DEFAULT_LOG "\\RhinoCheats.log"
#define DEFAULT_XML "\\RhinoCheats.xml"

//=====================================================================================

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=====================================================================================

namespace RhinoCheats
{
	class cMainGUI
	{
	public:

		cMainGUI() : _thunkWindowProcess(&cMainGUI::WindowProcess, this) {}

		bool bInitialized = false;

		ImFont* Eurostile_Bold, * Eurostile_Extended, * Eurostile_Regular;
		float flEurostile_Bold, flEurostile_Extended, flEurostile_Regular;

		struct sMenu
		{
			int iCommandTMR = clock();

			char szProfile[1024] = { NULL }, szMessage[1024] = { NULL };
			bool bShowWindow = true, bShowConsole = false, bStyleChanged = false, bWriteLog = false;

			std::string szIniFileName, szLogFileName;

			struct sPlayerList
			{
				bool bShowWindow = false;
				sNetAdr NetAdr[MAX_CLIENTS];
			} PlayerList;

			struct sHostMenu
			{
				char szKickReason[64] = { NULL };
				bool bShowWindow = false;

				struct sPlayerMod
				{
					bool bGodMode = false, bInfiniteAmmo = false, bExplosiveBullets = false, bFreezePlayer = false;
					Vector3 szPosition{ NULL };
				} PlayerMod[MAX_CLIENTS];

				std::vector<std::pair<int, LPSTR>> vPlayers;
				std::vector<LPCSTR> szTeam = { "Free", "Federation", "Ghosts", "Spectator" };
				Vector3 vTeleport = { 0.0f };

				int iPlayer = 0, iWeaponID = 0;

				void GetPlayerSelection()
				{
					for (auto& Player : vPlayers)
					{
						bool bSelected = (iPlayer == Player.first);

						ImGui::PushID(Player.first);

						if (ImGui::Selectable(Player.second, bSelected))
							iPlayer = Player.first;

						if (bSelected)
							ImGui::SetItemDefaultFocus();

						ImGui::PopID();
					}
				}
			} HostMenu;

			bool bSaveButton, bLoadButton;
			ImGuiFs::Dialog SaveDialog, LoadDialog;
		} Menu;

		struct sVirtualKeys
		{
			bool bKey, bDown, bUp;
		} VirtualKeys[MAX_VIRTUALKEYS];

		HWND hWindow;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pDeviceContext;

		void InitInterface();
		void SetMenuColor(int index);
		void SetMenuCursor(int index);
		void SetMenuFont(int index);
		void RefreshInterface(int color, int cursor, int font);
		bool GetKeyPress(int vkey, bool immediate);

		void WINAPI Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
		LRESULT CALLBACK WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		typedef LRESULT(CALLBACK* tWindowProcess)(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		tWindowProcess oWindowProcess;

		cWin32Thunk<tWindowProcess, cMainGUI> _thunkWindowProcess;
	} extern _mainGui;
}

//=====================================================================================
