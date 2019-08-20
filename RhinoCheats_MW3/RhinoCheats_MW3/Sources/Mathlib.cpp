#include "stdafx.h"

Math_t Math;

//========================================================================


void Math_t::MakeVector(vec3_t angle, vec3_t vector)
{
	float pitch = angle[0] * 0.01745329251994329576f;
	float yaw = angle[1] * 0.01745329251994329576f;

	vector[2] = -sinf(pitch);

	float tmp = cosf(pitch);
	vector[1] = sinf(yaw)*tmp;
	vector[0] = -tmp * -cosf(yaw);
}

float Math_t::VectorLength(vec3_t v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Math_t::GetDistance(float *p1, float *p2)
{
	vec3_t v;
	VectorSubtract(p2, p1, v);
	return VectorLength(v);	
}

void Math_t::VecToAngles(float * value1, float * angles)
{	
	float yaw, pitch;

	if (value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if (value1[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		if (value1[0])
			yaw = (atan2(value1[1], value1[0]) * 57.29577951308232087f);
		else if (value1[1] > 0)
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if (yaw < 0.0f)
			yaw += 360.0f;
	
		pitch = (atan2(value1[2], sqrt((value1[0] * value1[0]) + (value1[1] * value1[1]))) * 57.29577951308232087f);

		if (pitch < 0.0f)
			pitch += 360;
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
}

void Math_t::ToEulerAngles(vec3_t in, float * angles)
{
	float Length = VectorLength(in);

	if (Length > 0)
	{
		angles[0] = (atan2(-in[2], Length) * 57.29577951308232087f);

		if (angles[0] < 0)
		{
			angles[0] += 360;
		}

		angles[1] = (atan2(in[1], in[0]) * 57.29577951308232087f);

		if (angles[1] < 0)
		{
			angles[1] += 360;
		}
	}
	else
	{
		angles[0] = (in[2] > 0.0f) ? 270 : 90;
		angles[1] = 0;
	}
}

void Math_t::AngleVectors(float * angles, float * forward, float * right, float * up)
{	
	float angle = angles[YAW] * 0.01745329251994329f;
	float sy = sin(angle);
	float cy = cos(angle);

	angle = angles[PITCH] * 0.01745329251994329f;
	float sp = sin(angle);
	float cp = cos(angle);

	angle = angles[ROLL] * 0.01745329251994329f;
	float sr = sin(angle);
	float cr = cos(angle);
	
	forward[0] = cp*cy;
	forward[1] = cp*sy;
	forward[2] = -sp;
	
	right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
	right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
	right[2] = -1 * sr*cp;
	
	up[0] = (cr*sp*cy + -sr*-sy);
	up[1] = (cr*sp*sy + -sr*cy);
	up[2] = cr*cp;	
}

float Math_t::AngleSubstract(float a1, float a2)
{
	float a = a1 - a2;

	while (a > 180)
		a -= 360;	

	while (a < -180)
		a += 360;

	return a;
}

float Math_t::Radians(float deg)
{
	return  0.01745329238474369 * deg;
}

float Math_t::Degrees(float rad)
{
	return  rad * 57.29577951308232087;
}

float Math_t::GetFov(vec3_t angle, vec3_t src, vec3_t dst)
{
	vec3_t aim = { 0 };
	MakeVector(angle, aim);

	vec3_t vDelta = { 0 }, vAngles = { 0 };
	VectorSubtract(src, dst, vDelta);
	VecToAngles(vDelta, vAngles);

	vec3_t _qang = { 0 };
	MakeVector(vAngles, _qang);

	float mag_d = sqrtf((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_s = sqrtf((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float u_dot_v = (aim[0] * _qang[0]) + (aim[1] * _qang[1]) + (aim[2] * _qang[2]);

	return acosf(u_dot_v / (mag_s*mag_d)) * 57.29577951308232087f;
}