//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"
#include "RapidXML/rapidxml_wrap.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cProfiler
	{
	public:

		struct sCvar
		{
			LPCSTR szLabel;
			LPCSTR* szItems;

			union uCvarValue
			{
				bool bValue;
				int iValue;
				DWORD dwValue;
				float flValue;
				LPSTR szValue;
				ImVec4 cValue;

				uCvarValue(bool value) : bValue(value) {}
				uCvarValue(int value) : iValue(value) {}
				uCvarValue(DWORD value) : dwValue(value) {}
				uCvarValue(float value) : flValue(value) {}
				uCvarValue(LPSTR value) : szValue(value) {}
				uCvarValue(ImVec4 value) : cValue(value) {}
			} Custom, Default;

			union uMinValue
			{
				int	iMin;
				float flMin;

				uMinValue(int min) : iMin(min) {}
				uMinValue(float min) : flMin(min) {}
			} MinValue;

			union uMaxValue
			{
				int	iMax;
				float flMax;

				uMaxValue(int max) : iMax(max) {}
				uMaxValue(float max) : flMax(max) {}
			} MaxValue;

			sCvar(LPCSTR name, LPCSTR* items, bool value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(LPCSTR name, LPCSTR* items, int value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(LPCSTR name, LPCSTR* items, DWORD value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(LPCSTR name, LPCSTR* items, float value, float min, float max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(LPCSTR name, LPCSTR* items, LPSTR value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			sCvar(LPCSTR name, LPCSTR* items, ImVec4 value, float min, float max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
		};

		typedef enum
		{
			MENU_TAB_AIMBOT,
			MENU_TAB_WALLHACK,
			MENU_TAB_MISCELLANEOUS,
			MENU_TAB_TWEAKS,
			MENU_TAB_STYLES,
			MENU_TAB_MAX
		} eMenuTab;

		typedef enum
		{
			MENU_COLOR_NEUTRAL,
			MENU_COLOR_RED,
			MENU_COLOR_ORANGE,
			MENU_COLOR_YELLOW,
			MENU_COLOR_GREEN,
			MENU_COLOR_BLUE,
			MENU_COLOR_PURPLE,
			MENU_COLOR_MAX
		} eMenuColor;

		typedef enum
		{
			MENU_CURSOR_BLACK,
			MENU_CURSOR_WHITE,
			MENU_CURSOR_MAX
		} eMenuCursor;

		typedef enum
		{
			MENU_FONT_LIGHT,
			MENU_FONT_MEDIUM,
			MENU_FONT_BOLD,
			MENU_FONT_MAX
		} eMenuFont;

		typedef enum
		{
			AIMMODE_OFF,
			AIMMODE_MANUAL,
			AIMMODE_AUTOMATIC,
			AIMMODE_MAX
		} eAimmode;

		typedef enum
		{
			ANTIAIM_OFF,
			ANTIAIM_SPINBOT,
			ANTIAIM_JITTERBOT,
			ANTIAIM_RANDOMIZED,
			ANTIAIM_REVERSED,
			ANTIAIM_MAX
		} eAntiAim;

		typedef enum
		{
			BONESCAN_OFF,
			BONESCAN_ONTIMER,
			BONESCAN_IMMEDIATE,
			BONESCAN_MAX
		} eBonescan;

		typedef enum
		{
			PLAYER_BOXES_OFF,
			PLAYER_BOXES_BORDER,
			PLAYER_BOXES_CORNER,
			PLAYER_BOXES_BORDER_FILLED,
			PLAYER_BOXES_CORNER_FILLED,
			PLAYER_BOXES_MAX
		} ePlayerBoxes;

		typedef enum
		{
			PLAYER_BONES_OFF,
			PLAYER_BONES_DOTS,
			PLAYER_BONES_LINES,
			PLAYER_BONES_MAX
		} ePlayerBones;

		typedef enum
		{
			RIOTSHIELD_OFF,
			RIOTSHIELD_IGNOREPLAYER,
			RIOTSHIELD_TARGETFEET,
			RIOTSHIELD_MAX
		} eRiotshield;

		typedef enum
		{
			MASSKILL_OFF,
			MASSKILL_AXIS,
			MASSKILL_ALLIES,
			MASSKILL_ALL,
			MASSKILL_MAX
		} eMasskill;

		std::vector<LPCSTR> szMenuTabs = { "Aimbot", "ESP", "Misc.", "Tweaks", "Styles" };
		std::vector<LPCSTR> szMenuColor = { "Neutral", "Red", "Orange", "Yellow", "Green", "Blue", "Purple" };
		std::vector<LPCSTR> szMenuCursor = { "Black", "White" };
		std::vector<LPCSTR> szMenuFont = { "Light", "Medium", "Bold" };
		std::vector<LPCSTR> szAimmode = { "Off", "Manual", "Automatic" };
		std::vector<LPCSTR> szBonescan = { "Off", "On Timer", "Immediate" };
		std::vector<LPCSTR> szAntiAim = { "Off", "Spinbot", "Jitterbot", "Randomized", "Reversed" };
		std::vector<LPCSTR> szPlayerBoxes = { "Off", "Border", "Corner", "Border Filled", "Corner Filled" };
		std::vector<LPCSTR> szPlayerBones = { "Off", "Dots", "Lines" };
		std::vector<LPCSTR> szRiotshield = { "Off", "Ignore Player", "Target Feet" };
		std::vector<LPCSTR> szMasskill = { "Off", "Axis", "Allies", "All" };

		sCvar gMenuTabs = { "Menu Tab", szMenuTabs.data(), MENU_TAB_AIMBOT, MENU_TAB_AIMBOT, MENU_TAB_MAX };
		sCvar gMenuColor = { "Menu Color", szMenuColor.data(), MENU_COLOR_NEUTRAL, MENU_COLOR_NEUTRAL, MENU_COLOR_MAX };
		sCvar gMenuCursor = { "Menu Cursor", szMenuCursor.data(), MENU_CURSOR_BLACK, MENU_CURSOR_BLACK, MENU_CURSOR_MAX };
		sCvar gMenuFont = { "Menu Font", szMenuFont.data(), MENU_FONT_LIGHT, MENU_FONT_LIGHT, MENU_FONT_MAX };
		
		sCvar gAimmode = { "Aimmode", szAimmode.data(), AIMMODE_OFF, AIMMODE_OFF, AIMMODE_MAX };
		sCvar gBonescan = { "Bonescan", szBonescan.data(), BONESCAN_OFF, BONESCAN_OFF, BONESCAN_MAX };
		sCvar gAntiAim = { "Anti-Aim", szAntiAim.data(), ANTIAIM_OFF, ANTIAIM_OFF, ANTIAIM_MAX };
		
		sCvar gPlayerBoxes = { "Player Boxes", szPlayerBoxes.data(), PLAYER_BOXES_OFF, PLAYER_BOXES_OFF, PLAYER_BOXES_MAX };
		sCvar gPlayerBones = { "Player Bones", szPlayerBones.data(), PLAYER_BONES_OFF, PLAYER_BONES_OFF, PLAYER_BONES_MAX };

		sCvar gRiotshield = { "Riotshields", szRiotshield.data(), RIOTSHIELD_OFF, RIOTSHIELD_OFF, RIOTSHIELD_MAX };
		
		sCvar gAimbone = { "Aimbone", vBones.first.data(), BONE_HEAD, BONE_HEAD, BONE_MAX - 1 };
		sCvar gMasskill = { "Masskill", szMasskill.data(), MASSKILL_OFF, MASSKILL_OFF, MASSKILL_MAX - 1 };

		sCvar gAutozoom = { "Autozoom", nullptr, false, FALSE, TRUE };
		sCvar gAutofire = { "Autofire", nullptr, false, FALSE, TRUE };
		sCvar gAutowall = { "Autowall", nullptr, false, FALSE, TRUE };
		sCvar gSilentAim = { "Silent-Aim", nullptr, false, FALSE, TRUE };
		sCvar gTargetAgents = { "Target Agents", nullptr, false, FALSE, TRUE };
		sCvar gTargetMissiles = { "Target Missiles", nullptr, false, FALSE, TRUE };
		
		sCvar gPlayerSnaplines = { "Player Snaplines", nullptr, false, FALSE, TRUE };
		sCvar gPlayerInfo = { "Player Info", nullptr, false, FALSE, TRUE };
		sCvar gPlayerWeapons = { "Player Weapons", nullptr, false, FALSE, TRUE };
		sCvar gAgents = { "Agents", nullptr, false, FALSE, TRUE };
		sCvar gMissiles = { "Missiles", nullptr, false, FALSE, TRUE };
		sCvar gItems = { "Items", nullptr, false, FALSE, TRUE };
		
		sCvar gNoRecoil = { "No-Recoil", nullptr, false, FALSE, TRUE };
		sCvar gNoSpread = { "No-Spread", nullptr, false, FALSE, TRUE };
		sCvar gKillspam = { "Killspam", nullptr, false, FALSE, TRUE };
		sCvar gNameStealer = { "Name Stealer", nullptr, false, FALSE, TRUE };
		sCvar gCrosshair = { "Crosshair", nullptr, false, FALSE, TRUE };
		sCvar gAFKMode = { "AFK Mode", nullptr, false, FALSE, TRUE };
		sCvar gRadar = { "Radar", nullptr, false, FALSE, TRUE };
		sCvar gChatspam = { "Chatspam", nullptr, false, FALSE, TRUE };

		sCvar gAimAngle = { "Aim Angle", nullptr, 360.0f, 5.0f, 360.0f };
		sCvar gAimStrength = { "Aim Strength", nullptr, 100, 5, 100 };
		sCvar gAutoaimTime = { "Autoaim Time", nullptr, 0, 0, 1000 };
		sCvar gAutoaimDelay = { "Autoaim Delay", nullptr, 0, 0, 1000 };
		sCvar gAutozoomDelay = { "Autozoom Delay", nullptr, 0, 0, 1000 };
		sCvar gAutofireDelay = { "Autofire Delay", nullptr, 0, 0, 1000 };

		sCvar gColorAxisVisible = { "Axis Visible", nullptr, ImVec4(0.898039f, 0.235294f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorAxisInvisible = { "Axis Invisible", nullptr, ImVec4(0.898039f, 0.898039f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorAlliesVisible = { "Allies Visible", nullptr, ImVec4(0.235294f, 0.627451f, 0.898039f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorAlliesInvisible = { "Allies Invisible", nullptr, ImVec4(0.235294f, 0.898039f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorEntityVisible = { "Entity Visible", nullptr, ImVec4(0.898039f, 0.627451f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorEntityInvisible = { "Entity Invisible", nullptr, ImVec4(0.898039f, 0.627451f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorItemVisible = { "Item Visible", nullptr, ImVec4(0.627451f, 0.235294f, 0.898039f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorItemInvisible = { "Item Invisible", nullptr, ImVec4(0.627451f, 0.235294f, 0.898039f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorCrosshair = { "Crosshair", nullptr, ImVec4(1.000000f, 1.000000f, 1.000000f, 1.000000f), 0.0f, 1.0f };
		sCvar gColorShadow = { "Shadow", nullptr, ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f), 0.0f, 1.0f };

		acut::XmlDoc<char> XML;

		bool SaveProfile(const std::string& path);
		bool LoadProfile(const std::string& path);
		void DisableAll();
	} extern _profiler;
}

//=====================================================================================
