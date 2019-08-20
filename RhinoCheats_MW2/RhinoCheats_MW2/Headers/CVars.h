#pragma once

#include "../Headers/ImGUI/imgui.h"

#define SETTINGS_FILE_NAME "rc_config.ini"

#define LIMIT_VALUE(Val,Min,Max)	\
	if( (Val) > (Max) ) (Val) = (Max); \
	else if( (Val) < (Min) ) (Val) = (Min);

typedef enum
{
	SETTING_BOOL,
	SETTING_INT,
	SETTING_FLOAT,
	SETTING_VEC4,
	SETTING_STRING,
	SETTING_VEC2,
	SETTING_NONE
} SettingType_t;

typedef enum
{
	//Aim bot	
	aim_enabled,
	aim_key,
	aim_mode,
	auto_shoot,
	auto_knife,
	anti_antiaim,
	team_killer,
	fov,
	auto_wall,
	head_only,
	aim_prediction,
	enemies_with_riotshield,


	//Esp	
	esp_enabled,
	esp_enemy,
	esp_enemy_color,
	esp_visible_color,
	esp_friendly,
	esp_friendly_color,
	esp_name,
	esp_name_boxed,
	esp_weapon,
	esp_conected_bones,
	esp_misc,
	esp_misc_items,
	esp_misc_items_color,
	esp_misc_explosives,
	esp_misc_explosives_color,
	esp_misc_devices,
	esp_misc_devices_color,
	esp_compass_radar,
	esp_compass_radar_radius,
	esp_radar,
	esp_radar_x,
	esp_radar_y,
	esp_radar_w,
	esp_radar_h,
	esp_bright_colors,
	esp_box,
	esp_snap_lines,

	//D3D
	d3d_enabled,
	d3d_chams,
	d3d_chams_wireframe,
	d3d_chams_axis,
	d3d_axis_color,
	d3d_axis_visible_color,
	d3d_chams_allies,
	d3d_allies_color,
	d3d_allies_visible_color,
	d3d_chams_weapons,
	d3d_weapon_color,
	d3d_weapon_visible_color,
	d3d_weapon_customtexture,
	d3d_nosky,
	d3d_glass,
	d3d_nohands,
	d3d_noweapon,
	d3d_tripmap,
	d3d_nosmoke,

	//Misc	
	force_fov,
	fullbright,
	gun_x,
	gun_y,
	gun_z,
	kill_spam,
	kill_spam_text,
	no_recoil,
	no_spread,
	silent_aim,
	inverse_troller,
	kill_icons,
	kill_icons_origin,
	kill_icons_destination,
	kill_icons_size,
	kill_icons_count,
	kill_icons_rotate,
	kill_icons_type,
	draw_clock, //<--
	show_console,
	show_info,
	names_stealer,
	laser,
	kill_sounds,
	third_person,
	afk_mode,
	anti_aim, //rotation - shield
	rainbow_mode,
	draw_crosshair, //one color, constantly changing colors	
	drunken_style,
	watermark,

	//Host
	host_forcehost,
	host_map,
	host_gametype,
	host_autowall,
	host_masskill,
	host_unlimited_ammo, //<--
	host_antileave,
	host_unfair,
	host_rapidfire, //<--
	host_noclip,
	host_speed,
	host_god,
	host_partysize,
	host_killcam,
	host_teleport, //<--
	host_jump, //<--
	host_visions,
	host_visions_time,
	host_clap,
	host_killstreak,
	host_glitchgun,
	host_heareveryone,

	host_antiendgame,

	//Style
	style_color,
	style_font,
	style_rainbow,


		

	window_radar_pos,
	window_radar_size,

	window_info_pos,
	window_info_size,

	window_main_pos,
	window_main_size,

	window_console_pos,
	window_console_size,



	Configs_

} Configs_t;

typedef struct V
{
	int			iValue;
	int			iMin;
	int			iMax;

	float		fValue;
	float		fMin;
	float		fMax;

	bool		bValue;

	ImVec4      v4Value;
	ImVec2      v2Value;

	std::string	szValue;

	V()
	{
		iValue = 0;
		iMin = 0;
		iMax = 0;

		fValue = 0.0f;
		fMin = 0.0f;
		fMax = 0.0f;

		bValue = false;

		v4Value = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		v2Value = ImVec2(0.0f, 0.0f);
	}
	~V() {}

} Value_t;

class Config_t
{
public:
	SettingType_t eType;
	Value_t Value;
	
	std::string	pszAppName;
	std::string	pszKeyName;	
	std::string	szDefault;

	Config_t()
	{
	}

	Config_t(SettingType_t eType, std::string pszAppName, std::string pszKeyName, std::string szDefault, Value_t val)
	{
		this->eType = eType;

		this->pszAppName = pszAppName;
		this->pszKeyName = pszKeyName;
		this->szDefault = szDefault;

		this->Value.iValue = val.iValue;
		this->Value.iMin = val.iMin;
		this->Value.iMax = val.iMax;
		this->Value.fValue = val.fValue;
		this->Value.fMin = val.fMin;
		this->Value.fMax = val.fMax;
		this->Value.bValue = val.bValue;
		this->Value.v4Value = ImVec4(val.v4Value.x, val.v4Value.y, val.v4Value.z, val.v4Value.w);
		this->Value.v2Value = ImVec2(val.v2Value.x, val.v2Value.y);
		this->Value.szValue = val.szValue;			
	}
};

//========================================================================

extern Config_t Settings[Configs_];
extern char		szSettingsFile[MAX_PATH];

void		Bot_InitSettings();
bool		Bot_LoadSettings();
bool		Bot_SaveSettings();