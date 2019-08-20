// dear imgui, v1.71 WIP
// (drawing and font code)

/*

Index of this file:

// [SECTION] STB libraries implementation
// [SECTION] Style functions
// [SECTION] ImDrawList
// [SECTION] ImDrawListSplitter
// [SECTION] ImDrawData
// [SECTION] Helpers ShadeVertsXXX functions
// [SECTION] ImFontConfig
// [SECTION] ImFontAtlas
// [SECTION] ImFontAtlas glyph ranges helpers
// [SECTION] ImFontGlyphRangesBuilder
// [SECTION] ImFont
// [SECTION] Internal Render Helpers
// [SECTION] Decompression code
// [SECTION] Default font data (GroupType - Bank Gothic Pro Light.otf, GroupType - Bank Gothic Pro Medium.otf, GroupType - Bank Gothic Pro Bold.otf)

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include <stdio.h>      // vsnprintf, sscanf, printf
#if !defined(alloca)
#if defined(__GLIBC__) || defined(__sun) || defined(__CYGWIN__) || defined(__APPLE__) || defined(__SWITCH__)
#include <alloca.h>     // alloca (glibc uses <alloca.h>. Note that Cygwin may have _WIN32 defined, so the order matters here)
#elif defined(_WIN32)
#include <malloc.h>     // alloca
#if !defined(alloca)
#define alloca _alloca  // for clang with MS Codegen
#endif
#else
#include <stdlib.h>     // alloca
#endif
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127) // condition expression is constant
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global destructor           // similar to above, not sure what the exact difference is.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant              // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wcomma")
#pragma clang diagnostic ignored "-Wcomma"                  // warning : possible misuse of comma operator here             //
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"      // warning : macro name is a reserved identifier                //
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"       // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wstack-protector"          // warning: stack protector not protecting local variables: variable length buffer
#if __GNUC__ >= 8
#pragma GCC diagnostic ignored "-Wclass-memaccess"          // warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif
#endif

//-------------------------------------------------------------------------
// [SECTION] STB libraries implementation
//-------------------------------------------------------------------------

// Compile time options:
//#define IMGUI_STB_NAMESPACE           ImStb
//#define IMGUI_STB_TRUETYPE_FILENAME   "my_folder/stb_truetype.h"
//#define IMGUI_STB_RECT_PACK_FILENAME  "my_folder/stb_rect_pack.h"
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION

#ifdef IMGUI_STB_NAMESPACE
namespace IMGUI_STB_NAMESPACE
{
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456)                             // declaration of 'xx' hides previous local declaration
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wcast-qual"              // warning : cast from 'const xxxx *' to 'xxx *' drops const qualifier //
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"              // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#pragma GCC diagnostic ignored "-Wcast-qual"                // warning: cast from type 'const xxxx *' to type 'xxxx *' casts away qualifiers
#endif

#ifndef STB_RECT_PACK_IMPLEMENTATION                        // in case the user already have an implementation in the _same_ compilation unit (e.g. unity builds)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STBRP_ASSERT(x)     IM_ASSERT(x)
#define STBRP_SORT          ImQsort
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#ifdef IMGUI_STB_RECT_PACK_FILENAME
#include IMGUI_STB_RECT_PACK_FILENAME
#else
#include "STB/imstb_rectpack.h"
#endif
#endif

#ifndef STB_TRUETYPE_IMPLEMENTATION                         // in case the user already have an implementation in the _same_ compilation unit (e.g. unity builds)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_malloc(x,u)   ((void)(u), IM_ALLOC(x))
#define STBTT_free(x,u)     ((void)(u), IM_FREE(x))
#define STBTT_assert(x)     IM_ASSERT(x)
#define STBTT_fmod(x,y)     ImFmod(x,y)
#define STBTT_sqrt(x)       ImSqrt(x)
#define STBTT_pow(x,y)      ImPow(x,y)
#define STBTT_fabs(x)       ImFabs(x)
#define STBTT_ifloor(x)     ((int)ImFloorStd(x))
#define STBTT_iceil(x)      ((int)ImCeil(x))
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#ifdef IMGUI_STB_TRUETYPE_FILENAME
#include IMGUI_STB_TRUETYPE_FILENAME
#else
#include "STB/imstb_truetype.h"
#endif
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#ifdef IMGUI_STB_NAMESPACE
} // namespace ImStb
using namespace IMGUI_STB_NAMESPACE;
#endif

//-----------------------------------------------------------------------------
// [SECTION] Style functions
//-----------------------------------------------------------------------------

#include "Addons/TabWindow/imguitabwindow.h"

void ImGui::StyleColorsNeutral(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.49f, 0.49f, 0.49f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.49f, 0.49f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.49f, 0.49f, 0.49f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsRed(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsOrange(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.49f, 0.26f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.49f, 0.26f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.44f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.49f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.49f, 0.26f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.49f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.49f, 0.26f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.49f, 0.26f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.50f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.49f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.50f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.49f, 0.26f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsYellow(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.98f, 0.98f, 0.26f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.98f, 0.98f, 0.26f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.88f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.98f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.98f, 0.26f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.98f, 0.26f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.90f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.98f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsGreen(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.98f, 0.26f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.88f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.98f, 0.26f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.98f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.98f, 0.26f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 1.00f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.90f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsBlue(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.49f, 0.98f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.49f, 0.98f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.44f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.49f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.49f, 0.98f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.49f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.49f, 0.98f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.49f, 0.98f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.45f, 0.90f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.50f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.49f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.50f, 1.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.49f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void ImGui::StyleColorsPurple(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.67f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.49f, 0.26f, 0.49f, 0.25f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.26f, 0.49f, 0.95f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.26f, 0.49f, 0.67f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.24f, 0.44f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.49f, 0.26f, 0.49f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.26f, 0.49f, 0.80f);
	colors[ImGuiCol_Header] = ImVec4(0.49f, 0.26f, 0.49f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.49f, 0.26f, 0.49f, 0.80f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.26f, 0.49f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.26f, 0.49f, 0.95f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.26f, 0.49f, 0.67f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.35f, 0.50f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.00f, 0.45f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.00f, 0.50f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.49f, 0.26f, 0.49f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.00f, 1.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.49f, 0.26f, 0.49f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	TabLabelStyle::Get().rounding = 0.0f;
	TabLabelStyle::Get().fillColorGradientDeltaIn0_05 = 0.5f;

	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabel] = ColorConvertFloat4ToU32(ImVec4(0.98f, 0.98f, 0.98f, 0.40f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelText] = ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 1.00f));
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelected] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedHovered] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonHovered]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedActive] = ColorConvertFloat4ToU32(colors[ImGuiCol_ButtonActive]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedBorder] = ColorConvertFloat4ToU32(colors[ImGuiCol_Border]);
	TabLabelStyle::Get().colors[ImGui::TabLabelStyle::Col_TabLabelSelectedText] = ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

//-----------------------------------------------------------------------------
// ImDrawList
//-----------------------------------------------------------------------------

ImDrawListSharedData::ImDrawListSharedData()
{
    Font = NULL;
    FontSize = 0.0f;
    CurveTessellationTol = 0.0f;
    ClipRectFullscreen = ImVec4(-8192.0f, -8192.0f, +8192.0f, +8192.0f);
    InitialFlags = ImDrawListFlags_None;

    // Const data
    for (int i = 0; i < IM_ARRAYSIZE(CircleVtx12); i++)
    {
        const float a = ((float)i * 2 * IM_PI) / (float)IM_ARRAYSIZE(CircleVtx12);
        CircleVtx12[i] = ImVec2(ImCos(a), ImSin(a));
    }
}

void ImDrawList::Clear()
{
    CmdBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    Flags = _Data->InitialFlags;
    _VtxCurrentOffset = 0;
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.resize(0);
    _TextureIdStack.resize(0);
    _Path.resize(0);
    _Splitter.Clear();
}

void ImDrawList::ClearFreeMemory()
{
    CmdBuffer.clear();
    IdxBuffer.clear();
    VtxBuffer.clear();
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.clear();
    _TextureIdStack.clear();
    _Path.clear();
    _Splitter.ClearFreeMemory();
}

ImDrawList* ImDrawList::CloneOutput() const
{
    ImDrawList* dst = IM_NEW(ImDrawList(NULL));
    dst->CmdBuffer = CmdBuffer;
    dst->IdxBuffer = IdxBuffer;
    dst->VtxBuffer = VtxBuffer;
    dst->Flags = Flags;
    return dst;
}

// Using macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug builds
#define GetCurrentClipRect()    (_ClipRectStack.Size ? _ClipRectStack.Data[_ClipRectStack.Size-1]  : _Data->ClipRectFullscreen)
#define GetCurrentTextureId()   (_TextureIdStack.Size ? _TextureIdStack.Data[_TextureIdStack.Size-1] : (ImTextureID)NULL)

void ImDrawList::AddDrawCmd()
{
    ImDrawCmd draw_cmd;
    draw_cmd.ClipRect = GetCurrentClipRect();
    draw_cmd.TextureId = GetCurrentTextureId();
    draw_cmd.VtxOffset = _VtxCurrentOffset;
    draw_cmd.IdxOffset = IdxBuffer.Size;

    IM_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
    CmdBuffer.push_back(draw_cmd);
}

void ImDrawList::AddCallback(ImDrawCallback callback, void* callback_data)
{
    ImDrawCmd* current_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
    if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        current_cmd = &CmdBuffer.back();
    }
    current_cmd->UserCallback = callback;
    current_cmd->UserCallbackData = callback_data;

    AddDrawCmd(); // Force a new command after us (see comment below)
}

// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList::UpdateClipRect()
{
    // If current command is used with different settings we need to add a new command
    const ImVec4 curr_clip_rect = GetCurrentClipRect();
    ImDrawCmd* curr_cmd = CmdBuffer.Size > 0 ? &CmdBuffer.Data[CmdBuffer.Size-1] : NULL;
    if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
    if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId() && prev_cmd->UserCallback == NULL)
        CmdBuffer.pop_back();
    else
        curr_cmd->ClipRect = curr_clip_rect;
}

void ImDrawList::UpdateTextureID()
{
    // If current command is used with different settings we need to add a new command
    const ImTextureID curr_texture_id = GetCurrentTextureId();
    ImDrawCmd* curr_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
    if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
    if (curr_cmd->ElemCount == 0 && prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &GetCurrentClipRect(), sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL)
        CmdBuffer.pop_back();
    else
        curr_cmd->TextureId = curr_texture_id;
}

#undef GetCurrentClipRect
#undef GetCurrentTextureId

// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
void ImDrawList::PushClipRect(ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
    ImVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect && _ClipRectStack.Size)
    {
        ImVec4 current = _ClipRectStack.Data[_ClipRectStack.Size-1];
        if (cr.x < current.x) cr.x = current.x;
        if (cr.y < current.y) cr.y = current.y;
        if (cr.z > current.z) cr.z = current.z;
        if (cr.w > current.w) cr.w = current.w;
    }
    cr.z = ImMax(cr.x, cr.z);
    cr.w = ImMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    UpdateClipRect();
}

void ImDrawList::PushClipRectFullScreen()
{
    PushClipRect(ImVec2(_Data->ClipRectFullscreen.x, _Data->ClipRectFullscreen.y), ImVec2(_Data->ClipRectFullscreen.z, _Data->ClipRectFullscreen.w));
}

void ImDrawList::PopClipRect()
{
    IM_ASSERT(_ClipRectStack.Size > 0);
    _ClipRectStack.pop_back();
    UpdateClipRect();
}

void ImDrawList::PushTextureID(ImTextureID texture_id)
{
    _TextureIdStack.push_back(texture_id);
    UpdateTextureID();
}

void ImDrawList::PopTextureID()
{
    IM_ASSERT(_TextureIdStack.Size > 0);
    _TextureIdStack.pop_back();
    UpdateTextureID();
}

// NB: this can be called with negative count for removing primitives (as long as the result does not underflow)
void ImDrawList::PrimReserve(int idx_count, int vtx_count)
{
    // Large mesh support (when enabled)
    if (sizeof(ImDrawIdx) == 2 && (_VtxCurrentIdx + vtx_count >= (1 << 16)) && (Flags & ImDrawListFlags_AllowVtxOffset))
    {
        _VtxCurrentOffset = VtxBuffer.Size;
        _VtxCurrentIdx = 0;
        AddDrawCmd();
    }

    ImDrawCmd& draw_cmd = CmdBuffer.Data[CmdBuffer.Size-1];
    draw_cmd.ElemCount += idx_count;

    int vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

    int idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList::PrimRect(const ImVec2& a, const ImVec2& c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv(_Data->TexUvWhitePixel);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimRectUV(const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

// On AddPolyline() and AddConvexPolyFilled() we intentionally avoid using ImVec2 and superflous function calls to optimize debug/non-inlined builds.
// Those macros expects l-values.
#define IM_NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = 1.0f / ImSqrt(d2); VX *= inv_len; VY *= inv_len; } }
#define IM_FIXNORMAL2F(VX,VY)               { float d2 = VX*VX + VY*VY; if (d2 < 0.5f) d2 = 0.5f; float inv_lensq = 1.0f / d2; VX *= inv_lensq; VY *= inv_lensq; }

// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
// We avoid using the ImVec2 math operators here to reduce cost to a minimum for debug/non-inlined builds.
void ImDrawList::AddPolyline(const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness)
{
    if (points_count < 2)
        return;

    const ImVec2 uv = _Data->TexUvWhitePixel;

    int count = points_count;
    if (!closed)
        count = points_count-1;

    const bool thick_line = thickness > 1.0f;
    if (Flags & ImDrawListFlags_AntiAliasedLines)
    {
        // Anti-aliased stroke
        const float AA_SIZE = 1.0f;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;

        const int idx_count = thick_line ? count*18 : count*12;
        const int vtx_count = thick_line ? points_count*4 : points_count*3;
        PrimReserve(idx_count, vtx_count);

        // Temporary buffer
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2)); //-V630
        ImVec2* temp_points = temp_normals + points_count;

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1+1) == points_count ? 0 : i1+1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count-1] = temp_normals[points_count-2];

        if (!thick_line)
        {
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
                temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
                temp_points[(points_count-1)*2+0] = points[points_count-1] + temp_normals[points_count-1] * AA_SIZE;
                temp_points[(points_count-1)*2+1] = points[points_count-1] - temp_normals[points_count-1] * AA_SIZE;
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1+1) == points_count ? 0 : i1+1;
                unsigned int idx2 = (i1+1) == points_count ? _VtxCurrentIdx : idx1+3;

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y)
                dm_x *= AA_SIZE;
                dm_y *= AA_SIZE;

                // Add temporary vertexes
                ImVec2* out_vtx = &temp_points[i2*2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                // Add indexes
                _IdxWritePtr[0] = (ImDrawIdx)(idx2+0); _IdxWritePtr[1] = (ImDrawIdx)(idx1+0); _IdxWritePtr[2] = (ImDrawIdx)(idx1+2);
                _IdxWritePtr[3] = (ImDrawIdx)(idx1+2); _IdxWritePtr[4] = (ImDrawIdx)(idx2+2); _IdxWritePtr[5] = (ImDrawIdx)(idx2+0);
                _IdxWritePtr[6] = (ImDrawIdx)(idx2+1); _IdxWritePtr[7] = (ImDrawIdx)(idx1+1); _IdxWritePtr[8] = (ImDrawIdx)(idx1+0);
                _IdxWritePtr[9] = (ImDrawIdx)(idx1+0); _IdxWritePtr[10]= (ImDrawIdx)(idx2+0); _IdxWritePtr[11]= (ImDrawIdx)(idx2+1);
                _IdxWritePtr += 12;

                idx1 = idx2;
            }

            // Add vertexes
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = points[i];          _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
                _VtxWritePtr[1].pos = temp_points[i*2+0]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;
                _VtxWritePtr[2].pos = temp_points[i*2+1]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col_trans;
                _VtxWritePtr += 3;
            }
        }
        else
        {
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[(points_count-1)*4+0] = points[points_count-1] + temp_normals[points_count-1] * (half_inner_thickness + AA_SIZE);
                temp_points[(points_count-1)*4+1] = points[points_count-1] + temp_normals[points_count-1] * (half_inner_thickness);
                temp_points[(points_count-1)*4+2] = points[points_count-1] - temp_normals[points_count-1] * (half_inner_thickness);
                temp_points[(points_count-1)*4+3] = points[points_count-1] - temp_normals[points_count-1] * (half_inner_thickness + AA_SIZE);
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1+1) == points_count ? 0 : i1+1;
                unsigned int idx2 = (i1+1) == points_count ? _VtxCurrentIdx : idx1+4;

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertexes
                ImVec2* out_vtx = &temp_points[i2*4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                _IdxWritePtr[0]  = (ImDrawIdx)(idx2+1); _IdxWritePtr[1]  = (ImDrawIdx)(idx1+1); _IdxWritePtr[2]  = (ImDrawIdx)(idx1+2);
                _IdxWritePtr[3]  = (ImDrawIdx)(idx1+2); _IdxWritePtr[4]  = (ImDrawIdx)(idx2+2); _IdxWritePtr[5]  = (ImDrawIdx)(idx2+1);
                _IdxWritePtr[6]  = (ImDrawIdx)(idx2+1); _IdxWritePtr[7]  = (ImDrawIdx)(idx1+1); _IdxWritePtr[8]  = (ImDrawIdx)(idx1+0);
                _IdxWritePtr[9]  = (ImDrawIdx)(idx1+0); _IdxWritePtr[10] = (ImDrawIdx)(idx2+0); _IdxWritePtr[11] = (ImDrawIdx)(idx2+1);
                _IdxWritePtr[12] = (ImDrawIdx)(idx2+2); _IdxWritePtr[13] = (ImDrawIdx)(idx1+2); _IdxWritePtr[14] = (ImDrawIdx)(idx1+3);
                _IdxWritePtr[15] = (ImDrawIdx)(idx1+3); _IdxWritePtr[16] = (ImDrawIdx)(idx2+3); _IdxWritePtr[17] = (ImDrawIdx)(idx2+2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            // Add vertexes
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i*4+0]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i*4+1]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i*4+2]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i*4+3]; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Stroke
        const int idx_count = count*6;
        const int vtx_count = count*4;      // FIXME-OPT: Not sharing edges
        PrimReserve(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1+1) == points_count ? 0 : i1+1;
            const ImVec2& p1 = points[i1];
            const ImVec2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
            _VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
            _VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
            _VtxWritePtr += 4;

            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx+1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx+2);
            _IdxWritePtr[3] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[4] = (ImDrawIdx)(_VtxCurrentIdx+2); _IdxWritePtr[5] = (ImDrawIdx)(_VtxCurrentIdx+3);
            _IdxWritePtr += 6;
            _VtxCurrentIdx += 4;
        }
    }
}

// We intentionally avoid using ImVec2 and its math operators here to reduce cost to a minimum for debug/non-inlined builds.
void ImDrawList::AddConvexPolyFilled(const ImVec2* points, const int points_count, ImU32 col)
{
    if (points_count < 3)
        return;

    const ImVec2 uv = _Data->TexUvWhitePixel;

    if (Flags & ImDrawListFlags_AntiAliasedFill)
    {
        // Anti-aliased Fill
        const float AA_SIZE = 1.0f;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;
        const int idx_count = (points_count-2)*3 + points_count*6;
        const int vtx_count = (points_count*2);
        PrimReserve(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx+1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx+((i-1)<<1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx+(i<<1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2)); //-V630
        for (int i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const ImVec2& p0 = points[i0];
            const ImVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const ImVec2& n0 = temp_normals[i0];
            const ImVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            IM_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x); _VtxWritePtr[0].pos.y = (points[i1].y - dm_y); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x); _VtxWritePtr[1].pos.y = (points[i1].y + dm_y); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx+(i1<<1)); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx+(i0<<1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx+(i0<<1));
            _IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx+(i0<<1)); _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx+(i1<<1)); _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx+(i1<<1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count-2)*3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx+i-1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx+i);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
}

void ImDrawList::PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12)
{
    if (radius == 0.0f || a_min_of_12 > a_max_of_12)
    {
        _Path.push_back(centre);
        return;
    }
    _Path.reserve(_Path.Size + (a_max_of_12 - a_min_of_12 + 1));
    for (int a = a_min_of_12; a <= a_max_of_12; a++)
    {
        const ImVec2& c = _Data->CircleVtx12[a % IM_ARRAYSIZE(_Data->CircleVtx12)];
        _Path.push_back(ImVec2(centre.x + c.x * radius, centre.y + c.y * radius));
    }
}

void ImDrawList::PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments)
{
    if (radius == 0.0f)
    {
        _Path.push_back(centre);
        return;
    }

    // Note that we are adding a point at both a_min and a_max.
    // If you are trying to draw a full closed circle you don't want the overlapping points!
    _Path.reserve(_Path.Size + (num_segments + 1));
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
    }
}

static void PathBezierToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
    float dx = x4 - x1;
    float dy = y4 - y1;
    float d2 = ((x2 - x4) * dy - (y2 - y4) * dx);
    float d3 = ((x3 - x4) * dy - (y3 - y4) * dx);
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2+d3) * (d2+d3) < tess_tol * (dx*dx + dy*dy))
    {
        path->push_back(ImVec2(x4, y4));
    }
    else if (level < 10)
    {
        float x12 = (x1+x2)*0.5f,       y12 = (y1+y2)*0.5f;
        float x23 = (x2+x3)*0.5f,       y23 = (y2+y3)*0.5f;
        float x34 = (x3+x4)*0.5f,       y34 = (y3+y4)*0.5f;
        float x123 = (x12+x23)*0.5f,    y123 = (y12+y23)*0.5f;
        float x234 = (x23+x34)*0.5f,    y234 = (y23+y34)*0.5f;
        float x1234 = (x123+x234)*0.5f, y1234 = (y123+y234)*0.5f;

        PathBezierToCasteljau(path, x1,y1,        x12,y12,    x123,y123,  x1234,y1234, tess_tol, level+1);
        PathBezierToCasteljau(path, x1234,y1234,  x234,y234,  x34,y34,    x4,y4,       tess_tol, level+1);
    }
}

void ImDrawList::PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments)
{
    ImVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        // Auto-tessellated
        PathBezierToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, _Data->CurveTessellationTol, 0);
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
        {
            float t = t_step * i_step;
            float u = 1.0f - t;
            float w1 = u*u*u;
            float w2 = 3*u*u*t;
            float w3 = 3*u*t*t;
            float w4 = t*t*t;
            _Path.push_back(ImVec2(w1*p1.x + w2*p2.x + w3*p3.x + w4*p4.x, w1*p1.y + w2*p2.y + w3*p3.y + w4*p4.y));
        }
    }
}

void ImDrawList::PathRect(const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
    rounding = ImMin(rounding, ImFabs(b.x - a.x) * ( ((rounding_corners & ImDrawCornerFlags_Top)  == ImDrawCornerFlags_Top)  || ((rounding_corners & ImDrawCornerFlags_Bot)   == ImDrawCornerFlags_Bot)   ? 0.5f : 1.0f ) - 1.0f);
    rounding = ImMin(rounding, ImFabs(b.y - a.y) * ( ((rounding_corners & ImDrawCornerFlags_Left) == ImDrawCornerFlags_Left) || ((rounding_corners & ImDrawCornerFlags_Right) == ImDrawCornerFlags_Right) ? 0.5f : 1.0f ) - 1.0f);

    if (rounding <= 0.0f || rounding_corners == 0)
    {
        PathLineTo(a);
        PathLineTo(ImVec2(b.x, a.y));
        PathLineTo(b);
        PathLineTo(ImVec2(a.x, b.y));
    }
    else
    {
        const float rounding_tl = (rounding_corners & ImDrawCornerFlags_TopLeft) ? rounding : 0.0f;
        const float rounding_tr = (rounding_corners & ImDrawCornerFlags_TopRight) ? rounding : 0.0f;
        const float rounding_br = (rounding_corners & ImDrawCornerFlags_BotRight) ? rounding : 0.0f;
        const float rounding_bl = (rounding_corners & ImDrawCornerFlags_BotLeft) ? rounding : 0.0f;
        PathArcToFast(ImVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
        PathArcToFast(ImVec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
        PathArcToFast(ImVec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
        PathArcToFast(ImVec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
    }
}

void ImDrawList::AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    PathLineTo(a + ImVec2(0.5f,0.5f));
    PathLineTo(b + ImVec2(0.5f,0.5f));
    PathStroke(col, false, thickness);
}

// a: upper-left, b: lower-right. we don't render 1 px sized rectangles properly.
void ImDrawList::AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    if (Flags & ImDrawListFlags_AntiAliasedLines)
        PathRect(a + ImVec2(0.5f,0.5f), b - ImVec2(0.50f,0.50f), rounding, rounding_corners_flags);
    else
        PathRect(a + ImVec2(0.5f,0.5f), b - ImVec2(0.49f,0.49f), rounding, rounding_corners_flags); // Better looking lower-right corner and rounded non-AA shapes.
    PathStroke(col, true, thickness);
}

void ImDrawList::AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    if (rounding > 0.0f)
    {
        PathRect(a, b, rounding, rounding_corners_flags);
        PathFillConvex(col);
    }
    else
    {
        PrimReserve(6, 4);
        PrimRect(a, b, col);
    }
}

void ImDrawList::AddRectFilledMultiColor(const ImVec2& a, const ImVec2& c, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
    if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0)
        return;

    const ImVec2 uv = _Data->TexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+1)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+2));
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+2)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+3));
    PrimWriteVtx(a, uv, col_upr_left);
    PrimWriteVtx(ImVec2(c.x, a.y), uv, col_upr_right);
    PrimWriteVtx(c, uv, col_bot_right);
    PrimWriteVtx(ImVec2(a.x, c.y), uv, col_bot_left);
}

void ImDrawList::AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathFillConvex(col);
}

void ImDrawList::AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathFillConvex(col);
}

void ImDrawList::AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = IM_PI*2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(centre, radius-0.5f, 0.0f, a_max, num_segments - 1);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = IM_PI*2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(centre, radius, 0.0f, a_max, num_segments - 1);
    PathFillConvex(col);
}

void ImDrawList::AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(pos0);
    PathBezierCurveTo(cp0, cp1, pos1, num_segments);
    PathStroke(col, false, thickness);
}

void ImDrawList::AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    if (text_end == NULL)
        text_end = text_begin + strlen(text_begin);
    if (text_begin == text_end)
        return;

    // Pull default font/size from the shared ImDrawListSharedData instance
    if (font == NULL)
        font = _Data->Font;
    if (font_size == 0.0f)
        font_size = _Data->FontSize;

    IM_ASSERT(font->ContainerAtlas->TexID == _TextureIdStack.back());  // Use high-level ImGui::PushFont() or low-level ImDrawList::PushTextureId() to change font.

    ImVec4 clip_rect = _ClipRectStack.back();
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void ImDrawList::AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
    AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimRectUV(a, b, uv_a, uv_b, col);

    if (push_texture_id)
        PopTextureID();
}

void ImDrawList::AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);

    if (push_texture_id)
        PopTextureID();
}

void ImDrawList::AddImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    if (rounding <= 0.0f || (rounding_corners & ImDrawCornerFlags_All) == 0)
    {
        AddImage(user_texture_id, a, b, uv_a, uv_b, col);
        return;
    }

    const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
    if (push_texture_id)
        PushTextureID(user_texture_id);

    int vert_start_idx = VtxBuffer.Size;
    PathRect(a, b, rounding, rounding_corners);
    PathFillConvex(col);
    int vert_end_idx = VtxBuffer.Size;
    ImGui::ShadeVertsLinearUV(this, vert_start_idx, vert_end_idx, a, b, uv_a, uv_b, true);

    if (push_texture_id)
        PopTextureID();
}


//-----------------------------------------------------------------------------
// ImDrawListSplitter
//-----------------------------------------------------------------------------
// FIXME: This may be a little confusing, trying to be a little too low-level/optimal instead of just doing vector swap..
//-----------------------------------------------------------------------------

void ImDrawListSplitter::ClearFreeMemory()
{
    for (int i = 0; i < _Channels.Size; i++)
    {
        if (i == _Current) 
            memset(&_Channels[i], 0, sizeof(_Channels[i]));  // Current channel is a copy of CmdBuffer/IdxBuffer, don't destruct again
        _Channels[i].CmdBuffer.clear();
        _Channels[i].IdxBuffer.clear();
    }
    _Current = 0;
    _Count = 1;
    _Channels.clear();
}

void ImDrawListSplitter::Split(ImDrawList* draw_list, int channels_count)
{
    IM_ASSERT(_Current == 0 && _Count <= 1);
    int old_channels_count = _Channels.Size;
    if (old_channels_count < channels_count)
        _Channels.resize(channels_count);
    _Count = channels_count;

    // Channels[] (24/32 bytes each) hold storage that we'll swap with draw_list->_CmdBuffer/_IdxBuffer
    // The content of Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
    // When we switch to the next channel, we'll copy draw_list->_CmdBuffer/_IdxBuffer into Channels[0] and then Channels[1] into draw_list->CmdBuffer/_IdxBuffer
    memset(&_Channels[0], 0, sizeof(ImDrawChannel));
    for (int i = 1; i < channels_count; i++)
    {
        if (i >= old_channels_count)
        {
            IM_PLACEMENT_NEW(&_Channels[i]) ImDrawChannel();
        }
        else
        {
            _Channels[i].CmdBuffer.resize(0);
            _Channels[i].IdxBuffer.resize(0);
        }
        if (_Channels[i].CmdBuffer.Size == 0)
        {
            ImDrawCmd draw_cmd;
            draw_cmd.ClipRect = draw_list->_ClipRectStack.back();
            draw_cmd.TextureId = draw_list->_TextureIdStack.back();
            _Channels[i].CmdBuffer.push_back(draw_cmd);
        }
    }
}

static inline bool CanMergeDrawCommands(ImDrawCmd* a, ImDrawCmd* b)
{
    return memcmp(&a->ClipRect, &b->ClipRect, sizeof(a->ClipRect)) == 0 && a->TextureId == b->TextureId && !a->UserCallback && !b->UserCallback;
}

void ImDrawListSplitter::Merge(ImDrawList* draw_list)
{
    // Note that we never use or rely on channels.Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
    if (_Count <= 1)
        return;

    SetCurrentChannel(draw_list, 0);
    if (draw_list->CmdBuffer.Size != 0 && draw_list->CmdBuffer.back().ElemCount == 0)
        draw_list->CmdBuffer.pop_back();

    // Calculate our final buffer sizes. Also fix the incorrect IdxOffset values in each command.
    int new_cmd_buffer_count = 0;
    int new_idx_buffer_count = 0;
    ImDrawCmd* last_cmd = (_Count > 0 && _Channels[0].CmdBuffer.Size > 0) ? &_Channels[0].CmdBuffer.back() : NULL;
    int idx_offset = last_cmd ? last_cmd->IdxOffset + last_cmd->ElemCount : 0;
    for (int i = 1; i < _Count; i++)
    {
        ImDrawChannel& ch = _Channels[i];
        if (ch.CmdBuffer.Size && ch.CmdBuffer.back().ElemCount == 0)
            ch.CmdBuffer.pop_back();
        else if (ch.CmdBuffer.Size > 0 && last_cmd != NULL && CanMergeDrawCommands(last_cmd, &ch.CmdBuffer[0]))
        {
            // Merge previous channel last draw command with current channel first draw command if matching.
            last_cmd->ElemCount += ch.CmdBuffer[0].ElemCount;
            ch.CmdBuffer.erase(ch.CmdBuffer.Data);
        }
        if (ch.CmdBuffer.Size > 0)
            last_cmd = &ch.CmdBuffer.back();
        new_cmd_buffer_count += ch.CmdBuffer.Size;
        new_idx_buffer_count += ch.IdxBuffer.Size;
        for (int cmd_n = 0; cmd_n < ch.CmdBuffer.Size; cmd_n++)
        {
            ch.CmdBuffer.Data[cmd_n].IdxOffset = idx_offset;
            idx_offset += ch.CmdBuffer.Data[cmd_n].ElemCount;
        }
    }
    draw_list->CmdBuffer.resize(draw_list->CmdBuffer.Size + new_cmd_buffer_count);
    draw_list->IdxBuffer.resize(draw_list->IdxBuffer.Size + new_idx_buffer_count);

    // Write commands and indices in order (they are fairly small structures, we don't copy vertices only indices)
    ImDrawCmd* cmd_write = draw_list->CmdBuffer.Data + draw_list->CmdBuffer.Size - new_cmd_buffer_count;
    ImDrawIdx* idx_write = draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size - new_idx_buffer_count;
    for (int i = 1; i < _Count; i++)
    {
        ImDrawChannel& ch = _Channels[i];
        if (int sz = ch.CmdBuffer.Size) { memcpy(cmd_write, ch.CmdBuffer.Data, sz * sizeof(ImDrawCmd)); cmd_write += sz; }
        if (int sz = ch.IdxBuffer.Size) { memcpy(idx_write, ch.IdxBuffer.Data, sz * sizeof(ImDrawIdx)); idx_write += sz; }
    }
    draw_list->_IdxWritePtr = idx_write;
    draw_list->UpdateClipRect(); // We call this instead of AddDrawCmd(), so that empty channels won't produce an extra draw call.
    _Count = 1;
}

void ImDrawListSplitter::SetCurrentChannel(ImDrawList* draw_list, int idx)
{
    IM_ASSERT(idx < _Count);
    if (_Current == idx) 
        return;
    // Overwrite ImVector (12/16 bytes), four times. This is merely a silly optimization instead of doing .swap()
    memcpy(&_Channels.Data[_Current].CmdBuffer, &draw_list->CmdBuffer, sizeof(draw_list->CmdBuffer));
    memcpy(&_Channels.Data[_Current].IdxBuffer, &draw_list->IdxBuffer, sizeof(draw_list->IdxBuffer));
    _Current = idx;
    memcpy(&draw_list->CmdBuffer, &_Channels.Data[idx].CmdBuffer, sizeof(draw_list->CmdBuffer));
    memcpy(&draw_list->IdxBuffer, &_Channels.Data[idx].IdxBuffer, sizeof(draw_list->IdxBuffer));
    draw_list->_IdxWritePtr = draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size;
}

//-----------------------------------------------------------------------------
// [SECTION] ImDrawData
//-----------------------------------------------------------------------------

// For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
void ImDrawData::DeIndexAllBuffers()
{
    ImVector<ImDrawVert> new_vtx_buffer;
    TotalVtxCount = TotalIdxCount = 0;
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        if (cmd_list->IdxBuffer.empty())
            continue;
        new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
        for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
            new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
        cmd_list->VtxBuffer.swap(new_vtx_buffer);
        cmd_list->IdxBuffer.resize(0);
        TotalVtxCount += cmd_list->VtxBuffer.Size;
    }
}

// Helper to scale the ClipRect field of each ImDrawCmd.
// Use if your final output buffer is at a different scale than draw_data->DisplaySize,
// or if there is a difference between your window resolution and framebuffer resolution.
void ImDrawData::ScaleClipRects(const ImVec2& fb_scale)
{
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
            cmd->ClipRect = ImVec4(cmd->ClipRect.x * fb_scale.x, cmd->ClipRect.y * fb_scale.y, cmd->ClipRect.z * fb_scale.x, cmd->ClipRect.w * fb_scale.y);
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Helpers ShadeVertsXXX functions
//-----------------------------------------------------------------------------

// Generic linear color gradient, write to RGB fields, leave A untouched.
void ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = gradient_p1 - gradient_p0;
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(vert->pos - gradient_p0, gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = ImLerp((int)(col0 >> IM_COL32_R_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_R_SHIFT) & 0xFF, t);
        int g = ImLerp((int)(col0 >> IM_COL32_G_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_G_SHIFT) & 0xFF, t);
        int b = ImLerp((int)(col0 >> IM_COL32_B_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_B_SHIFT) & 0xFF, t);
        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
    }
}

// Distribute UV over (a, b) rectangle
void ImGui::ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp)
{
    const ImVec2 size = b - a;
    const ImVec2 uv_size = uv_b - uv_a;
    const ImVec2 scale = ImVec2(
        size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
        size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    if (clamp)
    {
        const ImVec2 min = ImMin(uv_a, uv_b);
        const ImVec2 max = ImMax(uv_a, uv_b);
        for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = ImClamp(uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale), min, max);
    }
    else
    {
        for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale);
    }
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontConfig
//-----------------------------------------------------------------------------

ImFontConfig::ImFontConfig()
{
    FontData = NULL;
    FontDataSize = 0;
    FontDataOwnedByAtlas = true;
    FontNo = 0;
    SizePixels = 0.0f;
    OversampleH = 3; // FIXME: 2 may be a better default?
    OversampleV = 1;
    PixelSnapH = false;
    GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
    GlyphOffset = ImVec2(0.0f, 0.0f);
    GlyphRanges = NULL;
    GlyphMinAdvanceX = 0.0f;
    GlyphMaxAdvanceX = FLT_MAX;
    MergeMode = false;
    RasterizerFlags = 0x00;
    RasterizerMultiply = 1.0f;
    memset(Name, 0, sizeof(Name));
    DstFont = NULL;
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontAtlas
//-----------------------------------------------------------------------------

// A work of art lies ahead! (. = white layer, X = black layer, others are blank)
// The white texels on the top left are the ones we'll use everywhere in ImGui to render filled shapes.
const int FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF = 108;
const int FONT_ATLAS_DEFAULT_TEX_DATA_H      = 27;
const unsigned int FONT_ATLAS_DEFAULT_TEX_DATA_ID = 0x80000000;
static char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
{
	"..-         -.......-    .    -           .           -.......          -          .......-     ..          "
	"..-         -.XXXXX.-   .X.   -          .X.          -.XXXXX.          -          .XXXXX.-    .XX.         "
	"---         -...X...-  .XXX.  -         .XXX.         -.XXXX.           -           .XXXX.-    .XX.         "
	".           -  .X.  - .XXXXX. -        .XXXXX.        -.XXX.            -            .XXX.-    .XX.         "
	"..          -  .X.  -.XXXXXXX.-       .XXXXXXX.       -.XX.X.           -           .X.XX.-    .XX.         "
	".X.         -  .X.  -....X....-       ....X....       -.X. .X.          -          .X. .X.-    .XX...       "
	".XX.        -  .X.  -   .X.   -          .X.          -..   .X.         -         .X.   ..-    .XX.XX...    "
	".XXX.       -  .X.  -   .X.   -    ..    .X.    ..    -      .X.        -        .X.      -    .XX.XX.XX..  "
	".XXXX.      -  .X.  -   .X.   -   .X.    .X.    .X.   -       .X.       -       .X.       -    .XX.XX.XX.X. "
	".XXXXX.     -  .X.  -   .X.   -  .XX.    .X.    .XX.  -        .X.      -      .X.        -... .XX.XX.XX.XX."
	".XXXXXX.    -  .X.  -   .X.   - .XXX......X......XXX. -         .X.   ..-..   .X.         -.XX..XXXXXXXX.XX."
	".XXXXXXX.   -  .X.  -   .X.   -.XXXXXXXXXXXXXXXXXXXXX.-          .X. .X.-.X. .X.          -.XXX.XXXXXXXXXXX."
	".XXXXXXXX.  -  .X.  -   .X.   - .XXX......X......XXX. -           .X.XX.-.XX.X.           - .XXXXXXXXXXXXXX."
	".XXXXXXXXX. -...X...-   .X.   -  .XX.    .X.    .XX.  -            .XXX.-.XXX.            -  .XXXXXXXXXXXXX."
	".XXXXXXXXXX.-.XXXXX.-   .X.   -   .X.    .X.    .X.   -           .XXXX.-.XXXX.           -  .XXXXXXXXXXXXX."
	".XXXX...... -.......-   .X.   -    ..    .X.    ..    -          .XXXXX.-.XXXXX.          -   .XXXXXXXXXXXX."
	".XXX.       ---------   .X.   -          .X.          -          .......-.......          -   .XXXXXXXXXXX. "
	".XX.        -       -....X....-       ....X....       -------------------------------------    .XXXXXXXXXX. "
	".X.         -       -.XXXXXXX.-       .XXXXXXX.       -    ..           ..    -           -    .XXXXXXXXXX. "
	"..          -       - .XXXXX. -        .XXXXX.        -   .X.           .X.   -           -     .XXXXXXXX.  "
	"                    -  .XXX.  -         .XXX.         -  .XX.           .XX.  -           -     .XXXXXXXX.  "
	"                    -   .X.   -          .X.          - .XXX.............XXX. -           -     ..........  "
	"------------        -    .    -           .           -.XXXXXXXXXXXXXXXXXXXXX.-           ------------------"
	"                    ----------------------------------- .XXX.............XXX. -                             "
	"                                                      -  .XX.           .XX.  -                             "
	"                                                      -   .X.           .X.   -                             "
	"                                                      -    ..           ..    -                             "
};

void ImGui::StyleCursorsBlack()
{
	const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS_BLACK[FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
	{
		"..-         -.......-    .    -           .           -.......          -          .......-     ..          "
		"..-         -.XXXXX.-   .X.   -          .X.          -.XXXXX.          -          .XXXXX.-    .XX.         "
		"---         -...X...-  .XXX.  -         .XXX.         -.XXXX.           -           .XXXX.-    .XX.         "
		".           -  .X.  - .XXXXX. -        .XXXXX.        -.XXX.            -            .XXX.-    .XX.         "
		"..          -  .X.  -.XXXXXXX.-       .XXXXXXX.       -.XX.X.           -           .X.XX.-    .XX.         "
		".X.         -  .X.  -....X....-       ....X....       -.X. .X.          -          .X. .X.-    .XX...       "
		".XX.        -  .X.  -   .X.   -          .X.          -..   .X.         -         .X.   ..-    .XX.XX...    "
		".XXX.       -  .X.  -   .X.   -    ..    .X.    ..    -      .X.        -        .X.      -    .XX.XX.XX..  "
		".XXXX.      -  .X.  -   .X.   -   .X.    .X.    .X.   -       .X.       -       .X.       -    .XX.XX.XX.X. "
		".XXXXX.     -  .X.  -   .X.   -  .XX.    .X.    .XX.  -        .X.      -      .X.        -... .XX.XX.XX.XX."
		".XXXXXX.    -  .X.  -   .X.   - .XXX......X......XXX. -         .X.   ..-..   .X.         -.XX..XXXXXXXX.XX."
		".XXXXXXX.   -  .X.  -   .X.   -.XXXXXXXXXXXXXXXXXXXXX.-          .X. .X.-.X. .X.          -.XXX.XXXXXXXXXXX."
		".XXXXXXXX.  -  .X.  -   .X.   - .XXX......X......XXX. -           .X.XX.-.XX.X.           - .XXXXXXXXXXXXXX."
		".XXXXXXXXX. -...X...-   .X.   -  .XX.    .X.    .XX.  -            .XXX.-.XXX.            -  .XXXXXXXXXXXXX."
		".XXXXXXXXXX.-.XXXXX.-   .X.   -   .X.    .X.    .X.   -           .XXXX.-.XXXX.           -  .XXXXXXXXXXXXX."
		".XXXX...... -.......-   .X.   -    ..    .X.    ..    -          .XXXXX.-.XXXXX.          -   .XXXXXXXXXXXX."
		".XXX.       ---------   .X.   -          .X.          -          .......-.......          -   .XXXXXXXXXXX. "
		".XX.        -       -....X....-       ....X....       -------------------------------------    .XXXXXXXXXX. "
		".X.         -       -.XXXXXXX.-       .XXXXXXX.       -    ..           ..    -           -    .XXXXXXXXXX. "
		"..          -       - .XXXXX. -        .XXXXX.        -   .X.           .X.   -           -     .XXXXXXXX.  "
		"                    -  .XXX.  -         .XXX.         -  .XX.           .XX.  -           -     .XXXXXXXX.  "
		"                    -   .X.   -          .X.          - .XXX.............XXX. -           -     ..........  "
		"------------        -    .    -           .           -.XXXXXXXXXXXXXXXXXXXXX.-           ------------------"
		"                    ----------------------------------- .XXX.............XXX. -                             "
		"                                                      -  .XX.           .XX.  -                             "
		"                                                      -   .X.           .X.   -                             "
		"                                                      -    ..           ..    -                             "
	};

	strcpy_s(FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS_BLACK);
}

void ImGui::StyleCursorsWhite()
{
	const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS_WHITE[FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
	{
		"..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX-     XX          "
		"..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X-    X..X         "
		"---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X-    X..X         "
		"X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X-    X..X         "
		"XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X-    X..X         "
		"X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X-    X..XXX       "
		"X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX-    X..X..XXX    "
		"X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      -    X..X..X..XX  "
		"X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       -    X..X..X..X.X "
		"X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        -XXX X..X..X..X..X"
		"X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         -X..XX........X..X"
		"X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          -X...X...........X"
		"X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           - X..............X"
		"X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            -  X.............X"
		"X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           -  X.............X"
		"X....XXXXXX -XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          -   X............X"
		"X...X       ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          -   X...........X "
		"X..X        -       -XXXX.XXXX-       XXXX.XXXX       -------------------------------------    X..........X "
		"X.X         -       -X.......X-       X.......X       -    XX           XX    -           -    X..........X "
		"XX          -       - X.....X -        X.....X        -   X.X           X.X   -           -     X........X  "
		"                    -  X...X  -         X...X         -  X..X           X..X  -           -     X........X  "
		"                    -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           -     XXXXXXXXXX  "
		"------------        -    X    -           X           -X.....................X-           ------------------"
		"                    ----------------------------------- X...XXXXXXXXXXXXX...X -                             "
		"                                                      -  X..X           X..X  -                             "
		"                                                      -   X.X           X.X   -                             "
		"                                                      -    XX           XX    -                             "
	};

	strcpy_s(FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS_WHITE);
}

static const ImVec2 FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[ImGuiMouseCursor_COUNT][3] =
{
    // Pos ........ Size ......... Offset ......
    { ImVec2( 0,3), ImVec2(12,19), ImVec2( 0, 0) }, // ImGuiMouseCursor_Arrow
    { ImVec2(13,0), ImVec2( 7,16), ImVec2( 1, 8) }, // ImGuiMouseCursor_TextInput
    { ImVec2(31,0), ImVec2(23,23), ImVec2(11,11) }, // ImGuiMouseCursor_ResizeAll
    { ImVec2(21,0), ImVec2( 9,23), ImVec2( 4,11) }, // ImGuiMouseCursor_ResizeNS
    { ImVec2(55,18),ImVec2(23, 9), ImVec2(11, 4) }, // ImGuiMouseCursor_ResizeEW
    { ImVec2(73,0), ImVec2(17,17), ImVec2( 8, 8) }, // ImGuiMouseCursor_ResizeNESW
    { ImVec2(55,0), ImVec2(17,17), ImVec2( 8, 8) }, // ImGuiMouseCursor_ResizeNWSE
    { ImVec2(91,0), ImVec2(17,22), ImVec2( 5, 0) }, // ImGuiMouseCursor_Hand
};

ImFontAtlas::ImFontAtlas()
{
    Locked = false;
    Flags = ImFontAtlasFlags_None;
    TexID = (ImTextureID)NULL;
    TexDesiredWidth = 0;
    TexGlyphPadding = 1;

    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
    TexWidth = TexHeight = 0;
    TexUvScale = ImVec2(0.0f, 0.0f);
    TexUvWhitePixel = ImVec2(0.0f, 0.0f);
    for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
        CustomRectIds[n] = -1;
}

ImFontAtlas::~ImFontAtlas()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    Clear();
}

void    ImFontAtlas::ClearInputData()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    for (int i = 0; i < ConfigData.Size; i++)
        if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
        {
            IM_FREE(ConfigData[i].FontData);
            ConfigData[i].FontData = NULL;
        }

    // When clearing this we lose access to the font name and other information used to build the font.
    for (int i = 0; i < Fonts.Size; i++)
        if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
        {
            Fonts[i]->ConfigData = NULL;
            Fonts[i]->ConfigDataCount = 0;
        }
    ConfigData.clear();
    CustomRects.clear();
    for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
        CustomRectIds[n] = -1;
}

void    ImFontAtlas::ClearTexData()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    if (TexPixelsAlpha8)
        IM_FREE(TexPixelsAlpha8);
    if (TexPixelsRGBA32)
        IM_FREE(TexPixelsRGBA32);
    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
}

void    ImFontAtlas::ClearFonts()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    for (int i = 0; i < Fonts.Size; i++)
        IM_DELETE(Fonts[i]);
    Fonts.clear();
}

void    ImFontAtlas::Clear()
{
    ClearInputData();
    ClearTexData();
    ClearFonts();
}

void    ImFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Build atlas on demand
    if (TexPixelsAlpha8 == NULL)
    {
		if (ConfigData.empty())
		{
			AddFontLight();
			AddFontMedium();
			AddFontBold();
		}
        Build();
    }

    *out_pixels = TexPixelsAlpha8;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
}

void    ImFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Convert to RGBA32 format on demand
    // Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp
    if (!TexPixelsRGBA32)
    {
        unsigned char* pixels = NULL;
        GetTexDataAsAlpha8(&pixels, NULL, NULL);
        if (pixels)
        {
            TexPixelsRGBA32 = (unsigned int*)IM_ALLOC((size_t)TexWidth * (size_t)TexHeight * 4);
            const unsigned char* src = pixels;
            unsigned int* dst = TexPixelsRGBA32;
            for (int n = TexWidth * TexHeight; n > 0; n--)
                *dst++ = IM_COL32(255, 255, 255, (unsigned int)(*src++));
        }
    }

    *out_pixels = (unsigned char*)TexPixelsRGBA32;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
}

ImFont* ImFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    IM_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
    IM_ASSERT(font_cfg->SizePixels > 0.0f);

    // Create new font
    if (!font_cfg->MergeMode)
        Fonts.push_back(IM_NEW(ImFont));
    else
        IM_ASSERT(!Fonts.empty() && "Cannot use MergeMode for the first font"); // When using MergeMode make sure that a font has already been added before. You can use ImGui::GetIO().Fonts->AddFontLight(), ImGui::GetIO().Fonts->AddFontMedium(), ImGui::GetIO().Fonts->AddFontBold(), to add the default imgui fonts.

    ConfigData.push_back(*font_cfg);
    ImFontConfig& new_font_cfg = ConfigData.back();
    if (new_font_cfg.DstFont == NULL)
        new_font_cfg.DstFont = Fonts.back();
    if (!new_font_cfg.FontDataOwnedByAtlas)
    {
        new_font_cfg.FontData = IM_ALLOC(new_font_cfg.FontDataSize);
        new_font_cfg.FontDataOwnedByAtlas = true;
        memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
    }

    // Invalidate texture
    ClearTexData();
    return new_font_cfg.DstFont;
}

// Default font TTF is compressed with stb_compress then base85 encoded (see misc/fonts/binary_to_compressed_c.cpp for encoder)
static unsigned int stb_decompress_length(const unsigned char *input);
static unsigned int stb_decompress(unsigned char *output, const unsigned char *input, unsigned int length);
static const char*  GetLightCompressedFontDataOTFBase85();
static const char* GetMediumCompressedFontDataOTFBase85();
static const char* GetBoldCompressedFontDataOTFBase85();
static unsigned int Decode85Byte(char c)                                    { return c >= '\\' ? c-36 : c-35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
    while (*src)
    {
        unsigned int tmp = Decode85Byte(src[0]) + 85*(Decode85Byte(src[1]) + 85*(Decode85Byte(src[2]) + 85*(Decode85Byte(src[3]) + 85*Decode85Byte(src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
        src += 5;
        dst += 4;
    }
}

// Load embedded GroupType - Bank Gothic Pro Light.otf at size 14, disable oversampling
ImFont* ImFontAtlas::AddFontLight(const ImFontConfig* font_cfg_template)
{
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (!font_cfg_template)
    {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.SizePixels <= 0.0f)
        font_cfg.SizePixels = 14.0f * 1.0f;
    if (font_cfg.Name[0] == '\0')
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "GroupType - Bank Gothic Pro Light.otf, %dpx", (int)font_cfg.SizePixels);

    const char* ttf_compressed_base85 = GetLightCompressedFontDataOTFBase85();
    const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : GetGlyphRangesDefault();
    ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, glyph_ranges);
    font->DisplayOffset.y = -1.0f;
    return font;
}

// Load embedded GroupType - Bank Gothic Pro Medium.otf at size 14, disable oversampling
ImFont* ImFontAtlas::AddFontMedium(const ImFontConfig* font_cfg_template)
{
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	if (!font_cfg_template)
	{
		font_cfg.OversampleH = font_cfg.OversampleV = 1;
		font_cfg.PixelSnapH = true;
	}
	if (font_cfg.SizePixels <= 0.0f)
		font_cfg.SizePixels = 14.0f * 1.0f;
	if (font_cfg.Name[0] == '\0')
		ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "GroupType - Bank Gothic Pro Medium.otf, %dpx", (int)font_cfg.SizePixels);

	const char* ttf_compressed_base85 = GetMediumCompressedFontDataOTFBase85();
	const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : GetGlyphRangesDefault();
	ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, glyph_ranges);
	font->DisplayOffset.y = -1.0f;
	return font;
}

// Load embedded GroupType - Bank Gothic Pro Bold.otf at size 14, disable oversampling
ImFont* ImFontAtlas::AddFontBold(const ImFontConfig* font_cfg_template)
{
	ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
	if (!font_cfg_template)
	{
		font_cfg.OversampleH = font_cfg.OversampleV = 1;
		font_cfg.PixelSnapH = true;
	}
	if (font_cfg.SizePixels <= 0.0f)
		font_cfg.SizePixels = 14.0f * 1.0f;
	if (font_cfg.Name[0] == '\0')
		ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "GroupType - Bank Gothic Pro Bold.otf, %dpx", (int)font_cfg.SizePixels);

	const char* ttf_compressed_base85 = GetBoldCompressedFontDataOTFBase85();
	const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : GetGlyphRangesDefault();
	ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, glyph_ranges);
	font->DisplayOffset.y = -1.0f;
	return font;
}

ImFont* ImFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    size_t data_size = 0;
    void* data = ImFileLoadToMemory(filename, "rb", &data_size, 0);
    if (!data)
    {
        IM_ASSERT(0); // Could not load file.
        return NULL;
    }
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (font_cfg.Name[0] == '\0')
    {
        // Store a short copy of filename into into the font name for convenience
        const char* p;
        for (p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; p--) {}
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", p, size_pixels);
    }
    return AddFontFromMemoryTTF(data, (int)data_size, size_pixels, &font_cfg, glyph_ranges);
}

// NB: Transfer ownership of 'ttf_data' to ImFontAtlas, unless font_cfg_template->FontDataOwnedByAtlas == false. Owned TTF buffer will be deleted after Build().
ImFont* ImFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontData = ttf_data;
    font_cfg.FontDataSize = ttf_size;
    font_cfg.SizePixels = size_pixels;
    if (glyph_ranges)
        font_cfg.GlyphRanges = glyph_ranges;
    return AddFont(&font_cfg);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    const unsigned int buf_decompressed_size = stb_decompress_length((const unsigned char*)compressed_ttf_data);
    unsigned char* buf_decompressed_data = (unsigned char *)IM_ALLOC(buf_decompressed_size);
    stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontDataOwnedByAtlas = true;
    return AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges)
{
    int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
    void* compressed_ttf = IM_ALLOC((size_t)compressed_ttf_size);
    Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
    ImFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
    IM_FREE(compressed_ttf);
    return font;
}

int ImFontAtlas::AddCustomRectRegular(unsigned int id, int width, int height)
{
    IM_ASSERT(id >= 0x10000);
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    CustomRect r;
    r.ID = id;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

int ImFontAtlas::AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset)
{
    IM_ASSERT(font != NULL);
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    CustomRect r;
    r.ID = id;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    r.GlyphAdvanceX = advance_x;
    r.GlyphOffset = offset;
    r.Font = font;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

void ImFontAtlas::CalcCustomRectUV(const CustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max)
{
    IM_ASSERT(TexWidth > 0 && TexHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
    IM_ASSERT(rect->IsPacked());                // Make sure the rectangle has been packed
    *out_uv_min = ImVec2((float)rect->X * TexUvScale.x, (float)rect->Y * TexUvScale.y);
    *out_uv_max = ImVec2((float)(rect->X + rect->Width) * TexUvScale.x, (float)(rect->Y + rect->Height) * TexUvScale.y);
}

bool ImFontAtlas::GetMouseCursorTexData(ImGuiMouseCursor cursor_type, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2])
{
    if (cursor_type <= ImGuiMouseCursor_None || cursor_type >= ImGuiMouseCursor_COUNT)
        return false;
    if (Flags & ImFontAtlasFlags_NoMouseCursors)
        return false;

    IM_ASSERT(CustomRectIds[0] != -1);
    ImFontAtlas::CustomRect& r = CustomRects[CustomRectIds[0]];
    IM_ASSERT(r.ID == FONT_ATLAS_DEFAULT_TEX_DATA_ID);
    ImVec2 pos = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][0] + ImVec2((float)r.X, (float)r.Y);
    ImVec2 size = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][1];
    *out_size = size;
    *out_offset = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][2];
    out_uv_border[0] = (pos) * TexUvScale;
    out_uv_border[1] = (pos + size) * TexUvScale;
    pos.x += FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
    out_uv_fill[0] = (pos) * TexUvScale;
    out_uv_fill[1] = (pos + size) * TexUvScale;
    return true;
}

bool    ImFontAtlas::Build()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    return ImFontAtlasBuildWithStbTruetype(this);
}

void    ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_brighten_factor)
{
    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned int value = (unsigned int)(i * in_brighten_factor);
        out_table[i] = value > 255 ? 255 : (value & 0xFF);
    }
}

void    ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride)
{
    unsigned char* data = pixels + x + y * stride;
    for (int j = h; j > 0; j--, data += stride)
        for (int i = 0; i < w; i++)
            data[i] = table[data[i]];
}

// Temporary data for one source font (multiple source fonts can be merged into one destination ImFont)
// (C++03 doesn't allow instancing ImVector<> with function-local types so we declare the type here.)
struct ImFontBuildSrcData
{
    stbtt_fontinfo      FontInfo;
    stbtt_pack_range    PackRange;          // Hold the list of codepoints to pack (essentially points to Codepoints.Data)
    stbrp_rect*         Rects;              // Rectangle to pack. We first fill in their size and the packer will give us their position.
    stbtt_packedchar*   PackedChars;        // Output glyphs
    const ImWchar*      SrcRanges;          // Ranges as requested by user (user is allowed to request too much, e.g. 0x0020..0xFFFF)
    int                 DstIndex;           // Index into atlas->Fonts[] and dst_tmp_array[]
    int                 GlyphsHighest;      // Highest requested codepoint
    int                 GlyphsCount;        // Glyph count (excluding missing glyphs and glyphs already set by an earlier source font)
    ImBoolVector        GlyphsSet;          // Glyph bit map (random access, 1-bit per codepoint. This will be a maximum of 8KB)
    ImVector<int>       GlyphsList;         // Glyph codepoints list (flattened version of GlyphsMap)
};

// Temporary data for one destination ImFont* (multiple source fonts can be merged into one destination ImFont)
struct ImFontBuildDstData
{
    int                 SrcCount;           // Number of source fonts targeting this destination font.
    int                 GlyphsHighest;
    int                 GlyphsCount;
    ImBoolVector        GlyphsSet;          // This is used to resolve collision when multiple sources are merged into a same destination font.
};

static void UnpackBoolVectorToFlatIndexList(const ImBoolVector* in, ImVector<int>* out)
{
    IM_ASSERT(sizeof(in->Storage.Data[0]) == sizeof(int));
    const int* it_begin = in->Storage.begin();
    const int* it_end = in->Storage.end();
    for (const int* it = it_begin; it < it_end; it++)
        if (int entries_32 = *it)
            for (int bit_n = 0; bit_n < 32; bit_n++)
                if (entries_32 & (1u << bit_n))
                    out->push_back((int)((it - it_begin) << 5) + bit_n);
}

bool    ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas)
{
    IM_ASSERT(atlas->ConfigData.Size > 0);

    ImFontAtlasBuildRegisterDefaultCustomRects(atlas);

    // Clear atlas
    atlas->TexID = (ImTextureID)NULL;
    atlas->TexWidth = atlas->TexHeight = 0;
    atlas->TexUvScale = ImVec2(0.0f, 0.0f);
    atlas->TexUvWhitePixel = ImVec2(0.0f, 0.0f);
    atlas->ClearTexData();

    // Temporary storage for building
    ImVector<ImFontBuildSrcData> src_tmp_array;
    ImVector<ImFontBuildDstData> dst_tmp_array;
    src_tmp_array.resize(atlas->ConfigData.Size);
    dst_tmp_array.resize(atlas->Fonts.Size);
    memset(src_tmp_array.Data, 0, (size_t)src_tmp_array.size_in_bytes());
    memset(dst_tmp_array.Data, 0, (size_t)dst_tmp_array.size_in_bytes());

    // 1. Initialize font loading structure, check font data validity
    for (int src_i = 0; src_i < atlas->ConfigData.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        IM_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == atlas));

        // Find index from cfg.DstFont (we allow the user to set cfg.DstFont. Also it makes casual debugging nicer than when storing indices)
        src_tmp.DstIndex = -1;
        for (int output_i = 0; output_i < atlas->Fonts.Size && src_tmp.DstIndex == -1; output_i++)
            if (cfg.DstFont == atlas->Fonts[output_i])
                src_tmp.DstIndex = output_i;
        IM_ASSERT(src_tmp.DstIndex != -1); // cfg.DstFont not pointing within atlas->Fonts[] array?
        if (src_tmp.DstIndex == -1)
            return false;

        // Initialize helper structure for font loading and verify that the TTF/OTF data is correct
        const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
        IM_ASSERT(font_offset >= 0 && "FontData is incorrect, or FontNo cannot be found.");
        if (!stbtt_InitFont(&src_tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
            return false;

        // Measure highest codepoints
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.SrcRanges = cfg.GlyphRanges ? cfg.GlyphRanges : atlas->GetGlyphRangesDefault();
        for (const ImWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            src_tmp.GlyphsHighest = ImMax(src_tmp.GlyphsHighest, (int)src_range[1]);
        dst_tmp.SrcCount++;
        dst_tmp.GlyphsHighest = ImMax(dst_tmp.GlyphsHighest, src_tmp.GlyphsHighest);
    }

    // 2. For every requested codepoint, check for their presence in the font data, and handle redundancy or overlaps between source fonts to avoid unused glyphs.
    int total_glyphs_count = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.GlyphsSet.Resize(src_tmp.GlyphsHighest + 1);
        if (dst_tmp.GlyphsSet.Storage.empty())
            dst_tmp.GlyphsSet.Resize(dst_tmp.GlyphsHighest + 1);

        for (const ImWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            for (int codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++)
            {
                if (dst_tmp.GlyphsSet.GetBit(codepoint))    // Don't overwrite existing glyphs. We could make this an option for MergeMode (e.g. MergeOverwrite==true)
                    continue;
                if (!stbtt_FindGlyphIndex(&src_tmp.FontInfo, codepoint))    // It is actually in the font?
                    continue;

                // Add to avail set/counters
                src_tmp.GlyphsCount++;
                dst_tmp.GlyphsCount++;
                src_tmp.GlyphsSet.SetBit(codepoint, true);
                dst_tmp.GlyphsSet.SetBit(codepoint, true);
                total_glyphs_count++;
            }
    }

    // 3. Unpack our bit map into a flat list (we now have all the Unicode points that we know are requested _and_ available _and_ not overlapping another)
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        src_tmp.GlyphsList.reserve(src_tmp.GlyphsCount);
        UnpackBoolVectorToFlatIndexList(&src_tmp.GlyphsSet, &src_tmp.GlyphsList);
        src_tmp.GlyphsSet.Clear();
        IM_ASSERT(src_tmp.GlyphsList.Size == src_tmp.GlyphsCount);
    }
    for (int dst_i = 0; dst_i < dst_tmp_array.Size; dst_i++)
        dst_tmp_array[dst_i].GlyphsSet.Clear();
    dst_tmp_array.clear();

    // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    // (We technically don't need to zero-clear buf_rects, but let's do it for the sake of sanity)
    ImVector<stbrp_rect> buf_rects;
    ImVector<stbtt_packedchar> buf_packedchars;
    buf_rects.resize(total_glyphs_count);
    buf_packedchars.resize(total_glyphs_count);
    memset(buf_rects.Data, 0, (size_t)buf_rects.size_in_bytes());
    memset(buf_packedchars.Data, 0, (size_t)buf_packedchars.size_in_bytes());

    // 4. Gather glyphs sizes so we can pack them in our virtual canvas.
    int total_surface = 0;
    int buf_rects_out_n = 0;
    int buf_packedchars_out_n = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        src_tmp.Rects = &buf_rects[buf_rects_out_n];
        src_tmp.PackedChars = &buf_packedchars[buf_packedchars_out_n];
        buf_rects_out_n += src_tmp.GlyphsCount;
        buf_packedchars_out_n += src_tmp.GlyphsCount;

        // Convert our ranges in the format stb_truetype wants
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        src_tmp.PackRange.font_size = cfg.SizePixels;
        src_tmp.PackRange.first_unicode_codepoint_in_range = 0;
        src_tmp.PackRange.array_of_unicode_codepoints = src_tmp.GlyphsList.Data;
        src_tmp.PackRange.num_chars = src_tmp.GlyphsList.Size;
        src_tmp.PackRange.chardata_for_range = src_tmp.PackedChars;
        src_tmp.PackRange.h_oversample = (unsigned char)cfg.OversampleH;
        src_tmp.PackRange.v_oversample = (unsigned char)cfg.OversampleV;

        // Gather the sizes of all rectangles we will need to pack (this loop is based on stbtt_PackFontRangesGatherRects)
        const float scale = (cfg.SizePixels > 0) ? stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels) : stbtt_ScaleForMappingEmToPixels(&src_tmp.FontInfo, -cfg.SizePixels);
        const int padding = atlas->TexGlyphPadding;
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsList.Size; glyph_i++)
        {
            int x0, y0, x1, y1;
            const int glyph_index_in_font = stbtt_FindGlyphIndex(&src_tmp.FontInfo, src_tmp.GlyphsList[glyph_i]);
            IM_ASSERT(glyph_index_in_font != 0);
            stbtt_GetGlyphBitmapBoxSubpixel(&src_tmp.FontInfo, glyph_index_in_font, scale * cfg.OversampleH, scale * cfg.OversampleV, 0, 0, &x0, &y0, &x1, &y1);
            src_tmp.Rects[glyph_i].w = (stbrp_coord)(x1 - x0 + padding + cfg.OversampleH - 1);
            src_tmp.Rects[glyph_i].h = (stbrp_coord)(y1 - y0 + padding + cfg.OversampleV - 1);
            total_surface += src_tmp.Rects[glyph_i].w * src_tmp.Rects[glyph_i].h;
        }
    }

    // We need a width for the skyline algorithm, any width!
    // The exact width doesn't really matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
    // User can override TexDesiredWidth and TexGlyphPadding if they wish, otherwise we use a simple heuristic to select the width based on expected surface.
    const int surface_sqrt = (int)ImSqrt((float)total_surface) + 1;
    atlas->TexHeight = 0;
    if (atlas->TexDesiredWidth > 0)
        atlas->TexWidth = atlas->TexDesiredWidth;
    else
        atlas->TexWidth = (surface_sqrt >= 4096*0.7f) ? 4096 : (surface_sqrt >= 2048*0.7f) ? 2048 : (surface_sqrt >= 1024*0.7f) ? 1024 : 512;

    // 5. Start packing
    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    const int TEX_HEIGHT_MAX = 1024 * 32;
    stbtt_pack_context spc = {};
    stbtt_PackBegin(&spc, NULL, atlas->TexWidth, TEX_HEIGHT_MAX, 0, atlas->TexGlyphPadding, NULL);
    ImFontAtlasBuildPackCustomRects(atlas, spc.pack_info);

    // 6. Pack each source font. No rendering yet, we are working with rectangles in an infinitely tall texture at this point.
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbrp_pack_rects((stbrp_context*)spc.pack_info, src_tmp.Rects, src_tmp.GlyphsCount);

        // Extend texture height and mark missing glyphs as non-packed so we won't render them.
        // FIXME: We are not handling packing failure here (would happen if we got off TEX_HEIGHT_MAX or if a single if larger than TexWidth?)
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
            if (src_tmp.Rects[glyph_i].was_packed)
                atlas->TexHeight = ImMax(atlas->TexHeight, src_tmp.Rects[glyph_i].y + src_tmp.Rects[glyph_i].h);
    }

    // 7. Allocate texture
    atlas->TexHeight = (atlas->Flags & ImFontAtlasFlags_NoPowerOfTwoHeight) ? (atlas->TexHeight + 1) : ImUpperPowerOfTwo(atlas->TexHeight);
    atlas->TexUvScale = ImVec2(1.0f / atlas->TexWidth, 1.0f / atlas->TexHeight);
    atlas->TexPixelsAlpha8 = (unsigned char*)IM_ALLOC(atlas->TexWidth * atlas->TexHeight);
    memset(atlas->TexPixelsAlpha8, 0, atlas->TexWidth * atlas->TexHeight);
    spc.pixels = atlas->TexPixelsAlpha8;
    spc.height = atlas->TexHeight;

    // 8. Render/rasterize font characters into the texture
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbtt_PackFontRangesRenderIntoRects(&spc, &src_tmp.FontInfo, &src_tmp.PackRange, 1, src_tmp.Rects);

        // Apply multiply operator
        if (cfg.RasterizerMultiply != 1.0f)
        {
            unsigned char multiply_table[256];
            ImFontAtlasBuildMultiplyCalcLookupTable(multiply_table, cfg.RasterizerMultiply);
            stbrp_rect* r = &src_tmp.Rects[0];
            for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++, r++)
                if (r->was_packed)
                    ImFontAtlasBuildMultiplyRectAlpha8(multiply_table, atlas->TexPixelsAlpha8, r->x, r->y, r->w, r->h, atlas->TexWidth * 1);
        }
        src_tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    buf_rects.clear();

    // 9. Setup ImFont and glyphs for runtime
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFont* dst_font = cfg.DstFont; // We can have multiple input fonts writing into a same destination font (when using MergeMode=true)

        const float font_scale = stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&src_tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        const float ascent = ImFloor(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1));
        const float descent = ImFloor(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1));
        ImFontAtlasBuildSetupFont(atlas, dst_font, &cfg, ascent, descent);
        const float font_off_x = cfg.GlyphOffset.x;
        const float font_off_y = cfg.GlyphOffset.y + (float)(int)(dst_font->Ascent + 0.5f);

        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
        {
            const int codepoint = src_tmp.GlyphsList[glyph_i];
            const stbtt_packedchar& pc = src_tmp.PackedChars[glyph_i];

            const float char_advance_x_org = pc.xadvance;
            const float char_advance_x_mod = ImClamp(char_advance_x_org, cfg.GlyphMinAdvanceX, cfg.GlyphMaxAdvanceX);
            float char_off_x = font_off_x;
            if (char_advance_x_org != char_advance_x_mod)
                char_off_x += cfg.PixelSnapH ? (float)(int)((char_advance_x_mod - char_advance_x_org) * 0.5f) : (char_advance_x_mod - char_advance_x_org) * 0.5f;

            // Register glyph
            stbtt_aligned_quad q;
            float dummy_x = 0.0f, dummy_y = 0.0f;
            stbtt_GetPackedQuad(src_tmp.PackedChars, atlas->TexWidth, atlas->TexHeight, glyph_i, &dummy_x, &dummy_y, &q, 0);
            dst_font->AddGlyph((ImWchar)codepoint, q.x0 + char_off_x, q.y0 + font_off_y, q.x1 + char_off_x, q.y1 + font_off_y, q.s0, q.t0, q.s1, q.t1, char_advance_x_mod);
        }
    }

    // Cleanup temporary (ImVector doesn't honor destructor)
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
        src_tmp_array[src_i].~ImFontBuildSrcData();

    ImFontAtlasBuildFinish(atlas);
    return true;
}

void ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas* atlas)
{
    if (atlas->CustomRectIds[0] >= 0)
        return;
    if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
        atlas->CustomRectIds[0] = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_ID, FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF*2+1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
    else
        atlas->CustomRectIds[0] = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_ID, 2, 2);
}

void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent)
{
    if (!font_config->MergeMode)
    {
        font->ClearOutputData();
        font->FontSize = font_config->SizePixels;
        font->ConfigData = font_config;
        font->ContainerAtlas = atlas;
        font->Ascent = ascent;
        font->Descent = descent;
    }
    font->ConfigDataCount++;
}

void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque)
{
    stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
    IM_ASSERT(pack_context != NULL);

    ImVector<ImFontAtlas::CustomRect>& user_rects = atlas->CustomRects;
    IM_ASSERT(user_rects.Size >= 1); // We expect at least the default custom rects to be registered, else something went wrong.

    ImVector<stbrp_rect> pack_rects;
    pack_rects.resize(user_rects.Size);
    memset(pack_rects.Data, 0, (size_t)pack_rects.size_in_bytes());
    for (int i = 0; i < user_rects.Size; i++)
    {
        pack_rects[i].w = user_rects[i].Width;
        pack_rects[i].h = user_rects[i].Height;
    }
    stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
    for (int i = 0; i < pack_rects.Size; i++)
        if (pack_rects[i].was_packed)
        {
            user_rects[i].X = pack_rects[i].x;
            user_rects[i].Y = pack_rects[i].y;
            IM_ASSERT(pack_rects[i].w == user_rects[i].Width && pack_rects[i].h == user_rects[i].Height);
            atlas->TexHeight = ImMax(atlas->TexHeight, pack_rects[i].y + pack_rects[i].h);
        }
}

static void ImFontAtlasBuildRenderDefaultTexData(ImFontAtlas* atlas)
{
    IM_ASSERT(atlas->CustomRectIds[0] >= 0);
    IM_ASSERT(atlas->TexPixelsAlpha8 != NULL);
    ImFontAtlas::CustomRect& r = atlas->CustomRects[atlas->CustomRectIds[0]];
    IM_ASSERT(r.ID == FONT_ATLAS_DEFAULT_TEX_DATA_ID);
    IM_ASSERT(r.IsPacked());

    const int w = atlas->TexWidth;
    if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
    {
        // Render/copy pixels
        IM_ASSERT(r.Width == FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * 2 + 1 && r.Height == FONT_ATLAS_DEFAULT_TEX_DATA_H);
        for (int y = 0, n = 0; y < FONT_ATLAS_DEFAULT_TEX_DATA_H; y++)
            for (int x = 0; x < FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF; x++, n++)
            {
                const int offset0 = (int)(r.X + x) + (int)(r.Y + y) * w;
                const int offset1 = offset0 + FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
                atlas->TexPixelsAlpha8[offset0] = FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[n] == '.' ? 0xFF : 0x00;
                atlas->TexPixelsAlpha8[offset1] = FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[n] == 'X' ? 0xFF : 0x00;
            }
    }
    else
    {
        IM_ASSERT(r.Width == 2 && r.Height == 2);
        const int offset = (int)(r.X) + (int)(r.Y) * w;
        atlas->TexPixelsAlpha8[offset] = atlas->TexPixelsAlpha8[offset + 1] = atlas->TexPixelsAlpha8[offset + w] = atlas->TexPixelsAlpha8[offset + w + 1] = 0xFF;
    }
    atlas->TexUvWhitePixel = ImVec2((r.X + 0.5f) * atlas->TexUvScale.x, (r.Y + 0.5f) * atlas->TexUvScale.y);
}

void ImFontAtlasBuildFinish(ImFontAtlas* atlas)
{
    // Render into our custom data block
    ImFontAtlasBuildRenderDefaultTexData(atlas);

    // Register custom rectangle glyphs
    for (int i = 0; i < atlas->CustomRects.Size; i++)
    {
        const ImFontAtlas::CustomRect& r = atlas->CustomRects[i];
        if (r.Font == NULL || r.ID > 0x10000)
            continue;

        IM_ASSERT(r.Font->ContainerAtlas == atlas);
        ImVec2 uv0, uv1;
        atlas->CalcCustomRectUV(&r, &uv0, &uv1);
        r.Font->AddGlyph((ImWchar)r.ID, r.GlyphOffset.x, r.GlyphOffset.y, r.GlyphOffset.x + r.Width, r.GlyphOffset.y + r.Height, uv0.x, uv0.y, uv1.x, uv1.y, r.GlyphAdvanceX);
    }

    // Build all fonts lookup tables
    for (int i = 0; i < atlas->Fonts.Size; i++)
        if (atlas->Fonts[i]->DirtyLookupTables)
            atlas->Fonts[i]->BuildLookupTable();
}

// Retrieve list of range (2 int per range, values are inclusive)
const ImWchar*   ImFontAtlas::GetGlyphRangesDefault()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesKorean()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3131, 0x3163, // Korean alphabets
        0xAC00, 0xD79D, // Korean characters
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesChineseFull()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };
    return &ranges[0];
}

static void UnpackAccumulativeOffsetsIntoRanges(int base_codepoint, const short* accumulative_offsets, int accumulative_offsets_count, ImWchar* out_ranges)
{
    for (int n = 0; n < accumulative_offsets_count; n++, out_ranges += 2)
    {
        out_ranges[0] = out_ranges[1] = (ImWchar)(base_codepoint + accumulative_offsets[n]);
        base_codepoint += accumulative_offsets[n];
    }
    out_ranges[0] = 0;
}

//-------------------------------------------------------------------------
// [SECTION] ImFontAtlas glyph ranges helpers
//-------------------------------------------------------------------------

const ImWchar*  ImFontAtlas::GetGlyphRangesChineseSimplifiedCommon()
{
    // Store 2500 regularly used characters for Simplified Chinese.
    // Sourced from https://zh.wiktionary.org/wiki/%E9%99%84%E5%BD%95:%E7%8E%B0%E4%BB%A3%E6%B1%89%E8%AF%AD%E5%B8%B8%E7%94%A8%E5%AD%97%E8%A1%A8
    // This table covers 97.97% of all characters used during the month in July, 1987.
    // You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
    // (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
    static const short accumulative_offsets_from_0x4E00[] =
    {
        0,1,2,4,1,1,1,1,2,1,3,2,1,2,2,1,1,1,1,1,5,2,1,2,3,3,3,2,2,4,1,1,1,2,1,5,2,3,1,2,1,2,1,1,2,1,1,2,2,1,4,1,1,1,1,5,10,1,2,19,2,1,2,1,2,1,2,1,2,
        1,5,1,6,3,2,1,2,2,1,1,1,4,8,5,1,1,4,1,1,3,1,2,1,5,1,2,1,1,1,10,1,1,5,2,4,6,1,4,2,2,2,12,2,1,1,6,1,1,1,4,1,1,4,6,5,1,4,2,2,4,10,7,1,1,4,2,4,
        2,1,4,3,6,10,12,5,7,2,14,2,9,1,1,6,7,10,4,7,13,1,5,4,8,4,1,1,2,28,5,6,1,1,5,2,5,20,2,2,9,8,11,2,9,17,1,8,6,8,27,4,6,9,20,11,27,6,68,2,2,1,1,
        1,2,1,2,2,7,6,11,3,3,1,1,3,1,2,1,1,1,1,1,3,1,1,8,3,4,1,5,7,2,1,4,4,8,4,2,1,2,1,1,4,5,6,3,6,2,12,3,1,3,9,2,4,3,4,1,5,3,3,1,3,7,1,5,1,1,1,1,2,
        3,4,5,2,3,2,6,1,1,2,1,7,1,7,3,4,5,15,2,2,1,5,3,22,19,2,1,1,1,1,2,5,1,1,1,6,1,1,12,8,2,9,18,22,4,1,1,5,1,16,1,2,7,10,15,1,1,6,2,4,1,2,4,1,6,
        1,1,3,2,4,1,6,4,5,1,2,1,1,2,1,10,3,1,3,2,1,9,3,2,5,7,2,19,4,3,6,1,1,1,1,1,4,3,2,1,1,1,2,5,3,1,1,1,2,2,1,1,2,1,1,2,1,3,1,1,1,3,7,1,4,1,1,2,1,
        1,2,1,2,4,4,3,8,1,1,1,2,1,3,5,1,3,1,3,4,6,2,2,14,4,6,6,11,9,1,15,3,1,28,5,2,5,5,3,1,3,4,5,4,6,14,3,2,3,5,21,2,7,20,10,1,2,19,2,4,28,28,2,3,
        2,1,14,4,1,26,28,42,12,40,3,52,79,5,14,17,3,2,2,11,3,4,6,3,1,8,2,23,4,5,8,10,4,2,7,3,5,1,1,6,3,1,2,2,2,5,28,1,1,7,7,20,5,3,29,3,17,26,1,8,4,
        27,3,6,11,23,5,3,4,6,13,24,16,6,5,10,25,35,7,3,2,3,3,14,3,6,2,6,1,4,2,3,8,2,1,1,3,3,3,4,1,1,13,2,2,4,5,2,1,14,14,1,2,2,1,4,5,2,3,1,14,3,12,
        3,17,2,16,5,1,2,1,8,9,3,19,4,2,2,4,17,25,21,20,28,75,1,10,29,103,4,1,2,1,1,4,2,4,1,2,3,24,2,2,2,1,1,2,1,3,8,1,1,1,2,1,1,3,1,1,1,6,1,5,3,1,1,
        1,3,4,1,1,5,2,1,5,6,13,9,16,1,1,1,1,3,2,3,2,4,5,2,5,2,2,3,7,13,7,2,2,1,1,1,1,2,3,3,2,1,6,4,9,2,1,14,2,14,2,1,18,3,4,14,4,11,41,15,23,15,23,
        176,1,3,4,1,1,1,1,5,3,1,2,3,7,3,1,1,2,1,2,4,4,6,2,4,1,9,7,1,10,5,8,16,29,1,1,2,2,3,1,3,5,2,4,5,4,1,1,2,2,3,3,7,1,6,10,1,17,1,44,4,6,2,1,1,6,
        5,4,2,10,1,6,9,2,8,1,24,1,2,13,7,8,8,2,1,4,1,3,1,3,3,5,2,5,10,9,4,9,12,2,1,6,1,10,1,1,7,7,4,10,8,3,1,13,4,3,1,6,1,3,5,2,1,2,17,16,5,2,16,6,
        1,4,2,1,3,3,6,8,5,11,11,1,3,3,2,4,6,10,9,5,7,4,7,4,7,1,1,4,2,1,3,6,8,7,1,6,11,5,5,3,24,9,4,2,7,13,5,1,8,82,16,61,1,1,1,4,2,2,16,10,3,8,1,1,
        6,4,2,1,3,1,1,1,4,3,8,4,2,2,1,1,1,1,1,6,3,5,1,1,4,6,9,2,1,1,1,2,1,7,2,1,6,1,5,4,4,3,1,8,1,3,3,1,3,2,2,2,2,3,1,6,1,2,1,2,1,3,7,1,8,2,1,2,1,5,
        2,5,3,5,10,1,2,1,1,3,2,5,11,3,9,3,5,1,1,5,9,1,2,1,5,7,9,9,8,1,3,3,3,6,8,2,3,2,1,1,32,6,1,2,15,9,3,7,13,1,3,10,13,2,14,1,13,10,2,1,3,10,4,15,
        2,15,15,10,1,3,9,6,9,32,25,26,47,7,3,2,3,1,6,3,4,3,2,8,5,4,1,9,4,2,2,19,10,6,2,3,8,1,2,2,4,2,1,9,4,4,4,6,4,8,9,2,3,1,1,1,1,3,5,5,1,3,8,4,6,
        2,1,4,12,1,5,3,7,13,2,5,8,1,6,1,2,5,14,6,1,5,2,4,8,15,5,1,23,6,62,2,10,1,1,8,1,2,2,10,4,2,2,9,2,1,1,3,2,3,1,5,3,3,2,1,3,8,1,1,1,11,3,1,1,4,
        3,7,1,14,1,2,3,12,5,2,5,1,6,7,5,7,14,11,1,3,1,8,9,12,2,1,11,8,4,4,2,6,10,9,13,1,1,3,1,5,1,3,2,4,4,1,18,2,3,14,11,4,29,4,2,7,1,3,13,9,2,2,5,
        3,5,20,7,16,8,5,72,34,6,4,22,12,12,28,45,36,9,7,39,9,191,1,1,1,4,11,8,4,9,2,3,22,1,1,1,1,4,17,1,7,7,1,11,31,10,2,4,8,2,3,2,1,4,2,16,4,32,2,
        3,19,13,4,9,1,5,2,14,8,1,1,3,6,19,6,5,1,16,6,2,10,8,5,1,2,3,1,5,5,1,11,6,6,1,3,3,2,6,3,8,1,1,4,10,7,5,7,7,5,8,9,2,1,3,4,1,1,3,1,3,3,2,6,16,
        1,4,6,3,1,10,6,1,3,15,2,9,2,10,25,13,9,16,6,2,2,10,11,4,3,9,1,2,6,6,5,4,30,40,1,10,7,12,14,33,6,3,6,7,3,1,3,1,11,14,4,9,5,12,11,49,18,51,31,
        140,31,2,2,1,5,1,8,1,10,1,4,4,3,24,1,10,1,3,6,6,16,3,4,5,2,1,4,2,57,10,6,22,2,22,3,7,22,6,10,11,36,18,16,33,36,2,5,5,1,1,1,4,10,1,4,13,2,7,
        5,2,9,3,4,1,7,43,3,7,3,9,14,7,9,1,11,1,1,3,7,4,18,13,1,14,1,3,6,10,73,2,2,30,6,1,11,18,19,13,22,3,46,42,37,89,7,3,16,34,2,2,3,9,1,7,1,1,1,2,
        2,4,10,7,3,10,3,9,5,28,9,2,6,13,7,3,1,3,10,2,7,2,11,3,6,21,54,85,2,1,4,2,2,1,39,3,21,2,2,5,1,1,1,4,1,1,3,4,15,1,3,2,4,4,2,3,8,2,20,1,8,7,13,
        4,1,26,6,2,9,34,4,21,52,10,4,4,1,5,12,2,11,1,7,2,30,12,44,2,30,1,1,3,6,16,9,17,39,82,2,2,24,7,1,7,3,16,9,14,44,2,1,2,1,2,3,5,2,4,1,6,7,5,3,
        2,6,1,11,5,11,2,1,18,19,8,1,3,24,29,2,1,3,5,2,2,1,13,6,5,1,46,11,3,5,1,1,5,8,2,10,6,12,6,3,7,11,2,4,16,13,2,5,1,1,2,2,5,2,28,5,2,23,10,8,4,
        4,22,39,95,38,8,14,9,5,1,13,5,4,3,13,12,11,1,9,1,27,37,2,5,4,4,63,211,95,2,2,2,1,3,5,2,1,1,2,2,1,1,1,3,2,4,1,2,1,1,5,2,2,1,1,2,3,1,3,1,1,1,
        3,1,4,2,1,3,6,1,1,3,7,15,5,3,2,5,3,9,11,4,2,22,1,6,3,8,7,1,4,28,4,16,3,3,25,4,4,27,27,1,4,1,2,2,7,1,3,5,2,28,8,2,14,1,8,6,16,25,3,3,3,14,3,
        3,1,1,2,1,4,6,3,8,4,1,1,1,2,3,6,10,6,2,3,18,3,2,5,5,4,3,1,5,2,5,4,23,7,6,12,6,4,17,11,9,5,1,1,10,5,12,1,1,11,26,33,7,3,6,1,17,7,1,5,12,1,11,
        2,4,1,8,14,17,23,1,2,1,7,8,16,11,9,6,5,2,6,4,16,2,8,14,1,11,8,9,1,1,1,9,25,4,11,19,7,2,15,2,12,8,52,7,5,19,2,16,4,36,8,1,16,8,24,26,4,6,2,9,
        5,4,36,3,28,12,25,15,37,27,17,12,59,38,5,32,127,1,2,9,17,14,4,1,2,1,1,8,11,50,4,14,2,19,16,4,17,5,4,5,26,12,45,2,23,45,104,30,12,8,3,10,2,2,
        3,3,1,4,20,7,2,9,6,15,2,20,1,3,16,4,11,15,6,134,2,5,59,1,2,2,2,1,9,17,3,26,137,10,211,59,1,2,4,1,4,1,1,1,2,6,2,3,1,1,2,3,2,3,1,3,4,4,2,3,3,
        1,4,3,1,7,2,2,3,1,2,1,3,3,3,2,2,3,2,1,3,14,6,1,3,2,9,6,15,27,9,34,145,1,1,2,1,1,1,1,2,1,1,1,1,2,2,2,3,1,2,1,1,1,2,3,5,8,3,5,2,4,1,3,2,2,2,12,
        4,1,1,1,10,4,5,1,20,4,16,1,15,9,5,12,2,9,2,5,4,2,26,19,7,1,26,4,30,12,15,42,1,6,8,172,1,1,4,2,1,1,11,2,2,4,2,1,2,1,10,8,1,2,1,4,5,1,2,5,1,8,
        4,1,3,4,2,1,6,2,1,3,4,1,2,1,1,1,1,12,5,7,2,4,3,1,1,1,3,3,6,1,2,2,3,3,3,2,1,2,12,14,11,6,6,4,12,2,8,1,7,10,1,35,7,4,13,15,4,3,23,21,28,52,5,
        26,5,6,1,7,10,2,7,53,3,2,1,1,1,2,163,532,1,10,11,1,3,3,4,8,2,8,6,2,2,23,22,4,2,2,4,2,1,3,1,3,3,5,9,8,2,1,2,8,1,10,2,12,21,20,15,105,2,3,1,1,
        3,2,3,1,1,2,5,1,4,15,11,19,1,1,1,1,5,4,5,1,1,2,5,3,5,12,1,2,5,1,11,1,1,15,9,1,4,5,3,26,8,2,1,3,1,1,15,19,2,12,1,2,5,2,7,2,19,2,20,6,26,7,5,
        2,2,7,34,21,13,70,2,128,1,1,2,1,1,2,1,1,3,2,2,2,15,1,4,1,3,4,42,10,6,1,49,85,8,1,2,1,1,4,4,2,3,6,1,5,7,4,3,211,4,1,2,1,2,5,1,2,4,2,2,6,5,6,
        10,3,4,48,100,6,2,16,296,5,27,387,2,2,3,7,16,8,5,38,15,39,21,9,10,3,7,59,13,27,21,47,5,21,6
    };
    static ImWchar base_ranges[] = // not zero-terminated
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF  // Half-width characters
    };
    static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2 + 1] = { 0 };
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, IM_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + IM_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesJapanese()
{
    // 1946 common ideograms code points for Japanese
    // Sourced from http://theinstructionlimit.com/common-kanji-character-ranges-for-xna-spritefont-rendering
    // FIXME: Source a list of the revised 2136 Joyo Kanji list from 2010 and rebuild this.
    // You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
    // (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
    static const short accumulative_offsets_from_0x4E00[] =
    {
        0,1,2,4,1,1,1,1,2,1,6,2,2,1,8,5,7,11,1,2,10,10,8,2,4,20,2,11,8,2,1,2,1,6,2,1,7,5,3,7,1,1,13,7,9,1,4,6,1,2,1,10,1,1,9,2,2,4,5,6,14,1,1,9,3,18,
        5,4,2,2,10,7,1,1,1,3,2,4,3,23,2,10,12,2,14,2,4,13,1,6,10,3,1,7,13,6,4,13,5,2,3,17,2,2,5,7,6,4,1,7,14,16,6,13,9,15,1,1,7,16,4,7,1,19,9,2,7,15,
        2,6,5,13,25,4,14,13,11,25,1,1,1,2,1,2,2,3,10,11,3,3,1,1,4,4,2,1,4,9,1,4,3,5,5,2,7,12,11,15,7,16,4,5,16,2,1,1,6,3,3,1,1,2,7,6,6,7,1,4,7,6,1,1,
        2,1,12,3,3,9,5,8,1,11,1,2,3,18,20,4,1,3,6,1,7,3,5,5,7,2,2,12,3,1,4,2,3,2,3,11,8,7,4,17,1,9,25,1,1,4,2,2,4,1,2,7,1,1,1,3,1,2,6,16,1,2,1,1,3,12,
        20,2,5,20,8,7,6,2,1,1,1,1,6,2,1,2,10,1,1,6,1,3,1,2,1,4,1,12,4,1,3,1,1,1,1,1,10,4,7,5,13,1,15,1,1,30,11,9,1,15,38,14,1,32,17,20,1,9,31,2,21,9,
        4,49,22,2,1,13,1,11,45,35,43,55,12,19,83,1,3,2,3,13,2,1,7,3,18,3,13,8,1,8,18,5,3,7,25,24,9,24,40,3,17,24,2,1,6,2,3,16,15,6,7,3,12,1,9,7,3,3,
        3,15,21,5,16,4,5,12,11,11,3,6,3,2,31,3,2,1,1,23,6,6,1,4,2,6,5,2,1,1,3,3,22,2,6,2,3,17,3,2,4,5,1,9,5,1,1,6,15,12,3,17,2,14,2,8,1,23,16,4,2,23,
        8,15,23,20,12,25,19,47,11,21,65,46,4,3,1,5,6,1,2,5,26,2,1,1,3,11,1,1,1,2,1,2,3,1,1,10,2,3,1,1,1,3,6,3,2,2,6,6,9,2,2,2,6,2,5,10,2,4,1,2,1,2,2,
        3,1,1,3,1,2,9,23,9,2,1,1,1,1,5,3,2,1,10,9,6,1,10,2,31,25,3,7,5,40,1,15,6,17,7,27,180,1,3,2,2,1,1,1,6,3,10,7,1,3,6,17,8,6,2,2,1,3,5,5,8,16,14,
        15,1,1,4,1,2,1,1,1,3,2,7,5,6,2,5,10,1,4,2,9,1,1,11,6,1,44,1,3,7,9,5,1,3,1,1,10,7,1,10,4,2,7,21,15,7,2,5,1,8,3,4,1,3,1,6,1,4,2,1,4,10,8,1,4,5,
        1,5,10,2,7,1,10,1,1,3,4,11,10,29,4,7,3,5,2,3,33,5,2,19,3,1,4,2,6,31,11,1,3,3,3,1,8,10,9,12,11,12,8,3,14,8,6,11,1,4,41,3,1,2,7,13,1,5,6,2,6,12,
        12,22,5,9,4,8,9,9,34,6,24,1,1,20,9,9,3,4,1,7,2,2,2,6,2,28,5,3,6,1,4,6,7,4,2,1,4,2,13,6,4,4,3,1,8,8,3,2,1,5,1,2,2,3,1,11,11,7,3,6,10,8,6,16,16,
        22,7,12,6,21,5,4,6,6,3,6,1,3,2,1,2,8,29,1,10,1,6,13,6,6,19,31,1,13,4,4,22,17,26,33,10,4,15,12,25,6,67,10,2,3,1,6,10,2,6,2,9,1,9,4,4,1,2,16,2,
        5,9,2,3,8,1,8,3,9,4,8,6,4,8,11,3,2,1,1,3,26,1,7,5,1,11,1,5,3,5,2,13,6,39,5,1,5,2,11,6,10,5,1,15,5,3,6,19,21,22,2,4,1,6,1,8,1,4,8,2,4,2,2,9,2,
        1,1,1,4,3,6,3,12,7,1,14,2,4,10,2,13,1,17,7,3,2,1,3,2,13,7,14,12,3,1,29,2,8,9,15,14,9,14,1,3,1,6,5,9,11,3,38,43,20,7,7,8,5,15,12,19,15,81,8,7,
        1,5,73,13,37,28,8,8,1,15,18,20,165,28,1,6,11,8,4,14,7,15,1,3,3,6,4,1,7,14,1,1,11,30,1,5,1,4,14,1,4,2,7,52,2,6,29,3,1,9,1,21,3,5,1,26,3,11,14,
        11,1,17,5,1,2,1,3,2,8,1,2,9,12,1,1,2,3,8,3,24,12,7,7,5,17,3,3,3,1,23,10,4,4,6,3,1,16,17,22,3,10,21,16,16,6,4,10,2,1,1,2,8,8,6,5,3,3,3,39,25,
        15,1,1,16,6,7,25,15,6,6,12,1,22,13,1,4,9,5,12,2,9,1,12,28,8,3,5,10,22,60,1,2,40,4,61,63,4,1,13,12,1,4,31,12,1,14,89,5,16,6,29,14,2,5,49,18,18,
        5,29,33,47,1,17,1,19,12,2,9,7,39,12,3,7,12,39,3,1,46,4,12,3,8,9,5,31,15,18,3,2,2,66,19,13,17,5,3,46,124,13,57,34,2,5,4,5,8,1,1,1,4,3,1,17,5,
        3,5,3,1,8,5,6,3,27,3,26,7,12,7,2,17,3,7,18,78,16,4,36,1,2,1,6,2,1,39,17,7,4,13,4,4,4,1,10,4,2,4,6,3,10,1,19,1,26,2,4,33,2,73,47,7,3,8,2,4,15,
        18,1,29,2,41,14,1,21,16,41,7,39,25,13,44,2,2,10,1,13,7,1,7,3,5,20,4,8,2,49,1,10,6,1,6,7,10,7,11,16,3,12,20,4,10,3,1,2,11,2,28,9,2,4,7,2,15,1,
        27,1,28,17,4,5,10,7,3,24,10,11,6,26,3,2,7,2,2,49,16,10,16,15,4,5,27,61,30,14,38,22,2,7,5,1,3,12,23,24,17,17,3,3,2,4,1,6,2,7,5,1,1,5,1,1,9,4,
        1,3,6,1,8,2,8,4,14,3,5,11,4,1,3,32,1,19,4,1,13,11,5,2,1,8,6,8,1,6,5,13,3,23,11,5,3,16,3,9,10,1,24,3,198,52,4,2,2,5,14,5,4,22,5,20,4,11,6,41,
        1,5,2,2,11,5,2,28,35,8,22,3,18,3,10,7,5,3,4,1,5,3,8,9,3,6,2,16,22,4,5,5,3,3,18,23,2,6,23,5,27,8,1,33,2,12,43,16,5,2,3,6,1,20,4,2,9,7,1,11,2,
        10,3,14,31,9,3,25,18,20,2,5,5,26,14,1,11,17,12,40,19,9,6,31,83,2,7,9,19,78,12,14,21,76,12,113,79,34,4,1,1,61,18,85,10,2,2,13,31,11,50,6,33,159,
        179,6,6,7,4,4,2,4,2,5,8,7,20,32,22,1,3,10,6,7,28,5,10,9,2,77,19,13,2,5,1,4,4,7,4,13,3,9,31,17,3,26,2,6,6,5,4,1,7,11,3,4,2,1,6,2,20,4,1,9,2,6,
        3,7,1,1,1,20,2,3,1,6,2,3,6,2,4,8,1,5,13,8,4,11,23,1,10,6,2,1,3,21,2,2,4,24,31,4,10,10,2,5,192,15,4,16,7,9,51,1,2,1,1,5,1,1,2,1,3,5,3,1,3,4,1,
        3,1,3,3,9,8,1,2,2,2,4,4,18,12,92,2,10,4,3,14,5,25,16,42,4,14,4,2,21,5,126,30,31,2,1,5,13,3,22,5,6,6,20,12,1,14,12,87,3,19,1,8,2,9,9,3,3,23,2,
        3,7,6,3,1,2,3,9,1,3,1,6,3,2,1,3,11,3,1,6,10,3,2,3,1,2,1,5,1,1,11,3,6,4,1,7,2,1,2,5,5,34,4,14,18,4,19,7,5,8,2,6,79,1,5,2,14,8,2,9,2,1,36,28,16,
        4,1,1,1,2,12,6,42,39,16,23,7,15,15,3,2,12,7,21,64,6,9,28,8,12,3,3,41,59,24,51,55,57,294,9,9,2,6,2,15,1,2,13,38,90,9,9,9,3,11,7,1,1,1,5,6,3,2,
        1,2,2,3,8,1,4,4,1,5,7,1,4,3,20,4,9,1,1,1,5,5,17,1,5,2,6,2,4,1,4,5,7,3,18,11,11,32,7,5,4,7,11,127,8,4,3,3,1,10,1,1,6,21,14,1,16,1,7,1,3,6,9,65,
        51,4,3,13,3,10,1,1,12,9,21,110,3,19,24,1,1,10,62,4,1,29,42,78,28,20,18,82,6,3,15,6,84,58,253,15,155,264,15,21,9,14,7,58,40,39,
    };
    static ImWchar base_ranges[] = // not zero-terminated
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF  // Half-width characters
    };
    static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(accumulative_offsets_from_0x4E00)*2 + 1] = { 0 };
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, IM_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + IM_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesCyrillic()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesThai()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin
        0x2010, 0x205E, // Punctuations
        0x0E00, 0x0E7F, // Thai
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesVietnamese()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin
        0x0102, 0x0103,
        0x0110, 0x0111,
        0x0128, 0x0129,
        0x0168, 0x0169,
        0x01A0, 0x01A1,
        0x01AF, 0x01B0,
        0x1EA0, 0x1EF9,
        0,
    };
    return &ranges[0];
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontGlyphRangesBuilder
//-----------------------------------------------------------------------------

void ImFontGlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
    while (text_end ? (text < text_end) : *text)
    {
        unsigned int c = 0;
        int c_len = ImTextCharFromUtf8(&c, text, text_end);
        text += c_len;
        if (c_len == 0)
            break;
        if (c < 0x10000)
            AddChar((ImWchar)c);
    }
}

void ImFontGlyphRangesBuilder::AddRanges(const ImWchar* ranges)
{
    for (; ranges[0]; ranges += 2)
        for (ImWchar c = ranges[0]; c <= ranges[1]; c++)
            AddChar(c);
}

void ImFontGlyphRangesBuilder::BuildRanges(ImVector<ImWchar>* out_ranges)
{
    int max_codepoint = 0x10000;
    for (int n = 0; n < max_codepoint; n++)
        if (GetBit(n))
        {
            out_ranges->push_back((ImWchar)n);
            while (n < max_codepoint - 1 && GetBit(n + 1))
                n++;
            out_ranges->push_back((ImWchar)n);
        }
    out_ranges->push_back(0);
}

//-----------------------------------------------------------------------------
// [SECTION] ImFont
//-----------------------------------------------------------------------------

ImFont::ImFont()
{
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    FallbackChar = (ImWchar)'?';
    DisplayOffset = ImVec2(0.0f, 0.0f);
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    ConfigData = NULL;
    ConfigDataCount = 0;
    DirtyLookupTables = false;
    Scale = 1.0f;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
}

ImFont::~ImFont()
{
    ClearOutputData();
}

void    ImFont::ClearOutputData()
{
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    Glyphs.clear();
    IndexAdvanceX.clear();
    IndexLookup.clear();
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    DirtyLookupTables = true;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
}

void ImFont::BuildLookupTable()
{
    int max_codepoint = 0;
    for (int i = 0; i != Glyphs.Size; i++)
        max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

    IM_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
    IndexAdvanceX.clear();
    IndexLookup.clear();
    DirtyLookupTables = false;
    GrowIndex(max_codepoint + 1);
    for (int i = 0; i < Glyphs.Size; i++)
    {
        int codepoint = (int)Glyphs[i].Codepoint;
        IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
        IndexLookup[codepoint] = (ImWchar)i;
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
    if (FindGlyph((ImWchar)' '))
    {
        if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times
            Glyphs.resize(Glyphs.Size + 1);
        ImFontGlyph& tab_glyph = Glyphs.back();
        tab_glyph = *FindGlyph((ImWchar)' ');
        tab_glyph.Codepoint = '\t';
        tab_glyph.AdvanceX *= IM_TABSIZE;
        IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
        IndexLookup[(int)tab_glyph.Codepoint] = (ImWchar)(Glyphs.Size-1);
    }

    FallbackGlyph = FindGlyphNoFallback(FallbackChar);
    FallbackAdvanceX = FallbackGlyph ? FallbackGlyph->AdvanceX : 0.0f;
    for (int i = 0; i < max_codepoint + 1; i++)
        if (IndexAdvanceX[i] < 0.0f)
            IndexAdvanceX[i] = FallbackAdvanceX;
}

void ImFont::SetFallbackChar(ImWchar c)
{
    FallbackChar = c;
    BuildLookupTable();
}

void ImFont::GrowIndex(int new_size)
{
    IM_ASSERT(IndexAdvanceX.Size == IndexLookup.Size);
    if (new_size <= IndexLookup.Size)
        return;
    IndexAdvanceX.resize(new_size, -1.0f);
    IndexLookup.resize(new_size, (ImWchar)-1);
}

// x0/y0/x1/y1 are offset from the character upper-left layout position, in pixels. Therefore x0/y0 are often fairly close to zero.
// Not to be mistaken with texture coordinates, which are held by u0/v0/u1/v1 in normalized format (0.0..1.0 on each texture axis).
void ImFont::AddGlyph(ImWchar codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
    Glyphs.resize(Glyphs.Size + 1);
    ImFontGlyph& glyph = Glyphs.back();
    glyph.Codepoint = (ImWchar)codepoint;
    glyph.X0 = x0;
    glyph.Y0 = y0;
    glyph.X1 = x1;
    glyph.Y1 = y1;
    glyph.U0 = u0;
    glyph.V0 = v0;
    glyph.U1 = u1;
    glyph.V1 = v1;
    glyph.AdvanceX = advance_x + ConfigData->GlyphExtraSpacing.x;  // Bake spacing into AdvanceX

    if (ConfigData->PixelSnapH)
        glyph.AdvanceX = (float)(int)(glyph.AdvanceX + 0.5f);

    // Compute rough surface usage metrics (+1 to account for average padding, +0.99 to round)
    DirtyLookupTables = true;
    MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + 1.99f) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + 1.99f);
}

void ImFont::AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst)
{
    IM_ASSERT(IndexLookup.Size > 0);    // Currently this can only be called AFTER the font has been built, aka after calling ImFontAtlas::GetTexDataAs*() function.
    int index_size = IndexLookup.Size;

    if (dst < index_size && IndexLookup.Data[dst] == (ImWchar)-1 && !overwrite_dst) // 'dst' already exists
        return;
    if (src >= index_size && dst >= index_size) // both 'dst' and 'src' don't exist -> no-op
        return;

    GrowIndex(dst + 1);
    IndexLookup[dst] = (src < index_size) ? IndexLookup.Data[src] : (ImWchar)-1;
    IndexAdvanceX[dst] = (src < index_size) ? IndexAdvanceX.Data[src] : 1.0f;
}

const ImFontGlyph* ImFont::FindGlyph(ImWchar c) const
{
    if (c >= IndexLookup.Size)
        return FallbackGlyph;
    const ImWchar i = IndexLookup.Data[c];
    if (i == (ImWchar)-1)
        return FallbackGlyph;
    return &Glyphs.Data[i];
}

const ImFontGlyph* ImFont::FindGlyphNoFallback(ImWchar c) const
{
    if (c >= IndexLookup.Size)
        return NULL;
    const ImWchar i = IndexLookup.Data[c];
    if (i == (ImWchar)-1)
        return NULL;
    return &Glyphs.Data[i];
}

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
    // Simple word-wrapping for English, not full-featured. Please submit failing cases!
    // FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)

    // For references, possible wrap point marked with ^
    //  "aaa bbb, ccc,ddd. eee   fff. ggg!"
    //      ^    ^    ^   ^   ^__    ^    ^

    // List of hardcoded separators: .,;!?'"

    // Skip extra blanks after a line returns (that includes not counting them in width computation)
    // e.g. "Hello    world" --> "Hello" "World"

    // Cut words that cannot possibly fit within one line.
    // e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"

    float line_width = 0.0f;
    float word_width = 0.0f;
    float blank_width = 0.0f;
    wrap_width /= scale; // We work with unscaled widths to avoid scaling every characters

    const char* word_end = text;
    const char* prev_word_end = NULL;
    bool inside_word = true;

    const char* s = text;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        const char* next_s;
        if (c < 0x80)
            next_s = s + 1;
        else
            next_s = s + ImTextCharFromUtf8(&c, s, text_end);
        if (c == 0)
            break;

        if (c < 32)
        {
            if (c == '\n')
            {
                line_width = word_width = blank_width = 0.0f;
                inside_word = true;
                s = next_s;
                continue;
            }
            if (c == '\r')
            {
                s = next_s;
                continue;
            }
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX);
        if (ImCharIsBlankW(c))
        {
            if (inside_word)
            {
                line_width += blank_width;
                blank_width = 0.0f;
                word_end = s;
            }
            blank_width += char_width;
            inside_word = false;
        }
        else
        {
            word_width += char_width;
            if (inside_word)
            {
                word_end = next_s;
            }
            else
            {
                prev_word_end = word_end;
                line_width += word_width + blank_width;
                word_width = blank_width = 0.0f;
            }

            // Allow wrapping after punctuation.
            inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
        }

        // We ignore blank width at the end of the line (they can be skipped)
        if (line_width + word_width >= wrap_width)
        {
            // Words that cannot possibly fit within an entire line will be cut anywhere.
            if (word_width < wrap_width)
                s = prev_word_end ? prev_word_end : word_end;
            break;
        }

        s = next_s;
    }

    return s;
}

ImVec2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

    const float line_height = size;
    const float scale = size / FontSize;

    ImVec2 text_size = ImVec2(0,0);
    float line_width = 0.0f;

    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    const char* s = text_begin;
    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                if (text_size.x < line_width)
                    text_size.x = line_width;
                text_size.y += line_height;
                line_width = 0.0f;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsBlankA(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        const char* prev_s = s;
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                text_size.x = ImMax(text_size.x, line_width);
                text_size.y += line_height;
                line_width = 0.0f;
                continue;
            }
            if (c == '\r')
                continue;
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX) * scale;
        if (line_width + char_width >= max_width)
        {
            s = prev_s;
            break;
        }

        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

void ImFont::RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c) const
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') // Match behavior of RenderText(), those 4 codepoints are hard-coded.
        return;
    if (const ImFontGlyph* glyph = FindGlyph(c))
    {
        float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
        pos.x = (float)(int)pos.x + DisplayOffset.x;
        pos.y = (float)(int)pos.y + DisplayOffset.y;
        draw_list->PrimReserve(6, 4);
        draw_list->PrimRectUV(ImVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale), ImVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale), ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V1), col);
    }
}

void ImFont::RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // ImGui functions generally already provides a valid text_end, so this is merely to handle direct calls.

    // Align to be pixel perfect
    pos.x = (float)(int)pos.x + DisplayOffset.x;
    pos.y = (float)(int)pos.y + DisplayOffset.y;
    float x = pos.x;
    float y = pos.y;
    if (y > clip_rect.w)
        return;

    const float scale = size / FontSize;
    const float line_height = FontSize * scale;
    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    // Fast-forward to first visible line
    const char* s = text_begin;
    if (y + line_height < clip_rect.y && !word_wrap_enabled)
        while (y + line_height < clip_rect.y && s < text_end)
        {
            s = (const char*)memchr(s, '\n', text_end - s);
            s = s ? s + 1 : text_end;
            y += line_height;
        }

    // For large text, scan for the last visible line in order to avoid over-reserving in the call to PrimReserve()
    // Note that very large horizontal line will still be affected by the issue (e.g. a one megabyte string buffer without a newline will likely crash atm)
    if (text_end - s > 10000 && !word_wrap_enabled)
    {
        const char* s_end = s;
        float y_end = y;
        while (y_end < clip_rect.w && s_end < text_end)
        {
            s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
            s_end = s_end ? s_end + 1 : text_end;
            y_end += line_height;
        }
        text_end = s_end;
    }
    if (s == text_end)
        return;

    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    const int vtx_count_max = (int)(text_end - s) * 4;
    const int idx_count_max = (int)(text_end - s) * 6;
    const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
    draw_list->PrimReserve(idx_count_max, vtx_count_max);

    ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
    ImDrawIdx* idx_write = draw_list->_IdxWritePtr;
    unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;

    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                x = pos.x;
                y += line_height;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsBlankA(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                x = pos.x;
                y += line_height;
                if (y > clip_rect.w)
                    break; // break out of main loop
                continue;
            }
            if (c == '\r')
                continue;
        }

        float char_width = 0.0f;
        if (const ImFontGlyph* glyph = FindGlyph((ImWchar)c))
        {
            char_width = glyph->AdvanceX * scale;

            // Arbitrarily assume that both space and tabs are empty glyphs as an optimization
            if (c != ' ' && c != '\t')
            {
                // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
                float x1 = x + glyph->X0 * scale;
                float x2 = x + glyph->X1 * scale;
                float y1 = y + glyph->Y0 * scale;
                float y2 = y + glyph->Y1 * scale;
                if (x1 <= clip_rect.z && x2 >= clip_rect.x)
                {
                    // Render a character
                    float u1 = glyph->U0;
                    float v1 = glyph->V0;
                    float u2 = glyph->U1;
                    float v2 = glyph->V1;

                    // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
                    if (cpu_fine_clip)
                    {
                        if (x1 < clip_rect.x)
                        {
                            u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
                            x1 = clip_rect.x;
                        }
                        if (y1 < clip_rect.y)
                        {
                            v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
                            y1 = clip_rect.y;
                        }
                        if (x2 > clip_rect.z)
                        {
                            u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
                            x2 = clip_rect.z;
                        }
                        if (y2 > clip_rect.w)
                        {
                            v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
                            y2 = clip_rect.w;
                        }
                        if (y1 >= y2)
                        {
                            x += char_width;
                            continue;
                        }
                    }

                    // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
                    {
                        idx_write[0] = (ImDrawIdx)(vtx_current_idx); idx_write[1] = (ImDrawIdx)(vtx_current_idx+1); idx_write[2] = (ImDrawIdx)(vtx_current_idx+2);
                        idx_write[3] = (ImDrawIdx)(vtx_current_idx); idx_write[4] = (ImDrawIdx)(vtx_current_idx+2); idx_write[5] = (ImDrawIdx)(vtx_current_idx+3);
                        vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
                        vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
                        vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
                        vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
                        vtx_write += 4;
                        vtx_current_idx += 4;
                        idx_write += 6;
                    }
                }
            }
        }

        x += char_width;
    }

    // Give back unused vertices
    draw_list->VtxBuffer.resize((int)(vtx_write - draw_list->VtxBuffer.Data));
    draw_list->IdxBuffer.resize((int)(idx_write - draw_list->IdxBuffer.Data));
    draw_list->CmdBuffer[draw_list->CmdBuffer.Size-1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
    draw_list->_VtxWritePtr = vtx_write;
    draw_list->_IdxWritePtr = idx_write;
    draw_list->_VtxCurrentIdx = vtx_current_idx;
}

//-----------------------------------------------------------------------------
// [SECTION] Internal Render Helpers
// (progressively moved from imgui.cpp to here when they are redesigned to stop accessing ImGui global state)
//-----------------------------------------------------------------------------
// - RenderMouseCursor()
// - RenderArrowPointingAt()
// - RenderRectFilledRangeH()
// - RenderPixelEllipsis()
//-----------------------------------------------------------------------------

void ImGui::RenderMouseCursor(ImDrawList* draw_list, ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor)
{
    if (mouse_cursor == ImGuiMouseCursor_None)
        return;
    IM_ASSERT(mouse_cursor > ImGuiMouseCursor_None && mouse_cursor < ImGuiMouseCursor_COUNT);

    const ImU32 col_shadow = IM_COL32(0, 0, 0, 48);
    const ImU32 col_border = IM_COL32(0, 0, 0, 255);          // Black
    const ImU32 col_fill   = IM_COL32(255, 255, 255, 255);    // White

    ImFontAtlas* font_atlas = draw_list->_Data->Font->ContainerAtlas;
    ImVec2 offset, size, uv[4];
    if (font_atlas->GetMouseCursorTexData(mouse_cursor, &offset, &size, &uv[0], &uv[2]))
    {
        pos -= offset;
        const ImTextureID tex_id = font_atlas->TexID;
        draw_list->PushTextureID(tex_id);
        draw_list->AddImage(tex_id, pos + ImVec2(1,0)*scale, pos + ImVec2(1,0)*scale + size*scale, uv[2], uv[3], col_shadow);
        draw_list->AddImage(tex_id, pos + ImVec2(2,0)*scale, pos + ImVec2(2,0)*scale + size*scale, uv[2], uv[3], col_shadow);
        draw_list->AddImage(tex_id, pos,                     pos + size*scale,                     uv[2], uv[3], col_border);
        draw_list->AddImage(tex_id, pos,                     pos + size*scale,                     uv[0], uv[1], col_fill);
        draw_list->PopTextureID();
    }
}

// Render an arrow. 'pos' is position of the arrow tip. half_sz.x is length from base to tip. half_sz.y is length on each side.
void ImGui::RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col)
{
    switch (direction)
    {
    case ImGuiDir_Left:  draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Right: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), pos, col); return;
    case ImGuiDir_Up:    draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Down:  draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), pos, col); return;
    case ImGuiDir_None: case ImGuiDir_COUNT: break; // Fix warnings
    }
}

static inline float ImAcos01(float x)
{
    if (x <= 0.0f) return IM_PI * 0.5f;
    if (x >= 1.0f) return 0.0f;
    return ImAcos(x);
    //return (-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f; // Cheap approximation, may be enough for what we do.
}

// FIXME: Cleanup and move code to ImDrawList.
void ImGui::RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding)
{
    if (x_end_norm == x_start_norm)
        return;
    if (x_start_norm > x_end_norm)
        ImSwap(x_start_norm, x_end_norm);

    ImVec2 p0 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_start_norm), rect.Min.y);
    ImVec2 p1 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_end_norm), rect.Max.y);
    if (rounding == 0.0f)
    {
        draw_list->AddRectFilled(p0, p1, col, 0.0f);
        return;
    }

    rounding = ImClamp(ImMin((rect.Max.x - rect.Min.x) * 0.5f, (rect.Max.y - rect.Min.y) * 0.5f) - 1.0f, 0.0f, rounding);
    const float inv_rounding = 1.0f / rounding;
    const float arc0_b = ImAcos01(1.0f - (p0.x - rect.Min.x) * inv_rounding);
    const float arc0_e = ImAcos01(1.0f - (p1.x - rect.Min.x) * inv_rounding);
    const float half_pi = IM_PI * 0.5f; // We will == compare to this because we know this is the exact value ImAcos01 can return.
    const float x0 = ImMax(p0.x, rect.Min.x + rounding);
    if (arc0_b == arc0_e)
    {
        draw_list->PathLineTo(ImVec2(x0, p1.y));
        draw_list->PathLineTo(ImVec2(x0, p0.y));
    }
    else if (arc0_b == 0.0f && arc0_e == half_pi)
    {
        draw_list->PathArcToFast(ImVec2(x0, p1.y - rounding), rounding, 3, 6); // BL
        draw_list->PathArcToFast(ImVec2(x0, p0.y + rounding), rounding, 6, 9); // TR
    }
    else
    {
        draw_list->PathArcTo(ImVec2(x0, p1.y - rounding), rounding, IM_PI - arc0_e, IM_PI - arc0_b, 3); // BL
        draw_list->PathArcTo(ImVec2(x0, p0.y + rounding), rounding, IM_PI + arc0_b, IM_PI + arc0_e, 3); // TR
    }
    if (p1.x > rect.Min.x + rounding)
    {
        const float arc1_b = ImAcos01(1.0f - (rect.Max.x - p1.x) * inv_rounding);
        const float arc1_e = ImAcos01(1.0f - (rect.Max.x - p0.x) * inv_rounding);
        const float x1 = ImMin(p1.x, rect.Max.x - rounding);
        if (arc1_b == arc1_e)
        {
            draw_list->PathLineTo(ImVec2(x1, p0.y));
            draw_list->PathLineTo(ImVec2(x1, p1.y));
        }
        else if (arc1_b == 0.0f && arc1_e == half_pi)
        {
            draw_list->PathArcToFast(ImVec2(x1, p0.y + rounding), rounding, 9, 12); // TR
            draw_list->PathArcToFast(ImVec2(x1, p1.y - rounding), rounding, 0, 3);  // BR
        }
        else
        {
            draw_list->PathArcTo(ImVec2(x1, p0.y + rounding), rounding, -arc1_e, -arc1_b, 3); // TR
            draw_list->PathArcTo(ImVec2(x1, p1.y - rounding), rounding, +arc1_b, +arc1_e, 3); // BR
        }
    }
    draw_list->PathFillConvex(col);
}

// FIXME: Rendering an ellipsis "..." is a surprisingly tricky problem for us... we cannot rely on font glyph having it,
// and regular dot are typically too wide. If we render a dot/shape ourselves it comes with the risk that it wouldn't match
// the boldness or positioning of what the font uses...
void ImGui::RenderPixelEllipsis(ImDrawList* draw_list, ImVec2 pos, int count, ImU32 col)
{
    ImFont* font = draw_list->_Data->Font;
    const float font_scale = draw_list->_Data->FontSize / font->FontSize;
    pos.y += (float)(int)(font->DisplayOffset.y + font->Ascent * font_scale + 0.5f - 1.0f);
    for (int dot_n = 0; dot_n < count; dot_n++)
        draw_list->AddRectFilled(ImVec2(pos.x + dot_n * 2.0f, pos.y), ImVec2(pos.x + dot_n * 2.0f + 1.0f, pos.y + 1.0f), col);
}

//-----------------------------------------------------------------------------
// [SECTION] Decompression code
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array and encoded as base85.
// Use the program in misc/fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(const unsigned char *input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier_out_e, *stb__barrier_out_b;
static const unsigned char *stb__barrier_in_b;
static unsigned char *stb__dout;
static void stb__match(const unsigned char *data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e+1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char *data, unsigned int length)
{
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e+1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char *stb_decompress_token(const unsigned char *i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout-i[1]-1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout-(stb__in2(0) - 0x4000 + 1), i[2]+1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i+1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout-(stb__in3(0) - 0x180000 + 1), i[3]+1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout-(stb__in3(0) - 0x100000 + 1), stb__in2(3)+1), i += 5;
        else if (*i >= 0x08)  stb__lit(i+2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i+3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout-(stb__in3(1)+1), i[4]+1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout-(stb__in3(1)+1), stb__in2(4)+1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char *buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen, i;

    blocklen = buflen % 5552;
    while (buflen) {
        for (i=0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char *output, const unsigned char *i, unsigned int /*length*/)
{
    unsigned int olen;
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        const unsigned char *old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                IM_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2))
                    return 0;
                return olen;
            } else {
                IM_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        IM_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Default font data (GroupType - Bank Gothic Pro Light.otf)
//-----------------------------------------------------------------------------
// File: 'GroupType - Bank Gothic Pro Light.otf' (34216 bytes)
// Exported using binary_to_compressed_c.cpp
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
//-----------------------------------------------------------------------------
static const char bank_gothic_pro_light_otf_compressed_data_base85[34530 + 1] =
"7])#######Q5e0Y'/###ZE7+>'wPP&Ql#v#2mLT9EMdK-Ra+##:G###J,0h</pF8_3g###OC*##kK.e=6Zj$`WS)##iO4U%IL<`<?.41Da-34#D<F&#<;isBb<xeuZ/q.#Ngk&#W:N]D"
"DMkmr5_,oLH5P$M;$tLD6q;W%j&;*Mq1N*M?O%KDDTf/,,j,)#w%D5#e:V)Fo;>J#Yg3O+5O$iLoMbAF(('[J'j8*MPw-tL2,%4G%?c+vkG.%#p&3<#C9F&#/k8g1VpsSpplv^AeI1=#"
"?<5)MwKg:d2PZYNm:kx+'nE)<M(aw'K>YY#t+9;-kbrE/5Lc##1HdtL0aUY$pt8kMBuCp.nT%##QhSe$TAlY#g0_'#.M(Z#DY2uLr.BrM/lU%kP:c@tp#Z%._=4,M+:9/L=,GX7E$AnL"
"EPG8.L#TCHIC9?-mt:/:#^'LGp5_qD=HSt#Iv.>-XFE>.ct65/Qd<X(all-$xHce$AP'588=(^#AB<)=1:h#$RcHY>2C-?$hb+GM3LHZ$u]g1T4Udv$2:t@X7q`s%F>#J_8$&9&1$Hoe"
"5krS&Jo`PB;HX2(8i]F-H(u_.+MC;$WaEF.#^$0#.(1B#ML-Yu01L^#[nvl/1:h#$c-KA4.+_>$O?IZ$'QfrQ@$ev$Ma);Z7q`s%vi3Mg8$&9&K.u@t9-AT&&hrS.>Zt2(D86#>Z[/Q("
"NafYGk^T%J$'4RDCKlmB&8taG4sOVCmiAoDD0V6MHr5/G^SAO;:.C,38$ik1^DxkE/$A_59rCVC:1k(I^hx98)n[F-b1USDa9vc<*5s3Mmk&kM$2^gLPrj^-3G7]0)ir]5$&r(8W6DO+"
"(;1eG38vLF)-7L2)NOe-Dvk?Bj5x--0]6fG=9OG-c7$lE^=N880oiiF=i3nLBlSE-%O;9CuicdG/iCrCBHjfEo>pKF+(rE-&a'kEA%o'I9g0H-7PViFTqm$8X3L'>=O%F-FDSE-4#?lE"
"S2&aFewTt1S6Xw087M*HBsP^%a;,FHr(622d8_DI@.vLF,v_aHBFwgFXfCP8XhdxFeBEYGIYo+DUw%;HlBel/Zl,20j'(m9Ls%aFpB7L#80(@'?Cw?0sC-AFWj.@'YYbYH-&K21/HnJ;"
";X2X:[2g,3/pYc2L9*j1t'Bs7i-7L#IQdF@NMWq)sc8JC8C$^G%?g'&1G`f1rWu1B3`SGES'm`F8]WJD`_&mBVBi]GYc[>-$6nwTI<Pq2x'AVH'TC5B&aS:Vtk>xTGlv]5E.AZCh`t3t"
"i-?Ra0CDqD5t1kX[U'XLrRBe6J]/kX)9tQjY>*m9UVX>Hg/[-ZNwmKP7`DJ1pG&g;UDf]GomS9rmg3_]2vUk=+]+wpq,.wp@oNe?/_ufDd1o>-+JM'J*Xr?9a&s.Cor3)FI&x]G$#w9)"
"qcSfCZl%A-GuM>Md(crL&#/qLIMg-#T$S-#)EHL-Tr.>-S`.u-8S#TMRM4RM;NM/#j=$p-/W&E>#mGoLUkFrL7KG&#'9x/M6A1sLVe=rLGAwx-750sLa*8qLoLG&#.=QD-d?:@-Oh;H^"
"Z3CkLwXD/#^st.#e,KkL>?1sL$f`/#c5o-#KT]F-W(A>-=NJF-mSR2.-N*rLK&mM*Cd&Xh.a9<-qx8;-%GpV-Zfxj2h2#&#5(KgL[bv>#3BFV.$KA=#FxS,M71T,M(LsS%+6@D*TMw%+"
"XfW]+](9>,a@pu,eXPV-iq18.m3io.qKIP/ud*20#'bi0'?BJ10U?']bpYc232;D37Jr%4;cR]4?%4>5d(=ZGE8E)#IDW)#MPj)#Q]&*#Ui8*#YuJ*#^+^*#b7p*#fC,+#jO>+#n[P+#"
"rhc+#vtu+#$+2,#(7D,#,CV,#0Oi,#4[%-#8h7-#<tI-#@*]-#D6o-#HB+.#LN=.#PZO.#Tgb.#Xst.#])1/#8#krL_U(%M#xA8#F7I8#kRp2#qNB:#HCP##tVe#M=_YgL_w$##F>(7#"
"B2l6#vuO6#dTK:#tHY##v]n#MoRu+'t&,Vmh+6PSR<#Po1m-,)_LEcVj<>]X+Q7VZ`3+JUBbr4S/ahf(*S%GVf=MiTxpj=l0@TxX7aPM'@0,/(,sj4]hfi7eLcCVH<A`(Wk0(Pfin+Se"
"q[BigqOb1gLQE>#tv;fh#+$JhR=w4J1%xCW*LZ+ix^=PJ34no%=fr1Ktc6MK##RiKNLt=Y*V/GM,cJcM21G`N4=c%OZ+_Y#<n$>P>$@YPC9[uP6a;YYGQ<VQKp8SR5@35&;0A;$%o&ed"
"D16;-Q&.kX/WJ0#VYQC#f$2G#TY-K#7U5j#uapq#Uv@=-FZF>-V5:?-#YEA-Uhoi-v*w^tRle+MKKG&#H'wX.dt(G#YW_@--pO41rT^q#Rp7=-BN4>-q]_@-TgX@6$,Y:vp(>uu9f-4v"
"+a%1vM*T.v>sxXu`cGxkplanLn=8uO_lZ=OwLZ%*bBCr#B$E>#`)bJ261Ki#^Eg$'[Xq]lJu`4oBbpi#M6TO690?;#9UK:#m8u6#S1@8#B2l6#-Sp2#/rG3#g,EQ.hmGG)DkYf$PJ:;$"
"'2G>#HsABd--(V%<$UI(GnvK#SF?$#C&A`sbxKC*HsUo&+qW<$#.-uP84'It(m*2(9?FM(/^]6,*ir$#f-/]%OAP##FXI%#q2h'#ADW)#eUG+#1n@-#V/:/#$G31#Ke53#ov%5#frCg%"
"s_Ioe.34]kUq9fq2I82'i?320L?.29.9)2BjD?MK27l%OJs]oRcXNcV%?@VZ=%2J_Ua#>cnFk1g0-]%kHiMonaN?cr#/Y>#;eA2'SJ3&+q^WM0L?729+t55ATUZ]F+l=DN]Ls(W>4r+`"
"pkPfhQROip(V$W$Y1P;-8fN>5dS9,;6Q;2Bh2qlJIpooR&HnrZK$X`aP;Cp%(mSP]kwViF-M%/G]9`9CxdM=BO?'2Fk^iTC==^b%BxXVC/s>LF=/2t-JBnRMf7ULM6m7WCN)]'G9Y)iF"
"-]AF%aX0pM2KRSD<hZX(<n3cHV.]u-D2'pM3KeoDRGVt-Ja*sLWf&Z.m&4RDPPVX-uk<=:du_3O528F%68AF%8Do'&8A]b%6;Sb%68AF%8Do'&9Do'&7;Sb%/NcSAQ%dq)ILwwBP*^UD"
"V^:4FZ,ehG_YAFIe4(%KlndXLp?87NB*q/D>CRFHAKi<Le4`e$f7`e$g:`e$h=`e$i@`e$jC`e$kF`e$lI`e$mL`e$nO`e$oR`e$pU`e$qX`e$r[`e$s_`e$tb`e$ue`e$vh`e$wk`e$"
"xn`e$#r`e$$u`e$%x`e$&%ae$'(ae$N[tsBMB7nM*WAeG6bPC&6%VeG%^4rCH29pM>TnoD?gXO+(^IUC-mgoDTii=-o3I>%O2LNMVWbpBnG^kE,(u$'oowb@oowb@t7Oc@gsg4=I^qR3"
"I^qR3N&IS3Lp6S3u:clE:H7RM83%.Oq9..OZ8..OvfabPgAIIOT<6D-q36D-M%qk.XJcdGP2&Z$(A]F@(A]F@-`4G@vDLo<N*F%0AVXo*),E.?.dioMu6/PDIUaqDA(:oD_NYs8Z@LO;"
"cS8LWqa31#/4>>#:b/2'X:lr-4.GS7khEV?fvt4J-xF`Nt)7VZ:xsu5UaIM9xE08@aAVfL=q->P[UJ`W2/t4]]Dr7e(YVG2OJCMTv5erZWscuceF3j'rkeY>TbIYl`E6Gr93U,))^(d2"
"j',p@tt(vP#9[VZO6'&bkuRSe_*(/qmwCs-FwqP8&4xY>F>J/Cd?sYG:UUAO$NaS[fXLGiJiVSnjjdcr,PCZ#Op_,)QN[29/t#T@TIH&F.PjJL,/G2^ePiVd-U;,iD,L>lmY'vuB$p2'"
"jh:W-85Dd2Q$Qs6;T)QAZ[Q&F3F-^OdCnJU)3`>YInaDasb,jgKYmVmtcO>u;:es$LNYj'p0%9.9JIa3a&o29((A^=Aam5AlpOsH,8*NKAq6^Oj[<gU-E.ZYE+vM^[T0aasItPf4kM,i"
"KM?vlo/aDs;4RW$K?,3'e:p#,1W#01G+4B4ZE)97O8K=-COW2*#PC<-@QbB,rcWS&68^b*F+7=-Au@T%/AXS&`TeA,7:t9&]tB;&S<N_[wkB^s+q.7ng_`6nbrr8&-i<B*S(n'Wwa`]t"
"Yv7@:X.=LBw$v;dV2W#R/Pe;?itmf_,rWX52eR/QwB@S&@f9gU4hTWLMkmS&k]*ZL30:as$[O_N2<;<%/1x_<<*^(u8[lG,(GqA&#tGWg5(oF):3*Q&C1g0qf@X_[@S>/)f&;tVBkv_k"
"I?;vgG@<i0pM<^8qb,>%8VMOak)5hqpixqCcA<<%,:;al3O_$%I]6HgiD=N/a*a_FO<Jf,8$/^FY`e1-1n#^X'dIl,]8pPS#-tgTm;(4T+:qhL/CdwL3=Lr&'2_e[;'dU$1?+xC*?4e)"
"1fIF)3nFXGiU*t$,h8BF@C1/)l]w-)SXWS&*wln&6E.1(]iu8.9RF9%,F+HD$EVQ&4qE$*W&_(uVJbxsa8wU?LR7YD8#qHq:xh.[KuFH;iwnYD,YPK1=MFs6Y1?bsLIUXu)'3Xu-4K%4"
"E2Z(@=__*NE%T//&7YU&W$VHMpoW_SGZ7vY=D42-=UCeM9ehD1<Khr%8oE,]U*uDss<Vb;Bt-gk:h7`t>+,Y#iL=&-U$##[WG(GsC.R`_ni<2-AJOjt1cA1KE,/_$`wL[Fg7l0-)drqI"
"hZ8N+IR:QMJG'xX^_JiX@@,Q%?A_Y`BXK/%V/q6D^3((*)84pe[t#b,-^rMB]NKnEEx*PT):[VsXR+bduH,ft6Z:x=^?p:%PZa*lZ=v(%SS5Jfoahq)8)u7DODB'%MD:7X]h[5SLbAQ+"
"?*7MQ9=%4A4J*eIJ0gZPa<dg,j:=ZKFbmS&DCx8&Z[^3Eu%voA*VCd$w`Ei&$<Au,g:/I,SRqk/0uc#YO5JPsX?iu?</]@OQTIh^Uw;Ws;>.nj2@M#u_v%u,K:],)Z3B(T-5WFs7FI#u"
"gL2`tF+qDseoC_tkf+fC<KX;HdN:DI;9L_WnWNGUY^4`t3/a-_>fIk4;U#xUw;DS+$?4`tUbQP0-i_d$u0PqZ0D.7n'NX>Gcg?&JupU;.%E`5SbS+X$CR+]s<v510OAkI:c^pw4Xd&di"
"dLo_,lxL/UwvZ`s4#,`E;Ts=#sa$%%?j=]AGfQ8%bOO'@FeK/%O(-9WqT6)0eX=Rb$raFs:i]X#b[@7-SJ&f)1_aADB4:`tU+oSsXGm<cU*es$@Pti$PJdGVr6A+-OQ'GHlG;`tU7aO<"
"/ta*-38?,MVP`']M_L'88h%?5fl3RD_O^.(B^ZG&ONtw4@D&3Kt&/b@0`mr>Q>i>>rf)D%Q.5pVV$Ku$K^kWVkBbcOQcR>I1Pq<1nRiD,ch]>G><+,-wG0ZGSh5l,e6O`6rA(S,:L<R,"
"k2Y%*rr$E3Sf>x4fSw6[>c)`3a%wh$vp1;&jBEC,_-GJ2HUNmf'ZtFsu5M`sd&UnobsTh$I--1gG$_0gZ`tcidF+Q%PWMACtt:4%A&od)6U`#Uq3mI&]UHQA$)]HDXv?8u_<).h1U(O%"
"3PNF&3%Nd%Y>VQ&jn14%/Re9mEL8hXncfS&e9L]jaovh$&HI#uMM2`tol1asH]-0uH.:^4sk=oLr3D'%BWWc4#<FbsRQ4^CPLm.5*(+qCdJ`%uOwg^[V[&GspUH,]2]n]5ep5T1U/S'j"
"1.gtYAg5?Gh/Y-H6?^-Heq?I:#XaJ1K7w_EUa;p74sRht'-4kO%[IDsA@mc$kYB_tp'TKg-#6mSZ6lj,DH@as<le056:%#upn^+jm#x>MACb70c,0mto#iRsE(,Y#c#,B&74IaFF2C_a"
"L7VmLtk@iD/(p*u16nd)hBq_E2vVnD=6eI:1^q]4J6*Q&5SVQ&L/c'kAv9=>q8N1B(]lJ:CuBZ>%(677Q_<%*MBMDs.t:o$$:<A&v,&EsFYO_N?6cF%$D1v@4:r*-rN?iLWHR8[F[YQ&"
"ldu7SMu29IZHOC)?@4/)lW>Ao-MHR&rs6as6dXipEh+)M$-CQVUgQ9-9d76IrDACj/?0j&eW8D,mB,4TNoPFs%5^u$vQ9EscDi'W;V]S&1tY9&c.t(=[FGc0O5`-I&E9^&R@d@&dNtw4"
"c#uDsG:Q108N0B%?=Y]t)nl>u,QTas6[pY%o@-V-2M,bs:.9*uef)r-1fC$uM89asd&A8u?&u>%so@DscE@P%^];o@#(4N'wPqwsOF:FlITvd)2jRbD4r2Q&_mww4Wpmk%mB1iX38D<-"
"VR<I&`WiL,3K?[sxrFaj6Wn`sfDGn$-2ko5^E.#ujPGn$#g;Bk[r9i&ZuTeLP(B3tJj$ZsK(q*MA7at([u<[s7_ZfLJX/77qB$AIS,#5nbj')M^QSds_FWZG`lcL02*0LTL<gxTVpc6-"
"OZ5.;SNY^tr%L195'4`t>8lSpd6AX1jMl6<;nU/Cv*V*-KPBgho/U+X2q%P%4J;pV]FZ20/L$cDgJCh$L];^kjnmR?b::j&D),Ze]eQ&)iL^4E$[T^FK0Jf,Ou)9ICs:7@)YXt9'_QDs"
"xLuxtfk9t6[$#J(i=oT#_[`nItT+%bfrT97[ZBB%S?W_[[dN5]dD=e,V8IvRjPCD)UX<%=]3Qxk6tCUQL5ikojMCC&+p$lbRB]O9r-AQ,W6Ia3Dmxf_38v/YdWF>D#p;d+*G2Q&F;]S&"
"qdaX#e$]eg?LOs(?0w3g`d9h$lhdXh):C^`HZ&m%;iZAIELY/CeNVm,g1S8u^0;Q&TFkP&Qbi.IZ*rw=E1GljANEM,Zn'U#c)oh&,H$k&.&^S&)E>Q&a6RT,5o47$eSEPsh<e9vZ)St:"
"SR#r6SPiERd@<G]3qA7/k-8Q&;s*%nah.T%ur48$3FHHbWDs>ue`V#lLBnd)XR@o.pX.SF(Wo?P8+:K,2FNUJIX0k,^f&7[f+R4A$wCPsD5p%7+ZBF2rGoHs#5QxkIAVYj@P+f&.#:_&"
"ZFD]&&/LpLwvU%kke]IC/8,7[W5o5JLXLTf$H[7$@8L/[[vkP'<_QMh)Ltw4udivshGT?#3g%Es&_T?#iI(##WmYxOa>T2#9h)##rqi=c[w,;#lR(##8-Juc^-?;#jL(##Xrj4SrOD4#"
"xw(##iw$GVub`4#wt(##=71GDdJM^4JD'##0C2VZ(C]5#+@2##F(:c`<gh7#A**##d?+VdGMn8#E6*##q>vLgJiNT#LK*##$NV.hR=9U#Td*##0D0`jW_^:#Zv*##us`xFpC24#0N/##"
"S=x:HA.:/#d:(##t&5YYhi>3#KG'##1Z#AO[v&2#kO(##v-KlSYjj1#_.1##2(6PJ.g7-#f@(##H8h(EGU-K#'1)##>:qCa]&02#c7(##rP^%O7G4.#E5'##mlQS%n7v3#]&+##MOSiB"
"VTE1#<#B##aI=VHIX$0#%+)##-0PuY29hj$5_2##NLQ%b>mq7#`.(##*0+GMa2,f$6dA>#X*GJ:&),##BHLpLo#q<jE%ZP/YG#)<`Dr+Mb0(>Yu/PrmT(cV-EF7;HQ+s(WeB]ulgS'Z#"
"bPurH@]@PoM.q`Ef^08n>w2j9`wv]F9eQ>Phn8)W@.VJ_m7XPfB8?;mk)a`sa3Aj0(_#T@gi:)a:I1aN63]Djks;)s?<f,;%#*NKO#wSe^,'$#i'ep.=.w;Hai&Qfm//as=t`j974l)N"
"w>s8[)nW&kLaqg(#R[T.J@'$5F&jmA3bA01<EZT@jZ2<Z4PSjp8u-[#%9cK13[kZ>W&B<Q&3lvlpU,_+5:F6JUR[^kmi5h:J6w^O8M(n]ZhTNp15GBtf1#C+m%;e;l33bE5Y8kKiruTR"
",$D'Xr.O9eW(jWm;43=$xnG_+>;i-2D>'LC-SB-r:b1kpNp9I;ZCII;m>rb3%v949AZw6f)Q?.27g=Lhm<dRAP<8CtN@gC4rCI7SbNTI`P>HCk)kJr.wafCFd>#]P;?D+WwCgUe)NSIr"
"Dw7S8Rpm=?5EeCX81SY$<N^r.XJSr@F%KoJ5k>iUo2mFah@a@lrb_`+)vBS8$X/GEM@p4KBg[(Xr?_7of>@5'&xS`=n:,8A3eC2(&*/AYf2LDF_HIJM7XKPTc_MV[6So%caPUfi6WWlp"
"egGv$;k@&,htB,3>(E2:l:cSAES*vHtiGAPJsIGWw&LM_Klu+sYg$a=30^GE`g-)bB8giqqJ#T8*^V;Ivc#aO=T1)k`7f21^#R&>,$E)X$(p]lMlufrtcep&g_k/2j7ApSj[d>Z2#N,a"
"gJlMh1$7sn/F[a+.H/B5P'Pg;@,@^GjL+^ljWKB#:IDH*0)hs72-5HELA^&YiXmk/h'i'tEkBBu+7&I_$oC_tcnvk$)eH/hJ;JF22a%htWKs<Y^tZ$u,M;.LANmMB^,/FMx1[(u_D9Es"
"D]<[s?g%4J(557e.0f3%D:Sq)&IZqQ'B@Ssjr5HMdA,DIv,,#$7TL[XS.A^b.J7I@l5`49_uCvYZgI$tnunBjRa%Si?ko1hJUjL%aV25WB2wUU5kxJX]@,TVQJn4CeL&*uV^MUmnx<FD"
"$c_$MuVkP'=&&:H%b,Ns^B>rt.p=DswNhtu`3T%[<T%j$u;:N'flNkSoEYk$6=Y>@n7bw5RHQs60O[@%cTZ]trNJNgX@[egYW>D&W@b]m/aYn64ES%#=$[tua[+[lBK`OuiG3-)-W(H,"
"8]M<U]_:dr=lDgC^bO@jsN;=&>N._s7q3$Sp&I3m=[l$Oaqh<dR,k(?BrpDs4<ri(DVF<&IwxNoA?4D,@#6F&nf^PkZ.g]]#=BMs@M;UQUilI:a1.Cu$wkh0[Ihet(5-.1xc[RnRwG3<"
"l75=M]+HD-[ERu1gMaLudC[L,],J=hiO;'MJr9ba4'P^%AZ&I=?2LaO%B#De+OF2X,G=C3HF+GexOKR[6)/^tZWWiCSSW*rOC%.KmxKO/>]^4e,(V_/`b+ZsHeD_/R9OA&M-Zeswb1s^"
"trpqs<tL$tN6SeCFbo9ZOq%i5J;DR@qs+Ai8Oc7c-r^s=-h[FgYF13texnDs`+4-)^(u>&J&]&G;6(rtd20_-mout1mHtt1G6.$tN^=?->2hs.K3TN'hDj--1H[B#qaL`sYY`g;mxpk1"
"FXCPej>u$*N%EdtBle8uguaj0C,a0go2DehcfpP,wB9`5:i1=Ikx#VsihbC3t5BF2au]l`&v4L#I$'+2RWCF2[8p,3iVSk?oS_V%NaG(3+LI(<p]x]?X[l7$l@-%=:P$s$G>&e$D*Not"
"-BbjLqYX2c(pj1i(8,'tv42^td=)SX0LOLs;(]_3$j3Gi=XvNFJxYk]o(XH&q'YH&]e>*7K2vPJW[9)NvEHit,,K2%Kq.]$%c?vGxwDGWwx2,srES'MPA>TsMJhw-L]/.MX5W:6LuJw5"
"B]CP,1dvG&wVY%*-C5;&xcfwFDpqx$mgf]bkREZ6(P2etY]Lp$Q+get2+sJk1dxc$WclDsKr5PfZu]h9TGqwkimC.)0Q<,<+]r58L#F_N5<A_t,#PQssD9Esnh2-)E`Rea=d4cs+SAcL"
"h+;RY'7eRjTmdU#jm0qkdXT7$3BJ_tXVELsPG7D=,WHiLDUiUsbHx[sHf&:%:$FSoxGM8BNvhTC/Pg=%`D-Z^^80vLeA:i'F@8?K$Hm_tsMdc%Lu9#YL>mqW``N]sGkjX$W5t'<,/7Z$"
"/Q6:6V8CJsCol#uYw+w#RJ,%=4*m#uIE%2t]4w=cxvY-hcx8i'x@TO&gsrer%%m%dKlQS'7@e)eelT7BnX@`to#e;%]u3ut$aT7Bt+IA-L;e]-/Pa-6J/(F`vNH.3wMtt,'mpm,?8?Ds"
"_E>rt=u12MWaMJ<5<4Q'r]gvLjuBh3'^$u,L^:P'RcOI:i,9KIec<bbNu3xsX1Cj_SgALb#N&>qdX6sQT-_uE@'&?/h]?xnElUZ$j$X<qq_%N';'qsHRj#Zs66qx6T&M`s-ax#*TcDOs"
">eo[Fux:Ks$1I:hIq8N'A<:+M=OvwL&XDs@:s6&2i`0k,wL)gt`jQ0%r4)[$n+OcR+TSKU+@@OV-AH.MN&Q+Y#WQN'#S@777#)5ujF/+c='=Hr,gM)`V/2@AAo_P,m,38$ApjN,@epi("
"?]9N,>fmE,b-rFspwQ(tiYpGrSPHXP(^KOJ.&CwO$8`>lt=bp:HY[p.7Zk+`wYvB7''sM,EQi%tj*sGrrrsFLr^jr12L)M,'bQ&twxKnN3KxqD1S_;$q:b6ak<WeD&mkU$]B7H2.3DUQ"
"fbHeiY:=]s(/=]s>:DO%YOJ4A,D7N'O[:xfRFOT#q4mb2uG-=P@GlP,^iZX.nSf#*uq%31f/%W6YRTut6`Tut<1]T%$6eOAG0rlkJG>4&sm.:Hb_(Fsd<.^tT9:>6voq^sOm/:%x77f("
"WZ6qM4gm_tcmaU$O^OIs)tvrWiGrS++>Y=G%E.h%D3petdI8hf?/vYsG([3ev9sDqL?)Yh7'i%t-Y+Fse_;=,u;P^tdDWILq%0i<D5a=,L/0B%SY2rtAU4c,`B^rtk(&Y,aU*u#H#0B%"
"[E5pkNBwFsjeVX,bsHu#^9Mf6j4_ER)Et<YBEGLX9F]RRkh51FtF[C%/J5,`K`0Y#rwFmA6R.+-M?7dD_B2m,b4LDji)l<V>)kiYF[C%lS=L@O[?5nE>U6h+j',gLDtb/-3-&gQaS%Gs"
"=YmaNdqLUQJb3^kZit-LE%PLk1I,.:[2uq?@[#30HvV9FFAfQYVr&GsRS5asN]$)4H8c_FP?Jf,K$.@F*GhPSs0YB$jW%@'R))@'Gn9%U9XiP'Ip0G,'?j>&Dvxnj_WqJ,`p<:vt]QJR"
"tX<^bp%TL9Lcl&FaS;bid+iE3`'$)*q(@Su(G?biE*Utt3$ZC%ucvJ1?YXhK%gx4n)nQ8$e:P-Q?<74foG=itZaxlsA'1jt[Ko@WGi8;&F/GC,=]>kFC[+L>,+*F`fDl@2*]i%tio*%="
"akF6u0E>X3FfG^4m&abiv+=o7/G38$d9%%P8u?4k>R-K#G5o<lp.$@bdrNwtNN8xt'*8E%#*X]tdclDs`S1KUZj)+to5gICIn*(3Sv[]tITQ%+>hCDsVK7/4G`348kn*N'vtf9v>]qFs"
"G6r@l=oe<LwLf<LE+i]tuApet&/I`$4LDFlq_KDslZ3Lsux6`$:(8'o-x%=1/F%8$]P/+NutH?#vZb%t)4*:&v5:G,J@btYb^0N'2:7R69'ZnReKQ&t4%$)*S<#F&wF3JI]3FatkdMaN"
"</Lx6mX+pe)x()j>Kfh0%*P#>h/vv$:=u2;(f@o.Qh?uMD@b'M'IqSk#Mq6<6]H_t1vgj$R$8D,&+/L>=$((3$o%ptW^ums`u%YKxfV%ugn$os0/<(Toe=2:hE0j(1nTT%2R^R@nI3-)"
"OA3/)'kpx>*Ve0^0FVI1dnMbj*ovIsd'`:m1dYEnIAb^Oq>'SI53+w#[4.V5;`c>&+%.%=i[_]t;47st@wfm$6;@nnn-41%U%jd?B=VmLRw_E4gFa)ubh-gs3Y[%%rnUI_kL1I_XCGDs"
"C):Yd^f?gsPt2)uF'B2%,vg6<s&#2*8*/r$BuPwkSL_]t6?vnt=,Oi$sYEDsWaTut8Lqr$G[>XPiY&:QMl)N'8(nDsiGYf%^d,%=RuNOWLIxe(vk<SSjgnfhDic0)vhgE8ubo,)H'51B"
":2eAikfBBu-:ed`2e[$uH3gI=uIoq`X8BN,UV+X7;b][s87]'txh;k:M&l?-2ujX%@8Fm:$ungs6xW-qJf2L^;X;_tDGPn$b%%c;5.5pG?tt<YC6C=YI_6%FD,MR#]E9f(&t?%O5QTas"
"XOMA#v'Z>&lQX(jqHa8&t?d=CWkKLj8n;E<%f8csWK%6M)SNZsvi-;IK3ZesYp*%=kE-aJ[<^%44VA2p.?2P,UL/ZPpXQ^t']-#$R_4@c'ik>%uO[]tcQaM,$xEdt%3:1'Da9Zs@VsGI"
"'PK?<nx'XiG/_3p+Z0#t*`n*LktlLF6NF/)rEm=Mh>4i+8B7gC'=r*-'/pDEgs:YMOAHaI5@T(FPk<$LJgb,Hf]_1CT-=;EX1.%+LH/.(<q8OWb*M4%PKN<VJ;;nX87^sLOC/'-&>-J["
"LWib%b%o@+N,a1%TJ0_J^_&TIlGPg,;YS2%;n.(TP]JCSLC]m+U_hwFkGdHLJYnq6%W1N'X:rFscdTO,T^LT%$5qFsALP%=74IaFr'p*uFA&nJO8k:_+w:]c%LOG,^.Q.1>G]NI2D)>n"
"l?<O^FfX2-.Wc(u%qN/Uwn'(*hdKG,Fg;K,Jcw5/aa.FsS7&I_?j@Dse.MUmm1?_arh(T.jVpmth%FC3L<a%YO9+oL,vmt51F/FM6XiS%@v(fs#BUO/(4tX`0]aFs@JIJjj@K8[i)od,"
"JAuY@5:r*-h/OlWA`shDlxE,ML2nlLFubYjg;VP'9Zk(Nsf.9&Sp7J&&@O46ie/[sIj5EE]7qSg(_oJ.v1+)>f)Cm/+3RDsGH1^t3sJv.`%2Ds;`0tq]Z7J0Ippg$<st&=g@5N',=m=l"
"^bO@j4lRN'^7lXh2pUZscKrCsZ02DsfA=j)rGoHsZJOVJaQxn/HLv$tDGpFs&kSZhYDCJshUX<6Z5421-3Z=5d[uBac$7osCgox$(S[+)$;7+)]OGDs:%,f$krqR,@T3FsE`>A5v*1c)"
"+U=f(wVCv>7+`$u.P5bskkxx$cwZ$0hOLDsp3q^s)^bq$PwRnL8M_'YH#jOs]29Ms2*jw$8qdI:4X?I:LuFDsSr?kt%'`g$Ev1:6ZE177;4&N'rQKP8(>6FG[AhB#QBZQSuS'hhe)14)"
"KlCJ8Sxo?%[8HuK)>/Gs$EoQG1j2j,p4t-[.X@['>/s>UxKBnf<6Ov$Ye0_@027H%FwFgGT$A1W<rjEJTE['4Im0q_p)C4J21)Eex0ur)31=au8(e<CQU2UsCGrB)E`TUsH#%PH](V;U"
")Do3SY(0%]_8.j:2YAnfLeKjFi-/E%Lvg6`QZ%QMx+0K+QP=bFUBJf,Sh9n:.>cQYGDYn3o`wxU;HbP'<WKUs7V&vH5IJs(L-vBjGc=ps&FR&*H_^@%ovUdHZma]tQ7^,)&vE^JGnd(="
"`+bRDRSNnSQ?RM%4'ksV?YKDs4Kf--1kJwTPR4n+O(PmY]#lP'<NA`rQ+hasct.-):+NDunTA9&e,31'5*1R[GSd4$k<I_tmlGZ$K^KtVSmQ?&J@llWHBU[kUN'F,:L.%=B+O<ut_b1h"
"Cu6n=D^r&HV)Q(@vI5asDj0-)h:NT@h`tu@2i$A&+%-7R19NcsMrrM.hmpXsMRJ#*O^[D&t>kR,a(XfiwreJi65Dr?EL[<-QX_p$nP)vZJW4?-n[)60e<fG5$r1as'fUe$b_WD,Q4f6I"
"v999&53Vn[]E,h10b@A&Ndh8&>&B=#%c'jLdh:ZF1E6[s24NwlIsSX%D?fC,-Ai;mMk,%))#>&M+c<Yjtq4tL,<p%#.Q:@E$tq^f)e8csC>,g)Y[OTs+)H?&r+SK<[IWE,T(2D,&vT#?"
"Ax2P,u&Xg2r68@=^RcdM]65B,a1KPs2YFYHfhm-$'wYmJehKhLeRTA&K:Z2MY7>HrXBOC)27B6sb<6].(8GM,B6>gMrqs=Ej/`&P)dxA,`xA$g..JD,>f#m4L4mW-Hf&%'v[ID,'Vv=/"
"'8eNq#6RL,>?69&rhTPs42=X(/]>X(I*LX(8nx3Jiq_d<ajYc<RBhs?T>7Q0^(De7u^kKPqVIJiU0ba%==/B,vMEXGW_XbDVBfqR$o:7Us;qbX8gsl]'CE4&vv7]$LpCvs_jt$=i4xmc"
"53-5$%/DXc*R/<fwM@6:ZH;E:,O*Kf0*S1*?P.*?QM9Wau$$.$6x-*?P'*h^u/-T,`#qw4gm_#5vT_7%>r^9vALd:$Y`dh$/^O#DO6;`tXQ8h3eWqMBuwL*-@F>jBeWim,d=+]+RhYFs"
"fV?g[K.G`t/9@_tgRgY%RC-N'3NsFs)7R&teCB9%;_)`3qn/PBgr3FI7=KMs*'Y@FZI-j^?1Cn,_3TtchRd]kBWa;5a$D4%0*T]69/3N1WsaFD)%k*@Gu/b%t@tqapA1+)gZ/HKE4I/)"
"=m3e]ZF_1UB]H3hvrB')Y9p5i7W#7RdP0C)vWt]saG9M%r,r(B#)_hpNMmonDnt=%&E'_VZe6m]2j]Eed:uMB#xC*-Deb<m%K3a@PEo@%8^G7`A8]3o=K$gL4%KstT:j87Ao_?%vFkqL"
"4wSI]8lCMZLHrn%tEGwU;`Wu6eob;LgcLh,^F'+rK&:;mOY`>UkS%Gs]?(_$-eUeC-u%NI6>JPsN).J9MmS79M^57gU0mIgYYdP'Vlj;te_uCssJxocJ]n<tg=f8u&qP;BQ,D`tVuP;^"
"Xw&GsP#Ott0ov'9o8C.5Sk8:%QJ5g)TaSVe30;iOfpLehl]K')='M'6Gs^)hP.Cno15g't)UTJ%L-Q,Hq$JvGYaof,7[aJsa54hqR@B+HXA]vLRE:sLUK3s:fv-_$N-VJhmD+5-g%hJ_"
",J1@RNeQ,_v7MOcS'uYcsnTa,2_3mS[?p:%?B+t7RaHr60ts[=)^pWsds(F^`Ti+``LhWsL>L`sP2*m)<@vn?&J4Z,MMCr$^.x%-^>Ai`s;4Z,p7uq$ltR7-D3;ODVZJh,fEX-cD;NO%"
"9i?%iO]:dMFwtO;x>=o.Mgen$[-&[UF*KYd3[a;UDw;u5qn3wiK9sn6Ztfb)xVWRN]3*-a4FQiP^KjP'@Z[Ie?<p*M<5)T#;Z>2f)#'2nm]KOJ1'*FsEOfe(p.i_tZ-cC3O+%?7EK5f("
"Wq8:%YIxst=JMa,KFGDsLcF@+;/>596up71/,l$0>2L=oE^$I6:&PIo'^Sb.(4b^br_XJ2N[BXdk)(h$bOTMs?X.W9@W,h$a+)lGPW;`twnI.GvcD`t$afLXT:&Gs;S?rWGTkP'*ZcwH"
"s4NS#F;ZYsGJ%4JMr21BCX$FsC&j]tj13dtotb^$YV)2emTHDsVvbIs^7]]$w,j]tV-(s)N->u<[<D:Qnbuu:XB[8[[[5csTsvCs.Z3csibGepOoAK&@?A8m>I+)WYHPr-?@'v`@P6s$"
"$4?I:q>8xtXq/LT@2W1%`o/[t8g1N'L:0#IQ.'PpF+<M,bpbo/Wq)q$9,:D,vP@vYN,i=T0+I6?A^oZ$rfiI,8ZW)ESqWCK4;f^$_>4j)Mh$#uJLki$+w3CNkirw$9n^tY'31N'b#3%g"
"1]%Zs@LrCsw%M`sg?I&*TcDOsf>R:?N[&Q/I53:?5#]htMsw(ubK7N'u23as]Zi%tjOhODsdjltb%&Y,F14)6SkMe$h(^%mOf-:?&<6MjA_`#5FXiN9N@BPsrj8csX+XRA=-^(uehSF,"
"e%;sHNfn1-cLwCuYTeh9=._/ZFujIikhqaQa9ho+#9tv1UiA_eqTsKfLj[%k-`?qLlKxS:q:,Fs(?3`t;+f.G;_gcE*i$e$mw[W9d?7aEJH+,-IpFvGSh5l,>4NAb_M27X.RD/ht1=Ue"
"]?6@%iHYhB:FgJLCtb/-a5A,RbV%GsMnhJLE7s6E#A4(Nw4QbDS:NbhWra6$0<sk/;0)uLD%^4=u5l+%h.3rWftw[=GWR`k_&gfVV%6T@8&SbD=I/N'4(VP,(E6[sAGNJ)BxA41b*M4%"
"Xgv&J3Z'_KkDti+TALE,Nqd?&ZbJs?G&''%JPjjYLkK%S[wTo+-3*>&>72`tPhSF,jboEr$Ho_6,?WE&rltV$V)un/P[)q$XxkDEE^(mW^F5J`Ak.&:m@47ev%@rU-(<h,m+Z`s32,g)"
"V]JDsAFOq$K0MUmpA(,%<*$+riK0N'h53l8qJ9SI^EY[s2IVvka6-$tOYJ9&8J,oRQ'C^4=/S`tKa/:%puhe(>?4gQiSVFse'*V$W8hbtYQ@eGWXHL+`kb:&@9n>&2jK+2?4V+rc=SeC"
"E>2kL<m^[GD<+a<m/hLKKe4T%-[IT#lRX6$%gt$=5(J5u'Qtq6Mq8N'I;O,i9,MZsrx:`sh,ne(#p,1Kf2'4^OAw^[UFT.(UGRqCg9,rH#T/vtWPfnf6v4d$DDbA[Xk&GsRx)-]Ou/Gs"
"Dve/hH#q0K#9V?%^@Z$ZTxi3lXM#fC+WQfM]kDUs=O5DF?0_I,4P^Rs2amkA>qB>&W;/+NdsPHVR:+^A,`][sgRJaN2i$A&A@>UZJa<1M4-NJu5UA9&PPuVFJcaxsop>=-7FSt8>9UEu"
"u<37'UnxcL+0]'/nw68$0Z-$Xk=>N%4>k$OACA+MBCx*M1u3T%YxqFs9N.At1o-^tB]9GHaGC--Oq5>T70:@k?sk?k]:a'TG5>;I+uLQW9DjH,VRXI&W^ThKn$k$Ow)dwcT2a@%^*[>M"
"wx61K,lO]uaXGh1pPB@4U%mh:3Dq0*^.ugLQBOPCp*-WC]2tFs1fi=-p*ST%(=(I,a81Jsa3,7@idZAI.Zm7rHPRws<QqFs#nq$[N6(rtajVP'3?jh'GImd$sF`R%LsjA,+EIsm3vxE`"
";I`o%`mm8&(0=10mlL%uX23as)<1oCNtdK^Z-T/`Uq_bDUie96$2#wt8l1asVpm@lZgph$j[lI:Y(Ce_36^Wj7_:Js6Mjdt]Ko@WHl8;&PWhI,[D6N'NGIG&b>qIB,4ST%<K_nIGvA$5"
"ZG9dqD$fEsQWc$X<I8[svgrq6O.Fr6&wQktq=pEsNJtVhKmPv$ARi,)1H4esdu[`48h@_WaDf*`*+^et+WaM,Wp'=(lZs7%wrqS,4l7'*S3@(<Q*A=,QslFs0L+pR#i17URYKiU0']C<"
"Xj+>%(@+hH%402pTYSbDI_6%FFt+sL5sZC#u.BUQ_ZT/U(5Zg,g44ZK%5I;7gGI^KbS?3%8_%?UU2R4ATRBPsIMrCsa3VP'w]1@'LL@IstJ348$BWIL08IC<gIpnd_P6_kSYF)<:8J#u"
"5]NE,UlltlY2qL9&r'L,a9JRYU?c5o;9^b*K,CE,&mo$FIqxA&,)@jpg<2=-]fYh1,0DW@&]5asCj0-),g'5M+s6^rcRg`4R_N/.w9P]s+PBv)n=XE&e#HH,51ERIh*pA,2[<X1f*&r$"
"[w<o7;0ZpLGgpHs@d5%.o*XcMnDZ`ML[3Gspsj^-LN?_8*Y/HV8CpsLtW`oG%r&N,YL#C8I.^U66ON/sJe]D&=.hh0)J;br7f`Veo62/-UGvxY.J`VeZ/E5+_8_$bIu9Q0Sn@D,g,/gL"
"R`eG&Gb/RnRKpY6x(tVRIu'XM6<e9M*_uoJ>uO[jlJVP'qPfN,H0Um7(msYFdqGQsKUoG31a(*6^Mu`4_oQ>6v?_&=5LuCsON0(*KCE@;hfp<lVpmn.K].ZsLXj$Ge`>+2Ro]?%v0Xct"
",Kc$G=C'@%Y2^[]H%ws$K;Px_L>5u#]2-K#1soU$2wY9&$kvCbtdR$9mf1BaK8>Cq0xZ$XbpN@&)gMxLktwntMF4E&WN=J<A8`Rp_D-AIM#:+t1nDSJiSYR,6FHHb45#C/IUIntZ<(J%"
"2[=#GP_w`j^`e%%Y<dMK%p]?KJ(EsHOh^k,*1xcVu5%1->]^v>@fNh'07sFsN76as&5+FsseDDBRH;`tLwIHsYqqfhn`h>VRvX/_3GkC*RS/f1Zd#TFWoCC*r$_:-V_b;JVicP,B#TYe"
"_ENt$<@QIUkr@.)ZH5c)oQ6K1-lq'<Gq;_tVjbIs<KB*<ZlCC*?ZdtPnO5Og`^'h$k)1LT,=$ghEZGN%-.qHH@7uYddU.Y#e:'OEIs.(WZNtVE=[^X#NK?)r5l_MZodSFM:SaGs'`o9;"
")7*Q,@2]J&s1LP,qDVft)@/e+s6jRNtQ<=,q09stP1CvY&&+m,aD,:H(&hEU_W<O,B=Cm8@6`'-VLjsfrnwPJca`BJMfC?%M:5`VSo3x`Q`83-=>tE[bnbn+U=C*<I3A:-QvQ$t_n4(4"
"&ncft>vaLs.tVw$C9#iKD-gKscBwR.HFK?-fi<7.',7&cM6eM1(:?7^oP`1`;Yc9vS%P$<W#Fh/kOS'j>j;g1vnaoT1)fhgDh/Ce-#U]M,7*_n2Pj>%gmbYcahx'jb8K=%vB@:$bM`ls"
"sLaGIFahZssOm0U[/KI1U3-l$)$&w_1/e-JrZMm,f3ul`?JX,)9k2_@8XWu?%)PrC_ES)?VjdiaSYb:CoAaJ+w.]gXR:Wc>g)Ztau]sJU%R8eJsllB_8ck2-dIln>`^p-L/]po4nV0f3"
"Tmxtk5I:rl5KJ`,(OYG&2)T(t@[/^t3BsFsBI2DsFah4gCN_Es)eR(tFAX%c-VtFLbYYa5TF:M,50.8$surI,.<7cs>Wi%tHBBihZP;D`fH0A7>lpM,,Qi%tSEoDsQ-g8ntL47I%hlWO"
"=6.I&tA#E,vxpDsZs8cs3W+C&:@je1f'WL'Ff+4M0$n9SHG3D:5PpL,,6rK,%5];&::fR2XPM`ssmw)<A^_O/w*o6eoa$W%o,8:-]mKkfb%p$R)V%_kxOZ*L$['D&XGlB&-&<:&g.rFs"
"HU6asu.fm8@I;p&Xn<vd)hd=5/i<HNjO4FD'Ug0MXE4jLd&jB:^L;kFBG:kFCT[;&&<3M,I1Z0G+Zp@&or4G-][H]/.8`c3iG/.(^e;Ia+5)V$S$:wke2X(L)k'D&nIU0G^)<:&L.rFs"
"&CR`%W'o;&1#%qfV.NZsAd`(jo^o'NQ4.+<GHvNs=ZAVRMkHU[Llk2-0h1)>qYORek0x_<1AoM%=P-'XsmEB,$QfXhpq3ZsVBXxkIA;#j`k3ChIe8:%RHDXDlm./R:5JT+sE'F&WH*@&"
"a*Rt)g/@*I:JQVsLe^/%Om7*IcVsP$f5[EIdJ#d3oB;Z>FqiP'N$CG,6?5T,,<nvsj4L#ulJ7cs^u1?GS+c[j*MRN'$7b-IjfHZsxD,w#cJ::?V>pF;@F4[s,DL9hHTrq-tUsfOC0*3h"
"Hr;Hg`<*_j2ex(MSZe]G8cmJs(FOato(KEs7[vciC4>fA/S68%pSK=#`C[HDI'_Mh*x3sL@FmErCMUWneG+16$=>FrF(diMRaxvtbvl#ux7wDjufVP'vONF&WwxZs:`iB5T<Fbsa/[:?"
",Bqt58&5d$]TvJCbKVm,/[=e?8w2/Mh[Nqm#F$_s4PlYs47Qv$CYhRIr0pSMDDeF%nJ7%OJuGO,dBbr]S2UF]6%+x4wn#l/`8>dM(/:(E^Y]CE8g%T7>=4wsBMo*DOZZeCGWwxtmlH9I"
"_*Aws.<UMK[gZG&uE*w#6$P01=l$1B^uuPhR?t<Yi=cmUbE(1B_E8ht^<Bx7SHI*j)&K@k9Aj9&Xdc,)H2f7%n$g,)F4=7%k]0@&(q^O00AIF)m--e/oqjhs+5q7%<ba3=&_>Zs4:rDs"
"+pmp-21L-QSC,/q;7RJ[)(jq6DVg&45^Hs$:2&fConFkfW=j^JkLrn[WANn[-;a&KIRpN0w`)MgU1]?j2fRN'3eb-IJ8wT#^A0U;SLdpscJx8ubC/@FMsLws;9V?>YnT%kBnqC,xdm.M"
"I)ivsl5^x$Po:f1LVle1[LGDsbmnh$Kf<9[vCoqH2'_@65uL$u:D#bs@p*&%GsqC*ATLC*i^>j0>Xa1%-u_R%/1.S%59+Q'v?En<;)u=*@r<XY[u&=PZb_]tWhYft_T3a$`%L_E&dG4J"
"ZTtrmf=wKs)jAetLgP^tFQoDs%#O>>5.BQ/d5evs0k'2KBAt=&wv?XP)pt=&C'x(65p@jpBs4h2PR?Fs+CZ=/n^Nvm1(Dr$3cmiLrJq;(gf#7I'O-'X@o)C3[xKEskQ-a-/rqFs29/Ba"
"=vxJb(FL`sO@8+M;.k]XTI4LZjL6EEdZ5xLrNE1u:o.^tRg:[_9]j=5S5:Esgf;9c;e<T,FI0Lj#@D78nrhG%1u7(Waqxlo/D=Ds$2Ku$&?N]ttY:F;W4M4U%p2L`PZCG,_OOJ,8C/T,"
"61Q*r.sCp.*k.?GT/^?^Fn2asfXE_&5gm*)1b+Un<E.1(ep*B,`Xgt>+WGs-U16EEk<@Q0p(.@FkxY<-s[<X%js.K,:M+48#MHO,B.PDs1Wlka'R9]$$5DctbU8]?;f'4Zta$@%uqd@&"
"dCQ#l+5')jS`wsL>;x/t=fSP'oVDJs[=]f;c7`qH/?[G&98q)48'uw4Z6^K1]cTmS+MWi.*7F>H)?9X1)XUgL$<2=-DgOU%[1.48^r[b6w6QT,#HXlS<_bb)v^+dpg?G(D<Us9%Yo^OG"
"kiJ'h@?<t/`<i4%FcR7V,%3h,QOxxkm*)Fse4TLT]=&jK6'6D*oOKDst8*X$rE9EsSkg4gdTN_jp`M?8JG&M%oP^9vYr3@tqAe?t01?wdKg>D&M0?biA`jc:H^S49pg_;&Mf1`t4xH36"
"P<Gd2GO/w4BPNF&V.hh0X>.gs6$/@'=Mn*)(XqHid<$u5vNTk-FH6hE`2HT,,JcPj_0@>&j?ok&wMxLMjJ=0MVX[&Mua3Gs`'hhLa)#d$S^b/)e4Yw09vnk8x(kfOF$VXe/7WC&&w#n:"
"T%BHQdQ=Is`UdICY4+H&_4GPs7q148tTsfcEG$l)t5A?&hrH86@F4[sFP2KUPE&ps6Sow=EV^@4+VUuRO*w$CGl8q%?&oh&Mgw+RMmvmQsQ0EstO31BZp+%#.4>>#`TXc2Pv8G;E(H]F"
"#`b%OY+IfU%^j4]i1Flf`vhlAM(:DE.=hxOW%RfU0GPi^W#v:drm8/1G,(2T%BL;?@2SJCBS%)W.#05]M*X`ah01a*$v0#G[)KJ_0k`cij,s%t>n]2'r51d2$r%^=5*U8@pblYPXScPS"
"2#bS[)bIDjVU42p$pxxt81Rs$Q><a*8;V)3o0#T@Y_dAFWJP5S+,G2^hleSe^qXMpp2NDs1fhv#]Lq8.p_n>>NiX,DhK/ZG&vZ2Kt#[;ZS<'aap44pe0X)ghQR'jp*8lYuQDj&+1^5K1"
"Pk#<6hGOj92_Xv>a$<^F$^h5JHT/WQg@@jT;DBp[`)d>c1bM,iQxV8ne61jp$d]AtNg_g(iL5?,>`n&4mop,;1O+?>G/s2Ba*a#G5x+HMRs8WQe.i2T1d3WZSt[,`iVi;d70S)jV1a8n"
"hEU/q%aJ&tqwQ^=P:5c0]NDeh%.:?%PNVxCqpvj8e6IlB4OCe3B*j]t-x=jpY:9i'JwXS&_PbA,^]Au,lLX87@_I`t8]JOsX8he1=Ooj0A=_gC_2[r)$u`L'+X8Q&,vOFE?q8OWKfYL0"
"5*)-C,8xI,$8^b*bX0>tlLu#%d*W%=AU5f(+jhG,Za9NK_G-3%6U`#UB#6mS,D)h,cdb)Sd6jd%D`F$*?@?\?&gPmA4hVKU[KfX2-h?plU`8hnntc_[4cYIJ,4EJ#udsWS&K^_gaFkxPJ"
"pN:7%wKN/_4U&$SH.Y2]bj3X`)Z;vciw,b,)ZAj9Zp@(-p'&sT;=wg,c2_TL+GBK+a-FLW_Z'Gsp-m>W4;I/)V8FXGWIJ?KMt91TeFD?TtX&P%A9F<V-/7-%LW%EI4_[NS:uKe-Wr=g,"
"roJe-9:B<Vp,Q,V%B`k,c*bGVtaX,-/-J#uibUQ&J#XS&D3;L,iQtF,^g`LauKUV&Z)t'?d8wqMHXB*%x]-Q&HL.^C)L.9I@,3-)1%/-)vCZ?&jNtw49teh&MF_L%oxacR(KnfqAN?t."
"Qn9g@Rb2K=Y-/S%^CGg)MLA:F:W:vL@Yf$V]HIW+PdE*%ivw`E'&oG&d*tKps[1^t#/)]J&^;`thF6FD,M*wXihe+-,4`6TJV,Fs;Em@H:3d:&RI1)Ra1+Q'Fe=-Q.,7(%.)hOEKM:7%"
"xH3j^+ML`QH.Y2]d)96b@1l,=JdvfV#or?%_[ZYlRRjGVv&Y,-]7O;L2s;`tYd'+2$ZPEVvH<:)SPw9)<vF5%'#U29O[uBa0s-T%e4rwG*WLe3Nq-SHMM@`th$m3%.*Rt)_5I5fL.F`,"
"x86^47-V'-wvU?[`a'Gs.wFmYfP*2-^B;jt$hU77@2o_$EoZ_3jsk(-62,g))'xjOl:V.MQ9CLGH,t&%_VH0Ze/I&X8G=m,2IL8[p<x+-vm[i)JZ>_DMM9`ts:KuASX2*-YBnTsIcw0K"
"pJjm$h9r[F_Al0-YTq;6cQ_X#W,a1%aglpk)RhOLt4F7DE1vqZMU^C[D3DR%,@%$U6cR%_x:aFssG-rWv'iP'ZZEB,p%=<&IrLQ,mR+9uVjbIs,9qXG<L`R%.3=C3pdSh_x_EZV>lMss"
"3g#[$kZ*_t.]aX$'X9OJ&rjet9OS.(^NU^$s[+r)3^Hs(r@*UedJ@I298L%uI/(v$-TDwL$JS]&21G5f/D=DsW4`rtC89aso4+Fs)HvSFI';`tlZrGsqj6,i4%wK%OQDACqbu3%hTM=B"
"mDM<%SZo<^ixattY^_1f9,5d$Fl9'[UX&GsvChJ[XaoS&Wv7]$V=$'TH2w1h,<S@%B<>-Q;KAst,<`B7&UuiZA]/Gs#ND5GJ-;`tj[GFs,-7,i0iZK%PdrxC>BWw$os7uBgvl;%RHA[]"
",DbbtTsU5eEi[S&QL0_@vqNe`n;Ch$3$$c%ukbIskYdBTbKPR+l12lGMT;`tY'4+Hm[xl9u:Ud$,vMD>A`OitZIt;Lm2#O+2,kI%i(anXghU,D4)*piR;MQhMo,(%^raATm8RwbRmObD"
");3+3moT%k%v`qQDMcLZ=F'XuPMw_<3H.9IZHOC)g'0-)K-hF&dNtw4o2[^46wPJ&JdKJ&8oE[XjG9as4Gwt)O@w_<suId;1M)d$wJi*M*CZ]tQ.6FM`X,N%QI%BKgBP2KjY:--3,M^S"
"%w]udG,?aS6<U&t[kWS&v:QD,AVWP&'ht4T7?99WKm@vY%XMc,qo+UiPDpTitlCXs(%sfQQV$s@b/*RN:sWg;-[/Q&X^WO9JgFA&]MP%ur'4csDdL%ug,?rs:ua9%OTMACT)YMZ$OnAX"
"C@plU-FJRnCS-Rnr@Q'Ve9VM,cwmM^[8Oe,Ev[9C`R9/;ir)fO?b-Qgl28h&Ja^S,t$mS&Y:.qdhse=&KWmV&[x$t$;<*,]=<OB,6ZOB,'n)Jh),+DsY;5g)vAnd)M.?Z](0f%c3A^8X"
"roRxI-wE6J)>6S%>=cls?4xDsq4Jst<I0.9]XKutX'=Ls@Id*jsUo;6smq_E/N;OsF%+h:[+m<I6,Q2T/w;v_tW+dRC3fWqn4fTMHG<`t%<Y,;W6bA%TtQfYFpj&%7O?3Z:fH*js>isH"
"EI7^&'lvi7f=f>%9/]FD7an'Ei4QP%h0++D6l2k(e?p<dHK@I2N/Xb$U5UA&RkuDsP9qj%Jtk2-FLpqIc;cg,]R048l'/jT&9l,-,.o#>U7#[Lx4rC&d8AT[LfX2-O.)P,6QQ#u1RZS&"
">%$1hg;WDsEir[$^&k<8?\?n]tK%&]$>?9ase=CV%MNWDs4-BY$m4H#8HZn]t99BY$AHT&thkbTuNHvC[Us'fsfvltlhHplo^W_.9&9+oLEOI5)4Txnj'O?K&TD)pFY`cS&PVWP&LiSE&"
"5N6[s'v[&tg(/%bkJ0qs)_aDufw+@bjcG3ut7jmf[D#AkgTH_sqi=I%^1FL9tj<`t#PnZVb2tgj?:7^&awbeNF]e#ghC4J,Pbo5ftIHUH97)R<vaVK,MLHR&OS$_&5oVbDM$V5/54fS,"
"^$;mLeFxT&9G1`ttQvZXo4-10-SF9%Tu8wTx:b$OGn/xTcHJ%-[weUJ?dM@Od[[l)X97aHbo3-)v5`-IU5eJ,^8mGV$/6g,lY8%U1=mS&'6Itu.]MZsq,PTIF<GT,rPCD)vLbCN:#/**"
"t-m]tnwtV?T/Ug$0l7N'X/rbV1aMQAGqn%u>tAPs@)`C&EukkL-o$FsEO_V%0<gwFP;rx$3;nvsU@DWZZQ?B=/Dn9vlt+w#)hNbjc+=HM/B]NIuFNw$oDbi$-eV#lcNc&4-d4f(F#$c?"
"\?YF7$s[nR&(5xt,wYQ^t*7Lk$,<3NgK$x>I'nHL,gQT$f_2gSmtvni&.Y(d$);X&m1XoYs[5h?u@*'19SYWWSA*p'-J+R#lJq6asZGrP&.a5I`BacTVY^eS&f,xw4IpBZs7&`P&JE'(3"
"DF[;Ha;(Q&C1ws$(+31'Y$%K#A1rP&/KYS&vjInLY;l)mKox7$iuKLhARj_P*[GDI?V4f&+)419iXFIsY&nd)kB5`*p*4N'LX]ZKWtJZP`9dg,dp4B&X#p8mCu9%I=Q*K#;&=^t(5-.1"
"fQudh/^3%gihWZs]-fpR7N6^tgQAqD[-;%E67^e&YE`_3Atd2hIKrCs^k1Ds#dRKsp1u$F,8=K1RV^S&I;^S&?VSkj;BXxk<()Um4D###?vr.C8F#&+ZK8D<n985&s1_V$r%D_&*UJ)X"
"<l(m/KQ$4Cap4A=D+##,pUtx+(Vq(<7qY,EcKl+MW/>)4ZJ:&5j'YT8JWh+MNX?J:UIDxH7]###Ds0A=<Du(<F/rx4op2GD0L3F%^oJV6o3w(<ThOY5.t[p&aPor-F8SN:,OVf:1cef("
"&i0^#?cR^0h2=*$cfUv#JY<rL?9(sLUh?lL$A2pL6X+rL-x.qLv(doL/4fQMEShlLifI2N^phi#1uqv#^I%d#xF:;$sw^4.m;:pL>[>e#j==e#h+*q-RMYQs7+Lk+-''2#eR/(Mq=0vL"
"2tVxL;wXrL)``pL[9.*N=j@iLbW&%#:W]vL2iC*#_DB6v0t=6#B^h3vuhr0vm?J5#T)MP-N+W#MfZWmL4jYI#f4>o.::b]#=-ex-J`Kx7ImVG<A^jD#iYe[#E&pa#Hic+#d9p*#n+Yu#"
"Gnl+#n/`($qH+kLl+3:/$8A5#0(G%NLJ_1M<A;'#q&W<#IR7%#`ML_&vGVwK$9^c;S<GwK&eW]F?,fQst;cQsg%B9r-&I2CHueQ-06lwMWt11Npd#<-D/n6%,>T9V#h/.$<,-&MT76DM"
"1j30Qs2*6N>jNj0I1ml/9*',2p#NcDgttP9.l/wp)`XxX5,JS7YM9-v6U=G2hO*;?*Y$s$hHT;73K9N(A1gFrl.Ca=aY%)*1umD4-fDk=(w/.$T]GLM2oZb#Q/qB#SRrt-kpYlLeu#u#"
"L'niLq(`#$aZ9hNO^mP.Oo@S@[vrB#q@uu#c.>>##@;mL8-UE#%M#<-c=Ru1bk*]#RS1Z#coM<#IcHiLg5VB#X'niLnoC^#6Km-.4G6IMSL'=#fK8r/VtM<#F[<1#f;J'.EO&kN4O[Q/"
"&vQ]4$i?xkoo.Dtm]Mcs;tjV@mqb59ci*)NCA/crp1_c;3mq^o'kj-$/-k-$4<k-$=Xn-$[]l-$`il-$=#HF%i5i*M.NZY#9+uoLv(doL[6wlLHotjL'D;^#`XI)$VW.H#lm;%$:K7)$"
"5->)3kcv_#`C&&$CaId#jZu'$@<:pL_r-A-MW_@-E7T;-u5T;-$6T;-e5T;-p5T;-(6T;-r5T;-$`&g1TL31#Fn9'#lxt.#_MwqLMME,#KFBsLs^M/#&Ui,#'?8vLB3urL,*jxL_:B-#"
"Qq?0#lY3rLlhpvLimsmL;vRiLen#wLU6'5#KK.+M2ecgLROEmL2'2hL]>6)M,s+$MtsVxLV'C4NEE:sL8LoqL[FN$#m]#3#So<<8VTP-3>am`*@`gS.p$#,MAppV-sMj--.3k9;V=b.$"
"hh#9/SPGxkA-grePPN/i6]>>#R*P8MQj5s-@<`*M#f=rL5CsjO[sZQNe/I3#k7u^]V_P+#6),##-`($#d$Y6#gwCEHDTl##4X@U.#)>>#1r:T.&/5##3X@U..GY##=q.>-7M3X%lP;<B"
"H?bl8:FmuBFr9eG8#R`<m<v<B[w6fGR[CS@:c;2F'M*8MUAg*HeHx.#S=XS2<E',H'->/#^J:L,L=Oe?vLO&#s(B;-t4T;-*lP<-<fi6M-LYGM1e(HM*_uGM$:>GM)XlGM(RcGMZ-*$#"
"Ike%#GdsjLEd4)#a[P+#1h7-#Qst.#>u5`-=*OF%)md2NpLJ'Ne_R`EZ(5DkLDS'#oZY.q=?UG)Mc#(/PPY-NXn).NVbm-N]0N.N-lR2Nf/(eRi,'5Sb2JM9COpr6Zvei9PjBrmhpmCs"
"l2N%tpJ/]ttcf=ux%Guu<bsl&@$TM'D<5/(HTlf(LmLG)P/.)*TGe`*X`EA+]x&#,a:^Y,eR>;-ikur-m-VS.#wNM0)EKJ1A7tu5Nj^7n8lqi0Jj>ontZnl/GUTV6S5Z4o&H,,2%KGG2"
"/pcc27D``3:fwx4@r[]4Q6mo7.)vWU:tLV63X>WN]LrKG%PlKP,(r-Qh$S-Hf[;kFl*/LGn*sKGq?S-H?neERq$687*MN88s-m9V/Gvl/2Cjl&mZq.C1435&UoSY,jdGrdMbGs..f2(&"
"<+&(&^;>_/D_5F%iZ?X1vD<A+RT02'aQDp/hS>F%VFow'5qLcMorM7#O/5##<q.>-Z^r.M>0F7/3'Y6#[RtGMo'<$#>bYs-vaK%M-lrpLw[H>#u3G>#'%###u4T;-9:)=-j5)=-0s@U."
"ic>N#YA;=-prrt-@KwqL[>WmN[IQ>#&d&Z.D:r$#LTT40C8J5#C+1/#h.NX('POgLCkFrL7P4<#UmD<#Y#W<#^/j<#b;&=#v.Qt1V;P>#,Gc>#0Su>#4`1?#Q2(@-GE_w-3=id*4wj68"
"6W6X1IH<3t2)S&#.U]F-*6)=-0%Uo-V1c'&V@K&#RGJ#$i0$B#VK3L#Ni68%weT#$ZW3L#T=NP&#x5Z$l54L#t7-5/CNm3+xGP`<-k[[Huqdi0SJPlSMt](WP;Z`3L/9L#4gf4]f7W]4"
"g(:L#BRS%b_+3?6t3;ciR-<kF)qj-$I)###UH3L#XV,eQOGYY#YT3L#R135&xnp>$^a3L#NEcu>1%Pk+a*8SRB'q=Y](8>5^c9L#?7W(aX]Q>6sTH#$u9-T/-Vd'&Mg?S@^^dA#^RK&#"
".&1#?21XMCF:pV.pT(@0A+*s@4I_>$gRm;-h9I:Mk<$##Gl[+ME)^fLCp3<#=)c?-3(m<-ldd'8UoK#?Y2i+Mo(^fLp)8qL*#kr$J;NY5gCMn8wN?_#0x%/GWeXUCn%GG)_.Ph>0S^v$"
"`Cf+M[xSfLok6##&5>##Q9Z8-#h98.@1xfLT'*$#7*:*#w>Ur%E,tr6bu/AO3U=Yc.^Prmg)s@tA?uf(ei,)<6I:AOg^drZ8V3Mp?p`A+c=eY>(h35S[lAMg):xM'@F5s6W>3)NMAJMg"
"'A:Z#:YeA4=GB)EPfvfUd.TMgwFZs$4`/Z5OX%ZGVGP5So-B)WIBlYc`ag)3kU3Z>8*AsQW/7sdkMkYu(ah)3;)FgCNG$NTbfW5fu(_Z#2A3B4I.)BFi3uAX.E,ZlwYfAt#6WgC_Rf)W"
"jfRsde;0ZuhBSN0kJON9(,ENKT)gsQ>8Vsm<xfZ5`KtsH-v+6]PI9NptgfN0A;tgCee+*W*F`ghoA4Btn<W6//*M6Ax>1tHo^dZYDJrsmGp`**JeCt6n8Q6J;c_N^NPdZl)sFBtbca**"
"Xj&O9OMxNB%E8tZH9=8%X1'crNE]=l]cM1pj'?%tP_.F%n]0ci&JD_&&>ho.#dq%4xuJe-?iGe-W[He-I.b(WC;>f_+Y$G`8ubY,Gjx?0cGB8.BG>;$5cNcswVC2q.h4onu2KWnR:ho."
"oIw%43>+XCr[Zf::38G;NPxf;pkM,WFHAf_6VTJ`JHvE7N(D>#<MsOfF;8>,M#BkFaT,@'`(Mq;bJYc;?rK/`cGbY#?=0dsfK#Vd#=+_Jpo2_Jwv&v#F8RV[Z6*dapAb@kFB8p&oEC>Q"
"&fF2:n5P&5q1e?K$],XC4kOc2BOq%lcc6&YQvxjk;9R1geH#VdG-?JiH8QkO)`kgL=@UkLLS(:#CA=gLM+t4#h,p7.u1]vNKZ/%#%%kB-udEB-gAqS--_W#.hnRfLd,aaM-S448e)[;%"
"_@xcs)3dIqDrA>Q1qm%l@5icNaV$&YH(B>QcXps.>du%48v]'/7j&W%_v7j_C$&20WdUJDQWhJD]8c'&Cvu92wA-@'=8?8%L2#EG?,fiLbNcxum#Su-X8*jLt`(#vAAB>-fdpC-<HII-"
";4RA-LbN>.LWt&#-nx1q?@WW&9qWK-qkg%%JAFS7*jt(<b`d(W'U+kbs3IQ(j$Y3.*bCaMVYuxuB[V58k9S5'Oa10178p*#*C,+#/I5+#dee+%iEAf_:cTJ`ZSOw93^]V-(8mEIP@+KE"
"X*vG-(<D].NeZ(#D[AN-eo,D--n,D-wm,D-465O-=tu5&Pu@kbTKHXLdZs%caF*d*D3=,WcaB(JrTaY#qQel/XI,2_9[oT8t3YjDYK_+3Z<5'v%HG'vSNP'vw;:3vT,O$MNpdw8sL2<D"
"j>8>,7,U)4P*#q9R^PWO#-L19Hn<n4eBD,MY>m6#LB58f:OEr8f=794hDxu,jfHi$Mb*S88.Y8p;dmuR*)95+Mnw@#XuIO,og]13v95'vfGG'voMP'va5OxuB,niLtrlxuuJX&#WQ'#v"
"r$ffLRMu$Mk3O'M?)F,:UnaDu6igOUW$aC,I%a1:7k3A';,FS7mv,Eulpa$#4in@#NaqR#cBbA#T7%@#T$b30__R%#YcZ(#Q6>%1M)hw0mJg;-m0'C-%U<nL&L6'vP_ST0-wiEPUCcp7"
"sbf-+R7]x7<&_E#[8tW#hv3PUKx6j)Udli:eHG96IDMV.OWt&#+v<Z%.WpI_N8X1^dE#VdUfpI_GG]Y,ZiDX(wYDX(2VLJ:,KHJrt`2eddS=%tWN;'mdVZ)M3>ABN/p)*Mlp3<#$Ti]."
"icZ(#NK+Z-aDc'&QDa3;hmJwux=F&#%0wiLncc&#^.xfLocbjLL'2hLu-<$#oZbgL?p)*M0vu&#qfv$vnQ;e.;AC3vWl*J-i<8F-q<8F-/%jE-_0/b-<U[w0r)c?TLML'S<JF-Z`nG#$"
"9oNcsqZ2@'v7ho.w1W'AKKQ'AC3Q'AmONq2;w/9BVZn*.d:4gLk't>NP'2hLNwfN-_4f/M48A>-%Y$m%*hQ1gJaJp&hNfQaRa#Ra]ru;-%'G?-8fF?-r/*q-VJPq;SgwV%n.Cp7C#CpT"
"A_rr7W>RW&%QE@YYbm##9xr>-eUpw%wc(:.oeZ(#4umr7Y/X2i0r/E#fq;;$;QZ`**^)SewYg`<U)[;6U](#l)Fs/1&1,$5%%x#>4u=HMitLT[,B10UxDHTnqgD0CILZY#^9ku>xigY?"
"#s,v?$&H;@.irs@o`[iBuxw.C.+XMC;XtiC4I9/D5RTJD6[pfD8nPGE=E.&GEPuoJNr:5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJMNLjfMOU/,NT-c`O,g)&PZKCAPWH_]PXQ$#Q"
"YZ?>Qs<=AYtEX]Y.vnP^g1D5&d(%b4RLdl/@80VQQb.F%B*0F%?_n-$&jp-$'mp-$Cw#:)C.3F%XS5VQp4Xo7.:g=Ybvj-$,$k-$-'k-$.*k-$/-k-$00k-$13k-$39k-$4<k-$5?k-$"
"6Bk-$7Ek-$8Hk-$9Kk-$:Nk-$*dOw9YG_1TmIh9;Dmk-$Epk-$Fsk-$Gvk-$H#l-$kJ'F.K,l-$L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$UJl-$VMl-$WPl-$XSl-$YVl-$"
"ZYl-$[]l-$hoJwBo.KwB7Bi=Y/kV?^q%_?^mo^?^I(]M'$4xi'.*O2(/3kM(0<0j(1EK/)3W,g)4aG,*5jcG*6s(d*7&D)+8/`D+98%a+:A@&,GUYs.?f>5/EOlS/FX1p/GbL50HkhP0"
"J'I21d#fM1L9*j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4U5u`4V>:&5WGUA5XPq]5YY6#6ZcQ>6[lmY6pW-n9&Jt+DcTe7R_>c'&oX=rdc>,F%iab%=4D_1T&5-F%08qI_/8l-$"
"<$EcD`YNk+o@m-$L9p92x[m-$#`m-$$cm-$_OvL^74Co[c'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%(2ArdnHn-$A?G_&1<k9;?_n-$^IPk+Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$"
"L0o-$M3o-$N6o-$O9o-$3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$#VK-Zw<Fk='mp-$67bw'=slQa,&q-$M-X-H?x2F%XSk--=Yq-$[]k--0?JcDlme7ROiFV-ifC_&+wj-$"
",$k-$-'k-$.*k-$/-k-$00k-$13k-$39k-$4<k-$5?k-$6Bk-$7Ek-$8Hk-$9Kk-$:Nk-$.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$cWMk+K,l-$L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$"
"RAl-$TGl-$UJl-$VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$dkG-Z)BM3XL+A2'._XJ(7&D)+98%a+Sh+3)XlgX#r3S>-V5T;-W5T;-X5T;-Y5T;-m->)3YTS=#UDluujX1vuQP'#vRv7nL"
"=RxqLk$v*v*[<rL/mx#M<s+$M2Ac$Mn=S4vn62h7#3&U0jiLvutJtxux?<jL%`I$vo:^kL,44%vAJxnLR3urLC9(sL%HA#M>NJ#M+TS#M<(i%ME.r%M?:.&Mxn45v%nLvutS>d0`$`vu"
"e*ivu9q'hL.'2hL/-;hL<3DhL59MhL3E`hL4KihL5QrhL6W%iL7^.iLPd7iL=j@iL>pIiLlAGxuWJEjL3WOjLE]XjLFcbjLGikjLHotjLJ%1kLl+:kLm1CkLM7LkLN=UkLOC_kLPIhkL"
"QOqkLRU$lLTb6lLUh?lL%oHlLWtQlLX$[lLY*elLZ0nlL[6wlL]<*mLmt&nL>xu&##TS=#pG5s-sL<48tMN*Fw_/]XJY4;-O0qOfg*b20rO'#vMV0#vcc'w.L*t4#]<Kp8l2SDX2d1R3"
"#`m-$$cm-$TJf:Z<bvL^c'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%1`tOfKWS+`rm/F%Ed_w'fxj=Yu^n-$bnh--Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$"
"3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$'%dE[w<Fk='mp-$67bw'AA.kb,&q-$M-X-HAgT9`CFJ_&]x,F.=Yq-$`+-F.o,n=Y*x/]X*qa%=gO2W-`XVQ1rK`J#-j)P0^r*xu"
"Rx3xu7iHiLlAGxu;C<jLGPFjL7WOjLE]XjLFcbjLGikjLHotjLJ%1kL>*Lu.H-$#5*lVA5XPq]5YY6#6ZcQ>6[lmY6]u2v6o+J59wbbm9C,<MB(lgWhgt+/(+N/VQ]XI%b&5-F%PY,ci"
"/8l-$<l3'o0mNk+@x(F.x[m-$#`m-$$cm-$:s7R*)rP.hc'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%9Ln-$cKA_/?_n-$UVw9)Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$"
"N6o-$O9o-$3uJk4Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$m>Z9Vw<Fk='mp-$67bw'1[%_],&q-$-]4kX?x2F%XSk--=Yq-$[]k--+vFo[R6qOfR/LoIN[xUdK:a=cgG,F%-'k-$"
".*k-$;D[w'4T,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+<m,F%=p,F%>s,F%.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$g&f--h)f--L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$"
"$;?_/VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$^d$Vd0W]Y#cf>8%E`GT&>Gtr-_5wV.Rd/K2=Iac2YMCa4Cf:D<1asc<TxM3Vw/@b:'vbYH*1)vHDx&eXnHA#MrOJ#MxTS#M*go#MEIl$M"
"2*i%MM.r%M?:.&M.%55vThCvuhnLvugi)'%d>cWA$xOT#WY6Y-D$[WAv0PT#oFKU#.cEB-D5T;-E5T;-F5T;-G5T;-H5T;-%sW<9Afvof`bVw0WPl-$XSl-$YVl-$ZYl-$[]l-$nI-ci"
"07NP&&eN`<(7?\?$$`_@kaxp-$,&q-$;Sq-$V%78%`&&.-rb.F%NwJpAa[rCalhBW.r[,29QED#$_sX>d;&%VdPBF>dAT&^d>8X<#<8q*8J$(2qOqnF%oru8.H0`$#jV5<-NS2M/@fB#v"
"oUWjLFcbjLGikjLHotjLi1o(#Ve$nRo)doL65voL#;)pL$A2pL&MDpLX=p)v[o-qL.(8qL7.AqL44JqL5:SqL6@]qL8LoqL=kFrLWmnI-UA;=-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-"
"L6T;-M6T;-N6T;-O6T;-3q-A-YN#<-V6T;-W6T;-X6T;-Y6T;-s6T;-00GDNRio#Mc*_k.v2/X#E5`P-bE88RwebjLUZ(:#o#a<#lYlS.S>0:#x5T;-#6T;-(ZlS.TvW9#%cpC--6T;-"
":)m<-3N#<-4N#<-5N#<-6N#<-8N#<-96T;-MgG<-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-L6T;-M6T;-N6T;-O6T;-+@:@-YN#<-V6T;-W6T;-X6T;-Y6T;-s6T;-xZlS._HKU#(5T;-"
";5T;-o_e6/WHtxuB@<jL%`I$v%;^kL%HA#MD(i%MhIf4v(x*(2+/FxuAZ0#v>a9#vbONjLE]XjLFcbjLGikjLHotjL]<*mL_K_&vq:P>#X7%@#Pr0hLw,-D-P#H`-:hNl=#+/R<KN0R<"
"#+/R<*bWP/Zmsl/GbL50HkhP0hwk59hIRM'I]toSI]toSJwdpS/7e1T0:e1TE>5L5VU@N58%F_JR3-L5wKd%O>jDxLZhDxLZhDxLk)u;NF]]#MK[]#MK[]#M%4QQ#WnXQ#V`/,M`dCH-"
"-X3B-haCH-%(@A-%(@A-%(@A-w3@A-s'@A-s'@A-s'@A-s'@A-t0[]-TB0L5tfd`a'/###6QxAFS,DG))_?o[JUOfLa)2hL83.m/K<bxu]M]9vi3f]-nZD.-OdsX(s`;onf'K1p2@Y1g"
"aO=r)v+lX(,Ykxu=5(@-N#=R/hNwI_$`xOfDk5/$cg1*MbrV&NMddIqPi[+M<O5-MLP;V?DP>p&6BO2(LP9e?@ZEL,pVFX(F&>uuaG51N?K51NU6OcNoM=%t9d*SaFD9U<j6]2L'xd1T"
"'#_>?OSPe$g3r9DDu@1v</OR/p+R(W%9i:ZBF0.$(dexuC7`9M[5WR*=C)W%2oTV-+9<K3hVG=Q<:1UT)'l4SVgGA'@M)&YLq12_O6*eMjI>p7H5.a/+03j_Wgg=c5X&32F:S(WaJSh$"
"#6;KU*sP+`LLlxu$$jX-v#qx'.lkxu%+*KUaI.`aHLlxuWFEwYdJ3v%T9mxu6qo4QgSp@PSH(q)Mhl&#5.`$#ewiJCP0pI*EXrr$a@pu,Q%lr-c5LM'SAP`<]LZ[H.NQY5j$'DWm1g4]"
"rNHP8du9L#LesCakB$3:t3;ciR:KwBY%C#CVVuu#8ONP&Qu_3=r'88%Le@)4`Su`414.q&)OFV?Ox%8@SZw6049$5A[wTiBb=)/1f$7L#G*/DEq9x(3u@f(N=j[xOStZf:KgNH<$6r:Z"
"&YKo[I#gA>;V*XLqPMucSA.L5gI5H2@mC?#TChJ#OU@%#gx*A#h$eK#_04&#pFbA#,=3L#9gZ(#KKjD#LT,N#XxJ*#j+Yg.X'+&#P5J'.u+vlLxVL*#a7p*#hI5+#5gb.#86U@/C8u6#"
"tv0hL_0,A#+%kB-(X?T-H^@T--kv5.j]>lL%:+)#T>hH1LPj)#t[&*#$5i$#f;#s-Yt7nL]dA%#xeJG24UG+#6`R%#)=M,#CH`,#e^#d.;n@-#$wa?3^FX&#Xst.#q:L/#.@$(#nwiuL"
">p<4#cvJ*#OjUxLOcp5#nxLg%5xwCa=#MucA%nx=t3;ci05?5&@4PfCA'TM'M*1)*NqLcD`1B>,]P=8.ej.DE:S%&4+arr6,M$;HYmIM9%g*^#drjA#L;S#$u=#s-[.xfL$L>gL=H5gL"
"ggm##hiTK-.Y5<-OArP-)5T;-,p#-.1?<jL.3DhL8k?lLeC+)#_/5##V)B;-l]oN/c;G###82mLII<mLtTGgL0'(*#mb/*#sF`t-0]imL97U*#jSGs-8u7nLE[$+#xYu##n;#s-UHxnL"
"W33$#)=M,#j51pLYQ&%#%<#s-nACpL]dA%#'<#s-pYhpL`pJ%#p#@qL)Y#.#IG.%#1<#s-'5[qLYQ&%#3<#s-*AnqLZW/%#5<#s--M*rLb,p%#8<#s-0`ErL0?VhL,<(sLqY`/#Hox/."
"A;9sLmfr/#23H-.EGKsLVr.0#(@0,.S:dtL1-u1#^TGs-@9cwLbbE4#3;xu-OjUxLdsp5#<<r$#bJR#MAt56#>m@u-gVe#Mu)H6#J`98/KJ:7#*<k$MRw)$#8%q%M:S]8#h.:w-?76&M"
"6`o8#K$)t-DIQ&Mwq49#iZU).Qt;'MwWp0i[Yuo%9L<5&CtWP&;[jl&A'TM'Gm-)*H9KM'`1B>,V=:8.Ws,/(n0VS.ZUqo.ID'F.[S%&4#%Fv6WJ/L,#[il8*eY59_`/L,*wIM9=<xjk"
"crl-$XnFJ(2P9Z$qJ*XCHL+_JU.RS%^F$T&&8q;-aQFv-A93jLAdl&#PF@6//K6(#5]>lLJLH##h;#s-ZBonLG:-##o;#s-WsboLYKj$#Z61pLNmj,#5g1$#(NYS.D#S-#jRQ5.qs6qL"
"nFp-#9#)t-u)IqLdR,.#;#)t-#6[qLjk>.#Ba($#4<#s-$GwqLOkv##6<#s-$Y<rLOkv##A/RM0U:L/#H<G##S:dtLU&l1#9E3#.nwiuLB7[3#%h=(.OjUxLWap5#w&a..vSj,8%T9N("
">xc._F?<,a<aN9ip$xCaq76&cE0t92*0QucRHF>dI<t922ai7e$6j;63lC?#?=.@#:i1$#7xU?#7V(?#>uC$#?.i?#8x(t-t$:hLdT8@#;x(t-)O$iLe)#A#GF`t-0[6iLm55A#Cx(t-"
"4hHiL%gGA#O+i?#mTk&#Rd9B#?[,%.Ts/kL;M.C#o9xu-iY5lL-44D#lF`t-#5)mLhK_E#r]1?#^(T*#1W$o@[q6=#";

//-----------------------------------------------------------------------------
// [SECTION] Default font data (GroupType - Bank Gothic Pro Medium.otf)
//-----------------------------------------------------------------------------
// File: 'GroupType - Bank Gothic Pro Medium.otf' (36960 bytes)
// Exported using binary_to_compressed_c.cpp
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
//-----------------------------------------------------------------------------
static const char bank_gothic_pro_medium_otf_compressed_data_base85[37245 + 1] =
"7])#######jAw%B'/###ZE7+>'wPP&Ql#v#2mLT9@w93Gsf)##Fl###Cg/h<v5+E4)]*##UO*##%a+e=6Zj$`WS)##kO4U%IL<`<@7OLDF4;7#D<F&#<;isBb<xeuZ/q.#ZY-(#W:N]D"
"DMkmr&P`hLH5P$M;$tLD5k2W%S%pE@g&M*MKO%KDkVpA8-5J+#'Di5#e:V)Fo;>J#Yg3O+5O$iLoMbAF._uTL@_LpLPw-tL>u<5G%?c+vkG.%#&pJ=#C9F&#/k8g1l*>B$+sE]AeI1=#"
"?<5)MwKg:dbhLUNhc7D*Wu`MK`_aw'K>YY#t+9;-kbrE/5Lc##_WR#M*9lX$5$<X(Mnge$=4g*#Lah+MWhZY#NdvFr,V:;$oG<AOp/)&P2L$C#3S?(#v7YY#1r_;$.LQ?$(#x+2XlUf:"
"Ohbh#P(`l<H5%6/DA:v#Oa-6Mx)LhMO*W+r+g[k+U@Bh5`hn@#gwchLoTNmL[ABc.3(V$#01vhLOek%=1:h#$Todu>2C-?$kwb(N3LHZ$xxc.U4Udv$6Xp=Y3X;s%9`^f`8$&9&5H`1g"
"5krS&N=xiC:BO2(L$+N(mcxQEK5>>#>84kOJ,@MK-=`T.R>?_#0@`T.54.[#]@`T.1m[d#grG<-N2:W.D'F]#*5xU.S&q^#4tG<-/fG<-aB`T.[>H$$17D9.ciD@$24w2(N=F5Ag*0Q("
"ZlvlJk^T%J$'4RDCKlmB&8taG4sOVCmiAoDD0V6MHr5/G^SAO;:.C,38$ik1^DxkE4Qt<79rCVC:1k(I^hx98)n[F-b1USDa9vc<c+KRC)S;2FiMmw9$%4[9Ft&^5*)(hM*7<MMo1FB-"
"f;v11pkn+H5-xF-9-IL2*0+gM:*sE-)DbG3p*SfGgefP9,3fUC^O9QC9YvLFA_Nk+F#I>Hl6rTC%m>PE_K&XTko%oD#SI+HsO[MC6SnbH1gkVCHmI>H8=4cH=8lv%x>SxIEOL21<>E;I"
"?B%rLH]hCIc91@-VBh<.3rw+HV2k)3M3FGH,C%12cOFs11%fFH+r3,H<rY1F@c/*#Xst.#e)1/#I<x-#U5C/#lj9'#2_jjLjs&nLu'm.#:*B;-8sY<-A-:w-(T3rLZ[ldMV3urL-&1kL"
"WT-+#j[W3M-i@(#]8^kLL1CkLFht)#3*B;-IavD-P`.u-*0nQM_:2/#K;.6/a9q'#IZhpL5`ORMQf=rL8;SqLb/]QMTxXrL.LG&#BK?9/W;L/#fU4oL84JqL;RxqL?72/#&FV:VnF^wT"
"OX*R<cS1`AoWVkb2U%RE9BI-Z`$?qMte#F7N+G-Z-^5kk[D*m9UVX>HkSsE[SOSEn7`DJ1vS&g;WJf]G%aL3tq5Kw^427L>1=$qrwc&qrB+0F@3kufDd+]>-')s>-`Wr?9a&s.Cor3)F"
"-IYGEr8*aFq_U,XkeXRMM&iH-5cpC-vOP</VMh/#vGwqL0)crLW9(sLrkP.#e.;.Ml#YrL_MM/#k@qS-Qsx(%-%exFoIg--^<A_/<h&/1NtA5B@#B;IV5j`FRkDL5,Rx7Iao`MCF0q>-"
"d1*.?d)q92efFwg8g0j1H0[>HBRf?KY&$,2>sA;IO^j;IcHQfC+ZRDFG9hJDspVqDk@XEexLh%Fb5?^?VjQ+#cRG,t&rJfLwHccM^GVG)#4Ns-q++,M5:Mt-PAr*Mlqv##_BhhL7+>DN"
"RY$3Tu#/.*RA[`*VY<A+Zrsx+_4TY,cL5;-gelr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1?@<aN/K6(#3WH(#7dZ(#;pm(#?&*)#C2<)#G>N)#KJa)#OVs)#Sc/*#WoA*#[%T*#`1g*#"
"d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#:n@-#>$S-#B0f-#F<x-#JH4.#NTF.#RaX.#Vmk.#Z#(/#_/:/#g/+R-IAu`3E?S5#IQo5#*%.8#u=R8#@a_7#"
"=Z`=-Ad6#546u6#D,c6##rG3#>&Y6#J:v3#Bgh7#*DLV%a_qCaa@t:ZSJPlS`i`%X%pg1TdIIfU%Kn7[pj./:IxP%k__A`WFHGJ(k9#AX1UOucjRni''W3S[>n/2'4_PuYN&IoeM11P]"
"eZ]rH=A$8Il'ASIj(f4fsOFlfs['MgHmToI$(_.hXXLl]^Qcc)Kf9ci)OvFiP+;8%tlc.UsSUlJx%n.L$23JLOeOfL+Sj+M/rf(N3.,DN7L(AO[h>]O=k_xOB<w:Qq+?c`aMXrQHas7R"
"k*WS%AMU(a2<Nl]qwm,3GR;;$0U6H2M0PY#CiAE#/$]H#r(eK#,Cno#S-gR-<Bx=-Nsk>-er?@-7@KB-54Lv-3)YuuXF3#.g+PwLhw9E#YW_pLwdhsLKffo#OwSR-96f=-YDOJM8UJfN"
">7IuupJA=#wq.luHx[due#^au#&+&#(Gc##+>'xL&uoc2a-)fT@nM.Kwg[IKx-a]bf@8U)N5l-$]+l%l8%t-$gDHrmFZK_&8_*WSPYD:#A*6;#8$-;#F>(7#P,c6#R7I8#*%.8#F?bcM"
"dEY(Nq()(&4.UB#);5##;?hc)(Zo-MS3D.3%p*`ss0;ILT5VeLM6j-$3cK]t@M%,VX8vhK$TYY$2Tx]$8Lhn'1&IW('i9Pqu(IqLN'1'#4l:$#Z9F&#0dZ(#S%T*#u0;,#DN=.#h`-0#"
"8.92#^E24#V>@#M_CS6crk#JhBVDonmY4##L;EA+0A[]4h4;A=LCmxF'Gs+M>*euPVfUiToKG]X129P]In*DabSr7e$:d+i<vTulT[FipmA8]t/rH8%GW:,)`=,v,18.&4h(d`<CMFGD"
"i5hlJAEjrQ%QES[TdGYc8p#;mh,&At>*G/(up&j0QH%m8vweY>L+h`E07CAOceADW9]GM^],muc(AifUA[c^H+-&YBw?vsBtl>lE<Dnr1-d>-G$]eFHJ8k#H1,ZlE<3.RM0K.WCK)T,M"
"dhWj0:GViFP0+cH)HxUCDpN>HRMi9.-KFVCCq9jCT5=SM/RCnDRP`t-R5tSM.'MVCD+94MSR:nDApNSM/*7oD%@Me?cSFe?5vAe?7&Ke?8,Ke?7#Be?6#Be?7&Ke?92Te?8,Ke?;Ape?"
"8G-=_LBr$'J4(%'QOU['V[C@'Ye:@'_q(%'f6V['lEV['oK;@'@3o6BtUZm14iN88fRfS8g[+p8heF59inbP9jw'm9k*C2:l3_M:m<$j:nE?/;oNZJ;pWvf;qa;,<rjVG<ssrc<t&8)="
"u/SD=v8o`=wA4&>xJOA>#Tk]>$^0#?%gK>?&pgY?'#-v?i_]s7'+OGH;/;9.*G%'I@etmB-JraHtveUC_:x:.'wI:CO<(3CuAFVCNvaoMf36A8rK8s7$u3R/>9`9CrdM=Bo$G59olI59"
"olI59tlI59koaM:Ij`M:Ij`M:Nj`M:TM4K;-p(*HM/GG-p4<M-ZYnA-q36D-kU'C-e.j'Pq8..OmjlkM/]HrBT#9]8iIOA>(ORA>(ORA>-ORA>$RjY?N.iY?B]-v?kcR59]H@M.mX7fG"
"XjEZH[5KL2Sapw@j^CvN=W2r%gKVk#3Xfi')vq%43GG]FU7a(WH-9]bSQIxt9wOJ(ceUS.GT&,;F1mxF$>s+MEKEVQf+5]tM3w%FoDO5/-^>,2DFK;6mO.#>2SSYY'Y0DsjfnV6@IE8I"
"?9aVQj*g`WArEMp.i6W$E3#N'fOGv,ih8s?KfbGDt1]GMTM(mSq9TDWo*.)jl,a`s3([s$`75Z,3&;d2VQ%Q87BZ5AW_)^Fd*u]XLEP>c$x#jg(o`p%Od1K(c:#?,Mtl87;Nd5AW@U)E"
"q5(TI1`8gLH<e>PmE(^XDLeG`-[`GiF2U>lV=/pns;WDs6739%R)7H)uE@T.8)m,2H4F^4b;k/:;HQp@Qrb,DuA1TI>U:aNV;,TRiL[/U65Bp[V?kDal``;d(xT2g@^F&k`'1jp.PC?#"
"EwJQ&X;@H)^7:(E$Ar&=?lb(1/c*Oo`5D<%Mn^0@9rOk.3ARF,l5XB,:h1/)9Y/-)>P/-)2jUQ&Q_iGsx_fx$S&Q+)_Zw$4hJW(5:s(Z>Z.c)-=0YU$QPWWs+]&+idTXpLbgFatBTI5`"
"R*'GsU;9as8m-^tev3A@IsC`tY>%5`W-'GsrN2x))J]^s`f0s.v=Ii_99.[Kq#Bx3d,Y/A8U>IqojV]tvJN[tiU9<&pMKhT*:*M,4w/)4BuD^J_-*Np(:1$%RcjcRfw%TR1dC/ZwbXH&"
")=$h+oVL@*aJ7H&D&0^tlhe`tS,mv$Y:J)dBV?s$vvXS&OV2BT(Dl8n/xs8V;_d58%2rF,/6J#u7X9C,91,LLE%T//97YU&J))@ea@gS&8IIQ,bJ`:6cCA:6cM`:6Di'4?%&wHhjRBXa"
"v-od)GBmA%5+%:MFIEj0d4nd)Yb4`*/:o@4aqoC*#13B&kTM>&kA6?&Whq/MYIkE,U#PP`MGJF-PHJF-@eOU%5iQN'@6BJbc%gS&tC,FsTlRLA37C[,FEe]tRqZ_5tHm87Q3C=#,).-%"
"eUlcRh>3i0lM=e<E(:ofIfo9ZwAxq$laE`tdCkkKr5pN+IR:QM.J-xOcbWPW)`/P%lnthZH'-0%TX'JG*dY_F;YjC5NOYUGOx*atwT`4%CQZd)sKxQG6#jj,eP2RXdR[K%Z+I%;4MR-%"
"35eA@8CQDs,#(&k2)6jT%Bu,-(S0WL:,<`tF<>C3-23EJiBS@%nD<BJWZKlk&H'@0Zf/@%ZY?/bVG_Se/<45-Yo]f)wNW]D@Y9`t^*f.Gn,,,-T7cX#.nVe+Y*]]t@/#12SrL6a_3*K:"
"+Oxs,5t92p@2l5-mwxq)bKCdHRV^'%Ot-$UL%`8REC&Q+9bU1Qwm^h9.JA^KmDUas3t$K%[rKOGg?IBTn#mR+I[V5GKn:`t7;tmDw1;5B3%`d$Tfc;%QLrtL7/S,M)Gih,7uQ6%Q@VtL"
"5#8gL(Dih,lk+1%;m3mAT'xuDAAkw=Gq&`^M3:asPqd>uVUFB,[P'B,?a/^tuwtB,KQ7.0ULBI,2Kw;usSjDsApu[FT'PIql(pL,&Y>F;rxtDsfWSP%G[kut)bbkoAtEF2@3^u$kZ6W$"
"iOxPA/;X`slY=XVL=,p/)q`iFGgibVN$:VS8J/;[aFaMK0XD3bRvM1%urn/n/7sZ>GpaA%M/5Iiso$,V)/hUHapETstX?VW,mrS+`dqN?Oq:`tw]I`?(F?/2GQ0sHq/3DsUui]t3gfe$"
"M'YgGPubhs(l+b^c2lP'4f_`?acWXc_%o'Ns+F[`O$ZU$Rio8_c[LOfw?H0@ol?GV?7.g_hJioLra`'5px2v-I=dHArMCkf?@47e%fLC&HHsFsQmAm%c91GN&Qs`s8<(?#g8NI1a*x'N"
"8:$FsEqO+)KjI+2[KV_ElK7l/&:YU$fhg@4+oWS&pwJ:HM_&:H*J+t$m#b3EVl5C%Tn-bZVjNwVJQWJ;j`Hs(nob<mD8^b*C0Gf`#DZVs_`l9me/jO%v*]h$APZVsw]:a0u7*dVp/,d`"
"W/&d,x99^4I+v1k=dw0q3'@%%h+bAT9P?XPxU@@_,PaFs5c*X$#Lx[s:1o(SNbJQ+-,a2p6].<-tul>uX(V;&v+$D,r_Gh^(9jh^aIDE<(N7(-DZO%uEk:g%Kg_>7l-ULL`^Feslp7x$"
"k.Xw`35EO%stRh$^n]Xs$nl=3rxQGVi^/g_]88d,P^oC,Pe',3ToM^OkBN[=0#hmAb&nd)-Dnt5*M3:6QK5uGB./a3;G=itKcXgL1=Y$M<K'n`1E(7@atQkt`[]S&k_^XaVm4HV2H.9I"
"#;DC3=c=msc''86gfTk,^/UlTW-od)eG&eAIC+5BZI9/;Y&26GiTk1%Bjf[_aUggY3t=[l;/&PsfiNMs2^HctGs7M`Jp-H`]5J07H]?Q&*il'W6WOJ,`N.jFm`Hs(jA$-)%G3/)$5CPO"
"RoDGs2Ei'W?xYO,>`5C%N/=vDQHeZVa(SHsZa]A&D.BPssV4O+QrZO&,m?T%?1d7.OG]:-Kt>4&Zl5k&[,U@I:x?&@=Z)T/Xpj7.e_B-Mx*)c#hVo]t]54q$DN'UsjAX6%TtFIqv[ce:"
"fs>(WpTQDsEd@Ds)hpJ%7;&S7<tAu$-$b'ajN*%=TE]Q,kxR_NLpO%=SFk$O>`k.uAvtgLPr_O)wv%[sjD0Us9qQk]Zs==lLPRwsDEc6ui6Fh^RuW9u6m3A%6ZN:*=b(O%0oS'sC_T1B"
"PbgJ%3lAFr<c;Q&W[WS&]:XP&9U9'F_`0qi2_JC&W[[,54R4ZIF,5MX^N5v$^Ym6X%PTwON2X$k6MqK,j[uDsf.g-U/X*gj4EB9R+i6?0e;9`t=rF]s`aOIsj`VZs80>Ys@rk;JA0iVZ"
"R`=2-#<EDsk4&Q&0VVXHY6YxQc5`-I8MY@&`3N@&Ih#^s.A&Es^]?[`aOG`tiW57Jd;Yg,iNJ9%*pQ^ON>9[kn^-#$gm.Fs5D,Z&:XTJ%>ED`$O=R3ScL+>%OnsL'.,PI_ax6Q&*E^aq"
"g,SM,1Yqv#0%fqLNJ8%%;r_?Kkvd&=K'[S&Ia>v$QlVD@u'iM,>koR&G#HR&#-O.1>OgKI1%H.:Qa%.:(d&3I,p&K#;6d[p,hej&<iGR&(BcR&(dBV&pd,b&Ce0%+`G$&IIc'F.*aB:$"
"l^`X#X0@:$c6ia1NmaU$#;7(WUu9o.L^/qiX9e&4d(2Vp7RJI(*?tDs&8).1U=?(-Ujb4&>?R^OlV@>IYDDD='DVvlj=Ggh%+4HV0tYU$:;R'A#N$6MF>jJs;_kg1f[E7.%`x20.qx$1"
"1;YU$TsM?#0C&Kqx-qV&,D)Q&tf%j.R(3N'eK=Q&ivgsH#*5XUKob`s96u>#L$?^sk,fX#8S=7.8>]vtf?$h(Dd5^twqI0eliX8&6PKZ>G,Q?#iI(##WmYxOa>T2#9h)##rqi=c[w,;#"
"lR(##8-Juc^-?;#jL(##Xrj4SrOD4#xw(##iw$GVub`4#wt(##=71GDdJM^4JD'##0C2VZ(C]5#+@2##F(:c`<gh7#A**##d?+VdGMn8#E6*##q>vLgJiNT#LK*##$NV.hR=9U#Td*##"
"0D0`jW_^:#Zv*##us`xFpC24#0N/##S=x:HA.:/#d:(##t&5YYhi>3#KG'##1Z#AO[v&2#kO(##v-KlSYjj1#_.1##2(6PJ.g7-#f@(##H8h(EGU-K#'1)##>:qCa]&02#c7(##rP^%O"
"7G4.#E5'##mlQS%n7v3#]&+##MOSiBVTE1#<#B##aI=VHIX$0#%+)##-0PuY29hj$5_2##NLQ%b>mq7#`.(##*0+GMa2,f$6dA>#X*GJ:&),##FZhpLG>k0nQIml/iORY><.>VZlk(Gi"
"%F'g(*-22BwDES[o0.Dj,B@#,-JjY5%2ouYX8^G;(_@DjGMV;?wu9/_Bv'#l0+<T%nB*Q/I[,W6wnI#>Xb)^Fvo2jK.`ZG`Hu[Po=;Is?YP3E*4uxGMp<5aWhnFH)p4m)<VRtAXsg+Zl"
");Cvu-/1BF[57Wdl&sDs?*/0:@h6BO+&Ym]9#;Wm_rSB+5d>01#P2*<4cJKL,,;'=kNJ?Gd@CKhJ_8h1dm7q7YwnZGWBKEa0>u#u[fdB=VgW?Pt]RNpA)V$>[T#bau,Lh1._5X?<O6bN"
"l1`6SMOR0_WLkQoN$0:%rbl$,X_0C4x.6L:jE]$G=l/OKrtAhUYb`3^r`i?cfEmKqXxYwPeD]wY#r/o&u3]=$1#aqn>cL_t_MoeM_/lwu05OCX@BMi(j/gR]oTvkp4r`.;%01]Gkx$VR"
"0@iOg/RI],r/]u6EnFc<cWa:[Kg0fiq8`f22M?P9uw+MU*&-`t^U',3%bScEkGg%P[Cv:[JK+MhJC3]u6tE>-u,A>6pe-2CEY3;I:*v.Vcod(kK:[%u(xJD=p:#s@urL&#alVSSM@6pA"
"UkC2L/reVR[+-#Z;iFAcXvOMhv_a`k4-rrnHP,/r]t<Auq5m;%0cBj(HH4^,`%a50uQ6d34#Gv6sG'dNFJ5/rxiRp&aBc>H^%_AY?(s&#UU(E4Pw+K;Ai;p]t]RT&xZQd3K-])OK[-Zd"
"uI3djGSlJr<[%*+XsB^PhUv>ZBxX&c)=P#mb9K#va_,-3e,@E=1bajC,VIZQ[K:w$g4&q/6#cZ6,_JKD3(OWR[fIak++KR@fxk't;/CJu;s.Ud%rC_tmt)l$MNVDsu[Cbs+[s[=Ap@Ds"
"s_rp)Usn]tih)l$r9&Fsd7f,;=9,N'ZZVRsciY:p.&,@&FVw80bjbb)+^&Fs.Uh>p5%v'<6%'?JdA,DI:A(w#$qK[X`eAf`+;rH@+ifR]iUDO5>%Yd3s*`4%Y1/T%$AH5oO9M#f5.9j$"
"Q/1c)bn5LH>][6+9CS)ZbbuPWDa60Dq?>+ujQ((atAx'<o+l^LsMkP'l4rFs*6s-CcSsfk2bX[$5;7c)HhRj0&AT1`KF;.LA29N,M3O;mat&u<8'G:$jIpL9`wEGYGa%j$818N'?qBEU"
"mWaU$.[FF)NpGF)*AQtlx7?D,v$.I_4JB>&LnEB&sOT]sk&fHUiKaU$,_`U$iD$F,Y]+[tvb5PN(99%MbV6es7^0-)E^n)?33KN,],PR,Gj9171#IW%-]#1gp2t[VXM`Gs+TDOsL;H9I"
"7.-1Kl*)FsP=.8>PXU%uO;P^t#iELsGU%ds5s4VbuqG`tl[`_FM6Jf,%SRn3N^1jT((Odt(5-.1whr`HXv,GsmBc>u1j+K_E4n/)[Rx_MhF:Xs@Qoq)3$O48Zqq9de^]/.G)@sLT(>es"
"s[mDsHP*[s=jW*rw8Ns(CwaD)B+USc8n)]+P6<@&J-X'*6fHA&;_,JVF:-wsc)Ha4CP9brXgU>tC(p-i=SMJ4Hg:]j]VX=#G8Tfj>Dw$=ZL)r.djwp$0rDss>0.[tGL?OAl>]*`xbF7<"
"&ug1jbE4xia?5wsvfi%t;#s[%(pht>M9Lsm7T@4toe&x$30OQ,pNfe(1H>b`WJG7/EG$4=`v?'N'^[]i/'W?6SD)qLaWJo5Lptes]iRN'iE$2trNqDsRpP*r?d0R%vf@7N2-At-w[KW#"
"d,Ywb)FD_&K7_ls4U7`sDNWm)qdEI1iKeh0^HtL0M<em0(&7q9'`d'&I-0f1ap0FM@>eM1_G$N'-jo#u't<iq]OMeL)v=_tR+a4u*V`4%aD9x7C`g5TwmCPs=6.J47QrbPA$?Ns@UWP'"
"?V8vHo[vlnZDjjLo;h#vdSO=-Gt9o.M:?v$ss]cr08R[$LV2mJQ517%oE2[$LitTsnAOR..au$'u4e$'8KlMT:2Hv$I&i*.T%n1*p^U;.VF3T'bBCPsjB4:?V:Rkrl:J-r?*aGs:2*(l"
"L<ok&T##U;=%b)<)x1;=QR:`t;J3Ls-]GDs6Y:/_7F]:-mr1etvBGG,PPY)<w2ko,MlQ:?f.jILIlb4%Itmc$Eb8FcWQ'Gs=aYxk=oGL0dWN[tdPehLwHC%4'3n,2.T.FMXO?_tC+0Ms"
"^D9EsP42-)uX2:g&:2as=>?JTU3QeSLC(rr:NIW#11eGVd':ZGq5:v#f.L_tbVELsJn%W7k#o`t.EtTs@U`Zs$mZ8%r6+i$3p4bBB[:`tJ4BuA0:$j1WWudI;.huQwe'btq[.#KAIZL+"
"N%3Z>m&xn,nPN8[US;7X.(X>s:o_F;dfA:7FfG=P1MFDsRGqV$EnL'8X^Tp7e+-%=/dm#u1e?Mt2DWucrY*Ds`_?_t$XkKsoW*F+DFDI1BuAEsuZTYH_?w_<*CgF2_O?Y-ifT]n@QS_<"
"WO_-68Kr-6r/_5MI#eYsVKJn3*V`Hs]X-1a=g?[$aQtBeUtiI18Hw>>NguL'b4fe,B*>Dsh?V]sihoJ+?rcbs6M+Ofgd,107'pHq4K'o/@5kBCX^g_Q^6P#%$NcBcr5*3ALC]&:(LDC/"
"7,jGMSN?`$Lf^X#>8J[oB(+L5;G0%qNon[s9#.vtmJu[/E`=(EVg@ht7<T_E7QY:?K7BPsG+RW%/j4v)SLx4K?uC/%CE2[$t9][s%7x(SM_JQ+qFL#u45GPs&0UW.&?ao?s,rX%4xB<Z"
"w/xi)Mb$`NT&Y0A6'EH_6xJC*[SDC*_<>/?IBh-IFf'K#?Hcb)A+6QO7WrH&6KUF&LgD<&wvrFsnCP&t10tJq#v`qQ(17CWJE7+3&NWDsa`[bMrS@?X0tmCs,<j/M.gi154)YO,qt?w#"
"/E=A&Cp?\?X#Yne(O9,O4rV:D,eU4ds%-2`t9a#YULW$)tOh.Y-2A_5ktj8csdkJer@NYYs9v>&MS9ER%x;[FDri6N'18vYoTa;`s*7DW[,g>nt^fXP'nV:vd/MD[lCUKHsc9d;%eST.1"
"bj@o.g?%<?8AHRVp&f%se[;4&*C.Zlr`w3SMf+IrU(1ia@:;7['v=c><tr?%H*wc$$_f_0f7<dtxFV]JhsJN+#S64&B#2:6rXncVJG;nX,rJ*u=lTP'R:.c2P^SIpDo*%jnmZ:lg^]kJ"
":[u7$mRR&*@)SDsD&ie(9f:`txxoX>%au^tt3r_Ed;3>d)0KX>gZ<(ju%g]ts2)[sPqL_E)OQo[*U-],lRKDs%4XctmbSFMiLF?Kko.GstLJEBM-;`t#@DWBs@RMC>=Dd$+AP53c7Jp@"
"jXC*-o3dv]IYPooR+p#VUU=5B<K#*[U.4vcm074-'5xs)Rq(qLe9I%+5-/*%vkH?#j:mR/FY^Rsn09MsYo?Ks`w'^sKh.Hs]D`eL1%$Fs^2TM@:*di,L`RTbTbuQ%P%sX<:Y`Ouq=t.%"
">&]OdF5TEse[)]J&^;`t^-p;/w9bE&?m9hLK-lrL*iJ+MNbXAu;`3*JC#DHiew4cs]wmDshr#r$Q7-@&2oU$u+*nwFMO`,@#YbD7?p21B3h(6AK@UwbXJen7D[b%tYm_%*W:rqm(Jeb)"
"n?v>>vb]]tg&1?%oQ%_O[Y*48^qjLsH4MP,(SIF)tIZVs8D%CW.7DGs)6E)u,[VksZN%G&eA`;=MIU%u$wW-=/N_%u<=WEcY^BcssV_Sc9vlP'.7V0Ghw`U$GFPA]+S<Lsa`<t[*+^et"
"2fi4uiPMotcsov00$TetSwl#uxE`4p:%bGsB/cJ,NZCX1sC,K#61^EsGZEx#c3g<L$Eestf1bttPH;D%S<nc$GI.B&A3R>&p7j+4Bvf$ulZ&asik1o$[-`]t>Psl$x'uwB5_r`s#4Mo$"
")c5M,ae8c)`MH[FaMbW%'3ph'Nc*GMADA8%D3&M0D'WL0SiefLDZu`s9n$CW'G`'W9(qxtSH9EsS%tX%qahn@$)b%t$*YJ,V/>JC<ZOb`Z:/9&I6GPsX+*DswF*8RGoFmsK`s-C^uj0T"
"l2H;d<B=5-LLqv)JR/jFe;.+MshM7%$R9Eso>-tH4H-lNt+tN1AXK/%dl>Z$$$4d;n1KJh*R-`*lJ[4&F]rw4qeSqtVHF7I29LOAAks'M/V)xLE5fQM4le<*q?'@XRxacMSegt>H'T*X"
"4VFDs$-=i$Detj)bMW'JSs@ICo$#^Xl9f+-C;ng((UgT%P:[UH6cB>&[EFB&9xrFs?N3x#NEs<l2m4>upB<DsXwe[snB7P%/8Ri$%*&K#iZb%tI4d*3];%H,]d/%=UHpu$TUo[XL1J[X"
"_gDqm7uJ5%QiiD@tJ$6MvfD<%R#U'umhXes-HAx$pxpUHgPKUH>t/u$l<NQ,6?9@4^X@C3jx%gL=]^vL3?0HQPWuusjPBUs%]tR.)*)gt[u5Ns8/F(3#irotlvFqtBDOR.<G]PsB0Cjt"
"W:lZGV)7[b+>b%t=w/i<GBJ?#:w&xGb]oDssG8]t/f65)8nP]2;bp,),Zjb;B%>sk]m1cu?H@<Z6q[$u6D'A7S13LufLQENJiQX>Xs%C#SX7LTj(V7%T&2=%Gb,Au56@^=pmkw$E)f?t"
"`F'8I5<Yus9ET=>*i?j0t<petId(Os,qETs>D7h$hpjlt`U:`s2E5g)kF5u:wB^-qM=oCs3LRlT#<(C&s_SP'#63I,gv.qCtn4d2?iKsdPrnY`^B:dOT)):Q5iGe-//6B&A@]MX8n*%="
"kP5.UHx?b?EGgD&>L8Z>vMbo,tqh`jA=ge(xLX,O57$L,wkSP'4TPD&RWvCs5CrFsF%,R,,7EsHHpY=5idB;%.e3i0OgbSdSQ#qno35hh2rStqY._,)Y/3`s&_bjJihP1F3KF/)S]jsC"
"H6v_txCkA@O)D`tKo)OW*(ng,KkxJGV^>XT+ivg,5>LoDr<9E%g<-Ce4Z'7%mh<<a=K,d`Wjic,m?AsHvj%$Ui)%1-TSYG`oth3-'V0erVK[iQat9#u6$*e))$G,V'Q(--9P9sL8#<`t"
"IW'dV/Rd&TC9%_sCvkr)Wk3.(a^0N'8voDsphfrH[1d?&g6a4&QZqI,S&oDsWo,6AEch@&c=Q.1>n>LI>V8N0L3Dh)m_I$`KkFqto[>=3Q:8[$cGttGa])'TVg9M%KlPdIbuUA&CKGrL"
"f*d<I]3iM,/#0Fs&>7RR7Q@Ds'MOReS=E_WFMX]tr,?1K)-[`ssvTsH0eR(t[x@T%lu;$QVY%[s#I^Ms3=G;^&Uq8*r%6mSn_F)]Haj7.A`tn.UroS_@Zo%%A,4vB(JhcJTHig+r`h;A"
".1%+-7cEe-I)Q2TpkbD]7@9njlU/VA0e-L,:A(w#fHjN9[;&A&gmiTC&@5$p*([?&0_)BX;bZk%XR@`t44mP]i_`k5(U?@b*BUCEJ+P%LM-,UVRgK2qeDc-M7k$R9_@qHi-#Kbsq%AWV"
"9jep+/IGDsG>oig]7#t^JAu&%L,ouB$v`0_S%6N'qP,W-vhdnLmX&&$=YQ^t%8oDscKrCs,L7cspFGg)8NxHsRl?o%N5W1']mRvtZ`eP`R*'GsHwV?qZJLJsOf<9[Rj(P-j_#Z%7;xG&"
"fr4Fs<o+f$,J^]tORmvtS?Fq$,2FDs<%,f$4)&H&XG4FsiZqV%r4_]tFG[stL45n$eD(V$FE3Y#FN[]tp4#.:/v/,iwMaDsCc]]tYhP^t#59Y%EpK_t%rETsS@Ae$7l^]to>qbts+J]$"
">iFDsZxHkt)wUg$0JFDs7>wKsKhh_$=QKA#rN'e;cs;Q/sfVP'rF^Xs9O5F)i.u`2r--@%VDC3YUrPC%ULf.@&&(LRK(5U+DY*)YuVRl,?-9c)hrp2@c%P)-uTZ,DOH]f,rD;,6*d@:."
"G(j8DJ$Bx?cj[n,suY)I`Y*I&i'%CtAK[1K:uB[/V.^q-@bCnfCwu/1SRBRH:MtUG`:X2-qUdG`e&8d,/b;te:Y3EJlx:(E@.I:E2?uu?3.(c?PBpK1M_&NF[%`HsU13h[N8#731uoZU"
"dWC_%gHk0TDL=u5`J6AI<5A^s)uUnSn)GnnAdpm9]ahtL:xE)%3mwn$f60KHpcpj+<H@5DjNE/)M25Nf=<k(S-,,.%cjelL6E+(L.AK[TrExc`be_3-Yqj;mP*/1(kap8&gUwt>i(dR/"
"m.6cs<_E=#I&&I_Pli5$>dJ_t+G;[$@-4<Vb&qM%:9)lYn,Q%AYr1^FV$X$Lxw>Vu@[.^t<6rcYj7kUZV8+ttun@kdsnoFs)3Cp?IF)Q8p#.<HSk8:-f0:XVwFaO%RYBOG%u6TsYtYmJ"
"8%lq=t*GxsZ2pFsV=o@lp=(^s'L^Ms[6Ncsg783.hmpXsKw+$*,ij@&b[?-`OX`=-wvbG/sV9Bu'IEN9dnH>m1xH9Mw])aM.L2h)T[VC,ILL#tk$]a3d26CW/)hN,gEX[4'r[b2=:=9&"
"o.;^s&<2-))qJn3d%2csm^SP'%MPm@F%p^sN5h,)Sp%Y-Os>R*49j4mv[Z^r_,g1f<#8>->JUV%TR9wsTHmr76_jvU)jGjWVh*+2AApFsNsDK,A/bOsJVIQ8s[):H+r4&[_)Il1Nqm$F"
"8s$@'D6JN,X;rHioZSv0Ja5H--5fI,$6a>&xJo)=jB^A&TDiNXoopFs$aj$'b2BN,jA$-)W>9X1/ccD.JhJD&RT+#I$dGfAb(uxY.poA,S-^pIX^U:&>2gPsqdSiL@HXH3[Z/=GHX-L,"
"<,H8A(JX8/T^JjU+h`A,s/(emIqR]cv4>5T0CYs/BVCK:bTs:[e0IE.s#lk6ZY+_J^sHJihRgY%UWx8&Q0610c>oDs-V_itObNo%MY8vY;`/PsXfP^tmK3RguZYT#3:)(3?TL's*sg'k"
"R4r(5#Y;D5PiNXkk.7r,BM+6D`6IQDCgw/[Ep<K[m$`P'KVEPsNiNneuR,K#h%KEskJYO&am7+2`],Fj&VEdVW9xO&?gXO&Y]64&R#vN%K_04?344[lRfBu,+2Y(?m?EIhH(:o>mbst,"
"hw][sNb&u$%Rkw`oG@#Qqb0hL-k1k'`:'wOb+aU$<%_X#[9@:$gPhJsUA.[s#<lh'0%LR[#MlQ%,[+i$tF.gUnL9iUNBshFxSc,-Qqp*D)Y=+D2KR8[.AFm,'#E[XY(NL'2fIGTAj)1%"
"b*aw6xT0-2Y$_ut^(b7MKd2YK9FN9%?aS>XlO5ofdYLdd,2i,]-&$/)Zu@$`V,R68&DFeq4g0Ar@rv3S&M%F)Q2*Dslr#N%r/Mg@m^QXl&>olft9P]sv;KGrNB#N%M_tJ@2A@-%QUK$A"
",4(QA#=vSC>bg8@P-f3HglNttI1Y>%>,O=PH=`DF(kGp@lvRn,8O@,`b__3-#9w;WIi8T+f_04K1uQ6N5FCutD:l^%?>X2Tt.oJQl4'ut8f?t-D/EPsdV7`s?d*'*Qeqv3lkvg3Q8Oal"
"72JplO+XN18(HEj7(K0gcpL`su)hc)TGw@%G+t#Ah'&E%T;LQ`9[Yftj+mv$VZ>Dsg]8I]_CKcsm'0Eu:bk-tcu2N'UPT]sf_)ko;M5FsqM8Hoc?$-)`nk7*:YJ[O3/7HI1xND%[Hu,H"
"8p_9/G9>)WtW=,-vuZt(O>[t(5%^#%H3>Ds/HkHr8a3PdW(dC%PSC_:7a-bs.7?J[SZfH%a7%):S^m0%q$6N'+Po6[O=1k^lKg9bm^nOdl6@IcK__nspX)O%:rNutRNpI(,KCv>B#cV7"
"Uj@=36OvFs65uq$p$S7-@624D_)5I-hB4hb>#*O%3AubgQYuGMF<_1<&?]7.LW@n$T*j?UD:r(b6h&sUDW4o7$@+DhFT4L8[-GC*vMNRNbNIo^)b2/Q[BjP'a7wDh+962jU/=]kl[N]s"
"[snCiXQeSsK/,+)sX#:7qI4o7?9[%%EBgctiChw,4fg]tw$l@+rVxFs^Ken7YbPFsSoc/*$%4btS>hW.V<k.p%]sY.[>D]s3)IW%E,q[2F>Mbt?/kL%C$0d;_C*7@akp@%htmc`Ep?bZ"
"[>B.C]`e6DdoubDT$]>DN't-C8%./[of^^be6CPssouCsPFxw4>f=hkP$.oskXf6euuZ4&UEAo%@9rP8hg,[sX6<wtQWwq-itk>%&wXhK::dbi`#,^t$8jg:^V<p[O23asmt$@#+T$9&"
"/-[Rs*x(u#%mJA#msCH&4vc9O&n-@%@WOQ,F7GDsPDD[$1lqI1k(iK$f4KDs+aun$(F9Es<uJVqZ%]QIAV(w#:s[g)>5a`sa72asOKQWES+gk,lC7:^6Dqxt12WE,A_2Z]Mwhx7uM&&t"
"@<iBsa$6;uk[8*%7PG_tT4%Rn$?B2T@Y&iBsHJ[OxqB@I,Bhupq:HVsa#.vtOFmX/_DLOr9=[qpjNQDsxP=R*VqSCWUg@KqYhkk/l+a]sPV7`s3F*x#'p0%=`/vv$M*;4]DXl3]Xfrw$"
"[GKF`aa'L1dfHdt2P<j0QpGF)?G)7@5IA;g@UD^4*/A$52e='*H)M8]Q_D@&Q;uVHVSe1-nLd4&@O74&8*>mJ1@iQW(%n`j>6a,]/375X?]7lg@ZJ^4#DL9hm6<cgMGx@k6Q3<on''Ms"
"%L5UZAdQB[lU3xF[S@qCwRnC%<'H;Z](&ft[M'RDTYH;7kp1T%V8R>choV7X=YE5o*1m0hgEC?%*PCUHI6)dM@[F/-TgDJQZJ%GsKNiJLN_o3F#A4(NTuVb;s8xgo(%S)Mb-SL,U_EF,"
"3U:L,3wQqLPv2*MQ%6wLFC@ktS>wKs#E9EsY&KA#r/Jqdpf&At&l-^tc)HOEQmk,-JXuqUYL74]^TXqcI1l2-GgR5GZ)r_E/6YnI]iYQ,]uJVqs_]QI:A(w#9p[g)2,EDsUCrH,W/A^C"
"^QU,fCZnw.kBf%:b@;AC'sMPBNJ4xs>e(7@otAetkcA*%@pJ_t+Bo^fN<hk$wjP^toa_/s%],K#bXpDjR&VG^,#TasPQoFs1$Zo)RwZ4&M41bt))^:%TR0m$bjxCT7P[vtcqh]t-wT8@"
"nnT(@Qk/:%TM$Jb<4k.%-'6:b;G9l8/U,J,<jq*MAIw9M/-SnN%FieDu5-^4WRvXGP*EGsY>L[FoFx*D144Rem,&FsaT]?1hAo[F&0evt;lj3fUFI4r#YA:^i5tpsW1@K%S7Ex;K+ns$"
"x,9/=bmiU0Sl84fmKn]t[u/jB5g.q=fsNL'nLX<aupJqdk/nvtk(M7q$mP^tEh7csi&1H,Z<Ih@c]E$5d/mH,ifMpBJDYU$xLBB#8U-v)1=NL^Qv?#5kr4as.s0-)c4;37fUn>lWE.1("
"Wc<Lg;Qn'4<x+$*4hj@&ZZR`tw+hh0J,7=-d=Y'?^EhB>Pa19&p6tG,;E8csU%)$,o%iC&p@i,2;_#Ns@puU?,XSU?rwW0p)*+5Al(<T%thg9&H/4dDF$U%JvVoPEGEv_tx+oFs4>S1Z"
"&Er0-VDTfI`?Y7.JSRO&,+5`tVH1MkHUCRs22(:uNw6N'8l.=(?O,9M%;[1K=;EwZUSxls6d[<&0C`O9w1@['=%l0g>G?O,&]/7*2Zc'&80VlsmWSN'1AQL'2E2ZoHr'K#6cmh'Q1Oxs"
"CB0M%R1)*Jh-9pIa8l,-:&`>TwT*AcP`M^T6R@OA0$6RoJRo<%/-vR72$d9&n*W5K:X,oLjdhsL&AmB,4XrFsLF5/)?t/-)XU.)tg^SP'QYk=m=*7[bnhWt(DA-/.`];vs6)c=&CT(H,"
"nB*%=@q@KqPIWDsxb_/oi=(?#Bg__*[==H&TU`p)W@9w.5rOsIE5l=7Dkm]tku.^4HLZU$Psvut#<ee$s`UI1til2^fhg@4x2HJ2gNO(3EWj9dhQm+Nn,btGh4-nA?\?S[s5]b,::fv5/"
"/A$-)Cnk7*/x@_WBRrn%c);`*Fl@=%1=]]t554J,#[:#t6?SgLq0#ko*M5FsXZbhQ^)N**;a&P/Z4qFs0GXiLg$P9qEgZG&pR>:$?o0@I?xK<Hm$Vb;'+PB&)I=hHuL6ntp9.4V.lZ8%"
"(=f0H)'@DsRP&Ps?gP^tKwl)IvM&K#(,fwOqw1M,4njc3@fvAJHDT=%Q%g8V6?=O,7?kP'Z+mI1LP](M8Ni+ZwZW_/NrV8Il*C<.o(x2KhM?p.8FUP')hI_/51jJswW^kJ&cO(t3WU&k"
"8W<<oHIN7,8K'$*5*Z:?)LiAtU=?JT-?(<-xWVN,NAaeSY?,Tn^s^b*_](E,<3q*MVT#B&0:Js(xD;=-)`<C4(R'b=$s4asU^0-)a$p)7NdqG,+5WiL2Rias2u.-)R;JXu4)V;&>@p-U"
"*ob<mRw/G,ok4:M8gpHsx$e=.=fogM8./pAW]MqLMgpHsd/Z`-Ys9:)K0JX:;&l?k7(xof>&0X:+rM%uLJeA-k@rX/Z^LL,@6Uwk@U4Xe,/Uo%u-j@&GEX[4gSA.(1S`VecP):)a;-8f"
"lV_VeF.3@'0i6cs-lQX.Bkp)3aji2%tXi2L'B5esQlq@-342iLINfYM:GK;M$%kYHYl@[sTj:H,b6AHUsThBs`1Jnns:l0Luf+l/:RZnAD7fT%pP/-)(0RN'E=^U#<L>_s7ckhgnPTas"
"k/0Js1aCpj0oHC)Z0.<t)g-66,_R/7`oi,)A*;89SQK[DQj31'L6X0[`Nq&tK73.(a.=RIvJniLJdURdA#_xbPrr5$<nD10'F>R,tIUV%j*jO,hg0UJFrh5IJx'K#eX[U?Lw^st0L]]Z"
"UgJPshoZ5h+xr$'Q0T=>7p)jL3G6?ZQ/RbWf&Lh$@;GL%6YxP85ENb$pcA?KNrY>%QL12XY6gu$^VdqWa^jl%$&uJqVTg3o,O31'I5/S_`O3GsoTQ%%oc&p[NVx4`&_C3_wx[PT5xm8%"
"ZMCm`7NA]tU[b;dr,=d$TtWMaV0'Gsvgh$jHD]6ceIG`tQG#o)<6[,rhU&et=asGsV]3gLcvN8T$D]tdDh8h$a?/@F9b9,r=tAM%H3mdA&5ZS5[::mA]R:*-V+b2BlH%n,U+Qho)6hv`"
"V7G`tK5)u69uQ'MQ*+5tJGPeiu%7,VX_7mS8_,9<fBdMO2=[rn0m.vt&32dVh)rk,fO=$%pwRh$PWCn>Oh:`t.9U7Bi]cV7bf/st*nG5NeuFsLQK?#YP:nKm:`C_LWdKJMKe@RWh^#*^"
"K,R68*]i'WR6Tu$mZC.Cf9u-CsoR_<x;H#5/7D<@$cvIsxt74&`g=djNgbWs3B=lL7prqsUL]Jj+l^Jj=K5,cN$'Gs:MTW$gbm]tj?rM9E:/Zsl'*fC8wCp.9A6Z;gC=R*L*UK%Jngk?"
"J<IcT<f7N'(J*8n,,[L'-cMs(>gKbVc%cps0^@i):X6&X4&U^_+_Rv^#U2GaR9?5?\?en<Z'Tv;TWW6ZAD[r*-SU-Ma*TB4?'<0[JTWNu?.w(xCcNX%eu1%%MHL'_GnxWs_JgVaTTGH/)"
"UDl`$i>oq)u1Og<BDpG=^OYTcdh1qc2r^P'r[RxlVE9T.wd1`tn#%a$V/lw=*/+DsUs5Wd*x-_W2xX]8V6NO,FFP&tC?JpeopO_Nt+'1TKZgxVj<$_sNhrtGO1a;Xxh2j(bfjD39B5T["
"[<NZsRdgkJ'?S9&,-sAkZ#Dj0.'188of]>&xRjCPatQYmgD#W-r_1I$(^qpL.dRJ9bEJSof+)X-d^3I$>ZJkL20PV5KKGkoXcV.1cVAM,D9`*i4P%S%TA'2MUcrJB$pxBj;:&dsTf0`t"
"+aArGmJ[*r@O4U%s`A;&0Q]WnY3C^sx:Ifh0&:_aItZhK2%deDpU3LsOV9PT>09I`%4r3-l_mk$&H.%%Hr/;'5`$#QThnR7XBhL%ZKvmSarI9&.Ig_oe<P]sJ#>]kr31R[>ek/:>CI_k"
"Vr&%]E(9M_uibx_&_J,`Vs%d,@L8(Wc08)aw'r3-_KRnRWL]ZKExD:QSnu9Q5lo#LCXxkt&BJ>K8&<`tb<pEs2NYX6FwiP'EUbF,^hJH,R/aXqt*wA&_gmK,bM0P,N>7hGPo(w##vP^t"
")5GPs3YnE`;TFb`_*h0q'W:(W6OH[Fp6UvtA]=N'9(6BsXGI0q]^'Ofuoa%t$5,FsRwZu2mfK`td+`Zs=EQDs/)^P]Uloo_uo2:m2ud]@kQ&+iD%$1@5Xd9mi:=[`tMdNBbY0DG8wvAI"
"lsf6exxa%t3Di'W-#I4A?O+4Ah1bvL*^kOscZDD%:Pv3W.g-W%79VP'HP>P,(Lwn%%spk&=9M[]DDY#uYU^MsVl?o%XukF)H+L4?lc&:HNpZ`tuvM+4F$E_tCx#Vs^.Pf$JO_]t$BZc$"
"rQY_A49SDsMSJe$lu4as]o/EW8`=9W3[p_*qca%tb93G7[2E1B3[G.CU>>j0Z8w^sXjft>e8PX>lhY]t,*%hC:1$`#IT*u#nPVP'r]q(E_`%9&Y6I9i%W5Rqe<ZpQW%+]s[Bhk/OPJS_"
"m@<i0ltZR%gX#M,A&.G&G$jP'v=(5s*KgaqS;:1'G5:1',^L?>tAF6%v/DR@_C*Z$sd02sY1JH,vHGr)keBMsti%([,ZtR.h@FhgE+6lLHV<rpt%b9D-#p+*$xMX>X<xc`v3r3-T[%d`"
"l%FMNjLX7q'*;gt<BjOs`mOvK6)<`teaPFs@:haSLr&atZ`LA#l6jd%x:[R%-VRN'>0c-IJ7uW#6JUO&&+jsscHw;u]mPbD_^SO&n.7N'a$s.L(TBC,m[f7J2-E%t9#9i'p'^*ukL3hs"
"<b4f(fM,3uXJM4%rZ?K,SaVh,`&^]tXXKutaE%p$iV_7%Z>0V$W,]]tvbUb;(:Y3h#ZsDsWux]tA924%'*3jL]ZC+t]FuQs'J2Osv_)]+H9=at,l%Hs8vpO/8;+it*<Cjt3GZ[+S&(Js"
"UK?ct6gP^tFSqFsE0)/;)mD=7s7W$kQRtJ,/emDs0BW;3Cs?^4$,@N4&6`2pLSl12PR?Fs]rLA/bKKsmVT.gL>'2hLaPC:UwJvmSt=7.(+E.1((=4ILLHnaN<Un6ItBuCse+g1fMfS+*"
"xSGOJW1JvUVET:e?n/KXhf)-GG08@%G+GPWTbXOSCQr<&DN;R[:856u-cSN'aCF=q+KG#qYp;`s%Thv#^Z=E,,R2#uq.;o%sB#v$G;EDsvdFDsu,K19LQ=Is8mR-%5m>R,tXW.r^oMl^"
"-K)%+r^TBF36IPs(-5csp10EN>T2br$QV_&iQ6R*>0xlLnIpgSP;3-)9]L6a9?P<.N(OUC0Z]3/-RnDs+C$A'Bs6G,quYD4msPDOhxY<-+h#Z%^nPI,p,#7I<>.D&tlNDsB+Uph)n#^$"
"2J$P&PMHd2,`G2`PseF2YF+UC8a+?&J,*DsesB,)jt.O4Dp'l/cQ/k94[BOAs,f<&bDX83=.TgLjCEi2#T1:?MlP<-Yux(%h=5/)+%/-)O87IL.N(0M`V6.(`;rHi%JTwQhq_A,>nlxH"
"__#MqTi24<p+wDE4,EDsVXpFssxKvDHV&k,X+7:^>;U]t>]<=&J$E?]h]YT:'4NRIQ=v(%RhcehM#JDs%?3xF$b,NsIb@-%g#>pOZd:jp'vII&FehM,Fl?E,ptA^4^p>FqSm1Zsv3>3%"
"D)uJ_)hwN,V9qD3l$Q<&4i.gs&UTP'h[J+2eR)D,8<&x$,,W:&CZr5IC>a;&nSHE/t>,tnc0mB&]-JC,#?KdsApmq45I'I,Taqi(Cfun%TI:=&aC$u#VSR:&m^q<1Y,dofUb'?-<V94/"
"7[+?6s7W$kn8ka%SnHLI'B5es)dv)<N3vk&(j^8I/fO=9?/qXGn]nk&C;Eutfox]Xxu`-Irp(x+Qd%dQW%+]s[&ln.8fwJ1_v$utlM'UsqH)M,8jGND@DbKP=E&ps6Sow=EV^@4+VUuR"
"O*w$CGl8q%uh64&BwxaS<l64&vQ0EsE[X_NZp+%#p3>>#G&w%+ee18.V8Tc;#Cbr?VKLD*(;WY5g+v%=pLk1g3HXxkQeBfq,,WPAf&LPJCNmV6]&xi94EvlAGWsrQ,gNS[MJZ2'$BLGV"
"REBAb+[`ci-J&Z5nF`A=7<6p@-fxcMxgl]XFRRG`U@85o[]<a*q9.T.`:Ev>HJoJC(=;vPx;3sZP'=)aO.A5ov]f]t=RWp%nt:W-I6e298v<gC8t`;QtBgDWC[aDao+oSe;s5vl'Yms$"
"xa<Q/@Y.E3V3Zs62_Xv>XO?aE$v)NK^o_2TDFR,`hMDvc*@mJhR18pnufX>u]l8B+'qEQ/BfRa3Tw,<6&(f#>Zb)BF&&3NKVGl5S)';^XAc,Q]W6=d`'+?jgN]d;man=mos/3dr5fL?#"
"JBPN'oq:<-8sGK1I1=B4$J7B=>fX(1vY6pIb;l,--PTk$q3<L^f4U&t$]SP'5V9>&Mg:^43h#kA_)E**pjp*M+H7%%rs3cs*S.^t%lZ$u2ODksdrZ9.GUkVL.V<m-97YU&F$E_tganh$"
"SxxtYjos<&`AR9Ca1'wOlWJX>Yr]h9iH=wl$8g=#Pt.+2_P5F%Hj7hqxC97.HR1mfrn-XS`SvSdQPf&h=4-jgX_k`,5+0Z5+L('-j)Rs]7$xg,G4DqK<tgH+W2n8^]5CcsX)Oe^P`I/)"
"v*mFsJ[)]J%^;`t@.)]JgRl(kdkXWVc7W*i8D7-Ic,5hqLm9hL]3urL/&<*M`KInX)5+^XeXJe-ilk5%I:=iFJYiJU'2Qg,l2.@`fvU]Yrl-GVF877eRT^[42QB)aWjic,*&g(mMj%V-"
"7t)Q&?HvNsrq7Is8+(Q&U,[tdTS[FZB5DV[sJFx_s/vYcY9Qb,02Y,;IvI(-/'hMW.%[g,1PqOEC'A@%Jun@ejCY3%-'incE/KEsZ(1WL9,<`tNP0WL8=7DbJ8dATTqQhK-;jAKP?(mt"
"8YkV.6)<`t`[qV%OaU0-&A$'T$Lk%YA:(--8]A:&4bJ$lJjBTRDr8j.&7YU&OitW6Sw?pRLe=j,S?U#=AGgN,&Y>F;W>tDsB-jO%OQ3P6-I<noHN4a^5eKM%dJ0q5;#C7%b'nL<&cPDs"
"JT@DsM4PN%dMl>%L_AjF41^et1J^'%r@tXVTH<,3N;F>H)8wfr%A^]tC@])EuNWebXZBcs2V,C`N2mP'CR+-Q,v$(%xMXRDjeI$TK@fm+1PSPYSQoS&KjJeiN,#bm9e7`t/@d?A^9&+-"
"2xlTsE^&:Q#l$q$%ijOAX_g[O3CL?%=,vHsoKCkf>;1k^XXH`tCYrtH1rbU]Z_fiXolmN%.I@?U6cR%_x:aFsoQvg$39<Es4/,+)+8nvf8v?1KPe^W9<g4A%K6WEl<3e%%9;JfQ2Y?<I"
".v%ht+7nx21aVOs#<FI:=TWDsUG4A%8%`9MPTE2F5Wv_tmOkvK@hN]cQl.?Ue4ogugRa'TF'jP')DUQ&,qB@Q5Duq$gTBG,:/^U$C3oA&7elB&P_IC,j[uDsB1Dh)wOsob?Ai3]5nZ]t"
"pYnR7LAS19_A]]t^Ji*Mau0Q2D2oE/Vq+vcLW=5-=>1`tcR?HDmN2+NR0aS;0l88MfI4ReGt8uGIAfW$g%pfhE7Y`sni[stKn,Qo+To]t,fY]tkDXQ%#45UZ8tZg^uW`C^I`wYcOT)c,"
"*-f>eR<tL'>H1T%v#N'sQWU7@#%)aEHrvhF7JPh^AvTVcu?I4-GE^aqB:N9%L=S6>D1WWslDrk>u@E)WCOeG`w_^t(7;<0%,TC)?Jxm8%VS6jaDIoptH-dsaKa'WHHdLs?s/=o,S^=)a"
"-L74-Vo]f))F<ADAY9`t5dt`o5Kd0%XOxxt_2/htr^W0p/S]#L]bV?%bKCdHN>9'%^1lVV=]*otRsqjX06ep+5t92p?2l5-XhGO8AU;O,K]urIKX9jKEIRYLc^V48l`1C&bX>F;o[eW$"
"EINAbwdd0^eU*Z$mFY.r`f2N9>&od)kea4U9YrI(Dh`KrTcdaS5vnO^@H(IY=/hL9`.'4EYZx@%N6>8edpeC%ZrpL0M':9M;AHoA,;X8[?;=2-`S'^<7,r;?lTigtOX;$Sk3wktZSbRI"
"@a9OJ7O2,>EvC`t=8hd2@8NB4.5wE3.Aj^46Huq?G84e)/Zs>&d=:o.MXqjtCboPsZseF2IP39u_[04fNH?hg0=FE&cNo+/28^b*m9t7CNsH#%e#n&JTIApIK_Bk,:SQBT0tnS&[)S0M"
"Pdr/%E;'I_=eF'*HN0Q327pd))sQ;dQa)c,6=ck81T)$%)_+i$$nL#ulBGG,MC*i44Wgb2&;2Q&k^`jpgkLO%Kw,1@OZwcTRWI[<NE_s-x=S6ceIG`tv'@_tK1iZs<Xp&t=oL&tkLUQ&"
"_hM$e:UJK:sbrYlZWtDskYYU$(P877%>,#u7Rn`$LL>ft[5gEsoqO+)0:;_t#ko9Z(^(N'MRY'a>[cjBaRr(F7Q:11MrQO&$<4Y#hWK<%>^A@k=:Mwk)`qn%(Mg]t+PC_tJBIR7ju$N'"
"1(1CEPu*v`fg=_j^)2<%IcvW<Fc?s$:v*vBs,od)f@WPo&D)Q&CRIOADDXp[@'QNsB4=B*iJNDs#/_nI7m7DX<L;AFvC27nB]2l&TitE&0UPI,1`od)@kE4S7uJ5%&D*AA,PGS&Ic&Es"
"^goQ&`cWR,k/=_j+%)$CI+4rJIOhf;f?oq-2gWQ&0xt'E.1M?#G3O?15N7csHOI%``40GsbK/e%HT:`t8FUV&f'S_N2QInn[_a(3NGSDsIT@Ds*g7=%+9F=lFZGv$+2$S%k3-a$HTfkR"
"s[wH%%W]xtml_b;:[nc`aAOu#=:-#uh:Oi$9oEt$g>a^Jk<^5JOkKk,fgn.Gd(J+M<SA2`I)`ZsFBQ@+aE^hUt5@Psp&5k,j[)6S;Q93IO;(@ea@gS&J=ZS&2Z[3%P0ec$Hb8FcXZBcs"
"6OBTIpuo*>*kbU&w7g]t.t4UsCukF)*ifLB`dxf$idXOkvFi_MdUBj(d7fh9)LU:oX9Nj&A&0^tf`/LRw+&[s-%WWsxHChL^uc,cBvDd&8?[S&:mRDsQ&$S.S8HF%>W]7.gV`P'Zh?I2"
"MQtrHK#.T@p&xn,m56I2;j3[nW^QJV6h,k&3jxxtdB5csM5&Q&@#'B%]jL^$0?47.<D2.h&V@D&Oaqi(U76^tH@v#tZ[JR,:Mg;*X;'j&S0M_ED`(Um4Hm3px,_<%^.6cs;v7HVPZ92q"
"H+.vT]xR[spu#VsRcf^+@)1a<R]+)-kw'^sHUiGsbM-9I>i;#2%0wCssdd_scQ0.(E1hgCl]>^=k3['^PWQ#uj+xi)T&Xb;vF.N'+,?hgux4vcMEQb,54ZJ=UUlS&JLkiF_Y,O,(BS[s"
"tUOJ,;?sFs;CLv#R<.8>OXU%ufjELs3Y`qOe[:F;Hq^T8baM^O%)WhBUs802,IWP&Pw=i&1Iv;?Ppg-2%K$Un$m'Us9fox$m9vZ$Eb5^sl03Dst)c>#SAVZsCI4F^%/Qk&.F>bWc_#=P"
"jHga&jW/ZGm33+3R_7Q&Oard&hc1+Im-)K#Km<^tE+%2tg2]T%G+J9%HP&fC1g/@F7r$K#fC[S&TVWP&=5]S&/7*P&;@ww#bu)uH,ePJ,2`$w##&CKGcfI&Xto^f,)[)#23piOs`;p#G"
"L4E`sj^efL^x*Lsmp2DsJFo&addU,)s&&Jh142$#6sXf_Xq)#GY'^uc*F7J_8'lf(D+mP/`N9GVBUd(jiTF>>V$A/16(crZMk-Gi:kQM9k=RPJE%m-$,%Hig%30F%PS-F%YVl-$2W^c;"
"$o9AO=cl-$ehPI#T,>>#gw1G#[95b#O$N%$=^'+$L+W%$c/ED#s-pE#X&mlLW6W?#)5UhL4C3mLlQ-F#q1_-M6oxr6hLMZ?1qbV?I3s-$6Cn-$;Rn-$YVl-$.+n-$='TF%%(/F%aV[k4"
"jH<J:^R6&>.UOOLKwno%>+/;?\?Dx_ssW:MBIXE_&Ok+kbsNs-$Fhbw'C^_w''p<MB#bBQBbV_c2,I5R*>*;R*Afq-$Eqn-$f$i--O]QF%g@.F%bU_l8D?&v#E,gER-?,F%2Z:5&*G:;$"
"ConLpxJSS.T=PAG9<2F%8M]9M=%uFrs>Sh;2:k%XlrarZtl.>YMU.F%o[7F%owO6#-Qg2#Q^#3#4V@%#/`ewuv22,#4>r$#-[2uLoH7+Nq#*c#cq-A-9*OR/]gAE#<Ze[#B%`B>/oqV$"
"&4mi9Su[fLDhQM9;njEI$9^c;w&0F%23X]F3anQs8wrjt4V*;?h;Nq-Vi*Fe(`&FR&h:/$102/$^U@%#.l1p.ah4)*bq]wKZaX-ZxNl.$;Fai09*',29dr.Cr,J)=BR0wp%`XxX?&1F%"
"Io:-vH*^f1%rix=a,hi'h*J*4rgR:v^$1fq.;bE[wTkl&(##N1@'`QsM_Rc;#PP8.V?)58L#`A,hxep8X`8AONV'F.?DGDEwck-$'xpiB>v%:rwO(]$eZ[D$fF>F#q@uu#d7YY#c72mL"
"9-LE#Z72mL7-ha#6f9_#)N(Z#EjD<#DDqhL]NPA#u93jLouL^#<eg<M:&`1.EkUxL6OEmLtEQ^$:[G1MU>6Yujd]F%/:LB#HmvC#&+,)MF($Y#FJX,M@eP8.6ke)$&`'B#dT4oLDSQ)M"
"tt]+MmN>gL+klgL3FfqLl'ju7LNO&#1M#<-m3S>-ESgc.%+/5#s5T;-f5T;-b5T;-=5T;-=u)+2vkY+#tG24#3T6(##[WjLW*E.N67o(#AiR%#OVjY.a#n(#H?:@-m5T;-+3kB-%N#<-"
"^5T;-p5T;-x5T;-o5T;-x_&g1oJ24#vQi,#,XE1#p-dTMuC._#aARm/RJPb#0^Aa#vN#<-c)m<-6t:T.Z<cY#K_BK-dgG<-.O#<-16T;-Y5T;-8N.U.C);G#H4`T.n^B^#Js:T.T9r($"
"&M#<-B7T;-T5S>-XN#<-:O#<-*YCa32?j`#UY:Z#%j(c#BWCv#D:7[#834Z/bBSt#6@k]#N=T/$txa5v05&vLv_k3NY&9gL:$;B#_a(*MGkw&M5m<s#OsFu#]:v%cg&./$0(1hLXp$tL"
"OC8#M`&0,MI9T;-C_Nb.?6.[#8:gW4B;r$#$####`P?(#IQQ`Ehpf.#](ofLX=?uu[UXgLO@-##UxefL^XQ##^:4gL?RGgL1)JT&rCRFHR`P33DBYf:lEvsBIV&oDBSJY>-ggKF_k(MF"
"i9<MBqP'H3%<8uBvwE+HM28(#F78/G0t#HHen72L2g2I$slRfLrqJfLw-gfL(xSfL9jG<--lG<-+Gg;-*Gg;-$Gg;-)Gg;-0:@m/9l:$#`EX&#cSJ$2J]&*#w6D,#AB+.#<V5pM#)[xk"
"&VO&#/Z#<-riMa-t&TF%w5W?gQG:;$oltY#d*wl/Fn1^#TY#<-XY#<-VY#<-]Y#<-k+AT%XK_Y#^B<G;K1&&4QBMP8]p.29V6Q(sj&3`sn>j@trVJxtvo+Yu/:aP&>n82'B0pi'FHPJ("
"Ja1,)N#ic)R;ID*VS*&+Zla]+_.B>,cF#v,g_YV-kw:8.s^320&60/13D@A4M0258If)Sn7u8;64A>5/2YWY5QHil86MYP/$[ro..Zgf145DD365))3A+=>5GhPS7P'687sBtPB4YrXU"
"Ox9s._FpS/&O6p/26/j1jHeJ2hNE,3p#^D4o,#a4uV:#6FF+W7^&lEI[=9kX)T/D#Pbvu#jSMY56v=;$AKCG)h*.5/&OJ_&;@.5/>uYcaAJ%a+d]':)B?EX(b4ij(`Olr-#I(4+&qKM'"
"NwhP0C7dca3he6/dxK'#X4+gLJKI*NN%T,MaOH>#2+,##D?xu-wYB%MVr/-#;Cg;-YjVQ0uXM4#)@S5#QlRfL+q:HMj7'vL,6=RO[OQ>#io,tLnbC1P1N4L#EA;=-^frt-s$krL>@W$#"
"tw9hL+O2/#):P>#D<G##0(lA#2rY<-=58_3Sg;<#WsM<#[)a<#`5s<#eJJX#Y)AC#*AY>#.Ml>#2Y(?#cIFgLEa5oLYBad*0k&786W6X1IH<3t2)S&#.U]F-*6)=-0%Uo-V1c'&V@K&#"
"RGJ#$i0$B#VK3L#Ni68%weT#$ZW3L#T=NP&#x5Z$l54L#t7-5/CNm3+xGP`<-k[[Huqdi0SJPlSMt](WP;Z`3L/9L#4gf4]f7W]4g(:L#BRS%b_+3?6t3;ciR-<kF)qj-$I)###UH3L#"
"XV,eQOGYY#YT3L#R135&xnp>$^a3L#NEcu>1%Pk+a*8SRB'q=Y](8>5^c9L#?7W(aX]Q>6sTH#$u9-T/-Vd'&Mg?S@^^dA#^RK&#.&1#?21XMCF:pV.pT(@0A+*s@4I_>$gRm;-h9I:M"
"k<$##Gl[+ME)^fLCp3<#=)c?-3(m<-LKn*.JWajLZE<,#Umh3=bL[?T>i2q`9#Ls-m=nr8BiS^He3hiBx##/#u$2eG,Ym*NZQ^oLX]Z##It(F75ok3+U)###<H$)*O1*^ui^@e*VK3L#"
".Nc'&gJ>;$,/m/C.i@H#PJoP#2;/X#2UR[#n8-_#POPb#Wesh#1nWp#Yi4:$t7a%$r3^.$CI*5$t<@<$msnD$F&SL$=iFq$l])[$n3pe$xx.p$NSvv$T*i$%heA*%%Jq/%98fP%Kj#;%"
"cgw@%(euF%XW7H%qJOI%G*[o%iZn[%23`c%rSik%8<Bq%Jtqv%^XJ&&q=$,&.#S1&Ba,7&TB[<&h'5B&EH>J&j63l&*PxT&5X]]&pxeh&N;2o&TBns&pexx&5=j)'0xB/'@<g2'FAK:'"
"]0aD'W*<h'(7CR'KLfX'=Hcb'4mlj'gY,r'Dcg#([mL((WOZG(YP%1(<hH4(h?:;(5U]A(0/OE(@TYJ(Ng&Q(YKUV(i0/](h5G+)e()o(F&'u(9Uox(rSn%)rY1G)2[Q0)5f75)8ps9)"
"#or<)O<(?)x`3A)J.=I)FMc##X</=#No#;#XIm;#2qha3_b;<#JHKU#;TS=#TWt&##gv$vDSFv-M+JnLMWH'vl>=K2kK;4#I313v+AC3v85Oxuvjv9/<T'#v#0xfL[Js*Mqcm)MV46Y#"
"Z6>)MS>4<#M3=&#LZ9#vaeZ(#b4u(.q%AnLGa-+#/I5+#KRHx/B1l6#G<(7#o$c41n*,##<W*9#F*Fxu$bCH-atY<-h%?D-h)nA/m;:3v:u[fL1p)*Mb)s7#h6K$.K0SnLoB^nLv'^fL"
"05O'M05O'M6J<mLGM6'vUmx702TY'v3l'+vT#N.-UNZ9M<Rc-6I#a-6,)]9Mm^o(<HdLG)EsoW_YxW'AgFC;-<r6q`;'&s--IHqD02o-6,4HJ`/-Fm'aa=wTEE>G2>ufJil,TS%[UhpS"
"j3v[tf^9cV.kw%cECecWI[3dWRJ<A++H]cauH-2_8gi;.R2l?ToYI+M=Js*MC%GCN5eA+MQfH'voNP'vq-NM-.OM=-i3`j,-qp%4*G*W7J`'_]t?QwBLR4R3l?i9DqMf9DxkcE[Ebol&"
".W/VQM^o;-BSov$g>2'omn)AF+hb(W`=?f_4U.L5*5Rq2FD?R<_Fe?TJH%RNKN.RN==g%u1VNcsd*9Jr/&22qpxX3F$9b3Fwr[q)&?b3Fkfs3+Md&s-qkk;.oA4O,sfYq;LED+rShOcs"
"EpfumIVL8o39L1p^c0>muE?t.64GS7g[_f:(E%RN>$%RN-e&@01Xe'/:ZJe-Na(EP8Q1N-*a,t$;.w(<rEo2DpIs*MNKs*M7AR*0QZ9#voeZ(#BBGO-skv9/`OV0vxt<$M)P)7#TJL@-"
"dMck-gA6V<k#Ow9-5_f:he$,;(9DqM2cZc;9KWw9W&'s-?vo9;&PIQCU6nQ8H6=kOM`9_8duYf:*X7G;3$Sc;#f`Js4:SqLP:#&#)gHiLh$AK#K0ZtL>kqsL<aW0vsT#28[H8p&qJKJV"
"4en;-eIvD-9HvD--m7^.#MV0v/#Z=&3tFk=l#c2r]#=J-tv<J-cLL@-mc-x-.%suLaW%iLdX%iLrY[&Mq(q=#gFWDu24)=-s6)=-,r.l1sOZm8_>;<+75_f:/m=G;S4Zc;+Tb+`'N'G`"
"7j/BDtu%4Tcld##-b;<#;WT,.K:@#M9mw&MV'sAN$6hL9lm<n43VB,MY>m6#LB58f;Xa795#Zk6qtqjth>G39JDPK3umd(1p%)s7b&a$#ZH7C8j549'=MUZ3Uem##dA24#jYV4#6$@S#"
"o:9U#klWT#tFKU#V7%@#e2I8#?\?f>-RAf>-cAf>->F$M&A7hi`3#.40&V;s7Bd-Z$bN+D)QB&9&,N$VdJ`7T%NbRc;P&MG)X*dY,fR#v,J]=;-Y:YV-de:8.<u>>#aaSS%r_;@-W%3:/"
"8:F&#OA=gLN+E?#+r0hL*L6'v&p)%2oSFw[UCcp7^vX&H%_SmV3[&L,l?'Q_?\?C0;Tf=-=ovlp9lBW8&rTs;-u(uk%&xjl&PVpI_<G=%tN8X1^dE#VdG<HuYkUSq)xc6Jr$)3Gs+2q;-"
"#OM=-Xv'w-m`R+MCcA+M1cA+MA]SbM3WZ)Mso)*M@I;b;mlSWnK%ho.i2q%4bFSfL)r78%HFpV.63Ym/xV5N#NiC?#CYlS.Rl:$#[=eA-S@eA-hj>%.Ou[fLJiJ+M5,$>#dbEB-'&@A-"
"g7wx-o)O$M]cJwuMOOgLuvS%#6:@#Mkig5#<W4Q#shj#.V//vL1X/+MJ,kuLkB8#MDdWUMcD:%OhXuxu0Ke'uKVou,7Dpo.7YQ]=2beuG[MBkFkXd'/PxNk4/tx1q4@-_J;tZiLk2oiL"
"?8xiL_@RA-9e.u-of[+MCVfBNk-aaM)Ls*MwIs*MMJs*Mum:#vDjSN-NhSN-HhSN-K/]^.>24&#ucJn.ONb&#mK_S-Xj*J-uL:@-_>:@-_>:@-X@:@-MmbZ-%XZL,3I5kibZJwu?vc[."
"Rl:$#<w5l$,r0hLm',cMUNu$MbeK1;6j#K)pCZw0MOJ'JrlXV-Z@r3=P].R31wHwB*g/58p>r809x.C'78058lC5G.i5DjVe$Eb$s#t3glkk3.11:P8riKdOxS5E<_cXvIMJ(98I@?Z$"
"1=q191V2N9)Q3<#'####)Pj)#fmn@#tE4M$.#%W$b,SP%B<dZ&3heH'(lD`'Q,2O'n:d?(W>e<(b`lJ(-E]x()w*1#I2Puu5vboLx4voL#;)pL$A2pLV%9)vo(o)vLo-qL.(8qL;.AqL"
"44JqL5:SqL6@]qL8LoqL=kFrLE^_sLNdhsLGjqsLHp$tLIv-tLJ&7tLK,@tLL2ItLM8RtLN>[tLODetLTc<uL,jEuLZoNuLWuWuLX%buLY+kuLsmMxLtsVxLV2d2v>ROgL^Ej$#M4kj."
"aGY##rM#<-BN#<-?6T;-&7T;-'7T;-CB;=-Gt:T.MRu>#)5T;-C5T;-D5T;-E5T;-F5T;-G5T;-e3S>-F=pZ.-s2%vH,9$.C(trLC9(sL&NJ#M'TS#M`fD7#D/-&M$+55vkmLvuqrUvu"
"]dkgL,qugL-w(hL.'2hL/-;hL03DhL19MhL3E`hL4KihL5QrhL6W%iL7^.iL8d7iL9j@iL:pIiL;vRiL^m1#veIEjL7WOjLE]XjLFcbjLGikjLHotjLJ%1kLl+:kLL1CkLM7LkLN=UkL"
"OC_kLPIhkLQOqkLRU$lLTb6lLUh?lLVnHlLWtQlLX$[lLY*elLZ0nlL[6wlL]<*mLohjmLMRq&vM._'#u;/=#pG5s-Z+V*M'4N*M#r)*MRItW##-&X#-5T;-.5T;-/5T;-05T;-15T;-"
"35T;-45T;-55T;-65T;-75T;-85T;-95T;-:5T;-2%kB-D5T;-E5T;-F5T;-G5T;-H5T;-cq.>-K5T;-L5T;-M5T;-N5T;-O5T;-P5T;-Q5T;-R5T;-T5T;-U5T;-V5T;-W5T;-X5T;-"
"Y5T;-Z5T;-f:eM0[$a<#:3PuuL0xfL[qd##3V1vu%UXgL#muxuHO'#vc@<jL)`I$v::^kL,44%v`u7nL0NpnL=T#oL$)doLV5voL#;)pL$A2pLV%9)v<(o)vfn-qL.(8qL;.AqL44JqL"
"5:SqL6@]qL8LoqLftl*v4[<rL9kFrLF3urL6:(sLE^_sL_dhsLGjqsLHp$tLIv-tLJ&7tLK,@tLL2ItLM8RtLN>[tLODetLTc<uL4jEuLZoNuLWuWuLX%buLY+kuLsmMxLtsVxL%HA#M"
"(PJ#MxTS#M*go#M7mx#MBu+$M2Ac$MNHl$ME$`%MD(i%M^.r%M?:.&MHC55vIgCvu&nLvuarUvue#`vu9jtgL1w(hL.'2hL/-;hL<3DhL59MhL3E`hL4KihL5QrhL6W%iL7^.iLPd7iL"
"=j@iL>pIiLGPFjL/WOjLE]XjLFcbjLGikjLHotjLJ%1kLh+:kLi1CkLM7LkLN=UkLOC_kLPIhkLQOqkLRU$lLTb6lLUh?lL%oHlLWtQlLX$[lLY*elLZ0nlL[6wlLBQq&v-._'#'mx=#"
"pG5s-xn&P8Jx.DtqkScs#LlDt.)t[te)k-$Qs<%tkS,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+<m,F%=p,F%>s,F%QVQu-x%9kLi1CkLM7LkLN=UkLOC_kLPIhkLQOqkLRU$lLTb6lL"
"Uh?lLEoHlLWtQlLX$[lLY*elLZ0nlL[6wlL@Kq&vZ:E)#>6>)MU3L=#6kF6#T'c6#M$a<#[5T;-d5T;-f5T;-2XkV.lTS=#UXkV.PRL7#<N#<-TY`=-C6T;-m6T;-(7T;-)7T;-d3RA-"
",7T;-N5S>-`@:@-ChG<-]Z`=-KI5s-./*k8Y2bj;%]=)#aUT:#XD4.#FYlS.@b;<#SYlS.TYV4#-Y`=-BN#<-?6T;-=s.>-*O#<-'7T;-DhG<-=7T;-MH@6/AerO#50ZtLJ&iU#d_pU#"
",5T;--5T;-.5T;-/5T;-05T;-15T;-35T;-45T;-55T;-65T;-75T;-85T;-95T;->YlS.xw>V#2%kB-D5T;-E5T;-F5T;-G5T;-H5T;-g3S>-K5T;-L5T;-M5T;-N5T;-O5T;-P5T;-"
"Q5T;-R5T;-T5T;-U5T;-V5T;-W5T;-X5T;-Y5T;-Z5T;-[5T;-daDE-<`Pd0[P(vuK+E%v3.vlLdgjmLfs&nLnMpnLP?1sLGQLsLmHmwL(Z]#M)af#M3Gl$M]q*'MMw3'MuB16voX&0%"
"9pG-+@vNW#p,)P0+t*xu0$4xuZjHiLN%1kL@Hso.lB(#5*lVA5XPq]5YY6#6ZcQ>6[lmY6v&en9,+m%F0<sL^RLdl/O0qOf?ci20rO'#vAV0#v;ZsDu<u63#]<Kp8Oh@5^2d1R3#`m-$"
"$cm-$`'S+`fH/F%-(n-$.+n-$/.n-$01n-$14n-$27n-$4=n-$8]sE@Z3A_/<n/F%Ed_w'U<b1Tu^n-$MdG_&Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$THo-$"
"UKo-$VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$'%dE[w<Fk='mp-$67bw'AA.kb,&q-$M-X-HAgT9`CFJ_&]x,F.=Yq-$`+-F.iFe1T9TsL^.?#>>WVd1KR#l7IgG,F%-'k-$.*k-$;D[w'"
"4T,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+<m,F%=p,F%>s,F%.ts=Gx<>cMmIh9;Dmk-$Epk-$Fsk-$Gvk-$H#l-$oo>_/pr>_/L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$"
"$;?_/VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$hoJwBo.KwBw_f1K@2^Y#BN]V$'`?8%Y=lr-$S58.^ZRS.Vp/K2)c`c2YMCa4Rc7>5xknY6d_F59fq'm9h-_M:ndVG<L12#?ZhiY?#s,v?"
"$&H;@V2L,bx9P9#[vW9#:)m<-3N#<-4N#<-5N#<-6N#<-8N#<-96T;-9o,D-?6T;-/&kB-C6T;-^r.>-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-L6T;-M6T;-N6T;-O6T;-;KwA-YN#<-"
"V6T;-W6T;-X6T;-Y6T;-[&kB-wN#<-t6T;-7PYO--cDE-'7T;-<Z`=-)7T;->Z`=-YNXR-,7T;-A85O-?O#<-afF?-=7T;-dfF?-A7T;-mfF?-UNE/1Mx=Y#H#@S#BMOI#2r:T.ET_R#"
"0M#<--5T;-.5T;-;(m<-4M#<-15T;-35T;-45T;-55T;-65T;-Oq.>-<M#<-=M#<->M#<-.cEB-D5T;-E5T;-F5T;-G5T;-H5T;-g3S>-h3S>-L5T;-M5T;-N5T;-O5T;-P5T;-Q5T;-"
"R5T;-T5T;-$eF?-V5T;-W5T;-X5T;-Y5T;-Z5T;-[5T;-HkN?3<3PuuXOc##>iLvu>Ktxu%A<jL%`I$vk:^kLUh?lLGEX8(A;tc<H%2#?V[iY?#s,v?$&H;@&8)s@01FmBkYw.C.+XMC"
"7LtiC4I9/D5RTJD6[pfD8nPGE=E.&G>gaYH:b)vHD;g;ICj#5JEPuoJ_L;5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJMNLjfMOU/,NT-c`O<A*&PZKCAPWH_]PXQ$#QYZ?>Qm[DGW"
"b`?AYxQX]Y%0mr[8r88].XPS]8qCjr.go#MGGl$MF*i%MU.r%M?:.&MYF@&M*8MQ&.<+cisP^1TeA,F%w-:oegG,F%-'k-$.*k-$;D[w'4T,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+"
"<m,F%=p,F%>s,F%.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$g&f--h)f--L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$$;?_/VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$*iA-d-nOP&"
"aMTpp&T#oL)BUE),Z^<-s]Ks$RIxi'-U=/(NuU_$DlgX#9fG<-35T;-45T;-55T;-:YlS..-&X#<M#<-=M#<->M#<-NM#<-OM#<-L5T;-M5T;-N5T;-O5T;-P5T;-Q5T;-R5T;-)]vf:"
"O2BdsJl08.d9Ii^,p*2_;Lnxc8#m,*b-m=P,b&a42^WG<YWx^f%Zgl/*s_w'?_n-$&jp-$'mp-$KjRk+C.3F%w^A;$;[jl&P-SS%(8-F%Dmk-$Epk-$Fsk-$Gvk-$H#l-$?<TS%Wb8e?"
"Y2:D<N*xCaK[g$-<0cf(umcIqTBP%XU0F_&w3F_&NMu;-6B;=-'7T;-poJ1:Qv)KV7+rr$f,k-$7Ek-$9Kk-$c=8cV%mk-$Epk-$Fsk-$Gvk-$H#l-$cWMk+UJl-$VMl-$WPl-$XSl-$"
"YVl-$ZYl-$9hsr$iT[Y#+a%ZL4a7R*x[m-$#`m-$$cm-$&im-$Q0<xtc'n-$6tF_&3R/F%4U/F%5X/F%6[/F%8b/F%9Ln-$Vw%pJV4;5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJM"
"NLjfMOU/,NT-c`O4)*&PZKCAPWH_]PXQ$#QYZ?>Qs<=AYtEX]YD8P^$.go#M.,MQ&Ct(fqBmpdm8tvT0M2PuuVFluu5w7nLtZ[R('wZiBVIA2C:+ij;;5b<#0U*W-Q#ij;<8b<#vbWo8"
"KLDMqcbU2)w?8T._HKU#(5T;-;5T;-PV`E/UHtxuB@<jL%HA#M<(i%Mvtf4vRmLvu-/FxuAZ0#vlJEjLLVOjLE]XjLFcbjLGikjLHotjL]<*mL]E_&v):%@#tjVW#/5T;-75T;-95T;-"
"ncq@-VX`=-RfG<-)SZJ:VZ3v64-Z59bm3L#S/(PS;8t8S4L[=l/8l-$1(bw'<Vq-$L7h9D_soI_.ip%l^`A2_-`D_&Epk-$Fsk-$Gvk-$H#l-$:Xo92WTZ9MG'd9Mb^2&+=si]+?c*B,"
"Oe7/1YAu`4V>:&5WGUA5XPq]5YY6#6ZcQ>6[lmY6a7Wv6<gDA+_GxoS52TpSnFd1TrUq-$R:e1T)j32_jmh%ujmh%ujmh%ujmh%uaW0pRLH%$v0c>lLVnHlLWtQlLX$[lLY*elLZ0nlL"
"[6wlL:3_&vbgHiL)PvwL)PvwLOLs*MOLs*M1PvwL^oN4#jH(xLUf'HSA[]#MJ5QQ#UnXQ#F5T;-G5T;-w+ADWjPXR-M$>G-EIJF-EIJF-EIJF-EIJF-b0KF-MIJF-MIJF-S*C@/lx^#v"
"DisjLSUKF-ZT*oU:+[Y#HD>;$3Ja5Ai>su,8c###/@g=YQ#/l#&hj1#8MYS..af=#:@Rm/K<bxu]M]9vk3rlLYt@:.cmD%t/u'#vK4'`N7@7Q/BV;onf'K1p]kQ>mBI,cre]fh$=?J9v"
"Tig@MMr:T.d,(3vhiJWQkCHL-$8)QMbA09v&LlDNciTK-GH@2.u3UhL6ZZ##O<A>-N^we-Itr_/L@CkbN//#v610%Oi`+_NMv0<.r><%t.29@'k0ELuV4aD+9Cgi#&>jX:wXltPlcA+M"
"Rul*vE`V:M;3$>>ULaw'61D[9>jY/veMo5#pIV0vS&.>ME(KGM8/8^NSlRQ-sFYTRSbm##9KcF[1+c&#=ZUAN$%g+M?UP9'gVfH%CcgHMLFPGM%>^;-=4PDNPOonR^Ah1TP/gLU_UjJR"
"cpg_-TLW@'E(85#:qmw'7j$cVEOan%w+b0$nE<68^U2:2lO_tMGR-W-HQFI.BRlxuIVWvP3_oRn6xkxu_iE?P#TN.hgcKB'ojmxux2qaXW'hP'+2k:Zow+;)M(Xv@cY:W.5e+/(5QAq$"
"*/cc3Vww%#G-4&#Nl:$#wJ6(#gcZ(#7wU*3j`-0#UJ;4#^Ws)#6j=6#P*O$M86U*#7b:5/7B[8#tv7nLWi3VRbxm##0`($#a.xfLU?5&#&B2&.(A=gLR]-lLai?lLa@E$#)=M,#(61pL"
"'VM'#4[%-#]#S-#c3h'#Bx+)3lSF.#rWH(#uL<1#>qs1#.>+OM#+R+#$3A5#T=@#M#=3,#h4TC/M*78#++eQMWV?I#?7i$#ZSI@#d$eK#nG-(#1qvC#HsYN#LxJ*#`&a..3kQiLSwu&#"
"B8E)#<RI'2]+^*#eC,+#jO>+#ko_xLgad6#GGCR#JXR@#[-=A#20UZ$(PtS/A/Sv-Zcqdm_fR]4L/qA50^<3t#xGS7tL'58$8IJ(Ci5L#NL;J:2ZG,*@L-1;434D<6C^`*)OFV?CS%8@"
"8JwA,^b6L#%]TiB^Jor-itUKuHdfKuJjfKu]JgKu`MTKuxwC']Zfc._2ObKun(hKup.hKuq+UKu%34$#D/i?#FJFI#LI.%#P`[@#P]bI#Vh[%#W(4A#WitI#_*+&#^@XA#^u0J#s)U'#"
"n?-C##]6K#9gZ(#/'3D#k_psLdCMPM5F?##%+Qv$VK3L#Vq@3kS[`QjW.RS%uu,F%f^$)*`L$T&@x8v-5`2g)?45W.ddRKu63)L5c(4>5Et/^5urwE7m@ku5f`er6(P9_8u'HS7o='58"
"&7dv$UT`l8t?Y59BmY-?)']i9l?qM:JGrE@1Wt+;:&kG<XLKwB;>Q`<-<k#?=L')*WO6L#cP@S@6#;d*YU6L#hlw4A4gY,*^b6L#pFTiB/O)F.nSo+DMk1GD9>7a+f$7L#q$i(E:GR&,"
"h*7L#t9I`E;PnA,j07L#(kE]F;PnA,m97L#)6>VH%A#_]4kx7IILvVIwf*eZ8-YoI3lU8Ja=H'S<E:PJ)Y6pJ6$6_Ac6w:Qpe;,WlY+F.7('DW9BPAYb$BX(S&Oo[w2M8]K(Uw9YJKl]"
"?GA5^)>,F.^c,M^RnZ,aTh>>,g(:L#pRf:dC[%@02ai7eiM_Ve@.-F.6#JoeLZ>8f+_J_&?cBigj,;2hi[^'85wu9#]e:?#8c($#GrL?#;uU?#@%M$#A4r?#A+i?#LI.%#M_[@#I1r?#"
"Vh[%#P'4A#C4@m/_*+&#U?XA#fPii1kNb&#]^0B#]OI@#KgsjL^ArB#.D3#.Y5TkL+(xC#8D3#.p(mlLdXkD#lF`t-#5)mLle'E#ED3#..Y`mLr3_E#N%,s/[xJ*#2/5##nG5s-e'ChL"
"m;#gL)H5gLhP3e$(;f+MM_d##@;#s-w83jLV?a$#DMYS.VD-(#$;X3.j]>lLaC+)#cGY##[;#s-F[imLLXZ##f;#s-Jt7nLG:-##n;#s-MHxnLM_d##xMYS.9<M,#9g=(._<:pL(0t,#"
"?G.%#@*]-#tr6qLXKs$#/<#s-v(IqLXKs$#1<#s-x4[qLXKs$#3<#s-s@nqLYQ&%#5<#s-tL*rLWEj$#8<#s-w_ErL[W&%#</'sLLY`/#$@0,.EGKsLrv70#eg=(.hX;uL1-u1#d#)t-"
"4k+wLacE4#3;xu-OjUxLdsp5#-7>##bJR#M5t56#?m@u-i]n#Mn;Q6#A0`$#2=#s-q;k$MJF6##0cK%MH;88#Qm@u-8%q%MFS]8#twQx-UFV/8I->#%9L<5&AnWP&77RS%=esl&:e82'"
"?tJM'C95/(9W'N(nuC_&la1,)=&?g)oSKe$#W*&+HfrD+w:D_&'pa]+DfR&,#AD_&72B>,WFUS.R?45&ujNM0Ai^21Ej.L,R2``3q1mD47YLe$DwG,kjgs?#YrA*#w0pE#tYu##L4dL0"
"-][-#";

//-----------------------------------------------------------------------------
// [SECTION] Default font data (GroupType - Bank Gothic Pro Bold.otf)
//-----------------------------------------------------------------------------
// File: 'GroupType - Bank Gothic Pro Bold.otf' (36120 bytes)
// Exported using binary_to_compressed_c.cpp
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
//-----------------------------------------------------------------------------
static const char bank_gothic_pro_bold_otf_compressed_data_base85[36235 + 1] =
"7])#######tN_o*'/###OZf+>(g1$#Q6>###diT9<Ag,Ddpv<(SPYi96TdL<WB0PI0d?S@P-=G2/ExF>[pjHJ48cu>;Js20tC#f=ZZ[kEid*##D.$##TuA0F&94`N(u&##Bl###h/DUC"
"QwY9dOkLk+02q-$f'TqL+Gi8IJUE_&i0s-$T3n0FIa^w/#U&##N[*##q9T=BVgGJ:1u@Y-nJ0I$w:]=B1YSv*8K6_Af$i--w#FiF>TX%O7`###ds*##D7?>#0[Q_#Jnm6B=&$t'X#(v#"
"M5DX(_HGusij_F%Z4.wu5hO2$9Ei9.(/5##uCbA#vR0Q^)8###UFl--^?%sZ<njk+(r9B#hLN`<.esJ1wUN`W<3]V$;51G`N'm0,],LF%(]T:#P^]=#ul8Z$a&p*%%4[0#=3l)#iO(F."
"j*Cm92kmrLn5K`tq6#'vQPo7McYfb-CVHk=2[F3bXQd&#=jJgM^2(@-nVjfLmZGs-Pne/NaF6##TsB1NUL?##,6h1NVRH##@EV6NWXQ##Mib8NX_Z##`b-:N/RGgLM/(<N]w)$#_H2>N"
"1_YgLJ#f2N`<a$#InnM(:]k#$1:<p%(4oJMv:JcMS@-###sECNTF6##1^A,NUL?##6^l-N2'IAOpXQ##P&J7Ne_Z##un6:N[qv##GG)>N^*E?#r8@BN^'3$#M3N+NcEa$#lTX0N)Hk$#"
"0VJ[8DX=gG&>.FHMeZ<0m@UiF:X%F-f%?LF871@'hb%;1u1NX8'eZL2(A.@-^meF-0-4&H9CM*H,,lVCYUx8BeU+m9.WXnDjopbG[Z=lE.fsjM#2^gLNla^-2>r@0'fr]5vc:G7U'v3+"
"(;1eG38vLF)-7L2(E4I->QoBAh&Sh,0]6fG=9OG-c7$lE^=N880oiiF;]wmLBlSE-%O;9CuicdG/iCrC><jfEo>pKF+(rE-&a'kEA%o'I9g0H-7PViFTqm$8T3L'>=O%F-FDSE-4#?lE"
"S2&aFewTt1S6Xw087M*HBsP^%a;,FHr(622d8_DI@.vLF,v_aHBFwgFXfCP8XhdxFeBEYGIYo+DUw%;HlBel/Zl,20j'(m9Ls%aFpB7L#80(@'?Cw?0sC-AFWj.@'YYbYH-&K21/HnJ;"
";X2X:[2g,3/pYc2L9*j1t'Bs7i-7L#IQdF@NMWq)sc8JC8C$^G%?g'&1G`f1rWu1B3`SGES'm`F8]WJD`_&mBVBi]GYc[>-T/j`FtPQ2C'*p92x'AVH2u@5BxGsXUpR^@TEfv]5Al`#C"
"jij?BmGs.CQGkGN-%ZoLJxXrL(B]qLO_4rL3JhkLFQ6LM=*XB-(xRu-/B3RMiU#oL@)m.#IomL-.r.>-<edEMQ,TNM9gXRM79`P-kL-T-alLYSlA]qL4LG&#IGuG-*pWB-p2H-.#N*rL"
"r92/#FSrt-:-W$8*ZncEXnQwKr5oKPp'r?9'==SIHuK`E020#HWc'vHIW)dEC7*FIYDFwgx(p+Dvfi>-jwPx%9Jg--^<A_/<h&/1ItvQW_qHe-#7;_8t4'aFR'&kk4dx^]sO9MB:%PDF"
"i.,#HB-&?`U,j?B4F&;H#-QY5+v4L#KqLwBD+af1NCH]FPBI>HDUA5B`n-j1XC,aF4RHe-J2_=-eYN08<QF&#tUB#$&iB^#flRfLP:$##q++,M5:Mt-PAr*Mbqv##_BhhL7+>DNRY$3T"
"g&B%#R_R%#Vke%#lpi[%+5TY,cL5;-gelr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1ck`L21&v(35>V`39V7A4=onx4v-k^o.5@)#G>N)#KJa)#OVs)#Sc/*#WoA*#[%T*#`1g*#d=#+#"
"hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#:n@-#>$S-#B0f-#F<x-#JH4.#NTF.#RaX.#Vmk.#Z#(/#_/:/#g/+R-IAu`3E?S5#IQo5#*%.8#u=R8#@a_7#=Z`=-"
"Ad6#546u6#D,c6##rG3#>&Y6#J:v3#Bgh7#*DLV%a_qCaa@t:ZSJPlS`i`%X%pg1TdIIfU%Kn7[pj./:IxP%k__A`WFHGJ(k9#AX1UOucjRni''W3S[>n/2'4_PuYN&IoeM11P]eZ]rH"
"=A$8Il'ASIj(f4fsOFlfs['MgHmToI$(_.hXXLl]^Qcc)Kf9ci)OvFiP+;8%tlc.UsSUlJx%n.L$23JLOeOfL+Sj+M/rf(N3.,DN7L(AO[h>]O=k_xOB<w:Qq+?c`aMXrQHas7Rk*WS%"
"AMU(a2<Nl]qwm,3GR;;$0U6H2M0PY#CiAE#/$]H#r(eK#,Cno#S-gR-<Bx=-Nsk>-er?@-7@KB-54Lv-3)YuuXF3#.g+PwLhw9E#YW_pLwdhsLKffo#OwSR-96f=-YDOJM8UJfN>7Iuu"
"pJA=#wq.luHx[due#^au#&+&#(Gc##+>'xL&uoc2a-)fT@nM.Kwg[IKx-a]bf@8U)N5l-$]+l%l8%t-$gDHrmFZK_&8_*WSPYD:#A*6;#8$-;#F>(7#P,c6#R7I8#*%.8#F?bcMdEY(N"
"q()(&4.UB#);5##9-1,)jm[-MQ3D.3%p*`ss0;ILT5VeLM6j-$0YK]t@M%,V_onbM$TYY$rYU_$Jeg#(C>Hc(6$xCuu(IqLL'1'#4l:$#Z9F&#0dZ(#S%T*#u0;,#DN=.#h`-0#8.92#"
"^E24#V>@#M]CS6crk#JhBVDonmY4##L;EA+0A[]4h4;A=LCmxF'Gs+M>*euPVfUiToKG]X129P]In*DabSr7e$:d+i<vTulT[FipmA8]t/rH8%GW:,)`=,v,18.&4h(d`<CMFGDi5hlJ"
"AEjrQ%QES[TdGYc8p#;mh,&At>*G/(up&j0QH%m8vweY>L+h`E07CAOceADW9]GM^],muc(AifU?Uc^H+-&YBw?vsBtl>lE<Dnr1-d>-G$]eFHJ8k#H1,ZlE<3.RM0K.WCK)T,MdhWj0"
":GViFP0+cH)HxUCDpN>HRMi9.-KFVCCq9jCT5=SM/RCnDRP`t-R5tSM.'MVCD+94MSR:nDApNSM/*7oD%@Me?cSFe?5vAe?7&Ke?8,Ke?7#Be?6#Be?7&Ke?92Te?8,Ke?;Ape?8G-=_"
"LBr$'J4(%'QOU['V[C@'Ye:@'_q(%'f6V['lEV['oK;@'@3o6BtUZm14iN88fRfS8g[+p8heF59inbP9jw'm9k*C2:l3_M:m<$j:nE?/;oNZJ;pWvf;qa;,<rjVG<ssrc<t&8)=u/SD="
"v8o`=wA4&>xJOA>#Tk]>$^0#?%gK>?&pgY?'#-v?i_]s7'+OGH;/;9.*G%'I@etmB-JraHtveUC_:x:.'wI:CO<(3CuAFVCNvaoMf36A8rK8s7$u3R/>9`9CrdM=Bo$G59olI59olI59"
"tlI59koaM:Ij`M:Ij`M:Nj`M:TM4K;-p(*HM/GG-p4<M-ZYnA-q36D-kU'C-e.j'Pq8..OmjlkM/]HrBT#9]8iIOA>(ORA>(ORA>-ORA>$RjY?N.iY?B]-v?kcR59]H@M.mX7fGXjEZH"
"[5KL29vVX8WFJtN%_uq%.,Sl#-xmo%?-,/(QDw%+A0w.:(LFV?XT9PJhgm7[OT5Yc3)Donm`O>#Z(^Y,w>GG2Ic#8Rsk:##L7?AFi$u4f/*f%kQL4Mpv%c>#(wulAVt*;m0#D)30]H;H"
"OI'#Y5T9>lhjv(sRrb;-,ZP,2k$pS@uY-mJ54YDNhhscVTv_Vd3*JDjIYvrm,V_v#D0,N'_x8^+8Y3^4_,=j9%%J#>E;S/Cm/:pIH5hMT8.wc`U-mcrUuXv,.HKj0>S%E3WHMp7$r7^="
"POM#G&p?mJAq-^OWJY5S6)X8[OhI,`&rK2gQr2smv]o]tFNUg(jb(<-?i*B4b/4N9.L=Z>H>JjB[R$EEuYHmJQ,,TR*<.ZYE%Z2^^#d>c(4Q/hD&C#l^w0jp4%[W$LHGN'c(9B+,0bm/"
"<;;H2U6)97$fi&=F,s2BY@LdD.@rA,%=V=-va[A,$Tg*%H+;F`f4U&tJxIM1[q&X8;SKsJdvkes*2]x$9G2>1X8gj9nTV6_=JF;=lua-_E&B_t4/,CW41bDWwrpf,9)Q`I9ImS&+vH9I"
"-,4/).r.-)Dp<481GZrs)H8i').A5u?C56/Xm1I,4OBC*%w&R,;D1`t2po]tYxZs$%-<J82#^nfJgk5-?k9o.'V;gtvQMC30JjSeF0-b,AV@c9YO:`t3fUQ&J5/Psd;>GsG%(Q&U<3-)"
"b=o.M8=sl(S^MOBn2<]BU%<Ug.%'I_C+j,g]>NN90hW]37+od)D02B%5+7UMM3Y)3`4nd)[_9o72kr:$cX2F%YE3L,)T@<&=wx+<_F[S&J;A$5]e$TRL_4j,QBHV>Bw<j(p/+R-Dxq_%"
"Gi(h)mPm';3khj,Da*HTMcLE&GT:^4Y-u&-F%=C30Yh^s*t3E&I$BN,+:Gx+@qb,)*c3*Jba+A%G@jiF`&;?s#-+C/rD[?%WK`ckI(ws$*Hu&in$xZTM:]m+:+c2ZXZoS&Pa3LhRc?<p"
"KE8`tAE&a@hpo+-_MbTsC[+CW&E)(*C#%st2Y@`tD$<u5B7XE0OEwFsrort,sKf7-H^qq)aZ6aIS^m0%Q$R?URI[5SK_AQ+ZP90RS,I[FhQ9EsUA[K%^$>3?2JV>VM+,U+=W,><QO:`t"
"Q4M3;2[1)=ie&$HQINU?+>S>L;xND%(DPwK_-+atd4fx$Pi5[K*?vMKpY(h,q$-4%k/xc$@E(%eW^'Gskp)Me3dlP'[GpUH`i;H,P2on.#lf9&,p0k)Fq*JIH(0k,EHksV5m+I%Ing,A"
")`=#%GL><BNhufqSDEWs`n`fQrM=;IjSWnN6k=G46qr+RqJxuHgD<RN;QuR.,j(TJLDa2Bo&on,$U@/_jRL3-0#h@u#l6csLWI#u1B]S&MK$%b%ZQN'pjRXW>uZF&Ns[i/SUpI,bO;A&"
"8d_kA_7]`&[o_$lYF86rKE8`t@>4Ts5X4+)D:qn.0t^/%Gn>Z$b&lC&91t89JkW,%77)mX.1kot6AUv^M/Br+/Si/E_vkY'I$@`$1qt1LR_s9%EW1eQ_t*wt<3nd)P;xfhCEUa,83CC*"
"hs?%%R5?i$s#2^t/JK>&iC0T7/N>q,8KhC*W[g2%vDSc$YjrMiW>Ccs9SRN'(Y(R%Q#6i$gsDL%QEWkS4s[YV]#@uZf6f8gg=JRn4&.T%^jODsnN@DsqfhKAs-Tet(')hF.]>L+vcfM%"
"9-;3d6(b0pv<IeiZC6bctW=4StaT;CE)]HWA6:vcD*qa,ZZW,;o;7(-,<(@eUN'Gs-`M@O[ZiI1n.WD%a$CUg39)S=%d7I,8d_kAY:ba&_1s<1@dpeUU)$1KKSd/f6ZQN%Il`=<uC[#i"
"rwG'%;9ph$1Eg=<K=:`tr(?:9lJWF&Y1aCd/@jg71SE0%eE&3K7G?tCfb2@&/=6asg%&&$U]vw41go]tVfY]tOApasPPl^tuZ5=5h2]P@JEgu$@K9BFPdsFs-(Q$$v1sae&[?N%EHgh$"
"-4.Ss,sPM5E#SHWtEGI,`4<a3E#UC,0O:;&U:Q1M$VWS&6]$LgpAV9`&8A:&`9+t$^I(Qds&2H,id6Y,90,Ys+)>N%b7.cjqP'`32@$$SHN-Gsi8x8W9WIp+cc-N'4/w9&u-h[O(#KEs"
"fB9EsYL:RsaoJI&4-Y>cL2*c,?qDO,oiY&=&bjo,'j6F&Fpdr)v<eBSL*1N'sJpM%@6^h$%mFwLt0W*8@Wi,W5..C,='sFsK2[S&[1ZS&`_=wUuUi?Brkg%%vgWLTpH3LT1&k&%*i;Ws"
"Gu;Ws_%^lsL]High<-Dsk>t'*g>tDsp'Ad`6d,c2G1XMp4AgVQtU,aS0<GaVHUrq3RSk?ILTsFs@3OU&R:XP&hQwG&8'+i%HRhs?sd`m,[KoA_<RK;71mXP'q@fx$,Y`[$Jpj>&@DDJ8"
"ivbS&+?j#NJ&3H&41gn7>`hEj#/j<d'h?I24OBC*c.%D&Y-<%?wXL*-x':gqJ#uDsVEi'Wp&bP,D:.%=?\?Oi%J9^b2'>j2gGN6UZv]Zvc+i%CWwK'3KT86tHX&`I,7#f>IeZ9C&FbBQ/"
"r&[S&Ks*Fs4O,ftBj[b2aT04/t1p]t`(3l$%V.N'*il'Woj'o.Pc*TR[)WL%[B&Ca<%8(WHiuBaMfEtt@:9asqvJI(B$VDsuAro$(vjlt)oNI%dw?@k(&;Q&^Q%Fs0q=qtXdKO/4Yh^s"
"vBvhK?f=DsnD9Es)^hR@0Ue@%B.Y]tp6tDs:e$@%-@]hKq#8S.N&MC*d+3)u@P1?%.mge1dOde12ew@&.Bn@%iENB,@[b-Il/GR&pU'`$R/0Sfupl,;BmId`Ok_5QwP)S&_a24iRB&I_"
"q92oXXD%8-OD<s)'6icthS*]+K?mn7WDen7+:7@&$QKI1/(qn.'TjDs`RSQ&o:XP&M,DC*A<@(u&OgMs$UhBs0#'FsgXKut8J;3T97YU&eD.H;Y1LF&H+[D,E/rn@?`)V5T:PDsIGuxt"
"7^iN9NVN^ON()CaHSKF&>b&3*?>?u>VHUP'C8W?%:#OiWbKYQ&*pcJU(E(--@Jd&Jp:Gf%.GV4/R^XL'm?;o.Re5hqsxsI&heb]sK9<Ds`VP#uvA_.iJSXnI%TuB&ZUL?#LXDI1.r2Ws"
"kNVP'c-JHsFTqHI=h=ZK7&<`tD4kD<nJR(-aA9csh2xDsZXrGs%5A[s?M%B0j#Ex+p,-10GZIqDJqt$4=1N9%Q/(k7>r.PuTP'V&ei=C3sUB^#D7q<1H+eh0I2@@+WSHR&jv)uHffQG,"
">=clsd?9D4Ws[S&)E>Q&;?FbjANc1Xe>_Y&&M-oLWX`u$ceUe7pV.tH:G'K#Yr22i42j2gP'7^tJ0Y)M]0jGs'H5^tE>'2t#V7`s^XJC4(HOat'c-b$[%H?#N#)Zs4r.f1Pg7UDnX1J&"
"#?x_Od;g-U6h*t$Wq;d%2DuDsfXE9%_$T5%^)]RsmskIs`L;_tY-]HspV-9I$Fg_*P^k^&T8vn%fwvn%).Tx$DI*N'+0%iLkj<2*c+/&$uA,MTvhi4#Zu'##wB6`aVXT:#>w)##N](JU"
"qI;4#@'*##NJGiTm1m3#e=(##qso=YsUM4#o[(##sjSxX*`$s$6^&##5+SlJ-c;E,&.)##/gHo[40l6#6_)##Z6f:dDA[8#C33##iZ'SeH]<T#KH*##tG;igO.k9#NT3##-;kCjSIKU#"
"Ug*##9oc=lYn,V#>v&##&=s1B55o-#B,'##,iNoRt[V4##%)##::m1KQ9w0#Xo'##MRc.U2)]-#gC(##>2VuPcD^2#IA'##q$0PS3/f-#8d&##KMLrZ)If5#8e)##F`3SR=lk.#Wl'##"
"YXt7IeUs)#+;###Q*Dul?#.8#2WA>#lA-DEC+CJ#D50##J8l:Z,[+6#$()##C%Y+`8H:7#;q2##$E7VQP?aH$W(h>#n,l+DejLc@pa7##>IMmLD?bqaaAkY#xY_`3,?`iB`tF]XX8r7e"
"3KVA+UY_]=W-SVHCWJSRg]`rdZT`%tc#K]k?9O;Hlf,,`U6tG2If'dDdQoVHFafSR9%7,`tepigQLOMp.i?W$c@5Z,NjS,;kleGM9o,ZPeA[Jq<tFN'3]AZ5ui9j^<i/jps9.9.EA4KC"
"6hVvPWvJ?#'W_d2&XGT@=$Uvc@^X&k7:N9%LKjj9JDfjB+`XdM-J6K_.q*Ejjn]dV)T$9nrnuZ,&^$qI](30U:o=KquciB43<iQA@wB?c2@63gBf3b3LildiUIVq.BW)U@Od]<Q?,=wY"
"`Z^EaRu.tmarf'+46Xq.jG0O99UxB=mZ4[GXgN$Px6T-Vg8D$c0rtTe^R4qn3oBX65Qqn&a,r?cru-=HHqPbW6>c't<FdU7TjmwcUM;(=Qx3:n=rV@G3Orkg7[e49c7:lBYWE(O[6B.`"
"L5Oc*fH;]5G$Y(=m[_1C%xsOTq&hI`+(m(+/t6V@#k2`X),J%c'gB(t600;.,M;M:vQJcEuR3SSjNb1_X>U+jVBG;%^C+/2<Xt(=Na'>QV4v:[G6Jlgurwl'-TbiCv<H/)27%)O@BtY-"
"1JdP9ecJ;@>2I>HnDKDOFHmiUZl'&Yo988]-^HJ`A+Y]cUNjofl(@Gj*LPYm>palpR=r(tJ_d>6RLh`X*gMGam*qrnK#=s@aAU>Q?uOAcfdGZ$VL>d33dX>ZYYVDkYK1<%fw<dEhdwJM"
"74+WR(aiGaCRXZ$kbMjCb9i2LEK`/VjUQ#ZFL-ZdX$KB#1Skm'f['02DTu;RBFQvds7[,jRCnDtVSVT/`S4<@'Zt,N0Po-h,=>FuBCn@%h%O$t7$vftq`MXG/E(<-Ov*Fsi)ba$PQi`s"
"p_qs)?/k]tZq4f$CP)CWitL4%LUp%5^7UfWW_%Hsb>G,Eq3Eg7bBd3%%'8l/+hXU%0fMI(HTrq->b7I,ZR(K#HH+Ds(fNu)M-BE^iR_`Q_NPBCnA6c+)flDsZdGoo:_c=f+Yji$Q/1c)"
"b[g0H>c[6+T-X,YckC2XKM&IE-W7-u'FXOSmFlk/lc4#MuVkP'KJrFsaY4FDj'4Ls.wmB5a2KmtRQkQ%b[=rt)<lh$G_8N'lOOkS4E>[Di:9c$7(rI1JC&E32@Rgah9gL9[-blZmOGjg"
"I=(Xu)q_b*h($S%c*CtmlT*xkip(S,,NQsd,Y)FVu:+Ue%K@I2ucj=&7BOxli+3BIHHcYsDB5esm+3/)u2ub@O;-O,rN(4+Z&qh7K/)M1504<4=tg-ILM<=,:ul-$exhYT'9kTsw&;$^"
"?,nG2+mo]tGiELsQ)->QTAnvRbD7^$tWlv>wXL*-=?4f1x@u?&;i4?M^,aaMK4:XsfE]q)Q0c?Mbr9bag3)M1:cmDs%PT]s/>X*rbO'[Tl/'HstK1>t-bL=&MW^I(U3mYsm$rHim5:Es"
"NDtug53nYM_hJKsXLk]svdU>tVp)+.&TJK,pUZ@=?D]RR<iAPs;NrCs%HUv#Px&F7qJwIsIfk5luS7M^o<sWs+PqR%K>jh'+Ue0^*'q05`v?'N+W(&rM77$73k[RR)/xZTIY4`s=Mxh;"
"f-J+2=]NYb)Sh<b9+r$u0'WN'Hc2WsnLEko*DN9M)E0$-pRjBI.e]XshcQ10c'ik/Uh4Cleq:o.bUPR.WLik/g9R10`$s=4cIe--29vZ$V)3]+C5XwX)v=_t1c]5u1e]X#-#Z=>6C3'/"
"2l(xH`Z/R<kLF0tcP1^tCG5MR)xeKs)0sq-DS`ZsaXJ;I&2gasxT6;[:<39[xC7pIsJ[crCOB^$WDC_&TTw(sgDAr-n(*dV9c=Ds-FKSMuBdwLUL27%mT-W$dWS;i<:^`tUAs634&.@'"
"0J>']fdaX#W*ae1dDk-$/8otdZ;nk&UV@@+ubPFsO(mFs45sILH5vw4^a*ghCEUa,YK)b40Pa,)BYgk/rJUYVI=OrdEW>l&Z`pp.m1W/1[B/f10#k4%i#fc$NQ,d$h)nA#^eU_%7k_,)"
"^hLQmOOd%u.K-5baO-0uZCYP'YIiB&)/P@FhM7<&AZgnn.YtEaa.:@4b^Ybs(6u>#JBMGs_59#$AhD_tI4+oLUEg?/aMU.1.)YOSua>8e0eb<%:B>i$.Q&IA:=:`tK8Iw@'V+p/MtSKH"
":fo%PQQ>1PLqZ21Hvk,Hg#m_tPkLO`W<'Gs<rx5Y*s[1-ZDO>4kU3Ls76h>AN7t%p2>4poLwv[F9,Ks6wb-%==&^^bnsNL0)J$'PIQEtZl/^EsVxCR%4IR&M&`>tZfIfb)3eVO</&wct"
"e#e;%^CBvtkXVO<qr?]-6MIk4.@-_sRtm92w(2Hs<Mn*djb4c)Dw)IDQbu^tiJU[-h[e'&3/]e,p.(V%7vlFpR]1=>X$#J(F1BPs'NrCswE2DsTV`p)Xm8d_2WWe<.d<g1gn402mHLro"
"ng8p_:3PxM=6]5%)LT>=NU(6pG=vNskoZqL.t-RT*5F`s'i,w)7.>+D=LoF)?;A+D7lo*D=4JF)C_8N'#kBL9.c0LT^XMH,G8m<ModkQ^1,RN'&Dh[4'2<7u;_Kxn+B9EsKK5ZlcV6+)"
"Tec%t7&nDsYF(2tFU7`s^]W$587hM,oAWRs.B`h`1A#Fs:4q^sXC_n[+@'o/.$nk&8pGUZgXwm/pV[nRx)#7ef`j>.=32r$'eb]Y(:X7/qu$_s+TRfL@wVm52Kh#ZZ+eb2QLT0I,*(BY"
"/nRN,1UHkL9^%G&6,:fsUur%$Zp6;&<'2`ts1I0YJ2J_&40.mA&q/6I:-jERA]%%*8(h]t6hYFsIS,Z$@Hn(uGAZc$1_oN0nc1.(et$@#u).u,_=j-_ggoB&%<j+4lH)x)xuQ4A7vW]t"
"E_Hk]Dpnk]sCM_&VeQ^t%5GPsA[GFsPRw@#QXdh0m%6rs=WSU-Zb@sfledXPoY2T%H`QDsH6/,iRmd=,bYl^?uWNA5)LZVbErvJfI[4o7B[EE,mDgmtl$gEqH?EDqu::Yl7W=Isj^R9$"
"Sm5'*@)SDsJVa_*?x:`tNYhR@)/V_t/g_OJhY&vd4vCR@K5AWnJL_]thWg#sg-AE%@FJZso9c3f8Tj9[o>nW%B1:jKIQL0-B&(d`V#sc,Q2Hv>W.G)-K>/.(CK$VsBS&lJ.5>G>D08@%"
"VDUMae[5,rqW*bS@`L`][=aFssfEE_e_G`t7gLOK@J([B@P$s$Q5@JQ=;c;Q>k[v5Fpj&%[UIA#D'4o/cw1^Obax'3K]8:-wjb4oP?A4&NRc%tWj(h)lGQb:<q1j,xs7ieuu?4]r1t3]"
"*A@m/'GT'j>$4`t4I&V/Rq3E&HX1%*R4i;--aM[%-&x]XrHf+-tpR_OL?e[+wP9EsZ5pn.l;O@&Z`m(uxDfb2';n0@817`%$dc%tg>wD,[5GPs;N`GsSN+w#7?b$bR-XAukhELse%ZGA"
"MG9Es4tsFs:S>Fsn^^B#b>b_3PXc%t>p[i)eV3Y,R/u3J`j0`3qDc_3C[5=5/8hJ%Q-w<4,oO#%`1Q,4PCE9%Qd%)l(cO#%Cf]9lO0-m9ATil^.>:NUHSKF&fRj*MkmQ?*PwtU637Ho."
"F`wU6Y`om^IJ0._Nvm@+&)Ee-W'[9*:8Wp`n_,LIdu;=5Ccjn7f<-iL,cZqZo'nb`on9FD&tG`G;.;mL`.(xLW.aaM.#>[th21fspWQ_sg*:(EW@'utP76[bo45Ijr8/xt6%*qI6b(Ca"
"M5jK,k-On3Ug=OSJL(At3SR>192ICWq_]XYsxS]t3Ip831iX#%;(rtl9.e6no?C?%GL%nLsK2m1,W'+2_#P?*:abJ,Y-+#$-:xw4A3K`VfG1Rs(J3LsFqR(t@nSf)lj*P,<%WD&_D-E&"
"fva7%jUj.DB%bX%vLlDspLYqtp0t$497#x+#sE<&F<up)HSRKsi%B:@P0U]tZ:ip$)%hhLbVTvL_IWMMa3vB*erTkSu'vfhnuh6-&&JwBEA$r$uOMot+R8ls:NBN,-Xpq6Gl#nZutAEu"
"OpT$l/?uw4qL-/Z?%WWs3E`eL9+tG2cY`Gs8aAXsv,jL%vkH?#Vj9j%0^vw4a6:+CYmU;tt`K19LU$0uaK]lsC#%c;i@5c_C*Tu$,MY$C;O,9MGDVt.>UI%u#o7dsixvO&D$+&ubulbs"
";<rU6]LmW_x>NDsqaOIs)5A]$;B,N'8M]]%qBK_tDSaDsLm1o$rp`]t,aOit7M)d$*-IDsdmgut7@_r$jKHDscRnSs$'`g$jRoH,qqst5'1&9tOAAcaT=Z[FIueuHx;EDs1%pHq-c#Fs"
";2I=5c=P^t(bi<%Tuf(X`b_3tF_lhsVOe`$RpX?-%tX?-FL<t%jIXat?GKTn.ovatp^oN0slelsQT7`sapg(*Y2(Rn8ONt$]GLRn%9$Nspw$Ss+Klf$`?wkt(.;`sAJih)kjG.6u0'Lp"
"i+3BI)`bQ>IGG?#,dlV-/UGhO_PPcGRs,GsCrTBuk+R6gP4'WakGCPBMN5as>1dG,S,k>&Fl'K#xUO&t>n*%=A[f_WH`1e>O+)F&FRh2978Qq,cVoFsBlBNB)vg%O1n<B,)1(BU6)w?X"
"LOqDs(HjOsj2V:6TC@9%CTO.1BpB&raUYxk1NlGIY2QI(2$CC*>O+(*37B(*BU$FsdoEmA,Mw#JPRuQ[YZ,`BkNS*%h9j.=9$[/CTRRJIH,ZAJ%JwIDW90%L<'fJG3g=lKDejvDGa$Lg"
"4KASc(+4L%H,Cc98eBQJn*Z&T[KC0-ZXJ;d;-x4-D-::W>VN^Lh=Edt]PcHRi=iiLQ$PwKe:;7[N4chQe?lj,h$r3S;Kwvt(eS_E]^JRLFch5KxieI,jvhD3-WA3Kx0G@+bwXG&;pQO&"
":aP.:T4?T&mSM5Nbcfvt5nAA&<3oI1V6Y=%)6nW$n^lhBAdSx$ZHuKGZPE?%[OcpVNZ;=IdA,DId'YG&R71FsNMdwF.6@Ds=c64]:IK_NCDX]t&b-o@vgZ`sAK<nKHtR(t'Mn20FkwB&"
"wlge1+Gl-$NFk-aXf5<*W2>mSVDH2Yp25S%GhD7%aR$bcR'O)%662AA'/5NHBqlf+As-<A'(.+-A<+O4/3Y2T[c2jY=w1hlQnwjk9*VP'aMBMBtl79&VekC*higB>h2^VIAiwJ1rrTBu"
"TYEaW2CaB&<ZXKl`cirR_k&=ULXDD%pKSC&9wb-I3$cb)cDc-MtZiX.*OrHiM(1FsQ>-BT%?%p+2hGDs[U/&bp<@JbE47(%OBd@A(v`0_HA5N'%QPrdcK$qr:%j#$AYQ^t*,nDscKrCs"
"5+6as%OQd)]^F(3BFh]tgdW]t^52c)MM1G%WcZBd%S=#%/ajX$3'H_NBZHG&B:+9.DF`<-SvWL'omNps0seF2V8hbtqY4Is,^#]4[g&2u#qhg_-sulpNI.3MDG`u$#=P`0qwY]tS#lat"
"Cg=[XRA;w$bn?_/p0%6%[YO]q6&)N'O9fQ,;XD_t*l<TsYFJe$^3`]tO-+atSpYZ$h9HDsc.[kt+kCg$UeGDsmvtIs)Jf]$CdKA#72.-`pZ+B&T)oDsK-0#$<a^-53XE-;^@IWU>.)7@"
"XM`IYBX1Gav:?rac:Y97XJ`NYh&UL'S=a;?SY=)-o6$KCNBSf,fF5.%Ef)WCEbj@?ednn,suY)I%6#S,i'%Ct]JR[u7r6[BW8/[BNib<u7k1Lq3$Q%J5]/Gs,oo*BTn:`t#@69e;Yw)J"
"[EM7@ALa7F0wxx>-c+f>QK5h1JRa2F@guL'a-8)a0*.@MuEv]UNUNwBCpNEsFZHXGOJaWsvPBUsZqGhggr+Fsx$QU7d8*7NKXB*%-Q[n$3S[GQlKf%%2#2d>;HEDsTn@pRIR+j,?g[b2"
"7n-KYRCom+iAcF^xMlP'nY5lRI+hasm?1/)i@FIu&[J9&PA@T%.&qT#?6q^i6@RnnoeB>t83q&TlpcR+?H>]S)V)h,=Ra(3'a]ICQ7;Q,O%Sl>Hf:_t;tpagu$pFs=*d&XT=&GsHUQ7X"
"mOPcjAQl1XF.IuSU1B<&w$7cs$,3/)>+NDuj;wD,/%xG,d9b5o,oSR7'E8=-V#j*i)_1[cCR:+i2HGWH'7vq-4$5R*1NmQWZ[#r6&T4B,JmqZ^lWViL;k1HM-jPAukB_c3oe1:4t70DP"
"*Y)@&q]IO8QiBL9<ZSB,kcCmR%,4/)<rOw9ps'fspAbt(Bpdt(K9qt(,nVn[o0%hCf^1Bitr#Ysi?D6sQu110JD4L,lNH=nNI+I,gX-6i9^X#ULZ;)[>KD@+,(o@l)Phm80+`:4?SGs-"
"R>,9@dt498/<$Ysv9SiL<9v]sT;3-)L:/kXDAQhTG7PJ,B;b>&:0D)Flk4)6P>2KEt._.796/t%@'/[s?].HBe-[6T46KRL8lL@&BC2[ckVN#.Ac8kLOY0F,4nwC,fY]8S>a7I,<K'I3"
"0satPBD8I,gBCNBw4X8/u>t`b_lA/MAIx6MI%r@M`WbWs$8N/31a^>&-H9>&4?jh'a9^d485L2MVn=sLX$f*1bGe)<BQj4^QS/f)Jg:F;NGL%+2wHW$YqdrsX*(%+xT#.q&kg[s(eNf_"
"N?'XmcB?$tw`UI9R;#89Z5d7gPbGIgu1rf2+=;iFa8/iXVo[=MC`seFZ8:Es)TEPsEYGFsZGb[l)gb]spZXTmB.c%t<ePo)pK2i05P%/30K-28E#SHWn3GI,.)Od3c;6a3=#,^tBhc[+"
"+V1`t0v^O0Rf06%?OSm8P;11I<$?/rK8@@+5JoCsB'(Js,YD%$MGUL'/ao]t(C&Q/Q13ctjO)d+&U]OOU_%_s?SI`ti5+^XtSIl,@36$%u%IW%jN?F&iUc]t@+Zv6G];bt;=5`*3stKG"
"?Psa%fwD,`CmAUs6eW.fa9r5-6kgVN0T%Tc7slP'Xrrn%8Bxn%bue_**VI'Ms2;'8*Bc<m<os-Cu?3j^]).2W_#DZscmdK%i:;@=wHZ[+VB#N%Jxrp=05.-%:J%*?9[(QAq-mSC6Ig8@"
"O'=OHYs-stSs<A%qDA(Evc)]+1iU?%X%NgbDe^/%iREWa@$S2^/a8-]7pgXQr8*7NlUc<m`Wrn.9F@pRY?brMh[p^'wEN'Icl+K##iwKp2',9gl&D99Mt>H97w&HgxDN39+@r<uKE1br"
"IY<7$aa?_s'<7iBF6#wAq><.LIIVaBOxcq?S^sL0_$#]$h$0vc$0O5-H,w;61A.%>7>a1%3xl>&lDqg(LXDI1x]d_*ZwDI1xc)@XZD.[s:kd'Tc@./MDodaI5`J+2Xw`e17c/BK>_?x4"
"6J3:)<L>$%=mjX$>kb,)vY=5dPZVb;pQ*Kb0J>/)'55d[bnR.V`bm'dM;@@bTC@9%p=eKss?ww403JC_nFt/e&>8CE2>PeU`#8OgE?j@n>t7`tMMCr$Uxw%-jcX+bo;4Z,<oFDsYP+>k"
"Jbob<TpC3MrGH3DIMAPgexWSbGYhV%'YQ5>Z.ditevXrfbX&dV&Kr0-P%vkGKT;`tMh6qhtnXJh8#gPsWv2qM=@52:>Q7pR3;i--;wh$Y[@8^s@hYAij.,:lG7HL04I:=ksCQVsK/,+)"
"sX#:7qI4o7^Cm(%EBgctiChw,4fg]tw$l@+rVxFs&Len7YbPFsMJ,/*^WA1MX:hW.LkeLo#J<#.5`vCM9%IW%V:)u3LVrbt'Z*L%qb.h$Bx7Hs(FiW@qihJUE72j^_8Sd,/E?p@X:(*-"
"ss(QAjQ@n,20]J_$7($Mfv&*#e6CPssOvCs9HWDs9%4:?w88I_CX$Fs(*i]tCwn`tHdGZ$c+Vfo3gqlod<(^sTupO/Sl[[s5lqut@trn%Fxe05:mIwR76g,jiMInn5JoCsRZ3csfDguH"
"g2O3P1JMqmuma0_TKZQQb_DD%%MxH&RbPsH4Gs[sbvN$kF<^ptp=<UIbFP]tT<8IC%qXGVxU>I:-BsEDE1-K#PH+Ds)Mi-iOgYX5tuZO&e?agbd2F]c?:d07*tc=5'O0<?ScYtE&Ki]$"
"+aV'sO2j1%Uaa10UAf`s?l>J&MH<10&01N'R7YRstOWrLe*H?I&6mNs@0/I_hW=rB9kOdtuGE-=t<Ha3m;pEs$Qo*DANtB&D?)KVHB#FsG?xn%A^*K#NZlt5ZeK>t;QpFsO]0j$$]?o%"
"tbWet1gl8.fAmt5U3'9A(&eOAl]x*%c$,N'JxZ&I/SqHH)6+K#rnSkLR9M.3fGr-a(-bR%MY4d;>:gJLEEL0-6+7f1e?ae18<:jK)@eNXaY9,i@4p4Y+_YVVbfT7o[OWm8ntVjjovB6l"
"a8vCsQ11v#PcR*r,c<I%Jw,HABKw%%W*)xAesRDsIxRDWMr3gt.t,?A[,bY?Pk.'Cra'2%6GA0ZbJ+U%UrL_t7%l9urbPDsSj.q)r;+4OdaluZ*x(MPI,*.1>Dx^JQ^si+x+q0Gjrdb2"
"pWOWOI&ahBk#gD,HJgsLX(^fL>qKAT/f8J,=4MXGi1FJ,/(qn.g;h11*mU,%qu55E'3<PGXs[h+4oE9&jX-&kA?&H]XHK_^f*#`tsDlDsJi=V`=nPxH-/9frq'`H,3IX$.U'aq$8gBR,"
"XdvHAoAWag50;_-X.)?:FY'K@$EQSAo]?4&&s=_t)J`e166nYP'k-9*Lb_]t.w+f$hjP^tQ6*-=_n5Q8TA+K#S(Q$$Z.'u,^?qw4BYi'bI/SHW-;hUHidmN%<vf&hEstL0ep+N9s5%(-"
"?sX)<KHrFs`%O%u,rEbF4#w?X5eGd;;]]#*=KKb$2fS2BCI&N'^Yk$I;x2.(kwsLs_6Deh`Z%Fsb$sotWg9btIH9EsDZaDsw2kTsVk7Hs#TNO8<%l'a-rcq[wi;ohETo?9M.TG=#9C`t"
"4v*]+:,s35wBmL0[)$C%LOG/jEiZs$bRMGg%'XTs=4r_t*[]ginRM3935:(*0c#?&&vti@N`a?5T%bUCD]nt5CQ9Es@*NR7$UmO]=</^t>`TI$`oeD&I2g@&GBOtl39'o/8@o@leE.1("
"&Cq*i(JTq)_#=+i`L?[H1<0.1SOBPsxL5/)EwNL:HIo/1/N@u.;0_7@S^2H&.fMS,GM$>QPKQK%K<_[4UZW$ukqIds^MTasSiG?#n:*7%nvY31)gL,%ol,5E(3<PGWpRh+F$q:&e;jS["
"xFs7XY?k'^fwx_tstmDsRG=I&`@9FDF%k$O8/Fer&sh;%>p@;Mwx61K,lO]uaXGh1TF,F`MOZO9sMV5*mhugLh5Lts<+Hi_9mIVM?qugLcc7?R4-j?&$iRSqC%QNs[HaBIua,Ns-`sKp"
"9sbA&H<B1K=mW0p.G_A&:krFs3v2-)A$0-)tM$,tmYJl^hY5lR@HcYsN[FU%q7)B,X7WU?[`L%u?o5cs0[8uAXo04/q[UaqM>lYsT4JI(ePUI(A%moR%b*r.nNTet<WL&4+@e[+w':J%"
"V/-i$KZbKh`cjvtupde$vPZI(KNi*MhUgH@JEgu$89OA%5.0eVR`[[si(OdM?j1*kK%$@&K5$C&U0i_tABqg(a?EI1dIHI1pnHe_1KWDsCXWDsx'n0u'=C=,PS.T%M''^F4W;now#OA&"
"KfrFsI5Q&tLn*%=eP0^Fc^G0`Bct.utI53^nM+Fs;(%c;O18XG'J4CHYSoDsQ=dc-2,A(<:.Au,E8[B4EnEa$_RjOs8gP^tsCD%]sX'^swA6lLxV+dt4Ks*DnY<ZQAaSN'rRI['A-GLs"
",Xb'ateO(tg?Y)ji%0_k_i%sZ^uK%uDO7<&n-fnn(2]T%71t'G)QWta]wVH,j+3-)#m&p7o_][sQE]q)n9n0@N?h)W*DWcs0b.u-^u]kLm-xD,BI9[k4.K$l8xR.LgAqreB,Qa4@KnQ."
"5ClxsLFdw)wH@L,RMg@&*#p$F4wT<7bUNvliK@I2e:/L5Sax'<X=;pJB=TgLCnr&.1MtgLM0pr4E_DU)LKbq;`fgX#;4xofT+Hq;*cv*76w+O=b`3TM$U7L,3m4U;FR#+M?O;B,TE'3I"
"_LGM,iOLt:xjr'EBX;B,.#;Mqo`_Ve`GSe-8D/9@ueJGNFD[csW[su.(qk[3-EIqM+Js(t*wmi2--1]3n3#g`MEL-I<DMR7Kih0,`tA[ba*3C,(c*CW?#cC,'`]RLh9Vasb<I?#fNh;&"
"#cL@&F%]0pC4VC,R5TlL^kQ1M`M%I,alHnnN+FxskghutLY6N'm;+H1P[QwPbY2vm'nn?kKEn0ht80U[4-)<%X33_2,[jebVk&Gs`jD9bt]lP'3PJPs`KrCs'Zdt-%Kt?kN]rT#l`_Bi"
"qCv7tx72`t@0Q^h?lK8/Z0(AY+6@tLNGCPsWU7`s/g]U^kb+(*VZQ^tSuDUHN.t20i&2h)H_S-WEYN6_dvNq$a-5nCC:A?KjG:Y6ZZ,lGY'pq-v;WRXJA[thx7g=&_&aKCBfJI&g*O`:"
"2#QJ2.hP%%3tfh$=t<`:>Rur7E+6H5*J;?%Y+<Cd5cFl1au`=,`)$C%aeXtfXqiop/18F&@(nt>00e[jfIjdtag'Dl#t_KCLinq6Swm]t)_nJ:t2]2`5C?@+513L%EIkR>($rf/ci*Z>"
";SF)-&k0W?xd*o,VBJjo,=$1ejOG`t%ka[>J_v$M?[o)t*c*@lFp%-W`hV5SNsiM>rs;jOh1:GaJN0.(bRFL%#Y5&TqaM%?rZ.F.lNY>cT-`g%$eE&%.`FJ<0^ddtIY8?%klFSMwgt]t"
"9*iMm@Ngu+BI[<L)rMnWmHCo+#1kWaZ&30tkEwt)gN`]t;Vjdt?CC_$j?HDs#B0b$_XEI12h8kL)DHIsUpkh0>e@9]a7jV7_<SgVk:/(4&A9lLCZR&%ad6F&T^iI1mVgethp&1Be'OaZ"
"(tK#Zd=#B%*[`=-oF5,%9:LZVon8N'?**DsEKkrHmubU$5>U6o?Wt3J(4f0Uw09:6S*$l$m*h&^)/EEKoZ2m,g,K2i>JX,)<KFXB@KPoA8lhSD^<8d>#2<,b^LZSD(vAK+x`;nVU4Nc>"
"h6ctaYh5K1&@SaKag<H]+>O2-eOun>QvD[OCg:>;i1Pn:*[KMe:igieKRVb,]0GcaH)@,P]EDN?*2O.@,A8Hsl>J&$.maM,0(xfO2?D^sJ^j;6bc_4pb+:YXVh,25KNMH&1pPS,AB<?&"
"s>8cse0VT.]US+sR&W%@0njoTTHU6Z=)db24HL-IRTn@Y*_RN,_^1:&QeZ)j3`Nj0cF<KCMpYG,soKaQ*4-Z$orU5/9l>MCu[VpL?X629(@)W%]D#W->Pw9)ZYKkLQU'Z5>AdL'1@Zeq"
"2acD&JmOe:Y>%Iqf=f,ttNf'&ARZ<&GkqU6P`4[ti*I/tD@A['ErK;&U.JHsnOjb)'Yeb))CaB&?^COAID1`tFU=U%2qfS[$W'6Wie^CNJSd/fNk.CW%V9x=J,fF2)AaK%l&YKL,0ANI"
"$%TP'YJVO&>V#ajXlT%ku,<UQ[&bL'?fYn]gEEkaH9I`b8F8TbZ6Y>cGBZb,Mt8(W&p)sdD0+5-%k_wOT.a#K'V3xFn9m(%UD='JI#`LKZXRMLM/fAGNRWrLHBC`Jx>8w>i'pFsL<3AG"
"_H,FsEhK#ua#rFseZ750fW*H&5YQ^tY4GPsk3Sp[L;.wsf?cb)DY;(WIlDR@;4/+)fLgN%/$GS,s<X]s]dsFsD(Q$$KvCC*movct5mx'3]jTUshBK+AF,odWK5/mfAt^a,/T*Q8v#D'-"
"aWIj90m,q,ht>/hS)(6-Q,+UD$G'%+T4RJckD0Us'DVI1%g>ntG7geta,get$UAv0pjGnt@vl#u>FX(kV/NDFe;3R90<f(WuVu$4H:G`saRJq$CZDC*uTPwH,`H8%KrqM,%_)N,`h2+4"
"#gF_t,4?Vs=r4f$w)a]tRs#c$j-#(A#$SDsv(ad$mx4aspXD[Z<p0@M=ReFs:a#$$:^wKPaVg]tYeWltdQ1g$=u,eQo-Mn[d=Wdtud-KsKiQO]>TZ9MJ<=GsMGT#$7q5$Gh+=J,cJQN'"
"UpcX4`pNhq%OOE&#[Hk]B:N9%Jlr=<KJ5lL/4-T0F?KD&GZ$5Ki@G^sr2kTs]dsFsOXD%$PL(>&,F=a3&`O#%Pq=DsDIf8q/6(G,Ea?r),2avo5(<kWxmjjL'ku.%%4LwBAsHF)+64k3"
"1w1r?IokYP+-8ScZh#u#]L6Fck@7`%lAL5%^/jL9eZNwkcSrMST0J?#`#JHs.s(L)#0UP'rta-IIsCC*'<vNs#%d-hHh$FsV,.st<K%-udH=esYK@<Z(ZUC&,7TP'QCR(tQEwt)Sk_]t"
"<M9ut=:io$V[GDsgmnh$89,N'`5S>-YTTD4mmXatZ4b-_$qEm/LYGHsXE2i0I3+oL.)`x0(=&Xs7gmk$`(&_4&v+N1@<)uL&kb+FSuX]t+=hC*]M2c)IUU]t@+]qH/M@.Li:BDs9s,10"
"'wik/u4%N'#0*M,SjUG&1vP^tv*OI(sqi3(tiVP'rviq6<0xC,9dKN(PT-;&/dY.#p7BN2PR?Fs9Wm&0MIMX-W/<=&<3O7/YdXIs@$MS8hWsvmxaG:&<kjw=f%'jLCo*hs(cuCs%`_Bi"
"xc'0*7ow*MRl`#U:]Mr@W<,-I]5<`t9(^t:@=Dcsa&<V[1=0I,r_#7X?Vie(G@mg(E5gh0>,Q=>;R2.hrdZiL>X,48]xl(d*82IL-XM658*1Ms_SiI15rlFsmfI-%MlDJ+ev^F,V#HaW"
"c`l;-=?<Y$37F7F'u$Ts_Y650MdT3I(wQ=&Qe_>&;'Ap&+#Q<-xn7Y-Fcj$'tv(E&5x$UnN)/1(R:-B,/<j*Md*Ke$YE<>&r/Bf$UvX@&/0/mL/8al_jk5/)=x3/)CWbc.ZM^,if(inq"
"'r1]+_73@=BR/(Wo.l.bTEuh$Id5N'w[Lumm2A[sBuMWsAEEiIC('fNv'k8&Ok&9I0I,;&g/:Us?jj%G(IxF/hBq>c<jejLCTq#E$)4/)+%/-)9ie'Wei8&G(`TwL]<kBMkpOWsihr1L"
"4,1[FE1-K#ZAaY$U*t]rv]`S=2V*I%YWHh@sV/'h53M$.,_B/%GRh>]vw$g,kr>DsACEm)aH`]tWPIh$Vs4UZ*@:$%FFIeUwv0N'w&QI,D?t<Y[9FCNp$I4]t)oFsO)3^t^@'Us>4TQ]"
"QADE&Q*qD3.D.F,2]rfsR=RN'^t>x4:e,;&9EA=%&@wC,O2a5IQos;&Q$F$0wGgwm+gwENI&B_2IM.m0?NjA,vQK8MK&,BIQ(YQs%4QY<da<kq1m/lL4E,R,UvcofQU'?-NBB@4U>pP'"
"HF1gLJLA2)8C4/)o&lm<x$ZRsfZaU$&`0^ttX*V$2:W3N>^kh0EC2i0aFPS8ssf]s.Yne(o-SE&/r0CE#5#Q8;>S/h@_Btt9N'Us'KQD&Q@-APPE&ps6Sow=EV^@4+VUuRO*w$CGl8q%"
"1YkDshwbb)$L'Es^jL3'mW9Es=i).L_&,%#$4>>#w]q%4mtar?dd=SIVet=Ya1^.hvt[M'`IYV-K6aP/.mlc2Ugni9)Utr?JcFGDrqPfhOTwi9+&gl]PET]bhjm(s<b8m&T[^MBZtM>G"
"eiCDa'IDGi,Zj;-OsfG;+I=v>KfbGD<U:&OZfC2T=]#m]P-ODaCSVSn'>Jv5p@2a<70_8@g)qPJ?k$^O`1I/Ua>M;d;;`SnrV+#u9@<T%aL:W-_c:a<E/&NB;9]8RSmU;d)1-jgO@F2p"
"&&P>ul$%9.ZQVp75$Us?NcFgCkao;HV`$NT1v&T[te5jgQ.8pnkaH,r+5Y>uM^CK(mqLW-8#Zg1U0ds6r.6H;/Cf#>?N?T@gHAZG;F(ENS&TsQo-^)WIO@g_f5Q#c'+?jgS4Apnl^6gq"
")#,^tKKlj'jC^^+&bnp.Cow&4^a.68u1$-;/=S^=cRn*JNbdc<;MQ&JdNf@%Q=Y5W[WC)Wv>71-<*6D*IhS>cN&*c,gZG9&&i;E,3BUQ&Hr^b*D+7=-Ax@T%k&od)]`@@+Y__]thg<xY"
">$`iiD#.^$TnKb=ZUc3%B*<ehl*cC3/86VslSB/%A8UV.(H9asbx]+g#D0GsJlnFs<tVQ&g^;.=j?'E%g/xc$QFbH71J_)<5vR(-XOAf$SS9^tl&7`$E@_]t`m>u87Y5Fs*=pFsv0])a"
"NsX@=OnPGrP@hVQtO#aSPRJI(p:]x$Q/.JQNoc;QDFxi,g.'&%;#uDs'GUj?X3;`tFOba?S;D`t96bm`Lb&GsT(Uv`gdoS&#`5wtim9bX4R#'=xTU$%h('Y%@)Qkgm0e'd+G>/)=^cpa"
"BbeFamHA<fUIW.1OuR6:6#PA%D+/@eu)5hq-EJL<rED?%Q$_Qh/7:$%oW=&i>w6vtQvq%*JR/jFN?\?0%U>:nXLH)^X$ZIl,)Tv]XpBf+-isAmtoMf>-dJf>-WMf>-Kr-W%:Nuq$g-J,`"
"?gCTU;K8kYX,<r[W((.bND[[brASF>Tx$C%AOqaZ>bb^Igi/K+C$#(dWE'Gs9CLocrkG`t:>oq)&5,[KRV^'%R7>&T2J;mSX6lj,t,0mS+gC--co(6N`Z-pMCTS#M$D9vL]?1sL%N)[K"
"N,?p2^]w-)VbWS&;a=J,1k,9&Hu)C&2b+C&L]2C&gnuJqpj=<0OeoQ&C8<:g_CgS&8`8gq175T%x1TP's@,FsN++iAI6n],Sme]tV+ua4w^Mp7_l,+).5@-%L@,,R;w,Y#Phcb<o(<um"
"1>=Ds/L3Ls$DT,-T*<7%op_=LTH,nE4qdh+,L2^/bC.>iU.]ZU=:^*`Y'_TR+(.>LcGsMB&.`*-,t?t)Q^-pe__j:$w/P+%qodB7=m=A%T5mlq=9n%%#jMiPLa^bGcUsX>#Wc4&KRCRs"
"tHUU6b-JHsY4CB%BX*7NTsA/G8^v_tcTDqM)r6Db5n*,R5TPbD0U3e)&NxU6s/`iimWGI1%e48nNJ47.r5Z>cRDM^,Mm1jKx1kbIEM4Fo7aof+`rZW$wqqL0#(uBAf[%K:g/[U$rsF+%"
"SLI>i8QjI1v9WqMED<`tQo`q1S-UlOD<)YQF0x7W>s]kLo(.FsRR>hgOd:T%=xT^s=,pX#GAIs(?@*Ue_J@I2`]ZcthmO#b(1d2-nj(:ZYl%x$f?XXckB4Xca@fx$Q,$V?gt_q)NeInt"
"AAT&t].UQ&2O'`3+`P?&qwce:9I+t$ClR(iJs<Ds+R,YsDkkb)RGZ.(OI0k.x63]D;SP95J0@5b+e^/%/ZaX$v&;mA:[CDsMT&-;NkZq4uJF5;jx='-DMgY$@Tq$Md(>RI)O68[Pp(EE"
":NJ#uZpo#Gt/IPsuZUQ&[C]Q:BEZQ&W5sO%@<ph$P]F>%tgtw4v^Y@UA_J(XRo9ebqwD:Q_$_k;$Pg=%f)oc$G2c<KPf3tt-=tbH<C)M+TLh?%l77v`M&=DsEX5Tk(RB/%xO(Q&CY*T%"
"^@0D,P+GvGQw#l,c4tL%K(5mAk>47.W[o[Uv>;S+/)NdAL';`tqN^YAcM#s@Gb`d$$?6iL.BMgt5PrxJQ3jM+2[2L%7e-+],aHs(1Sc<mVr^b*;uubsmEO48T,UbtE@fHskA,i9[djlt"
"ZE6%FZs*=G7m64A6T3e)6op>&9(,SoQ*8QAp>tDs9mGeUGF1Tn8nj;U@vjw$XBCdHBP)WHOqgk,P1s?U6*oS&h<(;&)&XOS138OSR;o/3lXkS&_kj=,q)4cs3_.^t?JZ$uWK=esCVp&t"
":]H`OTRF9%Qcv18P)w`tp-]Hsfd/QAX`I4J/QxP/Br'jt9Lq8eB'T,YRN]%%W[K(@J>;Q&U<%Fsah.dsq&v;%(ApI:vDN`sE:]Hs3^.=%t6QR.b<8*%P7P_E/)^X#>Y(_t#7Spt6NN[t"
"x=$N0Pp3Y#-&oA#9fWX%ob;_tIhbb)5-#N'_C$#uq&Y,rS#q0KatQkt06cF%BUI`t&<rS&40WNtF'FXGQ:9sar9jf):mB4JBOTk;t50f13pQ>5K7)Q&c+OI(^w71'GnIdswcnT&BB%4A"
"x;Jk4#r5*X:-`=IWDaH&;Fgn7v.PbsxHlEr3oiOsMNlI%-gdNsM:Vt,aX^5UruN>dGK@I2]/R#24Q5$*=D5@=-pRFMw+9m8suuJs/mV,M^o_pLg[.Fs/MuR&S=d-=LJ-at,)6S%JOcK,"
"g+[q2q(>R&PffSchI4ReBj]x$eWW<9hXXBKulugLOgfmI5'Z2T@(es$e@0M%QcpKHdX[;HRd=,-:5erUwZe%c_[=wUAN6^tM2wCsZvkIs2f3HV<vIFI0V-eKpbX7*bImFgKpWkOAV7a3"
"fvO?*JN).h:6GYsn4r7uYhsDs=h;=c929j9*L*%=Dm+`<,*Qjs*5W1'B5(+2kQd`s+(.`$Y-1@cu+6Vs7ecX5`6'1Ut#`OswoLl,:?Yn3w2jK,cf2sLVYTHs,u/qi]rdb2#Z=I$XY2UO"
"G`-FOX($Fs8L]ptVbe?TS.PDs<BhjAB3Cg1n.WD%Qh6fYf))w$nOfkY^0q*WldU0-j.7*JYFdS&Z$Km/x5W]tQSdnt1j</%[e++)9NYW/8D1`t#M:RshE>T%wo`Zs&lhRsvmm@+WI+t$"
"Q#oAJ<EO8R3D..-PeTa:QsEn<&h`B%g/9ntM7$)OrEQitA<(d,?+[bOIMaX,O;6asni$%+Xs%Eso&9csZ4xDs_mQPdxqG`tcR;O,FX?-D<x?ctYXJRSv?[h&=kpJ%l`R_$a+NL'+xi=,"
"`6)-G477W?olnd)LmmYY#<B>lMAoq-gd7bi-NUN'JLkiFG@XE&2A7X+J<S08cWbS&$dkh0pOlk/JrAPs?6vv>he@Q,-.QO/dvXR,$t8@_hhrg(E^g^+6F#Q:b_6$gcF]HgD3hj%?qpii"
"U#k2gM04?Ih-c]s$q=59RLDP,^EVRsjW*opmWQ.%DqoVH#/ajKW6AF2IaPx5b-]Hs6#ZURgDM7W`9oHsR=cjB/@/*<([_R%8LZ21`b&Z.OM5FsIPP8.uu9o.qNq/1`_F=fIowcVfH@p."
"f;&V-Vf@st5Z$`&VdjP&EKHW&<=0+)-]OOg.F4HVHt%'.?EjjL1Df`tw*%2tCV7`s3:SgSUiAI&OH(w,ld=Q&,D)Q&Fd>ko#T'hL0FaR%XAdb):Y5i&l=db8+^1N'KQXP&ODdR&pWi*M"
"7xdO,j0CI&:(=kq%9v*r]pa*E'gpwkY.VC<%+M;Iw:rl-[eAX(6el1%]f.x$1>uu#$(+*$vYe[#.3,b#Tl+Y#KW+:$38a`#a$N%$-6At#SK1v#used#;a(($N?k]#WO9#$&3)mLT%BH#"
"UO*rLHVOjL`6wlL<3CE#-BFe#eK#lL$A2pLVQF&#1:3A=w=+87V/CG2aF5GDe^t(3<KY`3u:+87X7kr6.t[p&CJYV$)J$d3Ami2:U$gf(&i0^#^>;E/OL7)$[l](M3FfqL6X+rL]<*mL"
"8e=rL4LoqL/.AqLw.moL(`%QMuZ6+#QcEB-CL<B/&wm(#P)D*M*kZ'vSSLpL;hG.#T:-5.%uQiLmY0(MkIBvL#Tsq-rD_w'S`df1Ue.F%b><R*dwr-$g)p-$/-j--h4K#$:wlcEuteF4"
"QiEfqO5qoIis-s6$Qe(sr=F87FZGH*xqrP&6A@-dA8,_]gb+p8KHvU8oY+###K98%fuZ`3:/>G;.<EM9itbP9A^jD#ZZe[#VEs`#uMGF#N4#F#Ua+u#_r(G#M_(($JI+kLfJ:@-G^_@-"
"YeRU._hmC#6F_1M?T]0#Bte%#XXq/#_GC_&ZAx-#]FuG-nl+G-7&Vp.ork.#'ScQs%TcQsh4K#$lsmQaVM3_SCu:Db002/$tDtP'/'x5'1S0fhV@L]l%.UwTu1HR*xBG.$Za^f19*',2"
"WJ@PA3.8X@u'5;6X_XxXaXKS7n8H-dYQ'/17:R`<?OBD*c@UR0`oiLp#eG.qoGP]=KF?>#tsh;.Wm`QsK8JV6.Fv.:T:W`3'dYa4_9?JCC'j9;d5'F.1U/R<Dmk-$v8fo[=s%:rwO(]$"
"#=.D$v(gE#q@uu#d7YY#M2)mL8'CE#w1)mLb^j2M48'/L?aZD*GuJ`NVcg9;m3N]F$mk-$_Z-Mp$8Kfh;b6kb+*PiT5bkV7EH:W7;7&)3Hm_A,@7_,<Kl3,M,uhP/hqw(3wR3looo.Dt"
">5)d*KmQ.$oaGV?(]P8.VGP`<8,X`kkJm+ss=F]umIg>$)S%pARpxc3GbL50#r#Z$@x8v-lnP`kYXp&#;JU`<V>:&5h-_M:2NgJ)4aG,*o2Sh;CqN]=-]),2q46D<N7m-$'FF_&jO@F%"
"3+5#57o_YG%:fY>0,nk40J2F%rIm-$uRm-$WPl-$qFm-$sLm-$7P5.-e90DaNVFYPHwO`Wa^p(Won%)X;F5F%;FDM9GPNV6n[2F%1X:R*@%3F%O+vXu+&6A=kkI_&I@3F%RBo-$wXm-$"
"O-cw'/x7A=GXE_&BJAD*`12F%=J2A=$&-F%VOr-$qFl--ul1F%Ue3F%;l2F%%RY`3osWV$<5>G;iO_Y#e8hi'iIC>#8R0ipZ)su,MEV'RFlb%tm$F_JV=b.$b=tV.x[k&#=4-ciRc/gi"
"gIF+rDUuG-3jVfPw#0j0kkd9MfiIwK`lKwK;BIAGuAQ5g*l3DW$Wk@H6c>>#-(35&d:_._GuI=BddUS%f.<X(G)P:vc>,F%J8YY#e+<X(Riqr$s<Lk+O)Uw0hB-3Bf&G/#q7'%H7(GnC"
"ZIV-#[2+7DJmSfFi?f.#)1+cHg..*HfoWLD-d>-G#6x+HN&$;HE:]+H@F48Mf2eTM$E#W-R?3L#tNj-$*T_$'.0a9D,4$Z$1X;s%*=vV%$]'^#)4Z;%(+?v$4*J9&I2<A+pZEM0svK_5"
"aiP`<1<ZlAQRdxFs7)4=W$>%NT.x4AqD6Jr(dnA#=Z#<-+ncd0(Dc>#pIlY#<#^gMD3pf1XjA,3VWaJ2]8YD4-;dV@WT7gUgBlY#]MGF#KpvC#QoJE#[rA*#VG8=#jSJ=#n`]=#rlo=#"
"vx+>#0uL?#>(`?#B4r?#F@.@#JL@@#NXR@#Ree@#Vqw@#Z'4A#_3FA#c?XA#gKkA#kW'B#s&_B#&9$C#3v)D#L`&*#JQ)W#6;E)#5mBB#28ND#Pl8*#7sKB#$e9B#.E6C#4ddC#6^ZC#"
"A2ED#G]/E#';2mLE0K-#>-5'^sQFjL_^XjL*3$-N.3CkLj>UkLj]HLMnc6lLoi?lLu+elLpl0E#'GuG-kIvXMQ5eY#N5>##j+3)#6<G##AF.%#h^''#H$)t-Jpf,N<`U[M?j@iLenl-M"
"BbY-M3Xs$#`EX&#E'Su-3xTHMN+UKMjcM7#IU;daO_ZY#r9Lk+L-RL,Fd]q)YWf._,oQS%HaJp&rn2F%k8m=c-l[PBMKiB#uqC>#(1)##TQj-$9PNX(j1AX(,=5Y(6't4S:cw9)nh';)"
"5.V`EU+voJ8=:^#XeBe6iHcf(#75R*mC5VZC6JYGV.+GMa<=5&CW.&Ge8hqoUXxLpYqX.q^3:fqbKqFrGP[fL.udY#,VC;$0o$s$41[S%2;6X1En8X1=LIP/C8`I(KsEhL'V$lL,Yfb-"
"dI(RE*uAX(/jYKlo_OfL`/OJ-O[kR-kIm-.V.xfLH@6##%;#s-]FFgLJLH##);#s-pW6iLN_Z##P,KkL@+o(#^[Z.3j`-0#;^,3#CR?(#jQD4#*J(xL-7f(#6j=6#G<k$M,C=)#9b:5/"
"+B[8#h-vlL@`YgLLF6##T(ofLswSfL4aZ##Z@=gLIF?##(;#s-aXbgLMXQ##n`ErL<?80#-;F&#x&/5#@[n#M,C=)#0+B;-Vu;M-PW*+.K^jjLO:-##dV3B-_V0K-,.m<-$Hg;-2Hg;-"
"kK`.Mc$/>-8MM=-JE1x$Y0i3OCjr&#pcFuuEr+^#q&poosJT[03,[w'J-Z9M?Bb8.-Ur,#pA*^#pE^MCRRZ&vkJG&#]/@J(d1W/1mW[+Hc@WEEjjf.#r(1LM+3SB.7xL$#]WdA#FNe_#"
"&>uu#pk<UI0;C>#SX[V$,h79&TYDw$nwk3+#E>O#%*fH#D'dN#Oo$S#>HmV#&%`Z#')Cf#BqYj#YHKq#@<bx#%Zk*$PWi0$'jiS$vtoA$d?#J$=%RO$(b*X$cge`$vK>f$e2oh$lmGn$"
"*Uws$<7P#%Or))%cVX.%(TV4%01I8%Gwa9%xOS=%,W7H%ST5N%NlVW%3)$_%FdRd%YH,j%n6w4&*i4u%=Md$&P2=*&dml/&.H^6&'gg>&R1qF&r;TQ&_gEX&fq+^&TcBb&+uB/'0*Is&"
"*KR%'nl[-']:f5'n>I@'&;FI'Z.]P'74AX'N>'^'QHcb'LxTf'0<#j'_)9q'4W*x'Cbf&(S76G(bC=2('Y`8(6>9>(5CQc(263P(rd$W(un`[(xxEa(:S7h(]5gm(pp?s(-Uox(,/b&)"
"XRm(),*>F)RA,3)vGY##X</=#No#;#]b;<#jTS=#`kVW#I@Rm/PWt&##gv$v>)A>-;(A>-WLXV.@5Oxuj_98/<T'#vq/xfL)Js*Mw1N*MRrgX#Z6>)MM8X<#xvdiLrx:#vReZ(#Mgi&."
"#&AnL2b-+#+I5+#ZPQ9/d*,##jDQ&M1=Gxu^cDE-QtY<-f2'C-;C=?/X;:3v:u[fL9Rp$.r$ffL3fm##bU^C-nG5s-@=;mL.L6'v64(@-4E]+/pAb@kk,IwB(HB_/5o?_/b]JwBg^o(<"
"C3'K)0s^?TEn7X:gFC;-rO%XU?'&s-fp@8.Z)Uq2=Wk-$EE>G2rV@Jiw(#W%SZMk==Q)XLsxRG)P6(L5gWl34aK(44wfpI_GJ.W7jF;eH-4Me-P-'2q^6ccsf.<X(:%[c;.p<kO*3JqD"
"vQUj;>RGgL?k'=#k%a<#O/&X#`S[bMH-fiLqMcxu+=8F-#Di=/&[u##p,4)8HSe]G+nW2(P%?p&k8ji_56.2_8_[JVMDg9;>14_J)hk>[V0YsLJT0K-Qw&$.F+V*M$WfBNrp)*MG2N*M"
"ufB(MLH[V#[4u(.'eBYXIUY,Mn2kB-`3kB-K'kB-K'kB-@M#D/cCP##HWajLFq)*M)p)*M2hDxL?AC2#7jc>-%#$)dTA<$#iTS=#laf=#&2UXP?A.*0NZ9#voeZ(#B*#O-6'[6%acuW_"
"DRQo[nF5gLt^e;#6h1*M0(h(Mj6OW#,R2W#lA%+MYOGh825sS&iRG,MrdbjLV_D/#(/`$#.G.%#SWT,.$xE$M<k,Q#*k3$M$5[3#+If>-xa^C-:teQ-5U^C-XV^C-d#.>%+at%c:1--M"
"b[4?-RY4?-g&JjLek<^M1@1p7fgMcscXl%l:D+2_Df)T8[C_#v0HG'v9NP'vIs,Y+8>BP8ZHN9+)5_f:>2Q'Jx%R'JG3b+`'N'G`X6*w;qcCR]pF5gLtx3<#sNc)MlC8#M%Il$ML@m@N"
"vgO483m<n40L?,MY>m6#LB58fmh7]-jfCjr%(=79,V%KsPRLE'ts]R8mP@E59f$[3h`dC#5bQv.F8vt7m0up'*7XR9+gK^-q(ld:8OVs)/DmJ:N4Bg6HGl*X)<x.XSbDA;;H73)Yj>79"
"fK@l-[e<>-.gps$u9QuYX(i:m3*UG)N]]Y,^YaEl_>QQ#d;eS#H4J5#9qeU.=Bbxuw6)=-=2J3%%)]q)SOi;-/hr=--hr=-?)l?-A/l?-75l?-*Ws'%=p^V$X#-/(3Dl-$?DB%t+HZcs"
"eRhlL:u[(#Jlls-h+niLa9xiLAD4jL?.gfLSV/+MjuxbMm(^fL5V/+M`ifbM52$-NG?6)MG?6)MSJ'=#-a%^.qfv$vrg^w%x4+gL[9^7N?^&W;'qfcN+>Ncs',n+sC+&RUS=L=#K6Xxu"
"%^9#v7s1(vx3U+vU>dD-:)%:M2q)*Mc0hhTvOLp7Wn3^,U<q?BSCg-6.>rEIG]Nk4dDn+MMU448tM;s%K.7>d_M8p/#Cjv%IwjO#5JTp7Y]MW%Ws,/(nSEU-Q>:@-'f:@-X@:@-OmbZ-"
"%XZL,;(<r;;naq;wNkl&2Ou3`>Y_A&`Yt[t*_6kraaxFO@u,wL_'2hL$m2?#g]Gs-Zr[fL7x2?#0TS=#K)hW7nKLp^C2-t$ZiY68YnI,t9KA78pBJv$Z^tg$ZUT9n1_Nk)&oH68tEX*&"
":>'58(mF+/$Q59.G3Oxu:WHU%%0vb<'/###&Mfr6ae4)*mC8AX)E4Z#C?wfhm(ls-`j46SQltgC4PWNKGb0*WK1i6/Lr<*W#roNK@&TS.^(;ulUUm-$x[m-$#`m-$$cm-$;jqRntRN1p"
"c'n-$:B_w'3R/F%4U/F%5X/F%6[/F%8b/F%9Ln-$MdG_&Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$+,r92Yp0F%VNo-$WQo-$XTo-$YWo-$sMp-$tPp-$6IAul"
"c;,F%f@78%c,Q]4)h3D<BsaYHC&'vH&928]'BMS]3Y>GaOK0;eIQ5se;[WP&C=5s.DFP8/EOlS/FX1p/GbL50HkhP0r4v`478Xp/@80VQ5,Fp/x)0F%?_n-$&jp-$'mp-$/$kl/#.3F%"
"XS5VQp4Xo7.:g=Ybvj-$,$k-$-'k-$.*k-$/-k-$00k-$13k-$39k-$4<k-$5?k-$6Bk-$7Ek-$8Hk-$9Kk-$:Nk-$*dOw9YG_1TmIh9;Dmk-$Epk-$Fsk-$Gvk-$H#l-$kJ'F.K,l-$"
"L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$UJl-$VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$hoJwBo.KwB=Bi=YV<88%h'<D<5fPGa]?O3b'=O3b#1O3bI(]M'$4xi'.*O2(/3kM("
"0<0j(1EK/)3W,g)4aG,*5jcG*6s(d*7&D)+8/`D+98%a+:A@&,C=5s.3jR8/EOlS/FX1p/GbL50HkhP0J'I21d#fM1L9*j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4U5u`4V>:&5"
"WGUA5XPq]5YY6#6ZcQ>6[lmY6pW-n9)fp(E+e?o[_>c'&ct+/(JX=rdgcC_&w44PA4D_1T&5-F%08qI_/8l-$L`MoI`YNk+<.@_/se.F%T,Ik4x[m-$#`m-$$cm-$ZOvL^fH/F%-(n-$"
".+n-$/.n-$01n-$14n-$27n-$4=n-$<+5_A8In-$A?G_&5a,R<?_n-$MdG_&Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$THo-$UKo-$VNo-$WQo-$XTo-$YWo-$"
"sMp-$tPp-$'%dE[w<Fk='mp-$67bw'AA.kb,&q-$M-X-H=B=w^CFJ_&]x,F.=Yq-$`+-F.:%SoI4'@o[V])N0?$`vu(*ivuop'hL.'2hL/-;hL<3DhL59MhL3E`hL4KihL5QrhL6W%iL"
"7^.iLPd7iL=j@iL>pIiL>xR0+jNOJ1i8+j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4U5u`4Eb<&5WGUA5XPq]5YY6#6ZcQ>6[lmY6h':m9$n.?@$2N*MqKs*Mm3N*MJnBX#)EJX#"
".^oX#4M#<-15T;-35T;-45T;-55T;-65T;-Oq.>-<M#<-=M#<-a+l58[INcsoRNJrMP'F.M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$D]Pw9VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$"
"8G'crbQWV$j+ZS%Q078.nB+B5Q#RV6d_F59fq'm9h-_M:M^mcER)W)FRSK]FD/B;ISo>8Jm[DGW(Kio])T.5^+gel^ab,2_2P#,aWKoxcL#%2hYP@MhL5nih[`X^?/NVp9>9PP&t;sr-"
"k+PS.Rd/K2AUac2YMCa4j2T`<RMbYHC&'vH%0mr[>,38]+NMS]<U3>dEwNYd?q/;eOjGse%t]P&Qf<'f<D@2'eC[M'jXwi'.*O2(/3kM(<a0j(5QK/)3W,g)4aG,*5jcG*6s(d*7&D)+"
"Px`D+=D%a+>M@&,?V[A,C=5s.3jR8/EOlS/FX1p/GbL50HkhP0J'I21l;fM1i8+j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4U5u`4%V;&5WGUA5XPq]5YY6#6ZcQ>6[lmY6]u2v6"
"6W.n9)poo.eIrUmM=m-$?wxUmdJp;-<R)f'sMol8$lNVnKS4o$R9.#,A:7/1QPol8o?XVn`bVw0WPl-$XSl-$YVl-$ZYl-$[]l-$6#fIq=)^Y#?Q=8%KwRS%MVo,bv5]0#O_.W-T;h,b"
"<u63#]<Kp8slPGa2d1R3#`m-$$cm-$P*b5/=.x)vlt6qL;.AqL44JqL5:SqL6@]qL8LoqLj*m*v`et*v%[<rL=kFrLwTi+v`/'sLE^_sLcdhsLGjqsLHp$tLIv-tLJ&7tLK,@tLL2ItL"
"M8RtLN>[tLODetLTc<uL4jEuLZoNuLWuWuLX%buLY+kuLsmMxLtsVxLQj52v.DI#MxTS#M*go#MA$M.&)RxCahqn,bv8]0#bfF?-p3+K25rYN#CZhR#2BiG#6U4Q#7(m<-0M#<--5T;-"
".5T;-;(m<-4M#<-15T;-35T;-45T;-55T;-65T;-Oq.>-<M#<-=M#<-F@Rm/](CJ#>JMO#6=9C-D5T;-E5T;-F5T;-G5T;-H5T;-odF?-pdF?-L5T;-M5T;-N5T;-O5T;-P5T;-Q5T;-"
"R5T;-T5T;-$eF?-V5T;-W5T;-X5T;-Y5T;-Z5T;-[5T;-h#jE-3e,e5qLo5#@3Puu8Fluu#Pc##UHtxu,P'#v5A<jL)`I$vR:^kL,44%v*kPlLx6wlLdgjmLfs&nLh)9nLnMpnLL)doL"
"Z5voL#;)pL$A2pLV%9)v-(o)vun-qL.(8qL;.AqL44JqL5:SqL6@]qL8LoqL=kFrLWBbG%/tRw9Ckn-$^IPk+Ftn-$Gwn-$H$o-$I'o-$J*o-$K-o-$L0o-$M3o-$N6o-$O9o-$;h$F7"
"Yp0F%VNo-$WQo-$XTo-$YWo-$[MTw9wr1F%tPp-$7alQax/'<-<Z`=-)7T;->Z`=-YNXR-R9_M9=ulca@FD_/=Yq-$>,1W-LkD_/I(r-$'rC%t>,P+`J<s=GFiH%b?#pI_gG,F%-'k-$"
".*k-$;D[w'4T,F%13k-$39k-$4<k-$5?k-$6Bk-$OtLk+<m,F%=p,F%>s,F%.W8_8Dmk-$Epk-$Fsk-$Gvk-$H#l-$g&f--h)f--L/l-$M2l-$N5l-$O8l-$P;l-$Q>l-$RAl-$TGl-$"
"$;?_/VMl-$WPl-$XSl-$YVl-$ZYl-$[]l-$.7YEeg&^Y#N)>8%>bNP&LH?v-P_pV.Rd/K2=Iac2U5u`4dx$B5[ARV6d_F59fq'm9.^._6CcK=#Tp-A-x5T;-#6T;-,).m/J9R8#`>0:#"
"-6T;-:)m<-3N#<-4N#<-5N#<-6N#<-8N#<-96T;-9o,D-?6T;-3>9C-C6T;-^r.>-F6T;-G6T;-H6T;-I6T;-J6T;-K6T;-L6T;-M6T;-N6T;-O6T;-;KwA-YN#<-V6T;-W6T;-X6T;-"
"Y6T;-[&kB-wN#<-t6T;-(JvD-&7T;-'7T;-<Z`=-)7T;->Z`=-j)LS-LB;=-=7T;-?7T;-A7T;-as.>-^Au`3Kl+Y#R`ET#soYN#_FKU#O;eS#0M#<--5T;-.5T;-;(m<-4M#<-15T;-"
"35T;-45T;-55T;-65T;-Oq.>-<M#<-=M#<->M#<-.cEB-D5T;-E5T;-F5T;-G5T;-H5T;-g3S>-h3S>-L5T;-M5T;-N5T;-O5T;-P5T;-Q5T;-R5T;-T5T;-$eF?-V5T;-W5T;-X5T;-"
"Y5T;-Z5T;-[5T;-m+[i0_f1$#e(E%v-.vlLdgjmLfs&nLoS#oLP?1sLGQLsLmHmwL(Z]#M)af#MHq*'MIw3'MuB16v?CFR-;CFR-CTSg'UbDWo.<rI9l8l%l%0t;-[-6N/dt'#5>*C>5"
"XPq]5YY6#6ZcQ>6[lmY6pW-n9ml'v#8goI_M=m-$3;q-$ql08.d9Ii^,p*2_;Lnxc+@$C#VYgl/ILZY#mgku>xigY?#s,v?$&H;@&8)s@01FmB.Px.C.+XMC7LtiC4I9/D5RTJD6[pfD"
"8nPGE=E.&GEPuoJNr:5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJMNLjfMOU/,NT-c`O,g)&PZKCAPWH_]PXQ$#QYZ?>Qs<=AYtEX]Y.vnP^v_D5&mp5eQ064eQ&+KV?V6gr?$&H;@"
"*PMs@Z#[iBGq=2C.+XMC;XtiC4I9/D5RTJD6[pfD8nPGE=E.&GEPuoJNr:5KGcUPKHlqlKIu62LJ(RMLK1niLL:3/MMCNJMNLjfMOU/,NT-c`O,g)&PZKCAPWH_]PXQ$#QYZ?>Qs<=AY"
"tEX]Y,j[P^f84eQ4A3/(#[U%Oxfk-$%gp-$<Vq-$PT[%O%D7cV:A=%t=2&cr,]D_&Dmk-$Epk-$Fsk-$Gvk-$H#l-$#^f--2A'crA$92't-XJ(7&D)+98%a+;J[A,B4pV._dI21YAu`4"
"P?<9^q<*mL&hjmLw=7I-Yr/C/^IH$vc:^kLQj52v#ug%MhIf4v[l*J-[S[I-45TC/XfB#v1VWjLFcbjLGikjLHotjLdgjmLXqugLDW/+Mt/$Y#8j+Y#=YlS.>v=Y#NM#<-U5T;-V5T;-"
"W5T;-X5T;-Y5T;-Z5T;-`YlS._mx=#5&KV-3/[a.nS42v+Th`.0:e4v)6PG-^e'S-/5T;-75T;-=scT%3UM21oBow%R3_&vbgHiL5C8#M5C8#MOLs*MOLs*M=C8#M=C8#M=C8#MA[]#M"
"-Ls*M-Ls*M-Ls*MWqUE%qQ)qrqQ)qr'_sl/Wj820HkhP0iXGOGt*kof^?gofU'gofU'gofU'gofM47>db47>dM47>dM47>dM47>dRRe>dma#VdQUKF-ZT*oU:+[Y#F>>;$18*T@g8su,"
"8c###5x7cVQ#/l#xB31#'@H*NTwm##w_#7v;'7RRt)&cr8:(#v@%t$O&41@OIWKO0FKm;#fdV8vomk<O?l<SS@)&creas%cP=2_A2@wI_+eS_&WD-L,#SS1p8@(#v9/.'N;r&gLEr@QM"
")a/%#tZD'O8u]+M.[>pNS3$##jnpxu^j<nLCc@jNp=L[O[Js*M1.b^-Ic5(/:9@k=Ho&l4rCW-Q-65c`R`'##fHxIUD<,8f_^mxuP3^V-GZ<wT+'K.v+Xx;M6DT;-OBhPN?7$##lc/r-"
"Ai+l+=I)W%t1TV-Sm++%,Pkxu&nlHMT]lT'cD/h$Eo#IMoe6A/3S###Z:+GML,NR'HhpI$RLS.-R-`Js&Tts7OEPr)35''-NN)VZ^nqJW]RZM&oD/`&W`rI-1x9#v>u?4vIB;X-keMF%"
"$Skxu4</dVuS1M'60YxBNqlxu#R#-MXDtL^BqY/$VQMNRU'WX-2/&s)gO;/Q59a68ngBL5mYE5v_2VuPifPxPHtL=PpC6##'o/oM7?OC,5gsx+GDSY,NBkl&wDYc2g,q%4ZW^w'F3:PJ"
"U6^(W^[go76Mg1^#=wF`fw9m9g(:L#`)e7eFGBJ:8O^-6J4mW%0135&3rUV$/9H^,Mu08.&=2W%wV4L#00Q]454M9&(LFV?Mr%8@RQ[q/49$5AZtTiBa4di0f$7L#8'/DEJ^V3OAAf(N"
",m;&PJGFqDKWe,<x#;YYgbk8].56A=A7#RNg8MucxuG;@dV%6/6mC?#K]bI#NU@%#]rw@#vIqJ#9gZ(#@'3D#N-92#>ke%#6qZiLX8x(#u+vlLjJL*#a7p*#hI5+#_iWV2=,85#GGCR#"
"JXR@#[-=A#mB1k$H/5##<IwN)(s@v-A/b;%Q+%a4J)qA5.K[Qs#xGS7tL'58$8IJ(Ci5L#GEG3bQr&)*Gu5L#V34D<P$Td*)OFV?^(M;@6DwA,^b6L#%]TiB^Jor-gbujtJLg+MpO%(#"
"Z.As7nf&V-b&^7.J=@#MdD#OMjc5oL_IJ8#nj&V-V5Do%.5PfCB0pi'KnOG)Mth(EQ2.)*SN-&+T?I`E[oa]+Y#E>,Z^*AFdI#v,b`=8.kDbxF-dGG2(w'&4,.o1KMHIM9hfM?pdT_?p"
"vxe+MLXZ###;#s-`:4gLPq)$#&;#s-xLOgL2KihL`aYgL@>+jL5G`hLhfl&#4>,8.5iPlLH@6##X;#s-9uclLLRH###82mLxnk)#mYu##cMYS.mb/*#sF`t-0]imL77U*#jSGs-8u7nL"
"AO$+#6_Xv-DConLLnQ+#&B=?/)=M,#m51pL]dA%#%<#s-qACpL`v]%#'<#s-sYhpLc,g%#p#@qL)Y#.#LYI%#1<#s-*5[qL]dA%#3<#s--AnqL^jJ%#5<#s-0M*rLe>5&#8<#s-3`ErL"
"`pJ%#</'sLUZ`/#;-PO/jMh/##@BsLKRQ##C<#s-(LTsLQqv##S:dtL1-u1#^TGs-@9cwLbbE4#uox/.OjUxL^ap5#vw]7.bJR#MAt56#>m@u-gVe#Mu)H6#J`98/KJ:7#-<k$MRw)$#"
"8%q%M:S]8#h.:w-?76&M6`o8#K$)t-DIQ&M'.59#QCP##Qt;'MwWp0i[Yuo%9L<5&F'XP&;[jl&A'TM'@-pi'A$KM'MpLG)L&.)*IBgi'WV*&+OMa]+r]Ke$12B>,Tuxu,8B.L,C%;8."
"[RUS.]DDG)ujNM04;Bm0cv_-6NdGG2W5P)4m>`-6eC9;6:e+Z6J0>X(nkpr6B3c;7$g`-6#[il8HjZ59'p`-6.wIM92]YY#K7m-$Ywbf(mN)Z$)<a;%nE?^#YT3L#ZONP&$+Qv$tM4L#"
"li08.-'Fm'vS4L#%4=G2WNiWh_fR]48R8B5niHJ(9J5L#Fr#29Nl_s-Ut7nLG:-##n;#s-MHxnLLXZ##xMYS.9<M,#=5U@/1Oi,#eACpLaj/%#@*]-#tr6qLXKs$#/<#s-v(IqLXKs$#"
"1<#s-x4[qLXKs$#3<#s-r@nqLYQ&%#5<#s-sL*rLV?a$#8<#s-v_ErL[W&%#</'sLLY`/#$@0,.EGKsLrv70#eg=(.hX;uL1-u1#d#)t-4k+wLacE4#3;xu-OjUxLdsp5#-7>##bJR#M"
"5t56#?m@u-i]n#Mn;Q6#@*V$#2=#s-p;k$MJF6##0cK%MH;88#Qm@u-8%q%MFS]8#twQx-UFV/8T;jP(9L<5&AnWP&6.78%=esl&9b82'>k/2'C95/(9W'N(nuC_&la1,)=&?g)oSKe$"
"#W*&+HfrD+w:D_&'pa]+DfR&,#AD_&72B>,VCUS.Q6oo%ujNM0Ai^21Ej.L,R2``3q1mD47YLe$fLTV6mUEv6dbv?0.k.29uqci9tATS%SJO-7oF(##";

static const char* GetLightCompressedFontDataOTFBase85()
{
	return bank_gothic_pro_light_otf_compressed_data_base85;
}

static const char* GetMediumCompressedFontDataOTFBase85()
{
	return bank_gothic_pro_medium_otf_compressed_data_base85;
}

static const char* GetBoldCompressedFontDataOTFBase85()
{
	return bank_gothic_pro_bold_otf_compressed_data_base85;
}
