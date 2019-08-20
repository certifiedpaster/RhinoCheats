//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cMainGUI _mainGui;

	void cMainGUI::InitInterface() {
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
	void cMainGUI::SetMenuColor(
		int index
	) {
		switch (index) {
		case cProfiler::MENU_COLOR_NEUTRAL:
			ImGui::StyleColorsNeutral();
			break;

		case cProfiler::MENU_COLOR_RED:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_YELLOW:
			ImGui::StyleColorsOrange();
			break;

		case cProfiler::MENU_COLOR_GREEN:
			ImGui::StyleColorsYellow();
			break;

		case cProfiler::MENU_COLOR_CYAN:
			ImGui::StyleColorsGreen();
			break;

		case cProfiler::MENU_COLOR_BLUE:
			ImGui::StyleColorsBlue();
			break;

		case cProfiler::MENU_COLOR_MAGENTA:
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
	void cMainGUI::SetMenuCursor(
		int index
	) {
		switch (index) {
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
	void cMainGUI::SetMenuFont(
		int index
	) {
		switch (index) {
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
	void cMainGUI::RefreshInterface(
		int color,
		int cursor,
		int font
	) {
		SetMenuColor(color);

		ImGui::GetIO().Fonts->Clear();

		SetMenuCursor(cursor);

		ImGui::GetIO().Fonts->AddFontLight();
		ImGui::GetIO().Fonts->AddFontMedium();
		ImGui::GetIO().Fonts->AddFontBold();

		ImGui_ImplDX11_CreateDeviceObjects();

		SetMenuFont(font);
	}
	/*
	//=====================================================================================
	*/
	bool cMainGUI::GetKeyPress(
		int vkey,
		bool immediate
	) {
		if (VirtualKeys[vkey].bKey) {
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = true;
		}

		else if (
			!VirtualKeys[vkey].bKey &&
			VirtualKeys[vkey].bDown
		) {
			VirtualKeys[vkey].bUp = true;
			VirtualKeys[vkey].bDown = false;
		}

		else {
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = false;
		}

		if (immediate) {
			return VirtualKeys[vkey].bDown;
		}

		else {
			return VirtualKeys[vkey].bUp;
		}
	}
	/*
	//=====================================================================================
	*/
	LRESULT cMainGUI::WindowProcess(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	) {
		switch (uMsg) {
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

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		*(bool*)OFF_MOUSEINPUT = !Menu.bShowWindow;

		if (bInitialized && Menu.bShowWindow && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return TRUE;

		return CallWindowProc(oWindowProcess, hWnd, uMsg, wParam, lParam);
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::Present(
		_In_ IDXGISwapChain* pSwapChain,
		_In_ UINT SyncInterval,
		_In_ UINT Flags
	) {
		if (!bInitialized) {
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)& pDevice);
			pDevice->GetImmediateContext(&pDeviceContext);
			InitInterface();
		}

		else {
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

			if (LocalClientIsInGame()) {
				_drawing.ESP();
			}

			ImGui::End();
			ImGui::PopStyleColor(2);

			if (Menu.bShowWindow) {
				if (Menu.bWriteLog) {
					ImGui::LogToFile();
					Menu.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(487.0f, 324.0f));
				ImGui::Begin("RHINO CHEATS", &Menu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);

				if (ImGui::TabLabels(_profiler.gMenuTabs.MaxValue.iMax, _profiler.gMenuTabs.szItems, _profiler.gMenuTabs.Custom.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(117.0f, 25.0f))) {
					Menu.bWriteLog = true;
				}

				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 204.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				switch (_profiler.gMenuTabs.Custom.iValue) {
				case cProfiler::MENU_TAB_AIMBOT: {
					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_OFF], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_OFF)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(147.0f);

					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_MANUAL], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_MANUAL)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);

					if (ImGui::RadioButton(_profiler.gAimmode.szItems[cProfiler::AIMMODE_AUTOMATIC], &_profiler.gAimmode.Custom.iValue, cProfiler::AIMMODE_AUTOMATIC)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutozoom.szLabel, &_profiler.gAutozoom.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);

					if (ImGui::Checkbox(_profiler.gAutofire.szLabel, &_profiler.gAutofire.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutowall.szLabel, &_profiler.gAutowall.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);

					if (ImGui::Checkbox(_profiler.gSilentAim.szLabel, &_profiler.gSilentAim.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gNoRecoil.szLabel, &_profiler.gNoRecoil.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);
					
					if (ImGui::Checkbox(_profiler.gNoSpread.szLabel, &_profiler.gNoSpread.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();
					
					if (ImGui::Combo(_profiler.gBonescan.szLabel, &_profiler.gBonescan.Custom.iValue, _profiler.gBonescan.szItems, _profiler.gBonescan.MaxValue.iMax)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gAntiAim.szLabel, &_profiler.gAntiAim.Custom.iValue, _profiler.gAntiAim.szItems, _profiler.gAntiAim.MaxValue.iMax)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();
					
					if (ImGui::Combo(_profiler.gRiotshield.szLabel, &_profiler.gRiotshield.Custom.iValue, _profiler.gRiotshield.szItems, _profiler.gRiotshield.MaxValue.iMax)) {
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_ESP: {
					if (ImGui::Combo(_profiler.gPlayerBoxes.szLabel, &_profiler.gPlayerBoxes.Custom.iValue, _profiler.gPlayerBoxes.szItems, _profiler.gPlayerBoxes.MaxValue.iMax)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBones.szLabel, &_profiler.gPlayerBones.Custom.iValue, _profiler.gPlayerBones.szItems, _profiler.gPlayerBones.MaxValue.iMax)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerInfo.szLabel, &_profiler.gPlayerInfo.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);

					if (ImGui::Checkbox(_profiler.gPlayerWeapons.szLabel, &_profiler.gPlayerWeapons.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerLines.szLabel, &_profiler.gPlayerLines.Custom.bValue)) {
						Menu.bWriteLog = true;
					} ImGui::SameLine(294.0f);

					if (ImGui::Checkbox(_profiler.gPlayerRadar.szLabel, &_profiler.gPlayerRadar.Custom.bValue)) {
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_TWEAKS: {
					if (ImGui::SliderInt(_profiler.gAimbone.szLabel, &_profiler.gAimbone.Custom.iValue, _profiler.gAimbone.MinValue.iMin, _profiler.gAimbone.MaxValue.iMax, _profiler.gAimbone.szItems[_profiler.gAimbone.Custom.iValue])) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gAimAngle.szLabel, &_profiler.gAimAngle.Custom.flValue, _profiler.gAimAngle.MinValue.flMin, _profiler.gAimAngle.MaxValue.flMax, "%.0f degrees")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAimStrength.szLabel, &_profiler.gAimStrength.Custom.iValue, _profiler.gAimStrength.MinValue.iMin, _profiler.gAimStrength.MaxValue.iMax, "%d%%")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoaimTime.szLabel, &_profiler.gAutoaimTime.Custom.iValue, _profiler.gAutoaimTime.MinValue.iMin, _profiler.gAutoaimTime.MaxValue.iMax, "%d ms")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoaimDelay.szLabel, &_profiler.gAutoaimDelay.Custom.iValue, _profiler.gAutoaimDelay.MinValue.iMin, _profiler.gAutoaimDelay.MaxValue.iMax, "%d ms")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutozoomDelay.szLabel, &_profiler.gAutozoomDelay.Custom.iValue, _profiler.gAutozoomDelay.MinValue.iMin, _profiler.gAutozoomDelay.MaxValue.iMax, "%d ms")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutofireDelay.szLabel, &_profiler.gAutofireDelay.Custom.iValue, _profiler.gAutofireDelay.MinValue.iMin, _profiler.gAutofireDelay.MaxValue.iMax, "%d ms")) {
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(444.0f, 35.0f))) {
						_profiler.gAimbone.Custom.iValue = _profiler.gAimbone.Default.iValue;
						_profiler.gAimAngle.Custom.flValue = _profiler.gAimAngle.Default.flValue;

						_profiler.gAimStrength.Custom.iValue = _profiler.gAimStrength.Default.iValue;
						_profiler.gAutoaimTime.Custom.iValue = _profiler.gAutoaimTime.Default.iValue;
						_profiler.gAutoaimDelay.Custom.iValue = _profiler.gAutoaimDelay.Default.iValue;
						_profiler.gAutozoomDelay.Custom.iValue = _profiler.gAutozoomDelay.Default.iValue;
						_profiler.gAutofireDelay.Custom.iValue = _profiler.gAutofireDelay.Default.iValue;

						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_STYLES: {
					if (ImGui::ColorEdit3(_profiler.gColorAxisVisible.szLabel, _profiler.gColorAxisVisible.Custom.cValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAxisInvisible.szLabel, _profiler.gColorAxisInvisible.Custom.cValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAlliesVisible.szLabel, _profiler.gColorAlliesVisible.Custom.cValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAlliesInvisible.szLabel, _profiler.gColorAlliesInvisible.Custom.cValue)) {
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gMenuColor.szLabel, &_profiler.gMenuColor.Custom.iValue, _profiler.gMenuColor.szItems, _profiler.gMenuColor.MaxValue.iMax)) {
						WritePrivateProfileString("MenuStyle", "COLOR", std::to_string(_profiler.gMenuColor.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuCursor.szLabel, &_profiler.gMenuCursor.Custom.iValue, _profiler.gMenuCursor.szItems, _profiler.gMenuCursor.MaxValue.iMax)) {
						WritePrivateProfileString("MenuStyle", "CURSOR", std::to_string(_profiler.gMenuCursor.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuFont.szLabel, &_profiler.gMenuFont.Custom.iValue, _profiler.gMenuFont.szItems, _profiler.gMenuFont.MaxValue.iMax)) {
						WritePrivateProfileString("MenuStyle", "FONT", std::to_string(_profiler.gMenuFont.Custom.iValue).c_str(), (acut::GetExeDirectory() + DEFAULT_CFG).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(444.0f, 35.0f))) {
						_profiler.gColorAxisVisible.Custom.cValue = _profiler.gColorAxisVisible.Default.cValue;
						_profiler.gColorAxisInvisible.Custom.cValue = _profiler.gColorAxisInvisible.Default.cValue;
						_profiler.gColorAlliesVisible.Custom.cValue = _profiler.gColorAlliesVisible.Default.cValue;
						_profiler.gColorAlliesInvisible.Custom.cValue = _profiler.gColorAlliesInvisible.Default.cValue;
						_profiler.gColorShadow.Custom.cValue = _profiler.gColorShadow.Default.cValue;

						Menu.bWriteLog = true;
					}
				} break;
				}

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::EndChild();

				if (ImGui::Button("Set as Default", ImVec2(155.0f, 30.0f))) {
					_profiler.SaveProfile("");
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);

				if ((Menu.bSaveButton = ImGui::Button("Save Profile", ImVec2(155.0f, 30.0f)))) {
					Menu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 3.0f);
				LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(Menu.bSaveButton, Menu.SaveDialog.getLastDirectory(), NULL, ".xml", "Save Profile");

				if (strlen(szSavePath)) {
					_profiler.SaveProfile(szSavePath);
				}

				if ((Menu.bLoadButton = ImGui::Button("Load Profile", ImVec2(155.0f, 30.0f)))) {
					Menu.bWriteLog = true;
				} LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(Menu.bLoadButton, Menu.LoadDialog.getLastDirectory(), ".xml", "Load Profile");

				if (strlen(szLoadPath)) {
					_profiler.LoadProfile(szLoadPath);
				}

				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
				if (ImGui::InputText("", Menu.szCommand, sizeof(Menu.szCommand), ImGuiInputTextFlags_EnterReturnsTrue)) {
					if (!strncmp(acut::ToLower(Menu.szCommand).c_str(), "crash\0", strlen("crash\0") + 1))
						AddReliableCommand(VariadicText("sl", *(DWORD*)OFF_SERVERID));

					else if(!strncmp(acut::ToLower(Menu.szCommand).c_str(), "end\0", strlen("end\0") + 1))
						AddReliableCommand(VariadicText("mr %d -1 endround", *(DWORD*)OFF_SERVERID));

					ZeroMemory(Menu.szCommand, sizeof(Menu.szCommand));
					Menu.bWriteLog = true;
				}
				ImGui::PopItemWidth();
				ImGui::End();
			}

			if (_profiler.gPlayerRadar.Custom.bValue && LocalClientIsInGame()) {
				ImGui::SetNextWindowSize(ImVec2(_drawing.Radar.flRadarSize, _drawing.Radar.flRadarSize + ImGui::GetFrameHeight()));
				ImGui::Begin("RADAR", &_profiler.gPlayerRadar.Custom.bValue, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::GetWindowDrawList()->PushClipRectFullScreen();

				_drawing.Radar.vRadarPosition[0] = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2.0f;
				_drawing.Radar.vRadarPosition[1] = (ImGui::GetWindowPos().y + ImGui::GetFrameHeight()) + (ImGui::GetWindowSize().y - ImGui::GetFrameHeight()) / 2.0f;

				ImGui::GetWindowDrawList()->AddLine(ImVec2(_drawing.Radar.vRadarPosition[0] - _drawing.Radar.flRadarSize / 2.0f + 1.0f, _drawing.Radar.vRadarPosition[1]),
					ImVec2(_drawing.Radar.vRadarPosition[0] + _drawing.Radar.flRadarSize / 2.0f - 1.0f, _drawing.Radar.vRadarPosition[1]), ImGui::GetColorU32(ImVec4(0.50f, 0.50f, 0.50f, 0.50f)));

				ImGui::GetWindowDrawList()->AddLine(ImVec2(_drawing.Radar.vRadarPosition[0], _drawing.Radar.vRadarPosition[1] - _drawing.Radar.flRadarSize / 2.0f),
					ImVec2(_drawing.Radar.vRadarPosition[0], _drawing.Radar.vRadarPosition[1] + _drawing.Radar.flRadarSize / 2.0f - 1.0f), ImGui::GetColorU32(ImVec4(0.50f, 0.50f, 0.50f, 0.50f)));

				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(_drawing.Radar.vRadarPosition[0] - 2.0f, _drawing.Radar.vRadarPosition[1] - 2.0f),
					ImVec2(_drawing.Radar.vRadarPosition[0] + 3.0f, _drawing.Radar.vRadarPosition[1] + 3.0f), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

				for (int i = 0; i < MAX_CLIENTS; i++) {
					if (_targetList.EntityList[i].bIsValid) {
						ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(_drawing.Radar.vBlipPosition[i][0], _drawing.Radar.vBlipPosition[i][1]), _drawing.Radar.flBlipSize / 2.0f,
							ImGui::GetColorU32(ImVec4(_targetList.EntityList[i].cColor[0], _targetList.EntityList[i].cColor[1], _targetList.EntityList[i].cColor[2], _targetList.EntityList[i].cColor[3] / 4.0f)));

						ImGui::GetWindowDrawList()->AddCircle(ImVec2(_drawing.Radar.vBlipPosition[i][0], _drawing.Radar.vBlipPosition[i][1]), _drawing.Radar.flBlipSize / 2.0f,
							ImGui::GetColorU32(ImVec4(_targetList.EntityList[i].cColor[0], _targetList.EntityList[i].cColor[1], _targetList.EntityList[i].cColor[2], _targetList.EntityList[i].cColor[3])));
					}
				}

				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			if (Menu.bStyleChanged) {
				RefreshInterface(_profiler.gMenuColor.Custom.iValue, _profiler.gMenuCursor.Custom.iValue, _profiler.gMenuFont.Custom.iValue);
				Menu.bStyleChanged = false;
			}

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE) {
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