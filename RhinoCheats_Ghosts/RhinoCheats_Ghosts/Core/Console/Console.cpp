//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats
{
	cConsole _console;

	cConsole::~cConsole()
	{
		ClearLog();

		for (int i = 0; i < vHistory.Size; i++)
			free(vHistory[i]);
	}
	/*
	//=====================================================================================
	*/
	void cConsole::Init()
	{
		static bool bFirstTime = false;

		if (!bFirstTime)
		{
			ClearLog();
			ZeroMemory(szInput, sizeof(char[256]));
			iHistoryPos = -1;

			vCommands.push_back("clear");
			vCommands.push_back("history");
			vCommands.push_back("quit");

			vCommands.push_back("rc_crash");
			vCommands.push_back("rc_name");
			vCommands.push_back("rc_minimap");
			vCommands.push_back("rc_thirdperson");
			vCommands.push_back("rc_hostawall");
			vCommands.push_back("rc_rapidfire");
			vCommands.push_back("rc_chatspam");
			vCommands.push_back("rc_masskill");
			vCommands.push_back("rc_experience");
			vCommands.push_back("rc_prestige");
			vCommands.push_back("rc_squadpoints");
			vCommands.push_back("rc_unlockall");
			vCommands.push_back("rc_resetstats");
			vCommands.push_back("rc_hostdvar");
			vCommands.push_back("rc_message");
			vCommands.push_back("rc_playerlist");

			AddLog("Ready.");

			bFirstTime = true;
		}
	}
	/*
	//=====================================================================================
	*/
	void cConsole::ClearLog()
	{
		for (int i = 0; i < vItems.Size; i++)
			free(vItems[i]);

		vItems.clear();
		bScrollToBottom = true;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::AddLog(LPCSTR format, ...) IM_FMTARGS(2)
	{
		char szBuffer[1024];
		va_list Args;
		va_start(Args, format);
		vsnprintf(szBuffer, IM_ARRAYSIZE(szBuffer), format, Args);
		szBuffer[IM_ARRAYSIZE(szBuffer) - 1] = 0;
		va_end(Args);
		vItems.push_back(Strdup(szBuffer));
		bScrollToBottom = true;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::Draw(LPCSTR title, bool* open)
	{
		ImGui::SetNextWindowSize(ImVec2(510.0f, 350.0f));

		if (!ImGui::Begin(title, open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close"))
				* open = false;

			ImGui::EndPopup();
		}

		ImGui::TextWrapped("\t\t\tRhino Cheats");
		ImGui::Spacing();
		ImGui::TextWrapped("Press Help for details, press Tab to use text completion.");

		if (ImGui::Button("Help", ImVec2(50, 0)))
		{
			AddLog("1. rc_crash\n\t\tCrash everyone in the lobby except yourself.");
			AddLog("2. rc_name <name>\n\t\tChange your name.");
			AddLog("3. rc_minimap <on|off>\n\t\tEnable/disable enemy blips on the minimap.");
			AddLog("4. rc_thirdperson <on|off>\n\t\tEnable/disable thirdperson view.");
			AddLog("5. rc_hostawall <on|off>\n\t\tEnable/disable host autowall (as host).");
			AddLog("6. rc_rapidfire <on|off>\n\t\tEnable/disable rapidfire weapon rate (as host).");
			AddLog("7. rc_chatspam <on|off> <message>\n\t\tEnable/disable custom chatspam message.");
			AddLog("8. rc_masskill <on|off> <axis|allies|all>\n\t\tEnable/disable player masskill (as host).");
			AddLog("9. rc_experience <all|index> <max|experience>\n\t\tSet your experience.");
			AddLog("10. rc_prestige <max|number>\n\t\tSet your prestige.");
			AddLog("11. rc_squadpoints <max|squadpoints>\n\t\tSet your squadpoints.");
			AddLog("12. rc_unlockall\n\t\tUnlock everything in the game.");
			AddLog("13. rc_resetstats\n\t\tCompletely erase your save game.");
			AddLog("14. rc_hostdvar <dvar> <value>\n\t\tSet DVAR value for all clients (as host).");
			AddLog("15. rc_message <self|index> <all|index> <lobby|team|private> <message>\n\t\tSend a message (as host).");
			AddLog("16. rc_playerlist\n\t\tList everyone in the lobby.");

		} ImGui::SameLine();

		if (ImGui::Button("Clear", ImVec2(50, 0)))
		{
			ClearLog();
		} ImGui::SameLine();

		bool bCopyToClipboard = ImGui::Button("Copy", ImVec2(50, 0));
		const float flFooterHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

		ImGui::Separator();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -flFooterHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear"))
				ClearLog();

			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		if (bCopyToClipboard)
			ImGui::LogToClipboard();

		ImVec4 cDefaultText = ImGui::GetStyleColorVec4(ImGuiCol_Text);

		for (int i = 0; i < vItems.Size; i++)
		{
			LPCSTR szItem = vItems[i];
			ImVec4 cTemp = cDefaultText;

			if (strstr(szItem, "[ERROR]"))
				cTemp = ImColor(1.0f, 0.4f, 0.4f, 1.0f);

			else if (strncmp(szItem, "# ", 2) == 0)
				cTemp = ImColor(1.0f, 0.50f, 0.3f, 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Text, cTemp);
			ImGui::TextUnformatted(szItem);
			ImGui::PopStyleColor();
		}

		if (bCopyToClipboard)
			ImGui::LogFinish();

		if (bScrollToBottom)
			ImGui::SetScrollHere();

		bScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		bool bReclaimFocus = false;
		ImGui::PushItemWidth(-70);

		if (ImGui::InputText("Command", szInput, IM_ARRAYSIZE(szInput), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
		{
			LPSTR szInputEnd = szInput + strlen(szInput);

			while (szInputEnd > szInput && szInputEnd[-1] == ' ')
			{
				szInputEnd--;
			} *szInputEnd = 0;

			if (szInput[0])
				ExecCommand(szInput);

			strcpy(szInput, "");
			bReclaimFocus = true;
		}

		ImGui::PopItemWidth();
		ImGui::SetItemDefaultFocus();

		if (bReclaimFocus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}
	/*
	//=====================================================================================
	*/
	void cConsole::SplitCommandLine(LPCSTR in, sCmdLine* out)
	{
		ZeroMemory(out, sizeof(sCmdLine));

		int iLength = (int)strlen(in) + 1;
		LPSTR szCmdBuffer = new char[iLength];

		strcpy_s(szCmdBuffer, iLength, in);

		LPSTR szToken = strtok(szCmdBuffer, " ");

		while (szToken != nullptr)
		{
			if (out->iArgNum == 0)
				strcpy_s(out->szCmdName, szToken);

			else
				strcpy_s(out->szCmdArgs[out->iArgNum - 1], szToken);

			szToken = strtok(nullptr, " ");

			if (szToken != nullptr)
				++out->iArgNum;
		}

		delete[] szCmdBuffer;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::ExecCommand(LPCSTR command)
	{
		AddLog("# %s\n", command);

		sCmdLine CmdLine;
		SplitCommandLine(command, &CmdLine);

		iHistoryPos = -1;

		for (int i = vHistory.Size - 1; i >= 0; i--)
		{
			if (Stricmp(vHistory[i], command) == 0)
			{
				free(vHistory[i]);
				vHistory.erase(vHistory.begin() + i);
				break;
			}
		}

		vHistory.push_back(Strdup(command));

		if (!Stricmp(CmdLine.szCmdName, "clear"))
		{
			ClearLog();
		}

		else if (!Stricmp(CmdLine.szCmdName, "history"))
		{
			int iFirst = vHistory.Size - 10;

			for (int i = iFirst > 0 ? iFirst : 0; i < vHistory.Size; i++)
				AddLog("%3d: %s\n", i, vHistory[i]);
		}

		else if (!Stricmp(CmdLine.szCmdName, "quit"))
		{
			exit(EXIT_SUCCESS);
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_playerlist"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
			for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				if (CharacterInfo[i].iInfoValid)
					AddLog(VariadicText("%i - %s", ClientInfo[i].iClientNum, ClientInfo[i].szName).c_str());
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_crash"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
			Cbuf_AddText("say \"\x5E\x01\x3D\x3D\xFF\"\n");
			AddLog("Game lobby has been crashed.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_name"))
		{
			if (CmdLine.iArgNum)
			{
				char szArgBuff[512] = { NULL };

				for (int i = 0; i < CmdLine.iArgNum; i++)
					strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

				LPSTR szUsername = strtok(szArgBuff, "\n");

				if (szUsername)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					strcpy_s((LPSTR)(*(POINTER*)OFF_STEAMAPI + OFF_STEAMNAME), 32, szUsername);
					Cbuf_AddText(VariadicText("name %s\n", szUsername));
					AddLog("Name has been changed to \"%s.\"", szUsername);
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Null argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_minimap"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("bg_compassShowEnemies 1\n");
					AddLog("Enemy minimap blips have been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("bg_compassShowEnemies 0\n");
					AddLog("Enemy minimap blips have been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_thirdperson"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("camera_thirdPerson 1\n");
					AddLog("Third person mode has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("camera_thirdPerson 0\n");
					AddLog("Third person mode has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_hostawall"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("perk_bulletPenetrationMultiplier 30\n");
					EnablePerk(CG->iClientNum, PERK_EXTRABP);
					AddLog("Host autowall has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("perk_bulletPenetrationMultiplier 2\n");
					DisablePerk(CG->iClientNum, PERK_EXTRABP);
					AddLog("Host autowall has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_rapidfire"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("perk_weapRateMultiplier 0.0\n");
					EnablePerk(CG->iClientNum, PERK_RATEOFFIRE);
					AddLog("Rapidfire has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					Cbuf_AddText("perk_weapRateMultiplier 0.75\n");
					DisablePerk(CG->iClientNum, PERK_RATEOFFIRE);
					AddLog("Rapidfire has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_chatspam"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (CmdLine.iArgNum > 1)
					{
						char szArgBuff[512] = { NULL };

						for (int i = 1; i < CmdLine.iArgNum; i++)
							strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

						LPSTR szMessage = strtok(szArgBuff, "\n");

						if (szMessage)
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							strcpy_s(_mainGui.Menu.szMessage, szMessage);
							_profiler.gChatspam.Custom.bValue = true;
							AddLog("Chatspam message \"%s\" has been enabled.", szMessage);
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else
						{
							AddLog("[ERROR] Null argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					ZeroMemory(_mainGui.Menu.szMessage, sizeof(char[1024]));
					_profiler.gChatspam.Custom.bValue = false;
					AddLog("Chatspam message has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_masskill"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (CmdLine.iArgNum > 1)
					{
						if (!Stricmp(CmdLine.szCmdArgs[1], "axis"))
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							_profiler.gMasskill.Custom.iValue = cProfiler::MASSKILL_AXIS;
							AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else if (!Stricmp(CmdLine.szCmdArgs[1], "allies"))
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							_profiler.gMasskill.Custom.iValue = cProfiler::MASSKILL_ALLIES;
							AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							_profiler.gMasskill.Custom.iValue = cProfiler::MASSKILL_ALL;
							AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else
						{
							AddLog("[ERROR] Invalid argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					_profiler.gMasskill.Custom.iValue = cProfiler::MASSKILL_OFF;
					AddLog("Masskill has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_experience"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "all"))
				{
					if (CmdLine.iArgNum > 1)
					{
						if (!Stricmp(CmdLine.szCmdArgs[1], "max"))
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							for (int i = 0; i < 10; i++)
								* (int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * i)) = 4805;
							AddLog("All squad members' experience have been set to %s.", acut::ToLower(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) <= 4805)
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							for (int i = 0; i < 10; i++)
								* (int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * i)) = atoi(CmdLine.szCmdArgs[1]);
							AddLog("All squad members' experience have been set to %i.", atoi(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else
						{
							AddLog("[ERROR] Invalid argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 1 && atoi(CmdLine.szCmdArgs[0]) <= 10)
				{
					if (CmdLine.iArgNum > 1)
					{
						if (!Stricmp(CmdLine.szCmdArgs[1], "max"))
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))) = 4805;
							AddLog("%s's experience has been set to %s.", (LPSTR)(OFF_SQUADMEMBERNAME + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))), acut::ToLower(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) <= 4805)
						{
							AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
							*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))) = atoi(CmdLine.szCmdArgs[1]);
							AddLog("%s's experience has been set to %i.", (LPSTR)(OFF_SQUADMEMBERNAME + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))), atoi(CmdLine.szCmdArgs[1]));
							AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
						}

						else
						{
							AddLog("[ERROR] Invalid argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_prestige"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "max"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					*(int*)OFF_PRESTIGE = 10;
					AddLog("Prestige has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) <= 10)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					*(int*)OFF_PRESTIGE = atoi(CmdLine.szCmdArgs[0]);
					AddLog("Prestige has been set to %i.", atoi(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_squadpoints"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "max"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					*(int*)OFF_SQUADPOINTS = 99000000;
					AddLog("Squad points have been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) <= 99000000)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
					*(int*)OFF_SQUADPOINTS = atoi(CmdLine.szCmdArgs[0]);
					AddLog("Squad points have been set to %i.", atoi(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_unlockall"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
			for (int i = 0; i < 0x2D0; i++)
				* (BYTE*)(OFF_CHALLENGES + i) = 0xFF;
			AddLog("Everything has been unlocked.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_resetstats"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
			Cbuf_AddText("statsReset\n");
			AddLog("All stats have been reset.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_hostdvar"))
		{
			if (CmdLine.iArgNum)
			{
				DWORD dwDvar;
				if (GetVariableIndex(FindVariable(CmdLine.szCmdArgs[0]), &dwDvar))
				{
					if (CmdLine.iArgNum > 1)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
						GameSendServerCommand(-1, SV_CMD_RELIABLE, VariadicText("%c %i \"%s\"", 0x71, dwDvar, CmdLine.szCmdArgs[1]));
						AddLog("DVAR \"%s\" has been set to \"%s.\"", CmdLine.szCmdArgs[0], CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "rc_message"))
		{
			if (CmdLine.iArgNum)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "self"))
				{
					if (CmdLine.iArgNum > 1)
					{
						if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
						{
							if (CmdLine.iArgNum > 2)
							{
								if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], NULL, 0, szMessage);
											AddLog("Message \"%s\" has been sent from you to everyone.", szMessage);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], NULL, 1, szMessage);
											AddLog("Message \"%s\" has been sent from you to everyone.", szMessage);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], NULL, 2, szMessage);
											AddLog("Message \"%s\" has been sent from you to everyone.", szMessage);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else
								{
									AddLog("[ERROR] Invalid argument(s).");
								}
							}

							else
							{
								AddLog("[ERROR] Missing argument(s).");
							}
						}

						else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
						{
							if (CmdLine.iArgNum > 2)
							{
								if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 0, szMessage);
											AddLog("Message \"%s\" has been sent from you to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 1, szMessage);
											AddLog("Message \"%s\" has been sent from you to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[CG->iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 2, szMessage);
											AddLog("Message \"%s\" has been sent from you to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else
								{
									AddLog("[ERROR] Invalid argument(s).");
								}
							}

							else
							{
								AddLog("[ERROR] Missing argument(s).");
							}
						}

						else
						{
							AddLog("[ERROR] Invalid argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) < FindVariable("sv_maxclients")->Current.iValue)
				{
					if (CmdLine.iArgNum > 1)
					{
						if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
						{
							if (CmdLine.iArgNum > 2)
							{
								if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 0, szMessage);
											AddLog("Message \"%s\" has been sent from %s to everyone.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 1, szMessage);
											AddLog("Message \"%s\" has been sent from %s to everyone.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 2, szMessage);
											AddLog("Message \"%s\" has been sent from %s to everyone.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else
								{
									AddLog("[ERROR] Invalid argument(s).");
								}
							}

							else
							{
								AddLog("[ERROR] Missing argument(s).");
							}
						}

						else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
						{
							if (CmdLine.iArgNum > 2)
							{
								if (!Stricmp(CmdLine.szCmdArgs[2], "lobby")) 
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 0, szMessage);
											AddLog("Message \"%s\" has been sent from %s to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 1, szMessage);
											AddLog("Message \"%s\" has been sent from %s to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
								{
									if (CmdLine.iArgNum > 3)
									{
										char szArgBuff[512] = { NULL };

										for (int i = 3; i < CmdLine.iArgNum; i++)
											strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]).c_str());

										LPSTR szMessage = strtok(szArgBuff, "\n");

										if (szMessage)
										{
											AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());
											Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 2, szMessage);
											AddLog("Message \"%s\" has been sent from %s to %s.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
											AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
										}

										else
										{
											AddLog("[ERROR] Null argument(s).");
										}
									}

									else
									{
										AddLog("[ERROR] Missing argument(s).");
									}
								}

								else
								{
									AddLog("[ERROR] Invalid argument(s).");
								}
							}

							else
							{
								AddLog("[ERROR] Missing argument(s).");
							}
						}

						else
						{
							AddLog("[ERROR] Invalid argument(s).");
						}
					}

					else
					{
						AddLog("[ERROR] Missing argument(s).");
					}
				}

				else
				{
					AddLog("[ERROR] Invalid argument(s).");
				}
			}

			else
			{
				AddLog("[ERROR] Missing argument(s).");
			}
		}

		else
		{
			Cbuf_AddText(VariadicText("%s\n", command));
		}
	}
	/*
	//=====================================================================================
	*/
	int cConsole::TextEditCallbackStub(ImGuiTextEditCallbackData* data)
	{
		cConsole* Console = (cConsole*)data->UserData;
		return Console->TextEditCallback(data);
	}
	/*
	//=====================================================================================
	*/
	int cConsole::TextEditCallback(ImGuiTextEditCallbackData* data)
	{
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			LPCSTR szEnd = data->Buf + data->CursorPos, szStart = szEnd;

			while (szStart > data->Buf)
			{
				const char c = szStart[-1];

				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;

				szStart--;
			}

			ImVector<LPCSTR> vCandidates;

			for (int i = 0; i < vCommands.Size; i++)
				if (Strnicmp(vCommands[i], szStart, (int)(szEnd - szStart)) == 0)
					vCandidates.push_back(vCommands[i]);

			if (vCandidates.Size == 0)
				AddLog("No match for \"%.*s\"!\n", (int)(szEnd - szStart), szStart);

			else if (vCandidates.Size == 1)
			{
				data->DeleteChars((int)(szStart - data->Buf), (int)(szEnd - szStart));
				data->InsertChars(data->CursorPos, vCandidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}

			else
			{
				int iMatchLength = (int)(szEnd - szStart);

				for (;;)
				{
					int c = 0;

					bool bAllCandidatesMatches = true;

					for (int i = 0; i < vCandidates.Size && bAllCandidatesMatches; i++)
						if (i == 0)
							c = tolower(vCandidates[i][iMatchLength]);

						else if (c == 0 || c != tolower(vCandidates[i][iMatchLength]))
							bAllCandidatesMatches = false;

					if (!bAllCandidatesMatches)
						break;

					iMatchLength++;
				}

				if (iMatchLength > 0)
				{
					data->DeleteChars((int)(szStart - data->Buf), (int)(szEnd - szStart));
					data->InsertChars(data->CursorPos, vCandidates[0], vCandidates[0] + iMatchLength);
				}

				AddLog("Possible matches:\n");

				for (int i = 0; i < vCandidates.Size; i++)
					AddLog("- %s\n", vCandidates[i]);
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			const int iPreviousHistoryPos = iHistoryPos;

			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (iHistoryPos == -1)
					iHistoryPos = vHistory.Size - 1;

				else if (iHistoryPos > 0)
					iHistoryPos--;
			}

			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (iHistoryPos != -1)
					if (++iHistoryPos >= vHistory.Size)
						iHistoryPos = -1;
			}

			if (iPreviousHistoryPos != iHistoryPos)
			{
				data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (iHistoryPos >= 0) ? vHistory[iHistoryPos] : "");
				data->BufDirty = true;
			}
		}
		}
		return 0;
	}
}