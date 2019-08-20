//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace RhinoCheats {
	cProfiler _profiler;

	bool cProfiler::SaveProfile(
		const std::string& path
	) {
		try {
			XML.create_document();
			std::wstring szFilePath;

			if (path.empty())
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + DEFAULT_XML, CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			XML.set("RhinoCheats.Aimbot.Aimmode", gAimmode.Custom.iValue);
			XML.set("RhinoCheats.Aimbot.Autozoom", gAutozoom.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.Autofire", gAutofire.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.Autowall", gAutowall.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.SilentAim", gSilentAim.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.NoRecoil", gNoRecoil.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.NoSpread", gNoSpread.Custom.bValue);
			XML.set("RhinoCheats.Aimbot.Bonescan", gBonescan.Custom.iValue);
			XML.set("RhinoCheats.Aimbot.AntiAim", gAntiAim.Custom.iValue);
			XML.set("RhinoCheats.Aimbot.Riotshield", gRiotshield.Custom.iValue);

			XML.set("RhinoCheats.Wallhack.PlayerBoxes", gPlayerBoxes.Custom.iValue);
			XML.set("RhinoCheats.Wallhack.PlayerBones", gPlayerBones.Custom.iValue);
			XML.set("RhinoCheats.Wallhack.PlayerInfo", gPlayerInfo.Custom.bValue);
			XML.set("RhinoCheats.Wallhack.PlayerWeapons", gPlayerWeapons.Custom.bValue);
			XML.set("RhinoCheats.Wallhack.PlayerSnaplines", gPlayerLines.Custom.bValue);
			XML.set("RhinoCheats.Wallhack.PlayerRadar", gPlayerRadar.Custom.bValue);
			
			XML.set("RhinoCheats.Tweaks.Aimbone", gAimbone.Custom.iValue);
			XML.set("RhinoCheats.Tweaks.AimAngle", gAimAngle.Custom.flValue);
			XML.set("RhinoCheats.Tweaks.AimStrength", gAimStrength.Custom.iValue);
			XML.set("RhinoCheats.Tweaks.AutoaimTime", gAutoaimTime.Custom.iValue);
			XML.set("RhinoCheats.Tweaks.AutoaimDelay", gAutoaimDelay.Custom.iValue);
			XML.set("RhinoCheats.Tweaks.AutozoomDelay", gAutozoomDelay.Custom.iValue);
			XML.set("RhinoCheats.Tweaks.AutofireDelay", gAutofireDelay.Custom.iValue);

			XML.set("RhinoCheats.Styles.AxisVisible.Red", gColorAxisVisible.Custom.cValue[0]);
			XML.set("RhinoCheats.Styles.AxisVisible.Green", gColorAxisVisible.Custom.cValue[1]);
			XML.set("RhinoCheats.Styles.AxisVisible.Blue", gColorAxisVisible.Custom.cValue[2]);

			XML.set("RhinoCheats.Styles.AxisInvisible.Red", gColorAxisInvisible.Custom.cValue[0]);
			XML.set("RhinoCheats.Styles.AxisInvisible.Green", gColorAxisInvisible.Custom.cValue[1]);
			XML.set("RhinoCheats.Styles.AxisInvisible.Blue", gColorAxisInvisible.Custom.cValue[2]);

			XML.set("RhinoCheats.Styles.AlliesVisible.Red", gColorAlliesVisible.Custom.cValue[0]);
			XML.set("RhinoCheats.Styles.AlliesVisible.Green", gColorAlliesVisible.Custom.cValue[1]);
			XML.set("RhinoCheats.Styles.AlliesVisible.Blue", gColorAlliesVisible.Custom.cValue[2]);

			XML.set("RhinoCheats.Styles.AlliesInvisible.Red", gColorAlliesInvisible.Custom.cValue[0]);
			XML.set("RhinoCheats.Styles.AlliesInvisible.Green", gColorAlliesInvisible.Custom.cValue[1]);
			XML.set("RhinoCheats.Styles.AlliesInvisible.Blue", gColorAlliesInvisible.Custom.cValue[2]);

			XML.write_document(szFilePath);

			return true;
		}

		catch (
			const std::runtime_error& e
		) {
			MessageBox(_mainGui.hWindow, e.what(), "Error", MB_OK | MB_ICONERROR);

			return false;
		}
	}
	/*
	//=====================================================================================
	*/
	bool cProfiler::LoadProfile(
		const std::string& path
	) {
		try {
			std::wstring szFilePath;

			if (path.empty())
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + DEFAULT_XML, CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			if (GetFileAttributes(acut::WstringToAnsi(szFilePath, CP_ACP).c_str()) == INVALID_FILE_ATTRIBUTES)
				return false;

			XML.read_from_file(szFilePath);

			XML.get_if_present("RhinoCheats.Aimbot.Aimmode", gAimmode.Custom.iValue);
			XML.get_if_present("RhinoCheats.Aimbot.Autozoom", gAutozoom.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.Autofire", gAutofire.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.Autowall", gAutowall.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.SilentAim", gSilentAim.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.NoRecoil", gNoRecoil.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.NoSpread", gNoSpread.Custom.bValue);
			XML.get_if_present("RhinoCheats.Aimbot.Bonescan", gBonescan.Custom.iValue);
			XML.get_if_present("RhinoCheats.Aimbot.AntiAim", gAntiAim.Custom.iValue);
			XML.get_if_present("RhinoCheats.Aimbot.Riotshield", gRiotshield.Custom.iValue);

			XML.get_if_present("RhinoCheats.Wallhack.PlayerBoxes", gPlayerBoxes.Custom.iValue);
			XML.get_if_present("RhinoCheats.Wallhack.PlayerBones", gPlayerBones.Custom.iValue);
			XML.get_if_present("RhinoCheats.Wallhack.PlayerInfo", gPlayerInfo.Custom.bValue);
			XML.get_if_present("RhinoCheats.Wallhack.PlayerWeapons", gPlayerWeapons.Custom.bValue);
			XML.get_if_present("RhinoCheats.Wallhack.PlayerSnaplines", gPlayerLines.Custom.bValue);
			XML.get_if_present("RhinoCheats.Wallhack.PlayerRadar", gPlayerRadar.Custom.bValue);

			XML.get_if_present("RhinoCheats.Tweaks.Aimbone", gAimbone.Custom.iValue);
			XML.get_if_present("RhinoCheats.Tweaks.AimAngle", gAimAngle.Custom.flValue);
			XML.get_if_present("RhinoCheats.Tweaks.AimStrength", gAimStrength.Custom.iValue);
			XML.get_if_present("RhinoCheats.Tweaks.AutoaimTime", gAutoaimTime.Custom.iValue);
			XML.get_if_present("RhinoCheats.Tweaks.AutoaimDelay", gAutoaimDelay.Custom.iValue);
			XML.get_if_present("RhinoCheats.Tweaks.AutozoomDelay", gAutozoomDelay.Custom.iValue);
			XML.get_if_present("RhinoCheats.Tweaks.AutofireDelay", gAutofireDelay.Custom.iValue);

			XML.get_if_present("RhinoCheats.Styles.AxisVisible.Red", gColorAxisVisible.Custom.cValue[0]);
			XML.get_if_present("RhinoCheats.Styles.AxisVisible.Green", gColorAxisVisible.Custom.cValue[1]);
			XML.get_if_present("RhinoCheats.Styles.AxisVisible.Blue", gColorAxisVisible.Custom.cValue[2]);

			XML.get_if_present("RhinoCheats.Styles.AxisInvisible.Red", gColorAxisInvisible.Custom.cValue[0]);
			XML.get_if_present("RhinoCheats.Styles.AxisInvisible.Green", gColorAxisInvisible.Custom.cValue[1]);
			XML.get_if_present("RhinoCheats.Styles.AxisInvisible.Blue", gColorAxisInvisible.Custom.cValue[2]);

			XML.get_if_present("RhinoCheats.Styles.AlliesVisible.Red", gColorAlliesVisible.Custom.cValue[0]);
			XML.get_if_present("RhinoCheats.Styles.AlliesVisible.Green", gColorAlliesVisible.Custom.cValue[1]);
			XML.get_if_present("RhinoCheats.Styles.AlliesVisible.Blue", gColorAlliesVisible.Custom.cValue[2]);

			XML.get_if_present("RhinoCheats.Styles.AlliesInvisible.Red", gColorAlliesInvisible.Custom.cValue[0]);
			XML.get_if_present("RhinoCheats.Styles.AlliesInvisible.Green", gColorAlliesInvisible.Custom.cValue[1]);
			XML.get_if_present("RhinoCheats.Styles.AlliesInvisible.Blue", gColorAlliesInvisible.Custom.cValue[2]);

			return true;
		}

		catch (
			const std::runtime_error& e
		) {
			MessageBox(_mainGui.hWindow, e.what(), "Error", MB_OK | MB_ICONERROR);

			return false;
		}
	}
	/*
	//=====================================================================================
	*/
	void cProfiler::DisableAll() {
		gAimmode.Custom.iValue = gAimmode.Default.iValue;
		gAutozoom.Custom.bValue = gAutozoom.Default.bValue;
		gAutofire.Custom.bValue = gAutofire.Default.bValue;
		gAutowall.Custom.bValue = gAutowall.Default.bValue;
		gSilentAim.Custom.bValue = gSilentAim.Default.bValue;
		gNoRecoil.Custom.bValue = gNoRecoil.Default.bValue;
		gNoSpread.Custom.bValue = gNoSpread.Default.bValue;
		gBonescan.Custom.iValue = gBonescan.Default.iValue;
		gAntiAim.Custom.iValue = gAntiAim.Default.iValue;
		gRiotshield.Custom.iValue = gRiotshield.Default.iValue;

		gPlayerBoxes.Custom.iValue = gPlayerBoxes.Default.iValue;
		gPlayerBones.Custom.iValue = gPlayerBones.Default.iValue;

		gPlayerInfo.Custom.bValue = gPlayerInfo.Default.bValue;
		gPlayerWeapons.Custom.bValue = gPlayerWeapons.Default.bValue;
		gPlayerLines.Custom.bValue = gPlayerLines.Default.bValue;
		gPlayerRadar.Custom.bValue = gPlayerRadar.Default.bValue;
	}
}

//=====================================================================================