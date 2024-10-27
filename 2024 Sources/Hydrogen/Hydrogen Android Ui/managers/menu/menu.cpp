#define IMGUI_DEFINE_MATH_OPERATORS

#include "menu.hpp"

#include <dependencies/imgui/imgui_internal.h>

#include "animator.hpp"
#include "routines.hpp"

#include "assets/images.hpp"
#include "assets/fonts.hpp"

#define IMG_FROMASSETS(name) menu::image_t::from_data( menu::assets::images::name, sizeof( menu::assets::images::name ) )

void managers::menu_manager_t::initiate( )
{
	run_img = IMG_FROMASSETS( run );
	trash_img = IMG_FROMASSETS( trash );
	upload_img = IMG_FROMASSETS( upload );
	appico_img = IMG_FROMASSETS( appico );
	cross_img = IMG_FROMASSETS( cross );
	key_img = IMG_FROMASSETS( key );
	files_img = IMG_FROMASSETS( files );
	check_icn = IMG_FROMASSETS( check_icn );
	cross_icn = IMG_FROMASSETS( cross_icn );
	info_icn = IMG_FROMASSETS( info_icn );
	warn_icn = IMG_FROMASSETS( warn_icn );
	
	tabs.push_back( { IMG_FROMASSETS( code ), "Code", menu::tabs::code } );
	tabs.push_back( { IMG_FROMASSETS( files ), "Files", menu::tabs::wip } );
	tabs.push_back( { IMG_FROMASSETS( terminal ), "Terminal", menu::tabs::wip } );
	tabs.push_back( { IMG_FROMASSETS( settings ), "Settings", menu::tabs::wip } );

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
		ImColor{42,42,42,0}, // Background
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

void managers::menu_manager_t::dispose( )
{
	run_img->dispose( );
	trash_img->dispose( );
	upload_img->dispose( );
	appico_img->dispose( );
	cross_img->dispose( );
	key_img->dispose( );
	files_img->dispose( );
	check_icn->dispose( );
	cross_icn->dispose( );
	info_icn->dispose( );
	warn_icn->dispose( );
	
	for ( auto& tab : tabs )
		if ( tab.icon ) tab.icon->dispose( );
		
	tabs.clear( );
}

void managers::menu_manager_t::render( )
{
	const auto display_size = ImVec2{ renderer_mgr.width, renderer_mgr.height };
	const auto global_padding = menu::aspect_ratio_constraint( display_size, 0.01f, 1.f );

	auto& anim_navbar = menu::animator_t::get( "navbar" );
	auto& anim_tab = menu::animator_t::get( "tab" );
	constexpr float anim_navbar_speed = 0.7f;
	
	/* Workspace definitions */
	const auto workspace_size = menu::aspect_ratio_constraint( display_size, 0.8f, 16.f / 10.f );
	auto workspace_position = ImVec2{ display_size.x / 2.f - workspace_size.x / 2.f, global_padding.y * 5.f };
	const auto vertical_offset = global_padding.y * 5.f;

	/* Navbar definitions */
	const auto navbar_size_opened = ImVec2{ workspace_size.x, workspace_size.y / 6.f };
	const auto navbar_position_opened = ImVec2{ workspace_position.x, workspace_position.y };	
	const auto navbar_size_closed = ImVec2{ navbar_size_opened.y, navbar_size_opened.y };
	const auto navbar_position_closed = ImVec2{ workspace_position.x + workspace_size.x / 2.f - navbar_size_closed.x / 2.f, navbar_position_opened.y };
	
	const auto navbar_size = anim_navbar.lerp( navbar_size_closed, navbar_size_opened, anim_navbar_speed, !menu_toggled, menu_toggled ? menu::easings::out_back : menu::easings::in_expo );
	const auto navbar_position = anim_navbar.lerp( navbar_position_closed, navbar_position_opened, anim_navbar_speed, !menu_toggled, menu_toggled ? menu::easings::out_back : menu::easings::in_expo );
	const auto navbar_font_size = navbar_size.y / 4.f;
	const auto navbar_elem_opacity = anim_navbar.lerp( 0.f, 1.f, anim_navbar_speed, !menu_toggled, menu_toggled ? menu::easings::out_expo : menu::easings::in_expo );
	const auto navbar_elem_opacity_i = static_cast< int >( navbar_elem_opacity * 255.f );
	
	/* Navbar */
	menu::window( "navbar", navbar_position, navbar_size, [ & ]( ) -> void {
		menu::draw_rect( navbar_position, navbar_size, { 27,27,27, 255 }, navbar_size.y / 2.f, global_padding.y * 0.8f, { 37,37,37, 255 }, true );
	
		const auto img_sz = ImVec2{ navbar_size_closed.x * 0.5f, navbar_size_closed.y * 0.5f };
		const auto img_pos = ImVec2{ navbar_position.x + img_sz.x / 2.f, navbar_position.y + navbar_size.y / 2.f - img_sz.y / 2.f };
		menu::draw_img( appico_img, img_pos, img_sz, { 214,214,214, 255 } );

		const auto img_pos_right = ImVec2{ ( navbar_position.x + navbar_size.x ) - ( img_sz.x + img_sz.x / 2.f ), navbar_position.y + navbar_size.y / 2.f - img_sz.y / 2.f };
		menu::draw_img( cross_img, img_pos_right, img_sz, { 0.9f,0.4f,0.4f, navbar_elem_opacity } );

		if ( game_joined && authed ) {
			// draw all tabs icons in the center of the navbar
			const auto elem_padding = global_padding.x * 2.f;
			for ( auto i = 0u; i < tabs.size( ); i++ ) {
				auto& tab = tabs[ i ];
				const auto clr = i != selected_tab ? ImVec4{ 0.5f,0.5f,0.5f, navbar_elem_opacity } : ImVec4{ 0.9f,0.9f,0.9f, navbar_elem_opacity };

				const auto tab_size = ImVec2{ navbar_size.y * 0.8f, navbar_size.y * 0.8f };
				const auto tab_pos = ImVec2{ navbar_position.x + navbar_size.x / 2.f - ( ( tab_size.x + elem_padding ) * tabs.size( ) / 2.f ) + ( ( tab_size.x + elem_padding ) * ( &tab - &tabs[ 0 ] ) ), navbar_position.y + navbar_size.y / 2.f - tab_size.y / 2.f };
				menu::draw_rect( tab_pos, tab_size, { 25,25,25, navbar_elem_opacity_i }, tab_size.y / 2.f, global_padding.y * .8f, { 29,29,29, navbar_elem_opacity_i }, true );

				const auto img_sz = ImVec2{ tab_size.x * 0.7f, tab_size.y * 0.7f };
				const auto img_pos = ImVec2{ tab_pos.x + tab_size.x / 2.f - img_sz.x / 2.f, tab_pos.y + tab_size.y / 2.f - img_sz.y / 2.f };
				menu::draw_img( tab.icon, img_pos, img_sz, clr );

				if ( menu_toggled ) {
					if ( menu::button_behavior( tab_pos, tab_size ) == menu::button_result_t::press ) {
						selected_tab = i;
						anim_tab.reset( );
					}
				}
			}
		}
		else if ( authed ) {
			std::string text = "Waiting for a game to launch...";
			const auto text_font = menu::font_t::get( menu::assets::fonts::inter_regular, navbar_font_size );
			const auto text_size = menu::calc_text_size( text.c_str( ), text_font );
			const auto text_pos = ImVec2{ navbar_position.x + navbar_size.x / 2.f - text_size.x / 2.f, navbar_position.y + navbar_size.y / 2.f - text_size.y / 2.f };
			menu::draw_text( text, text_font, text_pos, { .7f,.7f,.7f, navbar_elem_opacity } );
		}
		else {
			std::string text = "Waiting for authentication...";
			const auto text_font = menu::font_t::get( menu::assets::fonts::inter_regular, navbar_font_size );
			const auto text_size = menu::calc_text_size( text.c_str( ), text_font );
			const auto text_pos = ImVec2{ navbar_position.x + navbar_size.x / 2.f - text_size.x / 2.f, navbar_position.y + navbar_size.y / 2.f - text_size.y / 2.f };
			menu::draw_text( text, text_font, text_pos, { .7f,.7f,.7f, navbar_elem_opacity } );
		}

		if ( !menu_toggled )
		{
			if ( menu::button_behavior( img_pos, img_sz ) == menu::button_result_t::press ) {
				menu_toggled = true;
				anim_navbar.reset( );
			}
		}
		else
		{
			if ( menu::button_behavior( img_pos_right, img_sz ) == menu::button_result_t::press ) {
				menu_toggled = false;
				anim_navbar.reset( );
			}
		}
	} );

	if ( !authed ) {
		/* Second navbar "Blob" definitions */
		const auto blob_size = ImVec2{ navbar_size_opened.x / 2.5f, navbar_size_opened.y };
		// Blob position is horizontally centered and right below the navbar
		const auto blob_pos = ImVec2{ navbar_position.x + navbar_size.x / 2.f - blob_size.x / 2.f, navbar_position.y + navbar_size.y + global_padding.y };
		menu::window( "blob", blob_pos, blob_size, [ & ] ( ) {
			menu::draw_rect( blob_pos, blob_size, { 27,27,27, navbar_elem_opacity_i }, blob_size.y / 2.f, global_padding.y * 0.8f, { 37,37,37, navbar_elem_opacity_i }, true );

			const auto text_font = menu::font_t::get( menu::assets::fonts::inter_regular, navbar_font_size );			
			
			constexpr float button_space_padding = 4.f;
			const auto button_space_size = ImVec2{ blob_size.x - global_padding.x * button_space_padding, blob_size.y - global_padding.y * button_space_padding };
			const auto button_space_pos = ImVec2{ blob_pos.x + global_padding.x * ( button_space_padding / 2.f ), blob_pos.y + global_padding.y * ( button_space_padding / 2.f ) };

			// 2 buttons definitions
			// they're on the same line
			// a button is a rectangle with an image on the left and text on the right, both vertically centered
			
			// button 1 "Get Key"
			const auto button1_size = ImVec2{ button_space_size.x / 2.f - global_padding.x / 2.f, button_space_size.y };
			const auto button1_pos = ImVec2{ button_space_pos.x, button_space_pos.y };
			menu::draw_rect( button1_pos, button1_size, { 25,25,25, navbar_elem_opacity_i }, button1_size.y / 2.f, global_padding.y * 0.8f, { 29,29,29, navbar_elem_opacity_i }, true );
			
			const auto img1_sz = ImVec2{ button1_size.y * 0.5f, button1_size.y * 0.5f };
			const auto img1_pos = ImVec2{ button1_pos.x + button1_size.y / 2.f - img1_sz.x / 2.f, button1_pos.y + button1_size.y / 2.f - img1_sz.y / 2.f };
			menu::draw_img( key_img, img1_pos, img1_sz, { .7f,.7f,.7f, navbar_elem_opacity } );
			
			const auto text1 = "Get Key";
			const auto text1_size = menu::calc_text_size( text1, text_font );
			const auto text1_pos = ImVec2{ button1_pos.x + button1_size.y + global_padding.x, button1_pos.y + button1_size.y / 2.f - text1_size.y / 2.f };
			menu::draw_text( text1, text_font, text1_pos, { .7f,.7f,.7f, navbar_elem_opacity } );

			if ( menu::button_behavior( button1_pos, button1_size ) == menu::button_result_t::press ) {
				notifications.emplace_back( notification_type_t::info, "Link copied to clipboard!" );
			}

			// button 2 "Verify"
			const auto button2_size = ImVec2{ button_space_size.x / 2.f - global_padding.x / 2.f, button_space_size.y };
			const auto button2_pos = ImVec2{ button_space_pos.x + button_space_size.x / 2.f + global_padding.x / 2.f, button_space_pos.y };
			menu::draw_rect( button2_pos, button2_size, { 25,25,25, navbar_elem_opacity_i }, button2_size.y / 2.f, global_padding.y * 0.8f, { 29,29,29, navbar_elem_opacity_i }, true );

			const auto img2_sz = ImVec2{ button2_size.y * 0.5f, button2_size.y * 0.5f };
			const auto img2_pos = ImVec2{ button2_pos.x + button2_size.y / 2.f - img2_sz.x / 2.f, button2_pos.y + button2_size.y / 2.f - img2_sz.y / 2.f };
			menu::draw_img( files_img, img2_pos, img2_sz, { .7f,.7f,.7f, navbar_elem_opacity } );
			
			const auto text2 = "Verify";
			const auto text2_size = menu::calc_text_size( text2, text_font );
			const auto text2_pos = ImVec2{ button2_pos.x + button2_size.y + global_padding.x, button2_pos.y + button2_size.y / 2.f - text2_size.y / 2.f };
			menu::draw_text( text2, text_font, text2_pos, { .7f,.7f,.7f, navbar_elem_opacity } );
			
			if ( menu::button_behavior( button2_pos, button2_size ) == menu::button_result_t::press ) {
				notifications.emplace_back( notification_type_t::success, "Verified! Have fun!" );
				authed = true;
			}
			
		} );
	}
	else if ( game_joined ) {
		/* Main page definitions */
		const auto main_page_opacity = static_cast< int >( anim_navbar.lerp( 0.f, 255.f, anim_navbar_speed, !menu_toggled, menu_toggled ? menu::easings::out_expo : menu::easings::in_expo ) );
		const auto bg_opacity = static_cast< int >( anim_navbar.lerp( 0.f, 64.f, anim_navbar_speed, !menu_toggled, menu_toggled ? menu::easings::out_expo : menu::easings::in_expo ) );
		ImGui::GetBackgroundDrawList( )->AddRectFilled( ImVec2{ 0,0 }, display_size, ImColor( 0, 0, 0, bg_opacity ) );

		const auto main_page_size = ImVec2{ workspace_size.x, workspace_size.y - ( vertical_offset + navbar_size.y ) };
		const auto main_page_pos = ImVec2{ navbar_position_opened.x, navbar_position_opened.y + (navbar_size.y + vertical_offset) };

		/* Main page */
		menu::window( "main", main_page_pos, main_page_size, [ & ] ( ) {
			menu::draw_rect( main_page_pos, main_page_size, { 27,27,27, main_page_opacity }, global_padding.y * 5.f, global_padding.y * 0.8f, { 37,37,37, main_page_opacity }, true );
			auto& tab = tabs[ selected_tab ];
			tab.render( main_page_pos, main_page_size, navbar_elem_opacity );
		} );

		/* Watermark img (appico) */
		const auto img_sz = menu::aspect_ratio_constraint( display_size, 0.05f, 1.f );
		const auto img_pos = ImVec2{ global_padding.x, display_size.y - img_sz.y - global_padding.y };
		menu::window( "appico_img", img_pos, img_sz, [ & ] ( ) {
			menu::draw_img( appico_img, img_pos, img_sz, { 1.f,1.f,1.f, navbar_elem_opacity } );
		} );

		/* Watermark text */
		const auto watermark_padding = ImVec2{ global_padding.x * 2.f, global_padding.y * 2.f };
		const auto watermark_text = std::string{ "Hydrogen" };
		const auto watermark_font = menu::font_t::get( menu::assets::fonts::inter_regular, img_sz.y * 0.7f );
		const auto watermark_size = menu::calc_text_size( watermark_text, watermark_font );
		const auto watermark_pos = ImVec2{ img_pos.x + img_sz.x + global_padding.x, img_pos.y + img_sz.y / 2.f - watermark_size.y / 2.f };
		menu::window( "watermark", watermark_pos, watermark_size, [ & ] ( ) {
			menu::draw_text( watermark_text, watermark_font, watermark_pos, { 1.f,1.f,1.f, navbar_elem_opacity } );
		} );
	}

	{ // Render notifications
		ImGui::PushID( "notifications" );		
		for ( auto i = 0u; i < notifications.size( ); i++ ) {
			const auto& notification = notifications[ i ];
			
			// A notification is a window with an icon, text and a close button.
			const auto notif_size = menu::aspect_ratio_constraint( display_size, 0.5f, 10.f/1.25f );
			const auto notif_pos = ImVec2{ display_size.x - notif_size.x - global_padding.x, display_size.y - ( notif_size.y + global_padding.y ) * static_cast<float>( i + 1 ) };
			const auto notif_font = menu::font_t::get( menu::assets::fonts::inter_regular, notif_size.y / 3.f );
			
			menu::window( std::string{ "notif" } + std::to_string( i ), notif_pos, notif_size, [ & ] ( ) {
				menu::draw_rect( notif_pos, notif_size, { 27,27,27, 255 }, global_padding.y * 5.f, global_padding.y * 0.8f, { 37,37,37, 255 }, true );

				menu::image_t* icn = nullptr;
				switch ( notification.tt ) {
				case notification_type_t::info:
					icn = info_icn;
					break;
				case notification_type_t::success:
					icn = check_icn;
					break;
				case notification_type_t::warn:
					icn = warn_icn;
					break;
				case notification_type_t::error:
					icn = cross_icn;
					break;
				}

				const auto img_sz = ImVec2{ notif_size.y * 0.5f, notif_size.y * 0.5f };
				const auto img_pos = ImVec2{ notif_pos.x + global_padding.x * 3.f, notif_pos.y + notif_size.y / 2.f - img_sz.y / 2.f };
				menu::draw_img( icn, img_pos, img_sz, { 1.f,1.f,1.f, 1.f } );

				const auto text = notification.text;
				const auto text_size = menu::calc_text_size( text, notif_font );
				const auto text_pos = ImVec2{ img_pos.x + img_sz.x + global_padding.x * 2.f, img_pos.y + img_sz.y / 2.f - text_size.y / 2.f };
				menu::draw_text( text, notif_font, text_pos, { 0.7f,0.7f,0.7f, 1.f } );

				const auto close_sz = ImVec2{ notif_size.y * 0.8f, notif_size.y * 0.8f };
				const auto close_pos = ImVec2{ notif_pos.x + notif_size.x - close_sz.x - global_padding.x, notif_pos.y + notif_size.y / 2.f - close_sz.y / 2.f };

				const auto close_img_sz = ImVec2{ close_sz.y * 0.5f, close_sz.y * 0.5f };
				const auto close_img_pos = ImVec2{ close_pos.x + close_sz.y / 2.f - close_img_sz.x / 2.f, close_pos.y + close_sz.y / 2.f - close_img_sz.y / 2.f };
				menu::draw_img( cross_img, close_img_pos, close_img_sz, { .7f,.7f,.7f, 1.f } );

				if ( menu::button_behavior( close_pos, close_sz ) == menu::button_result_t::press ) {
					notifications.erase( notifications.begin( ) + i );
					i--;
				}
			} );
		}
		ImGui::PopID( );
	}

	{
		ImGui::SetNextWindowSize( { 300, 200 }, ImGuiCond_Once );
		if ( ImGui::Begin( "Debugging board" ) ) {
			if ( ImGui::Button( "Toggle Game Joined" ) ) {
				game_joined = !game_joined;
			}
			if ( ImGui::Button( "Toggle Authed" ) ) {
				authed = !authed;
			}
			if ( ImGui::Button( "Toggle Menu" ) ) {
				menu_toggled = !menu_toggled;
				anim_navbar.reset( );
			}
			ImGui::Text( "Selected tab: %d", selected_tab.load() );
			ImGui::Text( "Menu toggled: %s", menu_toggled ? "true" : "false" );
			ImGui::Text( "Game joined: %s", game_joined ? "true" : "false" );
			ImGui::Text( "Authed: %s", authed ? "true" : "false" );

			ImGui::End( );
		}
	}
}
