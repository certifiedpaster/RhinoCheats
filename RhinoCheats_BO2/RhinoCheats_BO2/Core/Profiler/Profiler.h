//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"
#include "RapidXML/rapidxml_wrap.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cProfiler {
	public:

		struct cvar_s {
			LPCSTR szLabel;
			LPCSTR* szItems;

			union cvarValue_u {
				bool bValue;
				int iValue;
				DWORD dwValue;
				float flValue;
				LPSTR szValue;
				ImVec4 cValue;

				cvarValue_u(bool value) : bValue(value) {}
				cvarValue_u(int value) : iValue(value) {}
				cvarValue_u(DWORD value) : dwValue(value) {}
				cvarValue_u(float value) : flValue(value) {}
				cvarValue_u(LPSTR value) : szValue(value) {}
				cvarValue_u(ImVec4 value) : cValue(value) {}
			} Custom, Default;

			union minValue_u {
				int	iMin;
				float flMin;

				minValue_u(int min) : iMin(min) {}
				minValue_u(float min) : flMin(min) {}
			} MinValue;

			union maxValue_u {
				int	iMax;
				float flMax;

				maxValue_u(int max) : iMax(max) {}
				maxValue_u(float max) : flMax(max) {}
			} MaxValue;

			cvar_s(LPCSTR name, LPCSTR* items, bool value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			cvar_s(LPCSTR name, LPCSTR* items, int value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			cvar_s(LPCSTR name, LPCSTR* items, DWORD value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			cvar_s(LPCSTR name, LPCSTR* items, float value, float min, float max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			cvar_s(LPCSTR name, LPCSTR* items, LPSTR value, int min, int max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
			cvar_s(LPCSTR name, LPCSTR* items, ImVec4 value, float min, float max) : szLabel(name), szItems(items), Custom(value), Default(value), MinValue(min), MaxValue(max) {}
		};

		typedef enum {
			MENU_TAB_AIMBOT,
			MENU_TAB_ESP,
			MENU_TAB_TWEAKS,
			MENU_TAB_STYLES,
			MENU_TAB_MAX
		} menuTab_e;

		typedef enum {
			MENU_COLOR_NEUTRAL,
			MENU_COLOR_RED,
			MENU_COLOR_YELLOW,
			MENU_COLOR_GREEN,
			MENU_COLOR_CYAN,
			MENU_COLOR_BLUE,
			MENU_COLOR_MAGENTA,
			MENU_COLOR_MAX
		} menuColor_e;

		typedef enum {
			MENU_CURSOR_BLACK,
			MENU_CURSOR_WHITE,
			MENU_CURSOR_MAX
		} menuCursor_e;

		typedef enum {
			MENU_FONT_LIGHT,
			MENU_FONT_MEDIUM,
			MENU_FONT_BOLD,
			MENU_FONT_MAX
		} menuFont_e;

		typedef enum {
			AIMMODE_OFF,
			AIMMODE_MANUAL,
			AIMMODE_AUTOMATIC,
			AIMMODE_MAX
		} aimMode_e;

		typedef enum
		{
			BONESCAN_OFF,
			BONESCAN_ONTIMER,
			BONESCAN_IMMEDIATE,
			BONESCAN_MAX
		} eBonescan;

		typedef enum {
			ANTIAIM_OFF,
			ANTIAIM_SPINBOT,
			ANTIAIM_JITTERBOT,
			ANTIAIM_RANDOMIZED,
			ANTIAIM_REVERSED,
			ANTIAIM_MAX
		} antiAim_e;

		typedef enum {
			RIOTSHIELD_OFF,
			RIOTSHIELD_IGNOREPLAYER,
			RIOTSHIELD_TARGETFEET,
			RIOTSHIELD_MAX
		} riotShield_e;

		typedef enum {
			PLAYER_BOXES_OFF,
			PLAYER_BOXES_BORDER,
			PLAYER_BOXES_CORNER,
			PLAYER_BOXES_BORDER_FILLED,
			PLAYER_BOXES_CORNER_FILLED,
			PLAYER_BOXES_MAX
		} playerBoxes_e;

		typedef enum {
			PLAYER_BONES_OFF,
			PLAYER_BONES_DOTS,
			PLAYER_BONES_SKELETONS,
			PLAYER_BONES_MAX
		} playerBones_e;

		std::vector<LPCSTR> szMenuTabs = { "Aimbot", "ESP", "Tweaks", "Styles" };
		std::vector<LPCSTR> szMenuColor = { "Neutral", "Red", "Orange", "Yellow", "Green", "Blue", "Purple" };
		std::vector<LPCSTR> szMenuCursor = { "Black", "White" };
		std::vector<LPCSTR> szMenuFont = { "Light", "Medium", "Bold" };
		std::vector<LPCSTR> szAimmode = { "Off", "Manual", "Automatic" };
		std::vector<LPCSTR> szBonescan = { "Off", "On Timer", "Immediate" };
		std::vector<LPCSTR> szAntiAim = { "Off", "Spinbot", "Jitterbot", "Randomized", "Reversed" };
		std::vector<LPCSTR> szRiotshield = { "Off", "Ignore Player", "Target Feet" };
		std::vector<LPCSTR> szPlayerBoxes = { "Off", "Border", "Corner", "Border Filled", "Corner Filled" };
		std::vector<LPCSTR> szPlayerBones = { "Off", "Dots", "Lines" };

		cvar_s gMenuTabs = { "Menu Tab", szMenuTabs.data(), MENU_TAB_AIMBOT, MENU_TAB_AIMBOT, MENU_TAB_MAX };
		cvar_s gMenuColor = { "Menu Color", szMenuColor.data(), MENU_COLOR_NEUTRAL, MENU_COLOR_NEUTRAL, MENU_COLOR_MAX };
		cvar_s gMenuCursor = { "Menu Cursor", szMenuCursor.data(), MENU_CURSOR_BLACK, MENU_CURSOR_BLACK, MENU_CURSOR_MAX };
		cvar_s gMenuFont = { "Menu Font", szMenuFont.data(), MENU_FONT_LIGHT, MENU_FONT_LIGHT, MENU_FONT_MAX };
		cvar_s gAimmode = { "Aimmode", szAimmode.data(), AIMMODE_OFF, AIMMODE_OFF, AIMMODE_MAX };
		cvar_s gBonescan = { "Bonescan", szBonescan.data(), BONESCAN_OFF, BONESCAN_OFF, BONESCAN_MAX };
		cvar_s gAntiAim = { "Anti-Aim", szAntiAim.data(), ANTIAIM_OFF, ANTIAIM_OFF, ANTIAIM_MAX };
		cvar_s gRiotshield = { "Riotshields", szRiotshield.data(), RIOTSHIELD_OFF, RIOTSHIELD_OFF, RIOTSHIELD_MAX };
		cvar_s gPlayerBoxes = { "Player Boxes", szPlayerBoxes.data(), PLAYER_BOXES_OFF, PLAYER_BOXES_OFF, PLAYER_BOXES_MAX };
		cvar_s gPlayerBones = { "Player Bones", szPlayerBones.data(), PLAYER_BONES_OFF, PLAYER_BONES_OFF, PLAYER_BONES_MAX };
		cvar_s gAimbone = { "Aimbone", vBones.first.data(), BONE_HEAD, BONE_HEAD, BONE_MAX - 1 };

		cvar_s gAutozoom = { "Autozoom", nullptr, false, FALSE, TRUE };
		cvar_s gAutofire = { "Autofire", nullptr, false, FALSE, TRUE };
		cvar_s gAutowall = { "Autowall", nullptr, false, FALSE, TRUE };
		cvar_s gSilentAim = { "Silent-Aim", nullptr, false, FALSE, TRUE };
		cvar_s gNoRecoil = { "No-Recoil", nullptr, false, FALSE, TRUE };
		cvar_s gNoSpread = { "No-Spread", nullptr, false, FALSE, TRUE };
		
		cvar_s gPlayerInfo = { "Player Info", nullptr, false, FALSE, TRUE };
		cvar_s gPlayerWeapons = { "Player Weapons", nullptr, false, FALSE, TRUE };
		cvar_s gPlayerLines = { "Player Snaplines", nullptr, false, FALSE, TRUE };
		cvar_s gPlayerRadar = { "Player Radar", nullptr, false, FALSE, TRUE };

		cvar_s gAimAngle = { "Aim Angle", nullptr, 360.0f, 5.0f, 360.0f };
		cvar_s gAimStrength = { "Aim Strength", nullptr, 100, 5, 100 };
		cvar_s gAutoaimTime = { "Autoaim Time", nullptr, 0, 0, 1000 };
		cvar_s gAutoaimDelay = { "Autoaim Delay", nullptr, 0, 0, 1000 };
		cvar_s gAutozoomDelay = { "Autozoom Delay", nullptr, 0, 0, 1000 };
		cvar_s gAutofireDelay = { "Autofire Delay", nullptr, 0, 0, 1000 };

		cvar_s gColorAxisVisible = { "Axis Visible", nullptr, ImVec4(0.898039f, 0.235294f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		cvar_s gColorAxisInvisible = { "Axis Invisible", nullptr, ImVec4(0.898039f, 0.898039f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		cvar_s gColorAlliesVisible = { "Allies Visible", nullptr, ImVec4(0.235294f, 0.627451f, 0.898039f, 1.000000f), 0.0f, 1.0f };
		cvar_s gColorAlliesInvisible = { "Allies Invisible", nullptr, ImVec4(0.235294f, 0.898039f, 0.235294f, 1.000000f), 0.0f, 1.0f };
		cvar_s gColorShadow = { "Shadow", nullptr, ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f), 0.0f, 1.0f };

		acut::XmlDoc<char> XML;

		bool SaveProfile(const std::string& path);
		bool LoadProfile(const std::string& path);
		void DisableAll();
	} extern _profiler;
}

//=====================================================================================
