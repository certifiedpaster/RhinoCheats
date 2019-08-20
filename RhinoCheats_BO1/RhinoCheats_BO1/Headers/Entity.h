#pragma once

enum TAGS
{
	/*Head*/  j_helmet, j_head, j_neck
	/*Arms*/, j_shoulder_le, j_shoulder_ri, j_elbow_le, j_elbow_ri, j_wrist_le, j_wrist_ri
	/*Back*/, j_mainroot, j_spineupper, j_spinelower, j_spine4
	/*Legs*/, j_hip_ri, j_hip_le, j_knee_le, j_knee_ri, j_ankle_ri, j_ankle_le, NUM_TAGS
};

class Entity_t
{
public:	

	std::vector<std::string> tagNames = {

		/*Head*/          "j_helmet"     , "j_head"         , "j_neck"
		/*Arms*/        , "j_shoulder_le", "j_shoulder_ri"  , "j_elbow_le"     , "j_elbow_ri", "j_wrist_le", "j_wrist_ri"
		/*Back*/        , "j_mainroot"   , "j_spineupper"   , "j_spinelower" , "j_spine4"
		/*Legs*/        , "j_hip_ri"     , "j_hip_le"       , "j_knee_le"    , "j_knee_ri" , "j_ankle_ri", "j_ankle_le"
	};

	std::vector<WORD> bones;	

	Entity_t()		
	{	
		dY, dX = 0.0f;		

		screen[2] = { 0.0f };
		base[2] = { 0.0f };

		vHead[3] = { 0.0f };

		colVisible = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		misc_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	bool IsValid(int i);
	bool IsEnemy(int i);
	bool IsBoneVisible(centity_t* pEntity, int iBone);

	void DrawEsp();

private:
	
	float dY, dX;	
	
	vec2_t screen, base;
	vec3_t vHead; 
	ImVec4 colVisible;
	ImVec4 misc_color;	
};

extern Entity_t Entity;