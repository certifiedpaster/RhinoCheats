//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cHooks {
	public:

		cHooks() : _thunkVectoredExceptionHandler(&cHooks::VectoredExceptionHandler, this) {}

		typedef enum {
			VEH_INDEX_DRAWBIGFPS = 33553,
			VEH_INDEX_MOUSEACCEL = 22152,
			VEH_INDEX_NODELTA = 33833
		} vehIndex_e;

		DWORD_PTR dwDrawBigFPS, dwMouseAccel, dwNoDelta;
		LPVOID pVectoredExceptionHandler;

		LONG VectoredExceptionHandler(LPEXCEPTION_POINTERS ExceptionInfo);
		typedef LONG(CALLBACK* tVectoredExceptionHandler)(_In_ LPEXCEPTION_POINTERS ExceptionInfo);

		void DrawBigFPS();
		void PredictPlayerState();
		void WritePacket();
		BOOL PatchAntiCheat();

		cWin32Thunk<tVectoredExceptionHandler, cHooks> _thunkVectoredExceptionHandler;
	} extern _hooks;
}

//=====================================================================================