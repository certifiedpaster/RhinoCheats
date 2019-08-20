#include "stdafx.h"



//========================================================================

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
		DrawRotatedPic(GSM(), x, y, w, h, 0.0f, vColor, Shader);
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
	DrawRotatedPic(GSM(), x, y, l2, size, angle, vColor, *(DWORD *)Offsets::white);
}

void Drawing_t::DrawRect(float x, float y, float w, float h, float thick, ImVec4 color, bool shadow)
{
	if (shadow)
	{
		DrawShader(x - 1, y - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x + w - thick - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x - 1, y + h - thick - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
	}

	DrawShader(x, y, w, thick, color, *(DWORD *)Offsets::white);
	DrawShader(x, y, thick, h, color, *(DWORD *)Offsets::white);
	DrawShader(x + w - thick, y, thick, h, color, *(DWORD *)Offsets::white);
	DrawShader(x, y + h - thick, w, thick, color, *(DWORD *)Offsets::white);
}

void Drawing_t::DrawRectFilled(float x, float y, float w, float h, float thick, ImVec4 color, ImVec4 filledcolor, bool shadow)
{	
	DrawShader(x, y, w, h, filledcolor, *(DWORD *)Offsets::white);

	if (shadow)
	{
		DrawShader(x - 1, y - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x + w - thick - 1, y - 1, thick + 2, h + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
		DrawShader(x - 1, y + h - thick - 1, w + 2, thick + 2, ImVec4(0, 0, 0, 1), *(DWORD *)Offsets::white);
	}

	DrawShader(x, y, w, thick, color, *(DWORD *)Offsets::white);
	DrawShader(x, y, thick, h, color, *(DWORD *)Offsets::white);
	DrawShader(x + w - thick, y, thick, h, color, *(DWORD *)Offsets::white);
	DrawShader(x, y + h - thick, w, thick, color, *(DWORD *)Offsets::white);


}

void Drawing_t::DrawCorners(int x, int y, int w, int h, int length, ImVec4 pColor)
{
	DrawShader(x, y, length, 1, pColor, *(DWORD *)Offsets::white); //top right a
	DrawShader(x, y, 1, length, pColor, *(DWORD *)Offsets::white); //top right b
	DrawShader(x + w - length, y, length, 1, pColor, *(DWORD *)Offsets::white); //top left a
	DrawShader(x + w, y, 1, length, pColor, *(DWORD *)Offsets::white); //top left b


	DrawShader(x, y + h, length, 1, pColor, *(DWORD *)Offsets::white); //bottom right a
	DrawShader(x, y + h - length, 1, length, pColor, *(DWORD *)Offsets::white); //bottom right b
	DrawShader(x + w - length, y + h, length, 1, pColor, *(DWORD *)Offsets::white); //bottom left a
	DrawShader(x + w, y + h - length, 1, length, pColor, *(DWORD *)Offsets::white); //bottom left b
}

void Drawing_t::DrawCornersFilled(int x, int y, int w, int h, int length, ImVec4 pColor, ImVec4 filledcolor)
{
	DrawShader(x, y, w, h, filledcolor, *(DWORD *)Offsets::white);
	

	DrawShader(x, y, length, 1, pColor, *(DWORD *)Offsets::white); //top right a
	DrawShader(x, y, 1, length, pColor, *(DWORD *)Offsets::white); //top right b
	DrawShader(x + w - length, y, length, 1, pColor, *(DWORD *)Offsets::white); //top left a
	DrawShader(x + w, y, 1, length, pColor, *(DWORD *)Offsets::white); //top left b

	DrawShader(x, y + h, length, 1, pColor, *(DWORD *)Offsets::white); //bottom right a
	DrawShader(x, y + h - length, 1, length, pColor, *(DWORD *)Offsets::white); //bottom right b
	DrawShader(x + w - length, y + h, length, 1, pColor, *(DWORD *)Offsets::white); //bottom left a
	DrawShader(x + w, y + h - length, 1, length, pColor, *(DWORD *)Offsets::white); //bottom left b


}

void Drawing_t::DrawGlowString(float x, float y, qfont_t  font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, char * text, ...)
{
	if (text == nullptr || !*text)
		return;

	char buffer[256] = { 0 };
	va_list vaList;
	va_start(vaList, text);
	vsprintf_s(buffer, text, vaList);
	va_end(vaList);		

	if (centered)
	{
		x -= static_cast<float>(StrWidth(buffer, 0, font, 0)) * (size * 0.5f);
		y += static_cast<float>(StrHeight(font)) * (size * 0.5f);

		/*typedef void(__cdecl *t_sub_4B6720)(float *a1, float *a2, float *a3, float *a4);
		static t_sub_4B6720 psub_4B672 = (t_sub_4B6720)0x4B6720;
		float a4 = 0.0f;
		psub_4B672(&x, &y, &size, &a4);*/
	}

	float vColor1[] = { Color1.x, Color1.y, Color1.z, Color1.w };
	float vColor2[] = { Color2.x, Color2.y, Color2.z, Color2.w };

	DrawGlowText(buffer, CG_DrawStrlen(buffer), font, x, y, size, size, vColor1, angle, vColor2, 0, 0, 0, 0, 0, 0);

}

void Drawing_t::DrawGlowStringBoxed(float x, float y, qfont_t  font, float size, ImVec4 Color1, ImVec4 Color2, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char * text, ...)
{
	if (text == nullptr || !*text)
		return;

	char buffer[256] = { 0 };
	va_list vaList;
	va_start(vaList, text);
	vsprintf_s(buffer, text, vaList);
	va_end(vaList);

	float strheight = static_cast<float>(StrHeight(font));
	float strwidth = static_cast<float>(StrWidth(buffer, 0, font, 0));

	if (strwidth + 8 < entityBoxWidth)
		DrawRectFilled(entityBox, y - strheight / 2 - 4, entityBoxWidth, strheight + 8, 1, boxcolor, boxcolor_, true);
	else
		DrawRectFilled(x - strwidth / 2 - 4, y - strheight / 2 - 4, strwidth + 8, strheight + 8, 1, boxcolor, boxcolor_, true);
	if (!centered)
		centered = true;

	if (centered)
	{
		x -= strwidth / 2.0f;
		y += strheight / 2.0f;		
	}	

	float vColor1[] = { Color1.x, Color1.y, Color1.z, Color1.w };
	float vColor2[] = { Color2.x, Color2.y, Color2.z, Color2.w };

	DrawGlowText(buffer, CG_DrawStrlen(buffer), font, x, y, size, size, vColor1, angle, vColor2, 0, 0, 0, 0, 0, 0);

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

	if (centered)
	{
		x -= static_cast<float>(StrWidth(buffer, 0, font, 0)) * (size * 0.5f);
		y += static_cast<float>(StrHeight(font)) * (size * 0.5f);

		/*typedef void(__cdecl *t_sub_4B6720)(float *a1, float *a2, float *a3, float *a4);
		static t_sub_4B6720 psub_4B672 = (t_sub_4B6720)0x4B6720;
		float a4 = 0.0f;
		psub_4B672(&x, &y, &size, &a4);*/
	}

	float vColor[] = { Color.x, Color.y, Color.z, Color.w };	

	DrawGlowText(buffer, CG_DrawStrlen(buffer), font, x, y, size, size, vColor, angle, nullptr, 0, 0, 0, 0, 0, 0);
}

void Drawing_t::DrawStringBoxed(float x, float y, qfont_t  font, float size, ImVec4 Color, float angle, bool centered, ImVec4 boxcolor, ImVec4 boxcolor_, float entityBox, float entityBoxWidth, char * text, ...)
{
	if (text == nullptr || !*text)
		return;

	char buffer[256] = { 0 };
	va_list vaList;
	va_start(vaList, text);
	vsprintf_s(buffer, text, vaList);
	va_end(vaList);

	float strheight = static_cast<float>(StrHeight(font));
	float strwidth = static_cast<float>(StrWidth(buffer, 0, font, 0));	
	
	if (strwidth + 8 < entityBoxWidth)
		DrawRectFilled(entityBox, y - strheight / 2 - 4, entityBoxWidth, strheight + 8, 1, boxcolor, boxcolor_, true);
	else
		DrawRectFilled(x - strwidth / 2 - 4, y - strheight / 2 - 4, strwidth + 8, strheight + 8, 1, boxcolor, boxcolor_, true);
	if (!centered)
		centered = true;	

	if (centered)
	{
		x -= strwidth / 2.0f;
		y += strheight / 2.0f;		
	}

	float vColor[] = { Color.x, Color.y, Color.z, Color.w };

	DrawGlowText(buffer, CG_DrawStrlen(buffer), font, x, y, size, size, vColor, angle, nullptr, 0, 0, 0, 0, 0, 0);

}


//========================================================================

void Drawing_t::DrawCompassIcon(float x, float y, float w, float h, vec3_t origin, vec3_t dest, ImVec4 cor, int shader, float fAngle)
{
	float angle, len;
	vec3_t v1, angles;

	VectorCopy(dest, v1);
	VectorSubtract(origin, v1, v1);
	len = Math.VectorLength(v1);
	VectorNormalize(v1);
	Math.VecToAngles(v1, angles);

	if (v1[0] == 0 && v1[1] == 0 && v1[2] == 0)
		return;

	angles[YAW] = Math.AngleSubstract(refdef->rdViewAngle[1], angles[YAW]);

	angle = ((angles[YAW] + 180.f) / 360.f - (0.50f / 2.f)) * 6.28318530717958647f;

	w /= 2;
	h /= 2;

	x += w;
	y += h;

	w = sqrt((w * w) + (h * h)) / 3.f * 2.f * 0.9f;

	x = x + (cos(angle) * (w + Settings[esp_compass_radar_radius].Value.fValue));
	y = y + (sin(angle) * (w + Settings[esp_compass_radar_radius].Value.fValue));

	len = 1 - min(1.f, len / 2000.f);



	float vColor[] = { cor.x, cor.y, cor.z, cor.w };

	DrawRotatedPic(GSM(),
		x - (14 * len + 4) / 2,
		y - (14 * len + 4) / 2,
		14 * len + 30,
		14 * len + 30, fAngle, vColor, shader);
}

//========================================================================

void Drawing_t::InitializeRainBow()
{	
	if (Settings[rainbow_mode].Value.bValue)
	{
		colRainBow.x = D3D::colRainBow.x;
		colRainBow.y = D3D::colRainBow.y;
		colRainBow.z = D3D::colRainBow.z;

		if (Settings[esp_bright_colors].Value.bValue)
			colRainBow.w = 1.0f;
		else
			colRainBow.w = 0.5;
	}	
}

void Drawing_t::NormalCrosshair(float x, float y, bool rb)
{
	//Black
	DrawShader(x - 18, y - .5f, 36, 1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
	DrawShader(x - .5f, y - 18, 1, 36, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
	
	if (rb)
	{
		DrawShader(x - 6.5f, y - .5f, 13, 1, colRainBow, *(DWORD *)Offsets::white);
		DrawShader(x - .5f, y - 6.5f, 1, 13, colRainBow, *(DWORD *)Offsets::white);		
	}
	else
	{
		//Cyan-Default color
		DrawShader(x - 6.5f, y - .5f, 13, 1, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), *(DWORD *)Offsets::white);
		DrawShader(x - .5f, y - 6.5f, 1, 13, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), *(DWORD *)Offsets::white);
	}

	
}

void Drawing_t::LilliCrosshair(float x, float y, float size, float thick, bool shadow, bool rainbow, bool shakey, int shakefactor, bool CrosshairAim)
{
	//Cyan
	ImVec4 color(0.0f, 1.0f, 1.0f, 1.0f);

	if (rainbow)
	{
		color.x = colRainBow.x;
		color.y = colRainBow.y;
		color.z = colRainBow.z;
		color.w = colRainBow.w;
	}

	if (shakey)
	{
		x -= shakefactor / 2;
		y -= shakefactor / 2;
		x += mtRandom.randInt(shakefactor);
		y += mtRandom.randInt(shakefactor);
	}

	float spread, MaxSpread;
	R_WeaponSpread(cg, cg_entities[cg->clientNum].nextState.weaponID, &spread, &MaxSpread);

	if (CrosshairAim && Aim.isReady[Aim_t::isReadyforFire])
	{
		vec2_t Screen;
		if (Engine.WorldToScreen(Aim.vAimLocation, Screen))
		{
			DrawLine(x, y, Screen[0], Screen[1], color, 1.0f);
			DrawShader(Screen[0] - 5, Screen[1] - 1, 10, 2, color, *(DWORD *)Offsets::white);
			DrawShader(Screen[0] - 1, Screen[1] - 5, 2, 10, color, *(DWORD *)Offsets::white);
		}

		DrawRect(x - (size + spread + 8) / 2, y - (size + spread + 8) / 2, size + spread + 8, size + spread + 8, thick / 2, color, true);
	}

	static bool DC = true;
	if (!spread)
	{
		if (shadow)
		{
			DrawShader(x - size / 2 - 1, y - thick / 2 - 1, size + 2, thick + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
			DrawShader(x - thick / 2 - 1, y - size / 2 - 1, thick + 2, size + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
		}

		DrawShader(x - size / 2, y - thick / 2, size, thick, color, *(DWORD *)Offsets::white);
		DrawShader(x - thick / 2, y - size / 2, thick, size, color, *(DWORD *)Offsets::white);

		if (DC)
		{
			DrawShader(x - size / 4, y - thick / 2, size / 2, thick, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
			DrawShader(x - thick / 2, y - size / 4, thick, size / 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
		}
	}
	else
	{
		int denominator = 50;
		if (*(int*)Offsets::host)
			denominator = 100;

		if (shadow)
		{
			spread = spread * *(float*)Offsets::nospread_numerator / denominator;
			DrawShader(x - size / 2 - spread / 2 - 1, y - thick / 2 - 1, size / 2 + 2, thick + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
			DrawShader(x + spread / 2 - 1, y - thick / 2 - 1, size / 2 + 2, thick + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
			DrawShader(x - thick / 2 - 1, y - size / 2 - spread / 2 - 1, thick + 2, size / 2 + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
			DrawShader(x - thick / 2 - 1, y + spread / 2 - 1, thick + 2, size / 2 + 2, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
		}

		spread = spread * *(float*)Offsets::nospread_numerator / denominator;
		DrawShader(x - size / 2 - spread / 2, y - thick / 2, size / 2, thick, color, *(DWORD *)Offsets::white);
		DrawShader(x + spread / 2, y - thick / 2, size / 2, thick, color, *(DWORD *)Offsets::white);
		DrawShader(x - thick / 2, y - size / 2 - spread / 2, thick, size / 2, color, *(DWORD *)Offsets::white);
		DrawShader(x - thick / 2, y + spread / 2, thick, size / 2, color, *(DWORD *)Offsets::white);
	}
}

void Drawing_t::DrawCrossHair(int typeCrosshair)
{
	if (!typeCrosshair)
		return;

	float mid_Width = refdef->width / 2.f;
	float mid_Height = refdef->height / 2.f;

	switch (typeCrosshair)
	{
	case 1:	
		NormalCrosshair(mid_Width, mid_Height, Settings[rainbow_mode].Value.bValue);
		break;

	case 2:
		LilliCrosshair(mid_Width, mid_Height, 13.0f, 1.0f, false, Settings[rainbow_mode].Value.bValue, false, 0, true);

	}
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
	if (!Settings[draw_clock].Value.bValue)
		return;

	static char Timestruct[13] = "hh':'mm':'ss";
	GetTimeFormat(NULL, NULL, NULL, NULL, Timestruct, sizeof(Timestruct));

	Drawing.DrawGlowString(refdef->width -120, 20, Fonts->hudsmallfont, FONTSCALE, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 1.0f, 1.0f, 1.0f), 0, 0, "%s", Timestruct);
}


void Drawing_t::DrawHealth() {	
	
	//Drawing.DrawString(refdef->width - 225.0f, 490, Fonts->smallfont, 0.7f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 0, 0, "X: %f, Y: %f", fValueX, fValueY);

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

	//refdef->width - 685.f

	Drawing.DrawShader(refdef->width / 2.0f - 52.0f, refdef->height - 22, 104.f, 13.f, ImVec4(0.0f, 0.0f, 0.0f, 1.0f), *(DWORD *)Offsets::white);
	Drawing.DrawShader(refdef->width / 2.0f - 50.0f, refdef->height - 20, Health_Value, 9.f, colHealth, *(DWORD *)Offsets::white);
}

