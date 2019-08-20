#include "stdafx.h"

cRadar_t		Radar;

void cRadar_t::CalcRadarPoint(float* origin, float* screenX, float* screenY)
{
	float fy = Math.Radians(pViewMatrix->vaX);
	float fc = cosf(fy);
	float fs = sinf(fy);

	float dx = origin[0] - refdef->vieworg[0];
	float dy = origin[1] - refdef->vieworg[1];

	float px = dy * (-fc) + (dx * fs);
	float py = dx * (-fc) - (dy * fs);

	float radarX = Settings[esp_radar_x].Value.fValue;  
	float radarY = Settings[esp_radar_y].Value.fValue;  

	float irrx = (radarX + (rw / 2.0f)) + (px / 10.0f);
	float irry = (radarY + (rh / 2.0f)) + (py / 10.0f);

	// force on border of radar if off range
	if (irrx < radarX + 5.0f)	     irrx = radarX + 5.0f;  // +5  to Fit on Imgui Radar X
	if (irrx > radarX + rw - 5.0f)	 irrx = radarX + rw - 5.0f; 
	if (irry < radarY + 25.0f)	     irry = radarY + 25.0f;
	if (irry > radarY + rh - 5.0f)   irry = radarY + rh - 5.0f;

	*screenX = irrx;
	*screenY = irry;	


}
