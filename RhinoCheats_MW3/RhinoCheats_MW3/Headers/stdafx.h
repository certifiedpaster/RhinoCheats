#pragma once

// Defines and Prototypes
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define ALASKA17 "Rhino Cheats"

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include <Mmsystem.h>
#pragma comment ( lib, "WinMM.lib" ) //Mmsystem

#include <sstream>
#include <iomanip>
#define STREAM(s) (((std::stringstream&)(std::stringstream() << s)).str())


#include <detours.h>
#pragma  comment (lib, "detours.lib")
#define HookModule(module, what, to) { DetourTransactionBegin(); DetourUpdateThread( module );	DetourAttach( &(PVOID&)what,(PVOID) to ); if ( DetourTransactionCommit( ) != NO_ERROR ) {	SafeMessageBox(0, "Error! at special level - X", ALASKA17, MB_ICONERROR ); exit(-1);} } 
#define UnHookModule(module, what, to) { DetourTransactionBegin(); DetourUpdateThread( module );  DetourDetach( &(PVOID&)what,(PVOID) to ); DetourTransactionCommit( ); } 


// Alaska17
#include "Mathlib.h"
#include "CVars.h"
#include "Offsets.h"
#include "Math.h"
#include "Classes.h"
#include "Engine.h"
#include "Drawing.h"
#include "Entity.h"
#include "Autowall.h"
#include "Aim.h"
#include "Nospread.h"
#include "Sounds.h"
#include "MTRand.h"
#include "Radar.h"

#include "Vmthook.h"
#include "Hook.h"

#include "D3d.h"
#include "DynImports.h"
#include "KillSpam.h"

#include "Host.h"