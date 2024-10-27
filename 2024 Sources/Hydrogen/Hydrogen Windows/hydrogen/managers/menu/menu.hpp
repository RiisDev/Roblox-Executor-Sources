#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "TextEditor.h"

#include <optional>

namespace ui
{
    static ImFont* px18;
    static ImFont* px24;
    static ImFont* fapx24;
    static ImFont* px32;
    static ImFont* px35;
    static ImFont* px20;

    void set_style();

    void draw_ui();

    std::shared_ptr<TextEditor> make_editor();

    ImVec2 calc_text_sz(int sz, const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    bool tab_change_button(bool is_left, std::optional< ImVec2 > arg_pos);
    bool tab_button(int& current_tab, int tab, const char* label);

    bool page_button(int& current_page, int page, const char* icon, ImVec2 sz);

    bool icon_btn(const char* label, const char* icon, ImVec2 sz, float fontsz, ImColor color, float yoffs);

    ImVec2 calc_txt_sz(ImFont* font, float sz, const char* text, const char* text_end = nullptr, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    bool button(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags);
}
