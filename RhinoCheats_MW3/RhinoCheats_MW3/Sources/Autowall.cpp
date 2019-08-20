#include "stdafx.h"

Autowall_t Autowall;

//========================================================================

float Autowall_t::GetRemainingPower(vec3_t start, vec3_t end, centity_t *ent)
{
	/*BulletFireParams bl_enter;

	bl_enter.worldEntNum = ENTITYNUM_WORLD;
	bl_enter.ignoreEntIndex = cg->clientNum;
	bl_enter.damageMultiplier = BULLET_POWER;
	bl_enter.methodOfDeath = pWeaponImpacttype(Engine.GetClient(cg->clientNum), cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum);
	VectorCopy(start, bl_enter.origStart);
	VectorCopy(start, bl_enter.start);
	VectorCopy(end, bl_enter.end);
	VectorSubtract(end, start, bl_enter.dir);

	if (!Engine.R_BulletPenetrationCheck(&bl_enter))
		return -1.0f;

	trace_t tr_enter;

	if (!TraceBullet(&bl_enter, &tr_enter, &cg_entities[cg->clientNum], 0))
		return 1.0f;

	// This is done by the game.
	if (tr_enter.allsolid)
		return -1.0f;

	int surfaces = 0;
	int perks = Engine.GetClient(cg->clientNum)->perk;
	BulletFireParams bl_exit;
	trace_t tr_exit;
	vec3_t end_pos;

	while (true) {
		auto pen_depth = pfnGetBulletPenetration(cg_entities[cg->clientNum].nextState.weaponID, 0, tr_enter.materialType);

		//if ( *(_DWORD *)(a5 + 212) == 1 && (unsigned int)&unk_4000000 & v41 )
		if (perks & 0x4000000)
			pen_depth *= (perk_bulletPenetrationMultiplier && perk_bulletPenetrationMultiplier->u6.fval) ? perk_bulletPenetrationMultiplier->u6.fval : 1.0f;


		if (pen_depth <= 0.0f)
			return -1.0f;

		// Save the trace's endpos.
		VectorCopy(tr_enter.endpos, end_pos);

		// Step into the hit wall.
		if (!StepForward(&bl_enter, &tr_enter, 0.13500001f) || !Engine.R_BulletPenetrationCheck(&bl_enter))
			return -1.0f;

		auto did_hit = TraceBullet(&bl_enter, &tr_enter, &cg_entities[cg->clientNum], tr_enter.materialType);


		CopyTrace(&bl_exit, &bl_enter, sizeof(BulletFireParams));
		VectorScale(bl_enter.dir, -1.0f, bl_exit.dir);
		VectorCopy(bl_enter.end, bl_exit.start);
		VectorMA(end_pos, 0.009999999776482582, bl_exit.dir, bl_exit.end);


		CopyTrace(&tr_exit, &tr_enter, sizeof(trace_t));
		VectorScale(tr_exit.surfaceDirection, -1.0f, tr_exit.surfaceDirection);

		if (did_hit)
			StepForward(&bl_exit, &tr_exit, 0.0099999998f);


		auto inv_trace = TraceBullet(&bl_exit, &tr_exit, &cg_entities[cg->clientNum], tr_exit.materialType) != 0;
		auto inv_solid = inv_trace && tr_exit.allsolid || (tr_enter.allsolid && tr_exit.startsolid);

		if (inv_trace || inv_solid) {
			auto wall_depth = 0.0f;

			if (inv_solid)
				wall_depth = Math.GetDistance(bl_exit.start, bl_exit.end);
			else
				wall_depth = Math.GetDistance(tr_exit.endpos, end_pos);

			wall_depth = max(wall_depth, 1.0f);

			if (inv_trace) {
				auto pen_depth2 = pfnGetBulletPenetration(cg_entities[cg->clientNum].nextState.weaponID, 0, tr_exit.materialType);

				if (perks & 0x4000000)
					pen_depth2 *= (perk_bulletPenetrationMultiplier && perk_bulletPenetrationMultiplier->u6.fval) ? perk_bulletPenetrationMultiplier->u6.fval : 1.0f;

				pen_depth = min(pen_depth, pen_depth2);

				if (pen_depth <= 0.0f)
					return -1.0f;
			}

			bl_enter.damageMultiplier -= wall_depth / pen_depth;

			if (bl_enter.damageMultiplier <= 0.0f)
				return -1.0f;
		}


		if (tr_enter.entityNum == ent->nextState.number)
			return bl_enter.damageMultiplier;

		surfaces++;

		if (surfaces > 5)
			break;
	}

	return -1.0f;

	*/

	BulletFireParams bl_enter;
	trace_t tr_enter;

	ZeroMemory(&bl_enter, sizeof(BulletFireParams));
	ZeroMemory(&tr_enter, sizeof(trace_t));

	VectorSubtract(end, start, bl_enter.dir);
	VectorNormalize(bl_enter.dir);

	bl_enter.worldEntNum = ENTITYNUM_WORLD;
	bl_enter.ignoreEntIndex = cg->clientNum;
	bl_enter.damageMultiplier = BULLET_POWER;
	bl_enter.methodOfDeath = pWeaponImpacttype(Engine.GetClient(cg->clientNum), cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum);

	VectorCopy(start, bl_enter.origStart);
	VectorCopy(start, bl_enter.start);
	VectorCopy(end, bl_enter.end);

	bool did_hit = TraceBullet(&bl_enter, &tr_enter, &cg_entities[cg->clientNum], 0);

	if (did_hit)
	{
		int surfaces = 0;
		float pen_depth = 0.0f;
		float pen_depth2 = 0.0f;
		float wall_depth = 0.0f;
		int perks = Engine.GetClient(cg->clientNum)->perk;
		BulletFireParams bl_exit;
		trace_t tr_exit;
		vec3_t end_pos = { 0.0f };

		while (true)
		{
			pen_depth = pfnGetBulletPenetration(cg_entities[cg->clientNum].nextState.weaponID, 0, tr_enter.materialType);

			//if ( *(_DWORD *)(a5 + 212) == 1 && (unsigned int)&unk_4000000 & v41 )
			if (perks & 0x4000000)
				pen_depth *= perk_bulletPenetrationMultiplier->u6.fval;//*(float *)(*(uintptr_t *)/*0x8DB7B0*/0x8DF7B0 + 0xC);

			if (pen_depth <= 0.0f)
				return 0.0f;

			// Save the trace's endpos.
			VectorCopy(tr_enter.endpos, end_pos);

			// Step into the hit wall.
			if (!StepForward(&bl_enter, &tr_enter, 0.13500001f))
				return 0.0f;

			if (!Engine.R_BulletPenetrationCheck(&bl_enter))
				return 0.0f;

			did_hit = TraceBullet(&bl_enter, &tr_enter, &cg_entities[cg->clientNum], tr_enter.materialType);

			CopyTrace(&bl_exit, &bl_enter, sizeof(BulletFireParams));
			VectorScale(bl_enter.dir, -1.0f, bl_exit.dir);

			VectorCopy(bl_enter.end, bl_exit.start);
			VectorMA(end_pos, 0.0099999998f, bl_exit.dir, bl_exit.end);

			CopyTrace(&tr_exit, &tr_enter, sizeof(trace_t));
			VectorScale(tr_exit.surfaceDirection, -1.0f, tr_exit.surfaceDirection);

			if (did_hit)
				StepForward(&bl_exit, &tr_exit, 0.0099999998f);

			bool inv_trace = TraceBullet(&bl_exit, &tr_exit, &cg_entities[cg->clientNum], tr_exit.materialType);
			bool inv_solid = (inv_trace && tr_exit.allsolid) || (tr_enter.startsolid && tr_exit.startsolid);

			if (inv_trace || inv_solid)
			{
				if (inv_solid)
					wall_depth = Math.GetDistance(bl_exit.end, bl_exit.start);

				else
					wall_depth = Math.GetDistance(end_pos, tr_exit.endpos);

				wall_depth = max(wall_depth, 1.0f);

				if (inv_trace)
				{
					pen_depth2 = pfnGetBulletPenetration(cg_entities[cg->clientNum].nextState.weaponID, 0, tr_exit.materialType);

					if (perks & 0x4000000)
						pen_depth2 *= perk_bulletPenetrationMultiplier->u6.fval;//*(float *)(*(uintptr_t *)/*0x8DB7B0*/0x8DF7B0 + 0xC);

					pen_depth = min(pen_depth, pen_depth2);

					if (pen_depth <= 0.0f)
						return 0.0f;
				}

				bl_enter.damageMultiplier -= wall_depth / pen_depth;

				if (bl_enter.damageMultiplier <= 0.0f)
					return 0.0f;
			}

			else if (!did_hit)
				return bl_enter.damageMultiplier;

			if (did_hit)
			{
				if (++surfaces < 5)
					continue;
			}

			return 0.0f;
		}
	}

	return bl_enter.damageMultiplier;
}

bool  Autowall_t::TraceBullet(BulletFireParams* bulletTrace, trace_t* trace, centity_t* cent, int material)
{
	bool result = 0;

	DWORD m_addr_TraceBullet = Offsets::m_addr_TraceBullet;

	__asm
	{
		push[material];
		push[cent];
		mov	esi, [trace];
		mov	eax, [bulletTrace];
		call[m_addr_TraceBullet];
		add	esp, 8;
		mov[result], al;
	}

	return result;
}

bool  Autowall_t::CopyTrace(void* output, void* input, int len)
{
	return pfnCopyTrace(output, input, len);
}

bool  Autowall_t::StepForward(BulletFireParams* bulletTrace, trace_t* trace, float factor)
{
	return pfnStepForward(bulletTrace, trace, factor);
}

//========================================================================

bool  Autowall_t::NormalVisible(vec3_t flTargetVector)
{
	trace_t tr;
	Trace(&tr, refdef->vieworg, flTargetVector, (float*)Offsets::bounding, 0x7FF, 0x803003);
	return tr.fraction >= 1.0f;
}

float Autowall_t::WallTraceSimulated(vec3_t vTarget, centity_t *cent)
{
	return GetRemainingPower(refdef->vieworg, vTarget, cent);
}

float Autowall_t::WallTrace(vec3_t vTarget, centity_t *cent)
{
	BulletFireParams bulletTrace;

	// Initialize the bullet trace.
	bulletTrace.worldEntNum = ENTITYNUM_WORLD;
	bulletTrace.ignoreEntIndex = cg->clientNum;
	bulletTrace.damageMultiplier = BULLET_POWER;
	bulletTrace.methodOfDeath = pWeaponImpacttype(reinterpret_cast<void*>(Offsets::client + (isTekno ? 0x560 : 0x564) * cg->clientNum), cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum);
	VectorCopy(refdef->vieworg, bulletTrace.origStart);
	VectorCopy(refdef->vieworg, bulletTrace.start);
	VectorCopy(vTarget, bulletTrace.end);
	VectorSubtract(vTarget, refdef->vieworg, bulletTrace.dir);


	bool v14 = 1;
	if (psub_4F0CF0() || psub_489F70())
	{
		v14 = 0;
	}

	int v43 = 0;
	if (psub_52A190(reinterpret_cast<void*>(Offsets::client + (isTekno ? 0x560 : 0x564) * cg->clientNum), cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum))
	{
		v43 = psub_46B370(cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum);
	}
	else
	{
		v43 = 1;
	}

	if (v43 > 0)
	{
		int v52 = v43;
		do
		{
			if (v14 || Engine.R_BulletPenetrationCheck(&bulletTrace))
			{
				DWORD dwOld = *(DWORD*)Offsets::bullettrail;
				*(DWORD*)Offsets::bullettrail = 0xFFFFFFFF;

				Engine.R_BulletPenetration(0, &bulletTrace, cg_entities[cg->clientNum].nextState.weaponID, cg->clientNum, &cg_entities[cg->clientNum], -1, refdef->vieworg, 0);

				*(DWORD*)Offsets::bullettrail = dwOld;

				if (bulletTrace.ignoreEntIndex == cent->nextState.number)
					return bulletTrace.damageMultiplier;
			}
			v52--;
		} while (v52);
	}

	return 0.0f;
}



