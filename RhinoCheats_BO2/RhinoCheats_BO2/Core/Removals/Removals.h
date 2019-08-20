//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats {
	class cRemovals {
	public:

		void NoRecoil();
		void NoSpread(usercmd_s* usercmd, int servertime);
	} extern _removals;
}

//=====================================================================================