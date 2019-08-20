#include "stdafx.h"

Config_t Settings[Configs_];
char	 szSettingsFile[MAX_PATH];


Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const bool bDefault);
Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const int iDefault, const int iMin, const int iMax);
Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const float fDefault, const float fMin, const float fMax);
Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const ImVec4 v4Default);

// For some reason this function won't work if i use "const std::string" instead of "const char*", 
// because: it redirects to the first function (with const bool bDefault) if i use "const std::string"
Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const char *pszDefault);

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const ImVec2 v2Default);


//========================================================================

void Bot_InitSettings()
{
	Settings[aim_enabled] = Bot_AddConfig("Aimbot Settings", "Aimbot_Enabled", false);
	Settings[aim_key] = Bot_AddConfig("Aimbot Settings", "Aim_Key", 0, 0, 4); //5: 0,1,2,3,4
	Settings[aim_mode] = Bot_AddConfig("Aimbot Settings", "Aimbot_Mode", 1, 0, 1); //fov, distance.	
	Settings[auto_shoot] = Bot_AddConfig("Aimbot Settings", "Aimbot_AutoShoot", true);
	Settings[auto_knife] = Bot_AddConfig("Aimbot Settings", "Aimbot_AutoKnife", true);
	Settings[anti_antiaim] = Bot_AddConfig("Aimbot Settings", "Aimbot_AntiAntiAim", false);
	Settings[team_killer] = Bot_AddConfig("Aimbot Settings", "Aimbot_TeamKiller", false);
	Settings[fov] = Bot_AddConfig("Aimbot Settings", "Aimbot_Fov", 15.0f, 0.0f, 360.0f);
	Settings[auto_wall] = Bot_AddConfig("Aimbot Settings", "Aimbot_AutoWall", 1, 0, 2);
	Settings[head_only] = Bot_AddConfig("Aimbot Settings", "Aimbot_HeadOnly", false);
	Settings[aim_prediction] = Bot_AddConfig("Aimbot Settings", "Aimbot_Prediction", true);
	Settings[enemies_with_riotshield] = Bot_AddConfig("Aimbot Settings", "Aimbot_EnemieswithRiotShield", true);

	//========================================================================

	Settings[esp_enabled] = Bot_AddConfig("Esp Settings", "Esp_Enabled", true);
	Settings[esp_enemy] = Bot_AddConfig("Esp Settings", "Esp_Enemies", true);
	Settings[esp_enemy_color] = Bot_AddConfig("Esp Settings", "Esp_EnemyColor", ImVec4(1.0f, 1.0f, 0.647f, 1.0f));
	Settings[esp_visible_color] = Bot_AddConfig("Esp Settings", "Esp_VisibleColor", ImVec4(0.385f, 0.003f, 0.767f, 1.0f));
	Settings[esp_friendly] = Bot_AddConfig("Esp Settings", "Esp_Friendlies", false);
	Settings[esp_friendly_color] = Bot_AddConfig("Esp Settings", "Esp_FriendlyColor", ImVec4(0.080f, 0.647f, 0.845f, 1.0f));
	Settings[esp_name] = Bot_AddConfig("Esp Settings", "Esp_Name", true);
	Settings[esp_name_boxed] = Bot_AddConfig("Esp Settings", "Esp_Name_Boxed", false);
	Settings[esp_weapon] = Bot_AddConfig("Esp Settings", "Esp_Weapon", true);
	Settings[esp_conected_bones] = Bot_AddConfig("Esp Settings", "Esp_Skeleton", true);
	Settings[esp_misc] = Bot_AddConfig("Esp Settings", "Esp_Misc", true);
	Settings[esp_misc_items] = Bot_AddConfig("Esp Settings", "Esp_Items", true);
	Settings[esp_misc_items_color] = Bot_AddConfig("Esp Settings", "Esp_ItemColor", ImVec4(0.0f, 0.974f, 1.0f, 1.0f));
	Settings[esp_misc_devices] = Bot_AddConfig("Esp Settings", "Esp_Devices", true);
	Settings[esp_misc_devices_color] = Bot_AddConfig("Esp Settings", "Esp_DeviceColor", ImVec4(0.578f, 0.0f, 1.0f, 1.0f));
	Settings[esp_misc_explosives] = Bot_AddConfig("Esp Settings", "Esp_Explosives", true);
	Settings[esp_misc_explosives_color] = Bot_AddConfig("Esp Settings", "Esp_ExplosiveColor", ImVec4(0.991f, 1.0f, 0.0f, 1.0f));
	Settings[esp_radar] = Bot_AddConfig("Esp Settings", "Esp_Radar", true);
	Settings[esp_radar_x] = Bot_AddConfig("Esp Settings", "Esp_RadarX", 174.0f, 1.0f, 1440.0f); //991.0f
	Settings[esp_radar_y] = Bot_AddConfig("Esp Settings", "Esp_RadarY", 59.0f, 1.0f, 900.0f); //496.0f
	Settings[esp_radar_w] = Bot_AddConfig("Esp Settings", "Esp_RadarW", 154.0f, 1.0f, 1400.0f); //140.0f
	Settings[esp_radar_h] = Bot_AddConfig("Esp Settings", "Esp_RadarH", 168.0f, 1.0f, 850.0f); //140.0f
	Settings[esp_compass_radar] = Bot_AddConfig("Esp Settings", "Esp_Compass", true);
	Settings[esp_compass_radar_radius] = Bot_AddConfig("Esp Settings", "Esp_CompassRadius", 0.0f, 0.0f, 100.0f);
	Settings[esp_box] = Bot_AddConfig("Esp Settings", "Esp_Box", 3, 0, 6); //Corn Corn-F 2D 2D-F 3D Lilly.
	Settings[esp_snap_lines] = Bot_AddConfig("Esp Settings", "Esp_SnapLines", 3, 0, 3);
	Settings[esp_bright_colors] = Bot_AddConfig("Esp Settings", "Esp_BrightColors", false);

	//========================================================================

	Settings[d3d_enabled] = Bot_AddConfig("D3D Settings", "D3D_Enabled", true);
	Settings[d3d_chams] = Bot_AddConfig("D3D Settings", "D3D_Chams", true);
	Settings[d3d_chams_wireframe] = Bot_AddConfig("D3D Settings", "D3D_Chams_Wireframe", true);
	Settings[d3d_chams_axis] = Bot_AddConfig("D3D Settings", "D3D_Chams_Axis", true);
	Settings[d3d_axis_color] = Bot_AddConfig("D3D Settings", "D3D_Axis_Color", ImVec4(0.845f, 0.029f, 0.948f, 1.0f));
	Settings[d3d_axis_visible_color] = Bot_AddConfig("D3D Settings", "D3D_Axis_VisibleColor", ImVec4(0.645f, 0.0f, 1.0f, 1.0f));

	Settings[d3d_chams_allies] = Bot_AddConfig("D3D Settings", "D3D_Chams_Allies", true);
	Settings[d3d_allies_color] = Bot_AddConfig("D3D Settings", "D3D_Allies_Color", ImVec4(0.004f, 0.329f, 0.922f, 1.0f));
	Settings[d3d_allies_visible_color] = Bot_AddConfig("D3D Settings", "D3D_Allies_VisibleColor", ImVec4(0.0f, 0.836f, 1.0f, 1.0f));

	Settings[d3d_chams_weapons] = Bot_AddConfig("D3D Settings", "D3D_Chams_Weapons", true);
	Settings[d3d_weapon_color] = Bot_AddConfig("D3D Settings", "D3D_Chams_WeaponsColor", ImVec4(0.049f, 0.492f, 0.716f, 1.0f));
	Settings[d3d_weapon_visible_color] = Bot_AddConfig("D3D Settings", "D3D_Chams_WeaponsVisibleColor", ImVec4(0.060f, 0.742f, 0.885f, 1.0f));
	Settings[d3d_weapon_customtexture] = Bot_AddConfig("D3D Settings", "D3D_Chams_WeaponCustomTexture", false);

	Settings[d3d_nosky] = Bot_AddConfig("D3D Settings", "D3D_NoSky", false);
	Settings[d3d_nohands] = Bot_AddConfig("D3D Settings", "D3D_NoHands", false);
	Settings[d3d_noweapon] = Bot_AddConfig("D3D Settings", "D3D_NoWeapon", false);
	Settings[d3d_nosmoke] = Bot_AddConfig("D3D Settings", "D3D_NoSmoke", true);
	Settings[d3d_glass] = Bot_AddConfig("D3D Settings", "D3D_Glass", false);
	Settings[d3d_tripmap] = Bot_AddConfig("D3D Settings", "D3D_TripMap", false);

	//========================================================================	

	Settings[force_fov] = Bot_AddConfig("Misc Settings", "Misc_ForceFov", 109.669f, 65.0f, 200.0f); /*, false);*/
	Settings[fullbright] = Bot_AddConfig("Misc Settings", "Misc_FullBright", false);
	Settings[gun_x] = Bot_AddConfig("Misc Settings", "Misc_GunX", 0, 0, 200);
	Settings[gun_y] = Bot_AddConfig("Misc Settings", "Misc_GunY", 0, 0, 200);
	Settings[gun_z] = Bot_AddConfig("Misc Settings", "Misc_GunZ", 0, 0, 200);
	Settings[kill_spam] = Bot_AddConfig("Misc Settings", "Misc_KillSpam", false);
	Settings[kill_spam_text] = Bot_AddConfig("Misc Settings", "Misc_KillSpamText", ":)");
	Settings[no_recoil] = Bot_AddConfig("Misc Settings", "Misc_NoRecoil", true);
	Settings[no_spread] = Bot_AddConfig("Misc Settings", "Misc_NoSpread", true);
	Settings[silent_aim] = Bot_AddConfig("Misc Settings", "Misc_SilentAim", true);
	Settings[inverse_troller] = Bot_AddConfig("Misc Settings", "Misc_InverseTroller", false);
	Settings[kill_icons] = Bot_AddConfig("Misc Settings", "Misc_KillIcons", true);
	Settings[kill_icons_rotate] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_Rotate", true);
	Settings[kill_icons_type] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_Type", 2, 0, 2);
	Settings[kill_icons_origin] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_InitialPosition", 0, 0, 8);
	Settings[kill_icons_destination] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_FinalPosition", 1, 0, 3);
	Settings[kill_icons_size] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_Size", 0, 0, 2);
	Settings[kill_icons_count] = Bot_AddConfig("Misc Settings", "Misc_KillIcons_Quantity", 5, 1, 10);
	Settings[show_console] = Bot_AddConfig("Misc Settings", "Misc_ShowConsole", false);
	Settings[show_info] = Bot_AddConfig("Misc Settings", "Misc_ShowInfo", false);
	Settings[draw_clock] = Bot_AddConfig("Misc Settings", "Misc_DrawClock", true);
	Settings[names_stealer] = Bot_AddConfig("Misc Settings", "Misc_NameStealer", false);
	Settings[laser] = Bot_AddConfig("Misc Settings", "Misc_Laser", true);
	Settings[kill_sounds] = Bot_AddConfig("Misc Settings", "Misc_KillSounds", true);
	Settings[third_person] = Bot_AddConfig("Misc Settings", "Misc_ThirdPerson", false);
	Settings[afk_mode] = Bot_AddConfig("Misc Settings", "Misc_AFKMode", false);
	Settings[anti_aim] = Bot_AddConfig("Misc Settings", "Misc_AntiAim", 0, 0, 2); //rotation, shield
	Settings[rainbow_mode] = Bot_AddConfig("Misc Settings", "Misc_RainbowMode", false);
	Settings[draw_crosshair] = Bot_AddConfig("Misc Settings", "Misc_DrawCrosshair", 2, 0, 2); //off, normal, lilli	
	Settings[drunken_style] = Bot_AddConfig("Misc Settings", "Misc_DrunkenStyle", false);
	Settings[watermark] = Bot_AddConfig("Misc Settings", "Misc_WaterMark", false);

	//========================================================================

	Settings[host_forcehost] = Bot_AddConfig("Host Settings", "Host_ForceHost", false);
	Settings[host_map] = Bot_AddConfig("Host Settings", "Host_Map", 0, 0, 25);
	Settings[host_gametype] = Bot_AddConfig("Host Settings", "Host_Gametype", 0, 0, 11);
	Settings[host_autowall] = Bot_AddConfig("Host Settings", "Host_Autowall", false);
	Settings[host_masskill] = Bot_AddConfig("Host Settings", "Host_MassKill", false);
	Settings[host_unlimited_ammo] = Bot_AddConfig("Host Settings", "Host_UnlimitedAmmo", false);
	Settings[host_antileave] = Bot_AddConfig("Host Settings", "Host_Antileave", false);
	Settings[host_unfair] = Bot_AddConfig("Host Settings", "Host_UnfairAimbot", false);
	Settings[host_rapidfire] = Bot_AddConfig("Host Settings", "Host_RapidFire", false);
	Settings[host_noclip] = Bot_AddConfig("Host Settings", "Host_NoClip", false);
	Settings[host_speed] = Bot_AddConfig("Host Settings", "Host_Speed", false);
	Settings[host_god] = Bot_AddConfig("Host Settings", "Host_God", false);
	Settings[host_partysize] = Bot_AddConfig("Misc Settings", "Misc_PartySize", 7, 2, 18);
	Settings[host_killcam] = Bot_AddConfig("Host Settings", "Host_KillCam", true);
	Settings[host_teleport] = Bot_AddConfig("Host Settings", "Host_Teleport", 0, 0, 2);
	Settings[host_jump] = Bot_AddConfig("Host Settings", "Host_Jump", false);
	Settings[host_visions] = Bot_AddConfig("Host Settings", "Host_Visions", false);
	Settings[host_visions_time] = Bot_AddConfig("Host Settings", "Host_VisionsTime", 1, 1, 10);
	Settings[host_clap] = Bot_AddConfig("Host Settings", "Host_ClapEveryone", false);
	Settings[host_killstreak] = Bot_AddConfig("Host Settings", "Host_SHBomb", false);
	Settings[host_glitchgun] = Bot_AddConfig("Host Settings", "Host_GlitchGun", false);
	Settings[host_heareveryone] = Bot_AddConfig("Host Settings", "Host_HearEveryone", false);
	Settings[host_antiendgame] = Bot_AddConfig("Host Settings", "Host_AntiEndGame", false);

	//========================================================================

	Settings[style_color] = Bot_AddConfig("Style Settings", "Style_Color", 13, 0, 14);
	Settings[style_font] = Bot_AddConfig("Style Settings", "Style_Font", 1, 0, 1);
	Settings[style_rainbow] = Bot_AddConfig("Style Settings", "Style_RainbowFx", true);

	Settings[window_radar_pos] = Bot_AddConfig("Radar", "Pos", ImVec2(642, 386));
	Settings[window_radar_size] = Bot_AddConfig("Radar", "Size", ImVec2(129, 152));

	Settings[window_info_pos] = Bot_AddConfig("Info", "Pos", ImVec2(60, 60));
	Settings[window_info_size] = Bot_AddConfig("Info", "Size", ImVec2(114, 254));

	Settings[window_main_pos] = Bot_AddConfig(ALASKA17, "Pos", ImVec2(12, 12));
	Settings[window_main_size] = Bot_AddConfig(ALASKA17, "Size", ImVec2(472, 522));

	Settings[window_console_pos] = Bot_AddConfig("Console", "Pos", ImVec2(464, 16));
	Settings[window_console_size] = Bot_AddConfig("Console", "Size", ImVec2(327, 370));

}

bool Bot_LoadSettings()
{
	sprintf_s<MAX_PATH>(szSettingsFile, "%s%s", Hook.m_szDllDirectory, SETTINGS_FILE_NAME);

	FILE *Settings_File = fopen(szSettingsFile, "r"); //"r" 	Opens a file for reading. The file must exist.
	if (!Settings_File)
	{
		//First time writing to file
		Settings_File = fopen(szSettingsFile, "w+"); //"w+" 	Creates an empty file for both reading and writing.
		if (!Settings_File)
		{
			return false;
		}

		for (size_t i = 0; i < Configs_; i++)
		{
			switch (Settings[i].eType)
			{
			case SETTING_BOOL:
				Settings[i].szDefault = (Settings[i].Value.bValue) ? "on" : "off";
				break;

			case SETTING_INT:
				LIMIT_VALUE(Settings[i].Value.iValue, Settings[i].Value.iMin, Settings[i].Value.iMax);
				Settings[i].szDefault = std::to_string(Settings[i].Value.iValue);
				break;

			case SETTING_FLOAT:
				LIMIT_VALUE(Settings[i].Value.fValue, Settings[i].Value.fMin, Settings[i].Value.fMax);
				Settings[i].szDefault = STREAM(std::fixed << std::setprecision(3) << Settings[i].Value.fValue);
				break;

			case SETTING_VEC4:
				Settings[i].szDefault = STREAM(
					std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.x << " " <<
					std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.y << " " <<
					std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.z << " " <<
					std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.w
				);
				break;

			case SETTING_VEC2:
				Settings[i].szDefault = STREAM(
					std::fixed << std::setprecision(3) << Settings[i].Value.v2Value.x << "," <<
					std::fixed << std::setprecision(3) << Settings[i].Value.v2Value.y
				);
				break;

			case SETTING_STRING:
				Settings[i].szDefault = Settings[i].Value.szValue;
			}

			WritePrivateProfileString(Settings[i].pszAppName.c_str(), Settings[i].pszKeyName.c_str(), Settings[i].szDefault.c_str(), szSettingsFile);
		}

		fclose(Settings_File);

		return true;
	}

	// Loading 

	char *temp = new char[1024];
	if (temp == nullptr)
	{
		fclose(Settings_File);
		return false;
	}

	for (size_t i = 0; i < Configs_; i++)
	{
		GetPrivateProfileString(Settings[i].pszAppName.c_str(),
			Settings[i].pszKeyName.c_str(),
			Settings[i].szDefault.c_str(),
			temp,
			1024,
			szSettingsFile);

		switch (Settings[i].eType)
		{
		case SETTING_BOOL:
			Settings[i].Value.bValue = (_stricmp(temp, "on") == 0);
			break;

		case SETTING_INT:
			Settings[i].Value.iValue = atoi(temp);
			LIMIT_VALUE(Settings[i].Value.iValue, Settings[i].Value.iMin, Settings[i].Value.iMax);
			break;

		case SETTING_FLOAT:
			Settings[i].Value.fValue = static_cast<float>(atof(temp));
			LIMIT_VALUE(Settings[i].Value.fValue, Settings[i].Value.fMin, Settings[i].Value.fMax);
			break;

		case SETTING_VEC4:
			sscanf_s(temp, "%f %f %f %f",
				&Settings[i].Value.v4Value.x,
				&Settings[i].Value.v4Value.y,
				&Settings[i].Value.v4Value.z,
				&Settings[i].Value.v4Value.w);
			break;

		case SETTING_VEC2:
			sscanf_s(temp, "%f,%f",
				&Settings[i].Value.v2Value.x,
				&Settings[i].Value.v2Value.y);
			break;

		case SETTING_STRING:
			Settings[i].Value.szValue = temp;
		}
	}

	delete[] temp;

	fclose(Settings_File);

	return true;
}

bool Bot_SaveSettings()
{
	FILE *fp = fopen(szSettingsFile, "a");
	if (!fp)
	{
		return false;
	}

	for (size_t i = 0; i < Configs_; i++)
	{
		switch (Settings[i].eType)
		{
		case SETTING_BOOL:
			Settings[i].szDefault = (Settings[i].Value.bValue) ? "on" : "off";
			break;

		case SETTING_INT:
			LIMIT_VALUE(Settings[i].Value.iValue, Settings[i].Value.iMin, Settings[i].Value.iMax);
			Settings[i].szDefault = std::to_string(Settings[i].Value.iValue);
			break;

		case SETTING_FLOAT:
			LIMIT_VALUE(Settings[i].Value.fValue, Settings[i].Value.fMin, Settings[i].Value.fMax);
			Settings[i].szDefault = STREAM(std::fixed << std::setprecision(3) << Settings[i].Value.fValue);
			break;

		case SETTING_VEC4:
			Settings[i].szDefault = STREAM(
				std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.x << " " <<
				std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.y << " " <<
				std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.z << " " <<
				std::fixed << std::setprecision(3) << Settings[i].Value.v4Value.w
			);
			break;

		case SETTING_VEC2:
			Settings[i].szDefault = STREAM(
				std::fixed << std::setprecision(3) << Settings[i].Value.v2Value.x << "," <<
				std::fixed << std::setprecision(3) << Settings[i].Value.v2Value.y
			);
			break;

		case SETTING_STRING:
			Settings[i].szDefault = Settings[i].Value.szValue;
		}

		WritePrivateProfileString(Settings[i].pszAppName.c_str(), Settings[i].pszKeyName.c_str(), Settings[i].szDefault.c_str(), szSettingsFile);
	}

	fclose(fp);

	return true;
}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const bool bDefault)
{
	Value_t val;

	val.bValue = bDefault;

	return Config_t(SETTING_BOOL, pszAppName, pszKeyName, (val.bValue) ? "on" : "off", val);
}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const int iDefault, const int iMin, const int iMax)
{
	Value_t val;

	val.iValue = iDefault;
	val.iMin = iMin;
	val.iMax = iMax;

	return Config_t(SETTING_INT, pszAppName, pszKeyName, std::to_string(val.iValue), val);
}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const float fDefault, const float fMin, const float fMax)
{
	Value_t val;

	val.fValue = fDefault;
	val.fMin = fMin;
	val.fMax = fMax;

	return Config_t(SETTING_FLOAT, pszAppName, pszKeyName, STREAM(std::fixed << std::setprecision(3) << val.fValue), val);

}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const ImVec4 v4Default)
{
	Value_t val;

	val.v4Value = v4Default;

	return Config_t(SETTING_VEC4, pszAppName, pszKeyName,

		STREAM(
			std::fixed << std::setprecision(3) << val.v4Value.x << " " <<
			std::fixed << std::setprecision(3) << val.v4Value.y << " " <<
			std::fixed << std::setprecision(3) << val.v4Value.z << " " <<
			std::fixed << std::setprecision(3) << val.v4Value.w
		),

		val);
}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const char* pszDefault)
{
	Value_t val;

	val.szValue = pszDefault;

	return Config_t(SETTING_STRING, pszAppName, pszKeyName,

		val.szValue,

		val);
}

Config_t Bot_AddConfig(const std::string pszAppName, const std::string pszKeyName, const ImVec2 v2Default)
{
	Value_t val;

	val.v2Value = v2Default;

	return Config_t(SETTING_VEC2, pszAppName, pszKeyName,

		STREAM(
			std::fixed << std::setprecision(3) << val.v2Value.x << "," <<
			std::fixed << std::setprecision(3) << val.v2Value.y
		),

		val);
}









