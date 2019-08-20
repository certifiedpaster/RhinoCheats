#pragma once

#define _USE_MATH_DEFINES

#define	PITCH					0
#define YAW						1
#define	ROLL					2

#define ANGLE2SHORT(x)          ((int)((x) * 65536 / 360) & 65535)
#define SHORT2ANGLE(x)          ((x)*(360.0 / 65536))

#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])

//ImGui copy
#define ImVec4Copy(i,v)	(v.x=i.x, v.y=i.y, v.z=i.z, v.w=i.w)

#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])


#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleNormalize(a) (SHORT2ANGLE(ANGLE2SHORT((a))))


typedef float vec_t;
typedef float vec2_t[2];
typedef float vec3_t[3];

//========================================================================

class Math_t
{
public:
	void  MakeVector(vec3_t angle, vec3_t vector);
	float VectorLength(vec3_t v);
	float GetDistance(float *p1, float *p2);
	void  VecToAngles(float *value1, float *angles);
	void  ToEulerAngles(vec3_t in, float *angles);
	void  AngleVectors(float * angles, float * forward, float * right, float * up);	
	float AngleSubstract(float a1, float a2);
	float Radians(float deg);
	float Degrees(float rad);
	float GetFov(vec3_t angle, vec3_t src, vec3_t dst);

};

extern Math_t Math;

