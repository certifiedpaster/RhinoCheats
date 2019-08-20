//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cMainGUI _mainGui;

	void cMainGUI::InitInterface()
	{
		hWindow = *(HWND*)OFF_WINDOWHANDLE;

		if (!hWindow || !pDevice || !pDeviceContext)
			return;

		oWindowProcess = (tWindowProcess)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)_thunkWindowProcess.GetThunk());

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		Menu.szIniFileName = acut::GetExeDirectory() + DEFAULT_INI;
		Menu.szLogFileName = acut::GetExeDirectory() + DEFAULT_LOG;

		ImGui::GetIO().IniFilename = Menu.szIniFileName.c_str();
		ImGui::GetIO().LogFilename = Menu.szLogFileName.c_str();

		_profiler.gMenuColor.Custom.iValue = GetPrivateProfileInt("MenuStyle", "COLOR", cProfiler::MENU_COLOR_NEUTRAL, (acut::GetExeDirectory() + DEFAULT_CFG).c_str());
		_profiler.gMenuCursor.Custom.iValue = GetPrivateProfileInt("MenuStyle", "CURSOR", cProfiler::MENU_CURSOR_BLACK, (acut::GetExeDirectory() + DEFAULT_CFG).c_str());
		_profiler.gMenuFont.Custom.iValue = GetPrivateProfileInt("MenuStyle", "FONT", cProfiler::MENU_FONT_LIGHT, (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

		RefreshInterface(_profiler.gMenuColor.Custom.iValue, _profiler.gMenuCursor.Custom.iValue, _profiler.gMenuFont.Custom.iValue);

		bInitialized = true;
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuColor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_COLOR_NEUTRAL:
			ImGui::StyleColorsNeutral();
			break;

		case cProfiler::MENU_COLOR_RED:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_ORANGE:
			ImGui::StyleColorsOrange();
			break;

		case cProfiler::MENU_COLOR_YELLOW:
			ImGui::StyleColorsYellow();
			break;

		case cProfiler::MENU_COLOR_GREEN:
			ImGui::StyleColorsGreen();
			break;

		case cProfiler::MENU_COLOR_BLUE:
			ImGui::StyleColorsBlue();
			break;

		case cProfiler::MENU_COLOR_PURPLE:
			ImGui::StyleColorsPurple();
			break;

		default:
			ImGui::StyleColorsNeutral();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuCursor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_CURSOR_BLACK:
			ImGui::StyleCursorsBlack();
			break;

		case cProfiler::MENU_CURSOR_WHITE:
			ImGui::StyleCursorsWhite();
			break;

		default:
			ImGui::StyleCursorsBlack();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuFont(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_FONT_LIGHT:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;

		case cProfiler::MENU_FONT_MEDIUM:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_MEDIUM];
			break;

		case cProfiler::MENU_FONT_BOLD:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_BOLD];
			break;

		default:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::RefreshInterface(int color, int cursor, int font)
	{
		SetMenuColor(color);

		ImGui::GetIO().Fonts->Clear();

		SetMenuCursor(cursor);

		ImGui::GetIO().Fonts->AddFontLight();
		ImGui::GetIO().Fonts->AddFontMedium();
		ImGui::GetIO().Fonts->AddFontBold();

		Eurostile_Bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_bold_compressed_data_base85, flEurostile_Bold = Window->iHeight / 80.0f);
		Eurostile_Extended = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_extended_compressed_data_base85, flEurostile_Extended = Window->iHeight / 60.0f);
		Eurostile_Regular = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_regular_compressed_data_base85, flEurostile_Regular = Window->iHeight / 80.0f);

		ImGui_ImplDX11_CreateDeviceObjects();

		SetMenuFont(font);
	}
	/*
	//=====================================================================================
	*/
	bool cMainGUI::GetKeyPress(int vkey, bool immediate)
	{
		if (VirtualKeys[vkey].bKey)
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = true;
		}

		else if (!VirtualKeys[vkey].bKey && VirtualKeys[vkey].bDown)
		{
			VirtualKeys[vkey].bUp = true;
			VirtualKeys[vkey].bDown = false;
		}

		else
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = false;
		}

		if (immediate)
			return VirtualKeys[vkey].bDown;

		else
			return VirtualKeys[vkey].bUp;
	}
	/*
	//=====================================================================================
	*/
	LRESULT CALLBACK cMainGUI::WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			VirtualKeys[VK_LBUTTON].bKey = true;
			break;

		case WM_LBUTTONUP:
			VirtualKeys[VK_LBUTTON].bKey = false;
			break;

		case WM_RBUTTONDOWN:
			VirtualKeys[VK_RBUTTON].bKey = true;
			break;

		case WM_RBUTTONUP:
			VirtualKeys[VK_RBUTTON].bKey = false;
			break;

		case WM_KEYDOWN:
			VirtualKeys[wParam].bKey = true;
			break;

		case WM_KEYUP:
			VirtualKeys[wParam].bKey = false;
			break;
		}

		if (GetKeyPress(VK_INSERT, false))
			Menu.bShowWindow = !Menu.bShowWindow;

		if (GetKeyPress(VK_DELETE, false))
			if (!LocalClientIsInGame())
				std::thread(&cHost::StartMatch, &_host).detach();

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		if (GetKeyPress(VK_PRIOR, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(PlayerState[CG->iClientNum].vOrigin, Menu.HostMenu.vTeleport);

		if (GetKeyPress(VK_NEXT, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(Menu.HostMenu.vTeleport, PlayerState[CG->iClientNum].vOrigin);

		*(bool*)OFF_MOUSEINPUT = !Menu.bShowWindow;
		FindVariable("cl_bypassMouseInput")->Current.iValue = Menu.bShowWindow;

		if (bInitialized && Menu.bShowWindow && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return TRUE;

		return CallWindowProc(oWindowProcess, hWnd, uMsg, wParam, lParam);
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags)
	{
		if (!bInitialized)
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pDeviceContext);
			InitInterface();
		}

		else
		{
			ImGui::GetIO().MouseDrawCursor = Menu.bShowWindow;

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));

			ImGui::Begin("INVISIBLE", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
			ImGui::GetWindowDrawList()->PushClipRectFullScreen();

			if (LocalClientIsInGame() && CG->iIsInGame & 0x4000)
			{
				_drawing.ESP();
				_drawing.Crosshair();
			}

			std::string szWatermark(VariadicText("RHINO CHEATS for Call of Duty: Ghosts - Multiplayer | Ping: %i ms", *(int*)OFF_PING));
			std::string szConnection(*(int*)OFF_ISCURRENTHOST ? "Hosting" : "Not Hosting");
			std::string szFramesPerSecond(VariadicText("%i", (int)ImGui::GetIO().Framerate));

			ImVec2 vWatermark(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szWatermark.c_str()));
			ImVec2 vConnection(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szConnection.c_str()));
			ImVec2 vFramesPerSecond(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szFramesPerSecond.c_str()));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y + 1.0f, flEurostile_Extended - vWatermark.y + 1.0f),
				0xFF000000, szWatermark.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y, flEurostile_Extended - vWatermark.y),
				0xFFFFFFFF, szWatermark.c_str());

			if (LocalClientIsInGame())
			{
				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f + 1.0f, flEurostile_Extended - vConnection.y + 1.0f),
					0xFF000000, szConnection.c_str());

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f, flEurostile_Extended - vConnection.y),
					*(int*)OFF_ISCURRENTHOST ? 0xFF00FF00 : 0xFF0000FF, szConnection.c_str());
			}

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y + 1.0f, flEurostile_Extended - vFramesPerSecond.y + 1.0f),
				0xFF000000, szFramesPerSecond.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y, flEurostile_Extended - vFramesPerSecond.y),
				0xFF00FFFF, szFramesPerSecond.c_str());
			
			ImGui::End();
			ImGui::PopStyleColor(2);

			if (Menu.bShowWindow)
			{
				if (Menu.bWriteLog)
				{
					ImGui::LogToFile();
					Menu.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(490.0f, 324.0f));
				ImGui::Begin("RHINO CHEATS", &Menu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
 
				if (ImGui::TabLabels(_profiler.gMenuTabs.MaxValue.iMax, _profiler.gMenuTabs.szItems, _profiler.gMenuTabs.Custom.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
				{
					Menu.bWriteLog = true;
				}

				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 204.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				switch (_profiler.gMenuTabs.Custom.iValue)
				{
				case cProfiler::MENU_TAB_AIMBOT:
				{
					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_OFF], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_OFF))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(148.0f);

					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_MANUAL], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_MANUAL))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_AUTOMATIC], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_AUTOMATIC))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutozoom.szLabel, &_profiler.gAutozoom.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAutofire.szLabel, &_profiler.gAutofire.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutowall.szLabel, &_profiler.gAutowall.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gSilentAim.szLabel, &_profiler.gSilentAim.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gTargetAgents.szLabel, &_profiler.gTargetAgents.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gTargetMissiles.szLabel, &_profiler.gTargetMissiles.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gBonescan.szLabel, &_profiler.gBonescan.Custom.iValue, _profiler.gBonescan.szItems, _profiler.gBonescan.MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gAntiAim.szLabel, &_profiler.gAntiAim.Custom.iValue, _profiler.gAntiAim.szItems, _profiler.gAntiAim.MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_WALLHACK:
				{
					if (ImGui::Combo(_profiler.gPlayerBoxes.szLabel, &_profiler.gPlayerBoxes.Custom.iValue, _profiler.gPlayerBoxes.szItems, _profiler.gPlayerBoxes.MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBones.szLabel, &_profiler.gPlayerBones.Custom.iValue, _profiler.gPlayerBones.szItems, _profiler.gPlayerBones.MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerSnaplines.szLabel, &_profiler.gPlayerSnaplines.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerInfo.szLabel, &_profiler.gPlayerInfo.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerWeapons.szLabel, &_profiler.gPlayerWeapons.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAgents.szLabel, &_profiler.gAgents.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gMissiles.szLabel, &_profiler.gMissiles.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gItems.szLabel, &_profiler.gItems.Custom.bValue))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_MISCELLANEOUS:
				{
					if (ImGui::Checkbox(_profiler.gNoRecoil.szLabel, &_profiler.gNoRecoil.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gNoSpread.szLabel, &_profiler.gNoSpread.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gKillspam.szLabel, &_profiler.gKillspam.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gNameStealer.szLabel, &_profiler.gNameStealer.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gCrosshair.szLabel, &_profiler.gCrosshair.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAFKMode.szLabel, &_profiler.gAFKMode.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gRadar.szLabel, &_profiler.gRadar.Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Console", &Menu.bShowConsole))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Player List", &Menu.PlayerList.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Host Menu", &Menu.HostMenu.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gRiotshield.szLabel, &_profiler.gRiotshield.Custom.iValue, _profiler.gRiotshield.szItems, _profiler.gRiotshield.MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_TWEAKS:
				{
					if (ImGui::SliderInt(_profiler.gAimbone.szLabel, &_profiler.gAimbone.Custom.iValue, _profiler.gAimbone.MinValue.iMin, _profiler.gAimbone.MaxValue.iMax, _profiler.gAimbone.szItems[_profiler.gAimbone.Custom.iValue]))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gAimAngle.szLabel, &_profiler.gAimAngle.Custom.flValue, _profiler.gAimAngle.MinValue.flMin, _profiler.gAimAngle.MaxValue.flMax, "%.0f degrees"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAimStrength.szLabel, &_profiler.gAimStrength.Custom.iValue, _profiler.gAimStrength.MinValue.iMin, _profiler.gAimStrength.MaxValue.iMax, "%d%%"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoaimTime.szLabel, &_profiler.gAutoaimTime.Custom.iValue, _profiler.gAutoaimTime.MinValue.iMin, _profiler.gAutoaimTime.MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoaimDelay.szLabel, &_profiler.gAutoaimDelay.Custom.iValue, _profiler.gAutoaimDelay.MinValue.iMin, _profiler.gAutoaimDelay.MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutozoomDelay.szLabel, &_profiler.gAutozoomDelay.Custom.iValue, _profiler.gAutozoomDelay.MinValue.iMin, _profiler.gAutozoomDelay.MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutofireDelay.szLabel, &_profiler.gAutofireDelay.Custom.iValue, _profiler.gAutofireDelay.MinValue.iMin, _profiler.gAutofireDelay.MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat("Field of View", &FindVariable("cg_fov")->Current.flValue, 65.0f, 120.0f, "%.0f fov"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt("Frames per Second", &FindVariable("com_maxfps")->Current.iValue, 0, 300, "%d fps"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gAimbone.Custom.iValue = _profiler.gAimbone.Default.iValue;
						_profiler.gAimAngle.Custom.flValue = _profiler.gAimAngle.Default.flValue;

						_profiler.gAimStrength.Custom.iValue = _profiler.gAimStrength.Default.iValue;
						_profiler.gAutoaimTime.Custom.iValue = _profiler.gAutoaimTime.Default.iValue;
						_profiler.gAutoaimDelay.Custom.iValue = _profiler.gAutoaimDelay.Default.iValue;
						_profiler.gAutozoomDelay.Custom.iValue = _profiler.gAutozoomDelay.Default.iValue;
						_profiler.gAutofireDelay.Custom.iValue = _profiler.gAutofireDelay.Default.iValue;

						FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
						FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_STYLES:
				{
					if (ImGui::ColorEdit3(_profiler.gColorAxisVisible.szLabel, _profiler.gColorAxisVisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAxisInvisible.szLabel, _profiler.gColorAxisInvisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAlliesVisible.szLabel, _profiler.gColorAlliesVisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAlliesInvisible.szLabel, _profiler.gColorAlliesInvisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorEntityVisible.szLabel, _profiler.gColorEntityVisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorEntityInvisible.szLabel, _profiler.gColorEntityInvisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorItemVisible.szLabel, _profiler.gColorItemVisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorItemInvisible.szLabel, _profiler.gColorItemInvisible.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorCrosshair.szLabel, _profiler.gColorCrosshair.Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gMenuColor.szLabel, &_profiler.gMenuColor.Custom.iValue, _profiler.gMenuColor.szItems, _profiler.gMenuColor.MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "COLOR", std::to_string(_profiler.gMenuColor.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuCursor.szLabel, &_profiler.gMenuCursor.Custom.iValue, _profiler.gMenuCursor.szItems, _profiler.gMenuCursor.MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "CURSOR", std::to_string(_profiler.gMenuCursor.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuFont.szLabel, &_profiler.gMenuFont.Custom.iValue, _profiler.gMenuFont.szItems, _profiler.gMenuFont.MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "FONT", std::to_string(_profiler.gMenuFont.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gColorAxisVisible.Custom.cValue = _profiler.gColorAxisVisible.Default.cValue;
						_profiler.gColorAxisInvisible.Custom.cValue = _profiler.gColorAxisInvisible.Default.cValue;
						_profiler.gColorAlliesVisible.Custom.cValue = _profiler.gColorAlliesVisible.Default.cValue;
						_profiler.gColorAlliesInvisible.Custom.cValue = _profiler.gColorAlliesInvisible.Default.cValue;
						_profiler.gColorEntityVisible.Custom.cValue = _profiler.gColorEntityVisible.Default.cValue;
						_profiler.gColorEntityInvisible.Custom.cValue = _profiler.gColorEntityInvisible.Default.cValue;
						_profiler.gColorItemVisible.Custom.cValue = _profiler.gColorItemVisible.Default.cValue;
						_profiler.gColorItemInvisible.Custom.cValue = _profiler.gColorItemInvisible.Default.cValue;
						_profiler.gColorCrosshair.Custom.cValue = _profiler.gColorCrosshair.Default.cValue;
						_profiler.gColorShadow.Custom.cValue = _profiler.gColorShadow.Default.cValue;

						Menu.bWriteLog = true;
					}
				} break;
				}

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::EndChild();

				if (ImGui::Button("Set as Default", ImVec2(156.0f, 30.0f)))
				{
					_profiler.SaveProfile("");
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);

				if ((Menu.bSaveButton = ImGui::Button("Save Profile", ImVec2(156.0f, 30.0f))))
				{
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);
				LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(Menu.bSaveButton, Menu.SaveDialog.getLastDirectory(), NULL, ".xml", "Save Profile");

				if (strlen(szSavePath))
				{
					_profiler.SaveProfile(szSavePath);
					strcpy_s(Menu.szProfile, szSavePath);
				}

				if ((Menu.bLoadButton = ImGui::Button("Load Profile", ImVec2(156.0f, 30.0f))))
				{
					Menu.bWriteLog = true;
				} LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(Menu.bLoadButton, Menu.LoadDialog.getLastDirectory(), ".xml", "Load Profile");

				if (strlen(szLoadPath))
				{
					_profiler.LoadProfile(szLoadPath);
					strcpy_s(Menu.szProfile, szLoadPath);
				}

				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
				ImGui::InputText("", Menu.szProfile, sizeof(Menu.szProfile), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();
				ImGui::End();

				if (Menu.bShowConsole)
				{
					_console.Init();
					_console.Draw("CONSOLE", &Menu.bShowConsole);
				}
			}

			if (clock() - Menu.iCommandTMR > 100)
			{
				if (_profiler.gChatspam.Custom.bValue)
					Cbuf_AddText(VariadicText("say \"%s\"\n", Menu.szMessage));

				Menu.iCommandTMR = clock();
			}

			if (_profiler.gRadar.Custom.bValue && LocalClientIsInGame() && CG->iIsInGame & 0x4000)
			{
				ImGui::SetNextWindowSize(ImVec2(_drawing.Radar.flRadarSize, _drawing.Radar.flRadarSize + ImGui::GetFrameHeight()));
				ImGui::Begin("RADAR", &_profiler.gRadar.Custom.bValue, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::GetWindowDrawList()->PushClipRectFullScreen();

				_drawing.Radar.vRadarPosition[0] = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2.0f;
				_drawing.Radar.vRadarPosition[1] = (ImGui::GetWindowPos().y + ImGui::GetFrameHeight()) + (ImGui::GetWindowSize().y - ImGui::GetFrameHeight()) / 2.0f;

				ImGui::GetWindowDrawList()->AddLine(ImVec2(_drawing.Radar.vRadarPosition[0] - _drawing.Radar.flRadarSize / 2.0f + 1.0f, _drawing.Radar.vRadarPosition[1]),
					ImVec2(_drawing.Radar.vRadarPosition[0] + _drawing.Radar.flRadarSize / 2.0f - 1.0f, _drawing.Radar.vRadarPosition[1]), ImGui::GetColorU32(ImVec4(0.50f, 0.50f, 0.50f, 0.50f)));

				ImGui::GetWindowDrawList()->AddLine(ImVec2(_drawing.Radar.vRadarPosition[0], _drawing.Radar.vRadarPosition[1] - _drawing.Radar.flRadarSize / 2.0f),
					ImVec2(_drawing.Radar.vRadarPosition[0], _drawing.Radar.vRadarPosition[1] + _drawing.Radar.flRadarSize / 2.0f - 1.0f), ImGui::GetColorU32(ImVec4(0.50f, 0.50f, 0.50f, 0.50f)));

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(_drawing.Radar.vRadarPosition[0] - 2.0f, _drawing.Radar.vRadarPosition[1] - 2.0f),
					ImVec2(_drawing.Radar.vRadarPosition[0] + 3.0f, _drawing.Radar.vRadarPosition[1] + 3.0f), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				{
					if (_targetList.EntityList[i].bIsValid)
					{
						ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(_drawing.Radar.vBlipPosition[i][0], _drawing.Radar.vBlipPosition[i][1]), _drawing.Radar.flBlipSize / 2.0f,
							ImGui::GetColorU32(ImVec4(_targetList.EntityList[i].cColor[0], _targetList.EntityList[i].cColor[1], _targetList.EntityList[i].cColor[2], _targetList.EntityList[i].cColor[3] / 4.0f)));

						ImGui::GetWindowDrawList()->AddCircle(ImVec2(_drawing.Radar.vBlipPosition[i][0], _drawing.Radar.vBlipPosition[i][1]), _drawing.Radar.flBlipSize / 2.0f,
							ImGui::GetColorU32(ImVec4(_targetList.EntityList[i].cColor[0], _targetList.EntityList[i].cColor[1], _targetList.EntityList[i].cColor[2], _targetList.EntityList[i].cColor[3])));
					}
				}

				ImGui::End();
			}

			if (Menu.bShowWindow && Menu.PlayerList.bShowWindow && LocalClientIsInGame())
			{
				ImGui::SetNextWindowSize(ImVec2(400.0f, 480.0f));
				ImGui::Begin("PLAYER LIST", &Menu.PlayerList.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::BeginChild("", ImVec2(0.0f, 0.0f), true);

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				{
					if (CharacterInfo[i].iInfoValid)
					{
						GetPlayerAddr(&Menu.PlayerList.NetAdr[i], ClientInfo[i].iClientNum);

						ImGui::PushID(i);
						ImGui::Checkbox("", (bool*)&_targetList.vIsTarget.data()[i]);
						ImGui::SameLine();

						if (*(int*)OFF_ISCURRENTHOST)
						{
							if (ImGui::Button("Crash", ImVec2(50.0f, 0.0f)))
							{
								Say(&GEntity[CG->iClientNum], &GEntity[i], 0, "\x5E\x01\x3D\x3D\xFF");
							} ImGui::SameLine();
						}

						ImGui::PushItemWidth(150.0f);
						ImGui::InputText(ClientInfo[i].szName, 
							(LPSTR)VariadicText("%u.%u.%u.%u",
							(BYTE)(Menu.PlayerList.NetAdr[i].iIP),
							(BYTE)(Menu.PlayerList.NetAdr[i].iIP >> 8),
							(BYTE)(Menu.PlayerList.NetAdr[i].iIP >> 16),
							(BYTE)(Menu.PlayerList.NetAdr[i].iIP >> 24)).c_str(),
							1024, ImGuiInputTextFlags_ReadOnly);
						ImGui::PopItemWidth();
						ImGui::PopID();
					}
				}

				ImGui::EndChild();
				ImGui::End();
			}

			if (Menu.bShowWindow && Menu.HostMenu.bShowWindow && LocalClientIsInGame() && *(int*)OFF_ISCURRENTHOST)
			{
				ImGui::SetNextWindowSize(ImVec2(484.0f, 592.0f));
				ImGui::Begin("HOST MENU", &Menu.HostMenu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				{
					if (CharacterInfo[i].iInfoValid)
						Menu.HostMenu.vPlayers.push_back(std::pair<int, LPSTR>(ClientInfo[i].iClientNum, ClientInfo[i].szName));

					else
					{
						Menu.HostMenu.PlayerMod[i].bGodMode = false;
						Menu.HostMenu.PlayerMod[i].bInfiniteAmmo = false;
						Menu.HostMenu.PlayerMod[i].bExplosiveBullets = false;
						Menu.HostMenu.PlayerMod[i].bFreezePlayer = false;
					}
				}

				if (ImGui::BeginCombo("Player", ClientInfo[Menu.HostMenu.iPlayer].szName))
				{
					Menu.HostMenu.GetPlayerSelection();
					ImGui::EndCombo();
				} ImGui::NewLine();

				ImGui::BeginChild("", ImVec2(0.0f, 0.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::Checkbox("God Mode", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bGodMode);
				ImGui::SameLine(292.0f);
				ImGui::Checkbox("Infinite Ammo", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bInfiniteAmmo);
				ImGui::NewLine();
				ImGui::Checkbox("Explosive Bullets", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bExplosiveBullets);
				ImGui::SameLine(292.0f);
				ImGui::Checkbox("Freeze", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bFreezePlayer);

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::InputText("Name", PlayerState[Menu.HostMenu.iPlayer].ClientState.szName, 16);

				if (ImGui::Combo("Team", (int*)&PlayerState[Menu.HostMenu.iPlayer].ClientState.iTeam, Menu.HostMenu.szTeam.data(), TEAM_MAX))
				{
					TeamChanged(ClientInfo[Menu.HostMenu.iPlayer].iClientNum);
				}

				ImGui::InputInt("Health", &GEntity[Menu.HostMenu.iPlayer].iHealth);

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::Combo("Weapon", &Menu.HostMenu.iWeaponID, vWeapons.first.data(), WEAPON_MAX);

				if (ImGui::Button("Give Weapon", ImVec2(150.0f, 25.0f)))
				{
					TakeWeapon(&PlayerState[Menu.HostMenu.iPlayer], GEntity[Menu.HostMenu.iPlayer].EntityState.iWeapon);
					GiveWeapon(&PlayerState[Menu.HostMenu.iPlayer], vWeapons.second[Menu.HostMenu.iWeaponID], false, false, false);
					GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", vWeapons.second[Menu.HostMenu.iWeaponID]));
					AddAmmo(&PlayerState[Menu.HostMenu.iPlayer], vWeapons.second[Menu.HostMenu.iWeaponID], false, 255, true);
				}

				ImGui::SameLine(0.0f, 4.0f);

				if (ImGui::Button("Take Weapon", ImVec2(150.0f, 25.0f)))
				{
					TakeWeapon(&PlayerState[Menu.HostMenu.iPlayer], GEntity[Menu.HostMenu.iPlayer].EntityState.iWeapon);
					GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, "a 0");
				}

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::SliderFloat3("Position", PlayerState[Menu.HostMenu.iPlayer].vOrigin, -8192.0f, 8192.0f, "%.0f");

				if (ImGui::Button("Teleport To", ImVec2(150.0f, 25.0f)))
					VectorCopy(PlayerState[Menu.HostMenu.iPlayer].vOrigin, PlayerState[CG->iClientNum].vOrigin);

				ImGui::SameLine(0.0f, 4.0f);

				if (ImGui::Button("Teleport From", ImVec2(150.0f, 25.0f)))
					VectorCopy(PlayerState[CG->iClientNum].vOrigin, PlayerState[Menu.HostMenu.iPlayer].vOrigin);

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::NewLine();

				if (ImGui::Button("Kick", ImVec2(304.0f, 25.0f)))
					KickClient(ClientInfo[Menu.HostMenu.iPlayer].iClientNum, Menu.HostMenu.szKickReason);

				ImGui::InputText("Reason", Menu.HostMenu.szKickReason, 64);

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::EndChild();

				Menu.HostMenu.vPlayers.clear();

				ImGui::End();
			}

			for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
			{
				if (LocalClientIsInGame() && *(int*)OFF_ISCURRENTHOST)
				{
					if (Menu.HostMenu.PlayerMod[i].bGodMode)
					{
						if (GEntity[i].iHealth != -9999999)
							GEntity[i].iHealth = -9999999;
					}

					else
					{
						if (GEntity[i].iHealth == -9999999)
							GEntity[i].iHealth = 100;
					}

					if (Menu.HostMenu.PlayerMod[i].bInfiniteAmmo)
					{
						if (PlayerState[i].iLethalAmmo1 < 255)
							PlayerState[i].iLethalAmmo1 = 255;

						if (PlayerState[i].iLethalAmmo2 < 255)
							PlayerState[i].iLethalAmmo2 = 255;

						if (PlayerState[i].iTacticalAmmo1 < 255)
							PlayerState[i].iTacticalAmmo1 = 255;

						if (PlayerState[i].iTacticalAmmo2 < 255)
							PlayerState[i].iTacticalAmmo2 = 255;

						if (PlayerState[i].iPrimaryAmmo1 < 255)
							PlayerState[i].iPrimaryAmmo1 = 255;

						if (PlayerState[i].iPrimaryAmmo2 < 255)
							PlayerState[i].iPrimaryAmmo2 = 255;

						if (PlayerState[i].iSecondaryAmmo1 < 255)
							PlayerState[i].iSecondaryAmmo1 = 255;

						if (PlayerState[i].iSecondaryAmmo2 < 255)
							PlayerState[i].iSecondaryAmmo2 = 255;
					}

					if (Menu.HostMenu.PlayerMod[i].bExplosiveBullets)
						EnablePerk(ClientInfo[i].iClientNum, PERK_EXPLOSIVEBULLETS);

					else
						DisablePerk(ClientInfo[i].iClientNum, PERK_EXPLOSIVEBULLETS);

					if (Menu.HostMenu.PlayerMod[i].bFreezePlayer)
						VectorCopy(Menu.HostMenu.PlayerMod[i].szPosition, PlayerState[i].vOrigin);

					else
						VectorCopy(PlayerState[i].vOrigin, Menu.HostMenu.PlayerMod[i].szPosition);
				}

				else
				{
					Menu.HostMenu.PlayerMod[i].bGodMode = false;
					Menu.HostMenu.PlayerMod[i].bInfiniteAmmo = false;
					Menu.HostMenu.PlayerMod[i].bExplosiveBullets = false;
					Menu.HostMenu.PlayerMod[i].bFreezePlayer = false;
				}

				if (!LocalClientIsInGame() || !CharacterInfo[i].iInfoValid)
					_targetList.vIsTarget.data()[i] = FALSE;
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			
			if (Menu.bStyleChanged)
			{
				RefreshInterface(_profiler.gMenuColor.Custom.iValue, _profiler.gMenuCursor.Custom.iValue, _profiler.gMenuFont.Custom.iValue);
				Menu.bStyleChanged = false;
			}

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE)
			{
				pDevice->Release();
				pDeviceContext->Release();

				ImGui_ImplWin32_Shutdown();
				ImGui_ImplDX11_Shutdown();
				ImGui::DestroyContext();

				bInitialized = false;
			}
		}
	}
}

//=====================================================================================