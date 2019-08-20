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

namespace RhinoCheats {
	class cMainGUI {
	public:

		cMainGUI() : _thunkWindowProcess(&cMainGUI::WindowProcess, this) {}

		bool bInitialized = false;

		struct menu_s {
			char szCommand[1024] = { NULL };
			bool bShowWindow = true, bStyleChanged = false, bWriteLog = false;

			std::string szIniFileName, szLogFileName;

			bool bSaveButton, bLoadButton;
			ImGuiFs::Dialog SaveDialog, LoadDialog;
		} Menu;

		struct virtualKeys_s {
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
		LRESULT WindowProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		typedef LRESULT(CALLBACK* tWindowProcess)(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		tWindowProcess oWindowProcess;

		cWin32Thunk<tWindowProcess, cMainGUI> _thunkWindowProcess;
	} extern _mainGui;
}

//=====================================================================================
