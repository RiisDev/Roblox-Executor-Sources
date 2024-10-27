#include "menu.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_internal.h>
#include <imgui/TextEditor.h>

#include <managers/managers.hpp>

#include <imgui/imgui_memory_editor.h>

void managers::menu_manager_t::set_style( )
{
	ImGuiStyle* style = &ImGui::GetStyle( );
	ImVec4* colors = style->Colors;

	colors[ ImGuiCol_Text ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
	colors[ ImGuiCol_WindowBg ] = ImVec4( 0.188f, 0.204f, 0.247f, 1.000f );
	colors[ ImGuiCol_ChildBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_PopupBg ] = ImVec4( 0.247f, 0.271f, 0.318f, 1.000f );
	colors[ ImGuiCol_Border ] = ImVec4( 0.157f, 0.173f, 0.220f, 1.000f );
	colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_FrameBg ] = ImVec4( 0.247f, 0.271f, 0.318f, 1.000f );
	colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.40f );
	colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.67f );
	colors[ ImGuiCol_TitleBg ] = ImVec4( 0.188f, 0.204f, 0.247f, 1.000f );
	colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.188f, 0.204f, 0.247f, 1.000f );
	colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.188f, 0.204f, 0.247f, 1.000f );
	colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.188f, 0.204f, 0.247f, 1.000f );
	colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.02f, 0.02f, 0.02f, 0.53f );
	colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
	colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
	colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
	colors[ ImGuiCol_CheckMark ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
	colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.24f, 0.52f, 0.88f, 1.00f );
	colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
	colors[ ImGuiCol_Button ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.40f );
	colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
	colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.06f, 0.53f, 0.98f, 1.00f );
	colors[ ImGuiCol_Header ] = ImVec4( 0.247f, 0.271f, 0.318f, 1.000f );
	colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.247f, 0.271f, 0.318f, 1.000f );
	colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.247f, 0.271f, 0.318f, 1.000f );
	colors[ ImGuiCol_Separator ] = colors[ ImGuiCol_Border ];
	colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.10f, 0.40f, 0.75f, 0.78f );
	colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.10f, 0.40f, 0.75f, 1.00f );
	colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.20f );
	colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.67f );
	colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.95f );
	colors[ ImGuiCol_Tab ] = ImLerp( colors[ ImGuiCol_Header ], colors[ ImGuiCol_TitleBgActive ], 0.80f );
	colors[ ImGuiCol_TabHovered ] = colors[ ImGuiCol_HeaderHovered ];
	colors[ ImGuiCol_TabActive ] = ImLerp( colors[ ImGuiCol_HeaderActive ], colors[ ImGuiCol_TitleBgActive ], 0.60f );
	colors[ ImGuiCol_TabUnfocused ] = ImLerp( colors[ ImGuiCol_Tab ], colors[ ImGuiCol_TitleBg ], 0.80f );
	colors[ ImGuiCol_TabUnfocusedActive ] = ImLerp( colors[ ImGuiCol_TabActive ], colors[ ImGuiCol_TitleBg ], 0.40f );
	colors[ ImGuiCol_PlotLines ] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
	colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
	colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	colors[ ImGuiCol_TableHeaderBg ] = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
	colors[ ImGuiCol_TableBorderStrong ] = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );   // Prefer using Alpha=1.0 here
	colors[ ImGuiCol_TableBorderLight ] = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );   // Prefer using Alpha=1.0 here
	colors[ ImGuiCol_TableRowBg ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_TableRowBgAlt ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
	colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.35f );
	colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
	colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
	colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
	colors[ ImGuiCol_NavWindowingDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
	colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );

	style->FrameRounding = 2.f;
	style->TabRounding = 2.f;
	style->GrabRounding = 2.f;
	style->WindowBorderSize = 0.0f;
}

void managers::menu_manager_t::render( )
{
	static TextEditor editor;

	static std::once_flag flag;

	std::call_once( flag,
		[ & ] ( )
		{
			editor.SetShowWhitespaces( false );
			editor.SetLanguageDefinition( TextEditor::LanguageDefinition::Luau( ) );
			editor.SetPalette( {
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
				0xff206020, // Comment (single line)
				0xff406020, // Comment (multi line)
				ImColor{42,42,42}, // Background
				0xffe0e0e0, // Cursor
				0x80a06020, // Selection
				0x800020ff, // ErrorMarker
				0x40f08000, // Breakpoint
				ImColor{159, 213, 171}, // Line number
				ImColor{85,85,85}, // Current line fill
				ImColor{85,85,85}, // Current line fill (inactive)
				ImColor{42,42,42}, // Current line edge
				} );
		}
	);

	ImGui::SetNextWindowSize( { 700, 500 }, ImGuiCond_Once );
	if ( ImGui::Begin( "Main", nullptr ) )
	{
		editor.Render( "Main editor", { 700, 445 } );

		if ( ImGui::Button( "Execute" ) )
		{
			managers::mgrs.roblox_mgr.queue_script( editor.GetText( ) );
		}
	}
	ImGui::End( );
}