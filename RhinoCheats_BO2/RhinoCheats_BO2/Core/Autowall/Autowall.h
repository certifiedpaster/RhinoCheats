//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cAutowall {
	public:

		float BulletFirePenetrate(Vector3 start, Vector3 end, int entitynum);
		bool BulletTrace(bulletTraceResults_t* br_, int localnum, bulletFireParams_t* bp_, int weapon, centity_t* attacker, int surfacetype);
		bool Trace(Vector3 start, Vector3 end, int entitynum);
		float GetWeaponDamageForHitLocation(int weapon, bulletFireParams_t* bp, bulletTraceResults_t* br, WORD hitloc);
		bool HitRiotshield(bulletTraceResults_t* br);
	} extern _autoWall;
}

//=====================================================================================
