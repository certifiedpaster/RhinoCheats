#include "stdafx.h"

Drawing_t Drawing;

int Drawing_t::CG_DrawStrlen(char *str)
{
	int count = 0;

	while (*str)
	{
		if (Q_IsColorString(str))
		{
			str += 2;
		}
		else
		{
			count++;
			str++;
		}
	}

	return count;
}

void Drawing_t::DrawShader(float x, float y, float w, float h, ImVec4 pColor, int Shader)
{
	if (Shader)
	{

		float vColor[] = { pColor.x, pColor.y, pColor.z, pColor.w };

		R_AddDrawStretchPic(x, y, w, h, 0, 0, 0, 0, vColor, Shader);
	}
}

void Drawing_t::DrawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size)
{
	float h1, l1, l2, x, y, angle;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrtf(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2.0f);
	y = y1 + (h1 / 2.0f);
	angle = atanf(h1 / l1) * 57.29577951308232087f;

	float vColor[] = { color.x, color.y, color.z, color.w };
	DrawRotatedPic(
		x, y, l2, size, 
		0.0f, 0.0f, 1.0f, 1.0f,	
		angle, vColor, *(DWORD*)Offsets::white);
}

void Drawing_t::DrawRect(float x, float y, float w, float h, float thick, ImVec4 color, bool shadow)
{
	if (shadow)
	{
		DrawShader(x - 1, y - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x + w - thick - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x - 1, y + h - thick - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
	}

	DrawShader(x, y, w, thick, color, *(DWORD*)Offsets::white);
	DrawShader(x, y, thick, h, color, *(DWORD*)Offsets::white);
	DrawShader(x + w - thick, y, thick, h, color, *(DWORD*)Offsets::white);
	DrawShader(x, y + h - thick, w, thick, color, *(DWORD*)Offsets::white);
}

void Drawing_t::DrawRectFilled(float x, float y, float w, float h, float thick, ImVec4 color, ImVec4 filledcolor, bool shadow)
{	
	DrawShader(x, y, w, h, filledcolor, *(DWORD*)Offsets::white);

	if (shadow)
	{
		DrawShader(x - 1, y - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x + w - thick - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
		DrawShader(x - 1, y + h - thick - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD*)Offsets::white);
	}

	DrawShader(x, y, w, thick, color, *(DWORD*)Offsets::white);
	DrawShader(x, y, thick, h, color, *(DWORD*)Offsets::white);
	DrawShader(x + w - thick, y, thick, h, color, *(DWORD*)Offsets::white);
	DrawShader(x, y + h - thick, w, thick, color, *(DWORD*)Offsets::white);


}

void Drawing_t::DrawCorners(int x, int y, int w, int h, int length, ImVec4 pColor)
{
	DrawShader(x, y, length, 1, pColor, *(DWORD*)Offsets::white); //top right a
	DrawShader(x, y, 1, length, pColor, *(DWORD*)Offsets::white); //top right b
	DrawShader(x + w - length, y, length, 1, pColor, *(DWORD*)Offsets::white); //top left a
	DrawShader(x + w, y, 1, length, pColor, *(DWORD*)Offsets::white); //top left b


	DrawShader(x, y + h, length, 1, pColor, *(DWORD*)Offsets::white); //bottom right a
	DrawShader(x, y + h - length, 1, length, pColor, *(DWORD*)Offsets::white); //bottom right b
	DrawShader(x + w - length, y + h, length, 1, pColor, *(DWORD*)Offsets::white); //bottom left a
	DrawShader(x + w, y + h - length, 1, length, pColor, *(DWORD*)Offsets::white); //bottom left b
}

void Drawing_t::DrawCornersFilled(int x, int y, int w, int h, int length, ImVec4 pColor, ImVec4 filledcolor)
{
	DrawShader(x, y, w, h, filledcolor, *(DWORD*)Offsets::white);
	

	DrawShader(x, y, length, 1, pColor, *(DWORD*)Offsets::white); //top right a
	DrawShader(x, y, 1, length, pColor, *(DWORD*)Offsets::white); //top right b
	DrawShader(x + w - length, y, length, 1, pColor, *(DWORD*)Offsets::white); //top left a
	DrawShader(x + w, y, 1, length, pColor, *(DWORD*)Offsets::white); //top left b

	DrawShader(x, y + h, length, 1, pColor, *(DWORD*)Offsets::white); //bottom right a
	DrawShader(x, y + h - length, 1, length, pColor, *(DWORD*)Offsets::white); //bottom right b
	DrawShader(x + w - length, y + h, length, 1, pColor, *(DWORD*)Offsets::white); //bottom left a
	DrawShader(x + w, y + h - length, 1, length, pColor, *(DWORD*)Offsets::white); //bottom left b


}

void Drawing_t::DrawGlowString(float x, float y, qfont_t  font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, char * text, ...)
{
	return;
}

void Drawing_t::DrawGlowStringBoxed(float x, float y, qfont_t  font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char * text, ...)
{
	return;
}

void Drawing_t::DrawString(float x, float y, qfont_t  font, float size, ImVec4 Color, float angle, bool centered, char * text, ...)
{
	if (text == nullptr || !*text)
		return;

	char buffer[256] = { 0 };
	va_list vaList;
	va_start(vaList, text);
	vsprintf_s(buffer, text, vaList);
	va_end(vaList);
	
	float vColor[] = { Color.x, Color.y, Color.z, Color.w };

	int len = CG_DrawStrlen(buffer);
	int multiple = 0;

	if (centered)
	{		
		if (font == Fonts->smallfont)
			multiple = 4;
		else if (font == Fonts->normalfont || font == Fonts_Dev->smallDevFont || font == Fonts->consolefont)
			multiple = 6;
		else if (font == Fonts->bigfont || font == Fonts->boldfont || font == Fonts_Dev->bigDevFont)
			multiple = 12;
		else
			multiple = 18;
	}	

	float finalX = x - (len * size * multiple);	

	R_AddCmdDrawText(buffer, len, font, finalX, y, size, size, 0, vColor, 3);


}

void Drawing_t::DrawStringBoxed(float x, float y, qfont_t  font, float size, ImVec4 Color, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char * text, ...)
{
	return;
}


//========================================================================

void Drawing_t::DrawCompassIcon(float x, float y, float w, float h, vec3_t origin, vec3_t dest, ImVec4 cor, int shader, float fAngle)
{
	if (!shader)
		return;
	
	float angle, len;
	vec3_t v1, angles;

	VectorCopy(dest, v1);
	VectorSubtract(origin, v1, v1);
	len = Math.VectorLength(v1);
	Math.VectorNormalize(v1);
	Math.VecToAngles(v1, angles);

	if (v1[0] == 0 && v1[1] == 0 && v1[2] == 0)
		return;

	angles[YAW] = Math.AngleSubstract(rdViewAngles->angles[1], angles[YAW]);

	angle = ((angles[YAW] + 180.f) / 360.f - (0.50f / 2.f)) * 6.28318530717958647f;

	w /= 2;
	h /= 2;

	x += w;
	y += h;

	w = sqrt((w * w) + (h * h)) / 3.f * 2.f * 0.9f;

	x = x + (cos(angle) * (w));
	y = y + (sin(angle) * (w));

	len = 1 - min(1.f, len / 2000.f);

	float vColor[] = { cor.x, cor.y, cor.z, cor.w };

	DrawRotatedPic(x - (14 * len + 4) / 2,
		y - (14 * len + 4) / 2,
		14 * len + 30,
		14 * len + 30, 0.0f, 0.0f, 1.0f, 1.0f, fAngle, vColor, shader);
}

//========================================================================

void Drawing_t::InitializeRainBow()
{	
	colRainBow.x = 0;
	colRainBow.y = 0;
	colRainBow.z = 0;	
	colRainBow.w = 1.0f;		
}

void Drawing_t::NormalCrosshair(float x, float y, bool rb)
{
	//Black
	DrawShader(x - 18, y - .5f, 36, 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD*)Offsets::white);
	DrawShader(x - .5f, y - 18, 1, 36, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD*)Offsets::white);
	
	if (rb)
	{
		DrawShader(x - 6.5f, y - .5f, 13, 1, colRainBow, *(DWORD*)Offsets::white);
		DrawShader(x - .5f, y - 6.5f, 1, 13, colRainBow, *(DWORD*)Offsets::white);		
	}
	else
	{
		//Cyan-Default color
		DrawShader(x - 6.5f, y - .5f, 13, 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), *(DWORD*)Offsets::white);
		DrawShader(x - .5f, y - 6.5f, 1, 13, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), *(DWORD*)Offsets::white);
	}
	
}

void Drawing_t::LilliCrosshair(float x, float y, float size, float thick, bool shadow, bool rainbow, bool shakey, int shakefactor, bool CrosshairAim)
{
	return;
}

void Drawing_t::DrawCrossHair(int typeCrosshair)
{	
	float mid_Width = refdef->width / 2.f;
	float mid_Height = refdef->height / 2.f;	

	NormalCrosshair(mid_Width, mid_Height, 0);
}

//========================================================================

void Drawing_t::DrawLineBone(centity_t * pEnt, WORD Bone1, WORD Bone2, ImVec4 Color)
{
	if (Engine.Getvectorfromtag(pEnt, Bone1, vBone1) && Engine.Getvectorfromtag(pEnt, Bone2, vBone2))
	{
		if (Engine.WorldToScreen(vBone1, Screen1) && Engine.WorldToScreen(vBone2, Screen2))
			DrawLine(Screen1[0], Screen1[1], Screen2[0], Screen2[1], Color, 3);
	}
}

void Drawing_t::DrawCube(vec3_t Min, vec3_t Max, ImVec4 col, float &x, float &y)
{
	VectorCopy(Min, v1);
	VectorCopy(Max, v8);
	VectorCopy(Min, v2);
	VectorCopy(Min, v3);
	VectorCopy(Min, v4);
	v2[0] = Max[0];
	v3[1] = Max[1];
	v4[0] = Max[0];
	v4[1] = Max[1];

	VectorCopy(v1, v5);
	v5[2] = Max[2];
	VectorCopy(v2, v6);
	v6[2] = Max[2];
	VectorCopy(v3, v7);
	v7[2] = Max[2];

	if (!Engine.WorldToScreen(v1, v1))
		return;
	if (!Engine.WorldToScreen(v2, v2))
		return;
	if (!Engine.WorldToScreen(v3, v3))
		return;
	if (!Engine.WorldToScreen(v4, v4))
		return;
	if (!Engine.WorldToScreen(v5, v5))
		return;
	if (!Engine.WorldToScreen(v6, v6))
		return;
	if (!Engine.WorldToScreen(v7, v7))
		return;
	if (!Engine.WorldToScreen(v8, v8))
		return;	

	x = v5[0];
	y = v5[1];

	Drawing.DrawLine(v1[0], v1[1], v2[0], v2[1], col, 1);
	Drawing.DrawLine(v1[0], v1[1], v3[0], v3[1], col, 1);
	Drawing.DrawLine(v1[0], v1[1], v5[0], v5[1], col, 1);
	Drawing.DrawLine(v2[0], v2[1], v4[0], v4[1], col, 1);
	Drawing.DrawLine(v2[0], v2[1], v6[0], v6[1], col, 1);
	Drawing.DrawLine(v3[0], v3[1], v4[0], v4[1], col, 1);
	Drawing.DrawLine(v3[0], v3[1], v7[0], v7[1], col, 1);
	Drawing.DrawLine(v4[0], v4[1], v8[0], v8[1], col, 1);
	Drawing.DrawLine(v5[0], v5[1], v6[0], v6[1], col, 1);
	Drawing.DrawLine(v5[0], v5[1], v7[0], v7[1], col, 1);
	Drawing.DrawLine(v7[0], v7[1], v8[0], v8[1], col, 1);
	Drawing.DrawLine(v6[0], v6[1], v8[0], v8[1], col, 1);

}

//========================================================================

void Drawing_t::DrawClock()
{
	return;
}


void Drawing_t::DrawHealth() {	
	
	//Health
	if (!cg->health)
		return;

	float Health_Value = static_cast<float>(cg->health);
	if (Health_Value > 100)
		Health_Value = 100;

	float fRed = 0;
	float fGreen = 255;
	float fSub = 255;
	fSub -= Health_Value * 2.55f;
	fGreen -= fSub;
	fRed += fSub;
	fGreen /= 255;
	fRed /= 255;
	MAKERGBA(colHealth, fRed, fGreen, fGreen, 1.0f);	

	Drawing.DrawShader(refdef->width / 2.0f - 52.0f, refdef->height - 22, 104.f, 13.f, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD*)Offsets::white);
	Drawing.DrawShader(refdef->width / 2.0f - 50.0f, refdef->height - 20, Health_Value, 9.f, colHealth, *(DWORD*)Offsets::white);
}

