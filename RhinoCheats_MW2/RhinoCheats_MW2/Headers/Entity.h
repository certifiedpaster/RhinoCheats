#pragma once

enum TAGS
{
	tag_eye, j_head, j_neck, j_helmet, j_spine4, j_spineupper, j_spinelower, j_shoulder_ri, j_shoulder_le,
	j_mainroot, pelvis, j_hip_ri, j_hip_le, j_clavicle_ri, j_clavicle_le, j_knee_ri, j_knee_le, j_elbow_ri, j_elbow_le, j_wrist_ri,
	j_wrist_le, j_ankle_ri, j_ankle_le, tag_shield_back, tag_weapon_left, NUM_TAGS
};

class Entity_t
{
public:

	std::vector<std::string> tagNames = {
		"tag_eye", "j_head", "j_neck", "j_helmet", "j_spine4", "j_spineupper", "j_spinelower", "j_shoulder_ri", "j_shoulder_le",
		"j_mainroot", "pelvis",	"j_hip_ri", "j_hip_le", "j_clavicle_ri", "j_clavicle_le", "j_knee_ri", "j_knee_le", "j_elbow_ri", "j_elbow_le", "j_wrist_ri",
		"j_wrist_le", "j_ankle_ri", "j_ankle_le", "tag_shield_back", "tag_weapon_left"
	};

	std::vector<WORD> bones;

	Entity_t()
	{
		icon = 0;

		x3, y3, dY, dX = 0.0f;

		isEnemy = false;
		bIsInsideSquareRadar = false;

		screen[2] = { 0.0f };
		base[2] = { 0.0f };

		vHead[3] = { 0.0f };

		colVisible = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		misc_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	bool IsValid(int i);
	bool IsEnemy(int i);
	bool IsUsingRiot(int i);

	void DrawEsp();

private:
	int icon;
	float x3, y3, dY, dX;
	bool isEnemy;
	bool bIsInsideSquareRadar;
	vec2_t screen, base;
	vec3_t vHead;
	ImVec4 colVisible;
	ImVec4 misc_color;
};

extern Entity_t Entity;