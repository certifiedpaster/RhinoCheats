//=====================================================================================

#pragma once

#include <Windows.h>
#include <intrin.h>
#include <Psapi.h>
#include <cstdio>
#include <thread>
#include <mutex>
#include <ctime>

#include <d3d11.h>
#include <detours.h>

#include "ImGui/Addons/FileSystem/imguifilesystem.h"
#include "ImGui/Addons/TabWindow/imguitabwindow.h"
#include "ImGui/DX11/imgui_impl_dx11.h"
#include "ImGui/Win32/imgui_impl_win32.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "Core/Aimbot/Aimbot.h"
#include "Core/AntiAim/AntiAim.h"
#include "Core/Autowall/Autowall.h"
#include "Core/Drawing/Drawing.h"
#include "Core/Mathematics/Mathematics.h"
#include "Core/Profiler/Profiler.h"
#include "Core/Removals/Removals.h"
#include "Core/TargetList/TargetList.h"
#include "Core/Hooks.h"
#include "Core/MainGUI.h"

#pragma intrinsic(_ReturnAddress)

//=====================================================================================