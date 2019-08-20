#pragma once

#define Q_COLOR_ESCAPE				'^'
#define Q_IsColorString( p )		( p && *(p) == Q_COLOR_ESCAPE && *((p)+1) && *((p)+1) != Q_COLOR_ESCAPE )
#define	MAKERGBA( v, r, g, b, a )   v.x=r;v.y=g;v.z=b;v.w=a

#define FONTSCALE 0.7f

//========================================================================

class Drawing_t
{
public:
	int CG_DrawStrlen(char *str);

	void DrawShader(float x, float y, float w, float h, ImVec4 pColor, int Shader);
	void DrawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size);
	void DrawRect(float x, float y, float w, float h, float thick, ImVec4 color, bool shadow);
	void DrawRectFilled(float x, float y, float w, float h, float thick, ImVec4 color, ImVec4 filledcolor, bool shadow);
	void DrawCorners(int x, int y, int w, int h, int length, ImVec4 pColor);
	void DrawCornersFilled(int x, int y, int w, int h, int length, ImVec4 pColor, ImVec4 filledcolor);

	void DrawGlowString(float x, float y, qfont_t font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, char *text, ...);
	void DrawGlowStringBoxed(float x, float y, qfont_t font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char *text, ...);
		
	void DrawString(float x, float y, qfont_t font, float size, ImVec4 Color, float angle, bool centered, char *text, ...);
	void DrawStringBoxed(float x, float y, qfont_t font, float size, ImVec4 Color, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char *text, ...);
		
	//========================================================================

	void DrawCompassIcon(float x, float y, float w, float h, vec3_t origin, vec3_t dest, ImVec4 cor, int shader, float fAngle);		

	void InitializeRainBow();

	void NormalCrosshair(float x, float y, bool rb);	
	void LilliCrosshair(float x, float y, float size, float thick, bool shadow, bool rainbow, bool shakey, int shakefactor, bool CrosshairAim);
	void DrawCrossHair(int typeCrosshair);

	void DrawLineBone(centity_t * pEnt, WORD Bone1, WORD Bone2, ImVec4 Color);
	void DrawCube(vec3_t Min, vec3_t Max, ImVec4 col, float &x, float &y);

	void DrawClock();	
	void DrawHealth();	

	ImVec4 colRainBow;

private:

	//for line-bone
	vec3_t vBone1, vBone2;
	vec2_t Screen1, Screen2;

	//for 3dBox
	vec3_t v1;
	vec3_t v2;
	vec3_t v3;
	vec3_t v4;
	vec3_t v5;
	vec3_t v6;
	vec3_t v7;
	vec3_t v8;

	ImVec4  colHealth;

};

extern Drawing_t Drawing;
