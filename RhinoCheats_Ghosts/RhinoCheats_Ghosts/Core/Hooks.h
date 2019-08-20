//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cHooks
	{
	public:

		cHooks() :
			_thunkDraw2D(&cHooks::Draw2D, this),
			_thunkWritePacket(&cHooks::WritePacket, this)
		{

		}

		void Draw2D();
		void WritePacket();

		void BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
		void Obituary(int localnum, sEntityState* entitystate, int weapon);
		void AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
		void ClientFrame(sGEntity* entity);

		cWin32Thunk<VEH::tCallback, cHooks> _thunkDraw2D;
		cWin32Thunk<VEH::tCallback, cHooks> _thunkWritePacket;
	} extern _hooks;
}

//=====================================================================================