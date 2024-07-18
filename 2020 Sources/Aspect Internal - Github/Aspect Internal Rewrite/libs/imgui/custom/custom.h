#pragma once

#include "../imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui_internal.h"
#include "../../../src/misc/config.h"

namespace ImGui {
	void CColorPicker(const char* name, Color& c);

	bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0, 0));
	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values);
}