#pragma once

#define ENTITYNUM_WORLD 2046
#define ENTITYNUM_NONE 2047
#define BULLET_POWER 1.0f

class Autowall_t
{
public:
	bool  NormalVisible(vec3_t vTarget);	
	float WallTraceSimulated(vec3_t vTarget, centity_t *cent);
	float WallTrace(vec3_t vTarget, centity_t *cent);

private:
	float GetRemainingPower(vec3_t start, vec3_t end, centity_t *cent);
	bool  TraceBullet(BulletFireParams * bulletTrace, trace_t * trace, centity_t * cent, int material);
	bool  CopyTrace(void * output, void * input, int len);
	bool  StepForward(BulletFireParams * bulletTrace, trace_t * trace, float factor);	

};

extern Autowall_t Autowall;