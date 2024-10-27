#include "menu.hpp"

#include <vector>
#include <string>
#include <mutex>
#include <Windows.h>
#include <optional>
#include <memory>

#include "font/hashes.h"
#include "font/faprolight.hpp"
#include "font/inter.hpp"
#include "font/tablericons.hpp"

#include <roblox/roblox.hpp>

//sizes
const auto main_sz = ImVec2{ 875, 600 };
const auto header_sz = ImVec2{ 875, 24 };

const auto control_buttons_sz = ImVec2{ 90, 24 };

const auto page_bar_sz = ImVec2{ 53,  main_sz.y - (header_sz.y * 2) };
const auto button_bar_sz = ImVec2{ main_sz.x - page_bar_sz.x, 36 };
const auto script_list_sz = ImVec2{ 307, 0 };
const auto script_list_search_sz = ImVec2{ script_list_sz.x, button_bar_sz.y };
const auto script_box_sz = ImVec2{ main_sz.x - script_list_sz.x - button_bar_sz.x, 0u };

const auto script_hub_left_sz = ImVec2{ 421, 0 };
const auto script_hub_search = ImVec2{ 421, 28 };

const auto bottom_padding = ImVec2{ 0, 5 };
const auto script_list_left_padding = ImVec2{ 5, 0 };
//sizes

//colors
const auto header_color = ImColor{ 46, 52, 64 };
const auto tab_bar_color = ImColor{ 46, 52, 64 };
const auto page_bar_color = ImColor{ 46, 52, 64 };
const auto button_bar_color = ImColor{ 54, 54, 54 };
const auto script_list_color = ImColor{ 42, 42, 42 };
const auto script_list_search_color = ImColor{ 74, 74, 74 };
const auto script_box_color = ImColor{ 42, 42, 42 };

const auto button_color = ImColor{ 113, 113, 113 };
const auto selected_tab_color = ImColor{ 59,66,82 };

const auto seperator_color = ImColor{ 120, 120, 120 };
const auto dark_seperator_color = ImColor{ 16, 16, 16 };
const auto text_color = ImColor{ 216,222,233 };
//colors

void ui::set_style()
{
    auto& style = ImGui::GetStyle();
    style.WindowPadding = { 0,0 };
    style.FramePadding = { 0,0 };
    style.WindowBorderSize = 0;
    style.PopupBorderSize = 0;
    style.PopupRounding = 5;
    style.WindowRounding = 6;
    style.ScrollbarSize = 17;
    style.ScrollbarRounding = 1;
    style.FrameRounding = 2;
    style.ChildBorderSize = 0;
    style.WindowMinSize = { 850, 500 };

    style.Colors[ImGuiCol_WindowBg] = ImColor{ 59,66,82 };
    style.Colors[ImGuiCol_FrameBg] = ImColor{ 46, 52, 64 };

    style.Colors[ImGuiCol_ResizeGrip] = ImColor{ 59,66,82 };
    style.Colors[ImGuiCol_ResizeGripActive] = ImColor{ 59,66,82 };
    style.Colors[ImGuiCol_ResizeGripHovered] = ImColor{ 59,66,82 };

    style.Colors[ImGuiCol_HeaderActive] = ImColor{ 85, 85, 85 };
    style.Colors[ImGuiCol_Header] = ImColor{ 85, 85, 85 };
    style.Colors[ImGuiCol_HeaderHovered] = ImColor{ 80, 80, 80 };

    style.Colors[ImGuiCol_Text] = ImColor{ 216,222,233 };

    static const ImWchar tablerranges[] = { 0xea77, 0xeb4d, 0 }; //eb20

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    static const ImWchar ranges[] = {
        0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
        0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
    };

    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;

    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 16, NULL, ranges);
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(faprolight, sizeof faprolight, 32, &icons_config, icon_ranges);

    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(tablericons_compressed_data, tablericons_compressed_size, 32, &icons_config, tablerranges);

    px18 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 18, NULL, ranges);
    px24 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 24, NULL, ranges);
    px32 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 24, NULL, ranges);

    px35 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 35, NULL, ranges);
    px20 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(interfont_compressed_data, interfont_compressed_size, 20, NULL, ranges);
}

struct script_data_t
{
    std::string name;

    std::shared_ptr<TextEditor> editor;
};

static std::vector<script_data_t> tabs = { script_data_t{ .name = "tab1", .editor = ui::make_editor()}};

static int current_tab, current_page = 0u;

static int script_hub_index = 0u;

void ui::draw_ui()
{
    ImGui::SetNextWindowSize(main_sz, ImGuiCond_Once);
    if(ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoTitleBar))
    {

        const auto dl = ImGui::GetWindowDrawList();

        const auto pos = ImGui::GetWindowPos();

        const auto window_sz = ImGui::GetWindowSize();

        { /* Header */
            const auto item_position = pos;
            auto size = header_sz;

            size.x = window_sz.x;

            dl->AddRectFilled(item_position, item_position + size, header_color, 6);
            dl->AddRectFilled(item_position + ImVec2{0u, size.y / 2}, item_position + size, header_color);

            ImGui::SetCursorScreenPos(item_position);

            const auto header = "Hydrogen";
            const auto middle_pos = item_position.x + (size.x / 2 - calc_text_sz(16.f, header).x / 2.f);

            dl->AddText(nullptr, 16.f, ImVec2{ middle_pos, item_position.y + (size.y / 2 - calc_text_sz(16.f, header).y / 2.f) }, text_color, header);

            
        }

        { /* Page Bar */
            const auto item_position = pos;
            auto size = page_bar_sz;

            size.y = window_sz.y;

            dl->AddRectFilled(item_position, item_position + size, page_bar_color, 6);
            dl->AddRectFilled(item_position + ImVec2{ size.x / 2, 0 } , item_position + size, page_bar_color);

            dl->AddText(nullptr, 20.f, item_position + ImVec2{ 9, 41 }, ImColor{ 129,161,193 }, (const char*)u8"\uea97");

            ImGui::SetCursorScreenPos(item_position + ImVec2{ 8, 75});
            ImGui::BeginChild("page_bar", size - ImVec2{ 8, 75 });
            {
                const auto page_button_sz = ImVec2{ size.x - 15.f, size.x - 15.f };

                //0xea77

                page_button(current_page, 0, (const char*)u8"\uea77", page_button_sz);

                page_button(current_page, 1, (const char*)u8"\ueb4d", page_button_sz);

                page_button(current_page, 3, (const char*)u8"\ueb20", page_button_sz);

                ImGui::EndChild();
            }
        }

        switch (current_page)
        {
            case 0: /* Executor */
            {
                { /* Tab Bar */
                    const auto item_position = pos + ImVec2{ page_bar_sz.x + 8, header_sz.y + 5 };
                    auto size = header_sz;

                    size.x = window_sz.x - page_bar_sz.x - control_buttons_sz.x - 5;

                    dl->AddRectFilled(item_position, item_position + size, tab_bar_color, 6);

                    ImGui::SetCursorScreenPos(item_position);
                    ImGui::BeginChild("tab_bar", size);
                    {
                        const auto tabs_that_fit_on_screen = std::size_t(size.x / 95);

                        if (tab_change_button(true, std::nullopt))
                        {
                            if (current_tab > 0)
                                --current_tab;
                        }
                        ImGui::SameLine();
                        if (tab_change_button(false, item_position + ImVec2{ size.x - 30.f, 0 }))
                        {
                            if (current_tab < tabs.size() - 1)
                                ++current_tab;
                        }

                        ImGui::SameLine();

                        const auto sp = ImGui::GetCursorScreenPos();

                        ImGui::SetCursorScreenPos(item_position + ImVec2{ 25, 0 });
                        ImGui::BeginChild("tab_container", size - ImVec2{ 52, 0 });
                        {
                            for (auto i = current_tab > tabs_that_fit_on_screen - 1 ? current_tab - (tabs_that_fit_on_screen - 1) : 0; i < tabs.size(); ++i)
                            {
                                const auto& tab = tabs[i];

                                tab_button(current_tab, i, tab.name.c_str());

                                if (i < tabs.size() - 1)
                                    ImGui::SameLine();
                            }
                            ImGui::EndChild();
                        }

                        ImGui::EndChild();
                    }

                    dl->AddRectFilled(item_position + ImVec2{ size.x + 5, 0 }, item_position + ImVec2{ window_sz.x - page_bar_sz.x - 15, size.y }, tab_bar_color, 6);
                    ImGui::SetCursorScreenPos(item_position + ImVec2{ size.x + 7, 0 });
                    ImGui::BeginChild("tab_bar_buttons", ImVec2{ window_sz.x - page_bar_sz.x - 15, size.y });
                    {
                        if (icon_btn("New Tab", ICON_FA_PLUS, { 25, 25 }, 6, text_color, 3))
                        {
                            tabs.push_back({ std::string{"Tab"}.append(std::to_string(tabs.size() + 1)), make_editor() });
                        }

                        ImGui::SameLine(0.f, 0.f);

                        if (icon_btn("Clear", ICON_FA_TRASH, { 25, 25 }, 6, ImColor{ 191,97,106 }, 3))
                        {
                            tabs[current_tab].editor->SetText("");
                        }

                        ImGui::SameLine(0.f, 0.f);

                        if (icon_btn("Execute", ICON_FA_CARET_RIGHT, { 25, 25 }, 10, ImColor{ 163,190,140 }, 5))
                        {
                            const auto script = tabs[current_tab].editor->GetText();

                            roblox::rbx_manager_t::get( ).queue_script( script );
                        }

                        ImGui::EndChild();
                    }
                }

                { /* Script Box */
                    const auto item_position = ImVec2{ pos.x + page_bar_sz.x + 8, pos.y + header_sz.y + header_sz.y + 10 };
                    auto size = script_box_sz;

                    size.x = window_sz.x - (page_bar_sz.x) - 15;
                    size.y = window_sz.y - (header_sz.y + header_sz.y + 10) - bottom_padding.y;

                    dl->AddRectFilled(item_position, item_position + size, script_box_color);

                    ImGui::SetCursorScreenPos(item_position);
                    ImGui::BeginChild("script_boxa", size);
                    {
                        if (current_tab >= tabs.size())
                            current_tab = tabs.size() - 1;

                        auto& tab = tabs[current_tab];

                        tab.editor->Render(std::string{ "editor" }.append(std::to_string(current_tab)).c_str(), size);

                        ImGui::EndChild();
                    }
                }
                break;
            }
        }
        ImGui::End();
    }
}

std::shared_ptr<TextEditor> ui::make_editor()
{
    auto editor = std::make_unique<TextEditor>();

    auto lang = TextEditor::LanguageDefinition::Luau();
    editor->SetShowWhitespaces(false);
    editor->SetLanguageDefinition(lang);
    editor->SetPalette({
            ImColor{224,224,224},	// Default
            ImColor{159, 213, 171},	// Keyword	
            ImColor{213, 211, 159},	// Number
            ImColor{233, 186, 127},	// String
            0xff70a0e0, // Char literal
            0xffffffff, // Punctuation
            0xff408080,	// Preprocessor
            ImColor{122, 187, 220}, // Identifier
            ImColor{122, 187, 220}, // Known identifier
            0xffc040a0, // Preproc identifier
            ImColor{235,203,139}, // Comment (single line)
            ImColor{235,203,139}, // Comment (multi line)
            ImColor{46, 52, 64}, // Background
            0xffe0e0e0, // Cursor
            0x80a06020, // Selection
            0x800020ff, // ErrorMarker
            0x40f08000, // Breakpoint
            ImColor{159, 213, 171}, // Line number
            ImColor{59,66,82}, // Current line fill
            ImColor{59,66,82}, // Current line fill (inactive)
            ImColor{42,42,42}, // Current line edge
        });

    return std::move(editor);
}

ImVec2 ui::calc_text_sz(int sz, const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
    ImGuiContext& g = *GImGui;

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = ImGui::FindRenderedTextEnd(text, text_end);      // Hide anything after a '##' string
    else
        text_display_end = text_end;

    ImFont* font = g.Font;
    const float font_size = sz;
    if (text == text_display_end)
        return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

    // Round
    // FIXME: This has been here since Dec 2015 (7b0bf230) but down the line we want this out.
    // FIXME: Investigate using ceilf or e.g.
    // - https://git.musl-libc.org/cgit/musl/tree/src/math/ceilf.c
    // - https://embarkstudios.github.io/rust-gpu/api/src/libm/math/ceilf.rs.html
    text_size.x = IM_FLOOR(text_size.x + 0.99999f);

    return text_size;
}

bool ui::tab_change_button(bool is_left, std::optional< ImVec2 > arg_pos)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const auto label = is_left ? ICON_FA_CHEVRON_LEFT : ICON_FA_CHEVRON_RIGHT;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(std::string{"tab_change_button"}.append(std::to_string(is_left)).c_str());

    ImVec2 pos = window->DC.CursorPos;

    if (arg_pos)
    {
        pos = *arg_pos;
    }

    ImVec2 size = ImVec2{ header_sz.y, header_sz.y };

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    ImGui::RenderNavHighlight(bb, id);
   // ImGui::RenderFrame(bb.Min, bb.Max, button_color, true, 2);

    const auto middle_x = pos.x + 11.f;
    const auto middle_y = pos.y - 7.f;

    window->DrawList->AddText(nullptr, 6, ImVec2{ middle_x, middle_y + 19.f }, ImColor{176, 176, 176}, label);

    return pressed;
}

bool ui::tab_button(int& current_tab, int tab, const char* label)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(std::string{ label }.append(std::to_string(tab)).c_str());

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos + ImVec2{0, 1};

    ImVec2 size = ImVec2{ 75.f, 22.f };

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    if (pressed)
    {
        current_tab = tab;

        current_page = 0;
    }

    const ImRect xpos = { ImVec2{bb.Max.x - 15.f, bb.Min.y + 6.f}, bb.Max - ImVec2{5.f, 5.f} };

    const ImGuiID id2 = window->GetID(std::string{ label }.append(std::to_string(tab)).append("close").c_str());

    ImGui::RenderNavHighlight(bb, id);

    ImGui::RenderFrame(bb.Min, bb.Max, current_tab == tab ? selected_tab_color : ImColor{ 0, 0, 0, 0}, true, 6);

    ImGui::RenderTextClipped(bb.Min + ImVec2{ 5, 3 }, bb.Max - ImVec2{ 20, 0 }, label, nullptr, nullptr, ImVec2{0, 0}, nullptr);

    const auto middle_x = pos.x + (size.x / 2 - calc_text_sz(18, label).x / 2);
    const auto middle_y = pos.y + (size.y / 2 - calc_text_sz(18, label).y / 2);

    window->DrawList->AddText(nullptr, 18, ImVec2{ pos.x + size.x - 15.f, middle_y }, ImColor{ 191,97,106 }, "X");

    if (ImGui::IsMouseHoveringRect(xpos.Min, xpos.Max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) { 
        if (tabs.size() - 1)
        {
            tabs.erase(tabs.begin() + tab);

            if (current_tab == tab)
                current_tab = tab - 1 >= 0 ? tab - 1 : tab;
        }
    }

    return pressed;
}

bool ui::page_button(int& current_page, int page, const char* icon, ImVec2 sz)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(icon);
    const ImVec2 label_size = ImGui::CalcTextSize(icon, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    const ImRect bb(pos, pos + sz);
    ImGui::ItemSize(sz, 0);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    if (pressed)
        current_page = page;

    ImGui::RenderNavHighlight(bb, id);

    const auto middle_x = pos.x + (sz.x / 2 - calc_text_sz(10, icon).x / 2);
    const auto middle_y = pos.y + (sz.y / 2 - calc_text_sz(10, icon).y / 2);

    if ( current_page == page )
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor{ 59,66,82 }, 10);

    window->DrawList->AddText(nullptr, 10, ImVec2{ middle_x + 1, middle_y + 5 }, ImColor{ 216,222,233 }, icon);

    return pressed;
}

bool ui::icon_btn(const char* label, const char* icon, ImVec2 sz, float fontsz, ImColor color, float yoffs)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    std::string text = label;
    text.append("_idthing");

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(text.c_str());

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = sz;

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    const auto middle_x = pos.x + (sz.x / 2 - calc_text_sz(fontsz, icon).x / 2);
    const auto middle_y = pos.y + (sz.y / 2 - calc_text_sz(fontsz, icon).y / 2) + yoffs;

    window->DrawList->AddText(nullptr, fontsz, ImVec2{ middle_x, middle_y }, color, icon);

    return pressed;
}

ImVec2 ui::calc_txt_sz(ImFont* font, float sz, const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
    ImGuiContext& g = *GImGui;

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = ImGui::FindRenderedTextEnd(text, text_end);      // Hide anything after a '##' string
    else
        text_display_end = text_end;

    const float font_size = sz;
    if (text == text_display_end)
        return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

    text_size.x = IM_FLOOR(text_size.x + 0.99999f);

    return text_size;
}

bool ui::button(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    ImGui::RenderNavHighlight(bb, id);
    ImGui::RenderFrame(bb.Min, bb.Max, ImColor{92,92,92}, true, 3);

    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    return pressed;
}
