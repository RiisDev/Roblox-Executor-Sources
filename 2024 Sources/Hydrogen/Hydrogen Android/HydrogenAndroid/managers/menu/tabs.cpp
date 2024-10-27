#include "tabs.hpp"

#include "animator.hpp"
#include "routines.hpp"

#include "assets/images.hpp"
#include "assets/fonts.hpp"

#include <managers/roblox/roblox.hpp>
#include <managers/jni/jni.hpp>

#include <string>

void menu::tabs::wip( ImVec2 pos, ImVec2 size, float opacity )
{
	const auto text = std::string{ "This tab is a Work In Progress." };
	const auto text_font = menu::font_t::get( menu::assets::fonts::inter_regular, size.y / 15.f );
	const auto text_color = ImVec4( 1.f, 1.f, 0.f, opacity );
	const auto text_size = menu::calc_text_size( text, text_font );
	const auto text_pos = ImVec2( pos.x + ( size.x / 2.f ) - ( text_size.x / 2.f ), pos.y + ( size.y / 2.f ) - ( text_size.y / 2.f ) );
	menu::draw_text( text, text_font, text_pos, text_color );
}

void menu::tabs::code( ImVec2 pos, ImVec2 size, float opacity )
{
	using notification_type_t = managers::menu_manager_t::notification_type_t;

	const auto opacity_i = static_cast< int >( opacity * 255.f );
	const auto text_font = menu::font_t::get( menu::assets::fonts::inter_regular, size.y / 15.f );
	const auto editor_font = menu::font_t::get( menu::assets::fonts::jetbrainsmono_regular, size.y / 15.f );
	const auto internal_padding = ImVec2{ size.x / 100.f, size.y / 100.f };

	{ // "Editor" Main Blob		
		const auto editor_padding = 10.f;

		// editor is a bit small than the tab
		const auto editor_pos = ImVec2( internal_padding.x * ( editor_padding / 2.f ), internal_padding.y * ( editor_padding / 2.f ) );
		const auto editor_size = ImVec2( size.x - ( internal_padding.x * editor_padding ), size.y - ( internal_padding.y * editor_padding ) );

		ImGui::PushStyleVar( ImGuiStyleVar_Alpha, opacity );
		ImGui::PushStyleVar( ImGuiStyleVar_ScrollbarSize, internal_padding.x * 2.f );
		ImGui::PushFont( editor_font->get_font( ) );
		ImGui::PushStyleColor( ImGuiCol_ScrollbarBg, ImVec4( 0, 0, 0, 0 ) );
		ImGui::SetCursorPos( editor_pos );
		managers::menu_mgr.editor.Render( "code_editor", editor_size, false );
		ImGui::PopStyleColor( );
		ImGui::PopFont( );
		ImGui::PopStyleVar( 2 );
	}

	{ // "Buttons" Blob
		struct button_t {
			menu::image_t* icon;
			std::function< void( ) > callback;
		};
		std::vector<button_t> buttons = {
			{ managers::menu_mgr.run_img, [ & ] ( )->void { 
				managers::roblox_mgr.queue_script( managers::menu_mgr.editor.GetText( ) );
			} },
			{ managers::menu_mgr.trash_img, [ & ] ( )->void {
				managers::menu_mgr.editor.SetText( "" );
				managers::roblox_mgr.unfocus_keyboard( );
				managers::roblox_mgr.set_keyboard_text( "" );
			} },
			{ managers::menu_mgr.files_img, [ & ] ( )->void {
				//managers::jni_mgr.set_clipboard( managers::menu_mgr.editor.GetText( ).c_str() );
				managers::menu_mgr.notifications.emplace_back( notification_type_t::info, "This feature is unimplemented." );
			} },
			{ managers::menu_mgr.upload_img, [ & ] ( )->void {

				/*managers::roblox_mgr.queue_script(  );*/

				//managers::menu_mgr.editor.SetText( managers::jni_mgr.get_clipboard( ) );
				//managers::menu_mgr.notifications.emplace_back( notification_type_t::info, "This feature is unimplemented." );
			} },
		};

		const auto square_size = ImVec2{ size.x / 10.f, size.x / 10.f };

		const auto blob_size = ImVec2( square_size.x, ( square_size.y + internal_padding.y ) * buttons.size() );
		const auto blob_pos = ImVec2( pos.x + size.x + internal_padding.x, pos.y );
		const auto blob_color = ImColor{ 37,37,37, opacity_i };
		const auto blob_border_color = ImColor{ 27,27,27, opacity_i };
		menu::window( "code buttons blob", blob_pos, blob_size, [ & ] ( ) -> void {
			menu::draw_rect( blob_pos, { blob_size.x, blob_size.y - internal_padding.y * static_cast<float>( buttons.size( ) ) }, blob_border_color, blob_size.x / 2.f, internal_padding.x / 2.f, blob_color, true );

			// render buttons
			// they are all the same size (square_size)
			// they are vertically centered in the blob
			// a button is a circle with an icon in the middle
			for ( auto i = 0u; i < buttons.size( ); i++ ) {
				const auto button_size = ImVec2{ square_size.x * 0.8f, square_size.y * 0.8f };
				const auto button_pos = ImVec2( blob_pos.x + ( blob_size.x / 2.f ) - ( button_size.x / 2.f ), blob_pos.y + ( square_size.y * i ) + ( square_size.y / 2.f ) - ( button_size.y / 2.f ) );
				const auto button_color = ImColor{ 40,40,40, opacity_i };
				const auto button_border_color = ImColor{ 32,32,32, opacity_i };
				const auto button_icon_color = ImColor{ 255,255,255, opacity_i };
				menu::draw_rect( button_pos, button_size, button_border_color, button_size.x / 2.f, internal_padding.x / 2.f,  button_color, true );

				const auto img_size = ImVec2{ button_size.x * 0.5f, button_size.y * 0.5f };
				const auto img_pos = ImVec2( button_pos.x + ( button_size.x / 2.f ) - ( img_size.x / 2.f ), button_pos.y + ( button_size.y / 2.f ) - ( img_size.y / 2.f ) );
				menu::draw_img( buttons[ i ].icon, img_pos, img_size, ImColor{ 170,170,170, opacity_i } );

				if ( menu::button_behavior( button_pos, button_size ) == menu::button_result_t::press ) {
					buttons[ i ].callback( );
				}
			}
		} );
	}
}
