#pragma once

class cRadar_t
{
public:
	float rw, rh, screenX, screenY, fRadarAngle;

	void CalcRadarPoint(float* origin, float* screenx, float* screeny);
};


extern cRadar_t	Radar;