//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace RhinoCheats
{
	class cPackets
	{
	public:

		void ModifyCommands();
		void OldCommand(sUserCMD* usercmd);
		void NewCommand(sUserCMD* usercmd);
	} extern _packets;
}

//=====================================================================================