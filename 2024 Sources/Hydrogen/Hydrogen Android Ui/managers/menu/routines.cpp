#define IMGUI_DEFINE_MATH_OPERATORS

#include "routines.hpp"

#include <dependencies/imgui/imgui_internal.h>

ImVec2 menu::aspect_ratio_constraint( const ImVec2& target, float scale, float ratio )
{
	const auto width_res = ImVec2{ target.x * scale, target.x * scale / ratio };
	const auto height_res = ImVec2{ target.y * scale * ratio, target.y * scale };

	const float wrm = sqrtf( width_res.x * width_res.x + width_res.y * width_res.y );
	const float hrm = sqrtf( height_res.x * height_res.x + height_res.y * height_res.y );

	return wrm < hrm ? width_res : height_res;
}

void menu::window( const std::string& name, const ImVec2& pos, const ImVec2& size, std::function<void( )> callback )
{
	ImGui::SetNextWindowPos( pos, ImGuiCond_Always );
	ImGui::SetNextWindowSize( size + ImVec2{ 1.f, 1.f }, ImGuiCond_Always );
	ImGui::SetNextWindowBgAlpha( 0.0f );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.f );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2{ 0.f , 0.f } );
	if ( ImGui::Begin( name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus ) ) {
		callback( );
		ImGui::End( );
	}
	ImGui::PopStyleVar( 2 );
}

menu::button_result_t menu::button_behavior( const ImVec2& pos, const ImVec2& size )
{
	if ( !ImGui::IsMouseHoveringRect( pos, { pos.x + size.x, pos.y + size.y }, false ) ) return button_result_t::none;
	if ( ImGui::GetIO( ).MouseDragMaxDistanceSqr[ 0 ] >= 100.f ) return button_result_t::none;

	if ( ImGui::IsMouseReleased( 0 ) ) {
		if ( ImGui::GetIO( ).MouseDownDurationPrev[ 0 ] < k_max_press_duration ) {
			ImGui::GetIO( ).MouseDownDurationPrev[ 0 ] = k_max_press_duration; // we only want to trigger this once
			return button_result_t::press;
		}
	}
	else if ( ImGui::IsMouseDown( 0 ) )
	{
		if ( ImGui::GetIO( ).MouseDownDuration[ 0 ] >= k_max_press_duration )
		{
			ImGui::GetIO( ).AddMouseButtonEvent( 0, false ); // manually unpress
			return button_result_t::long_press;
		}
		else return button_result_t::down;
	}

	return button_result_t::none;
}

ImVec2 menu::calc_text_size( const std::string& text, font_t* font )
{
	if ( !font ) return { 0.f, 0.f };
	if ( !font->get_font( ) ) return { 0.0f, 0.0f };
	return font->get_font( )->CalcTextSizeA( font->get_size( ), FLT_MAX, 0.0f, text.c_str( ) );
}

ImDrawList* get_current_draw_list( ) {
	auto window = ImGui::GetCurrentWindow( );
	if ( !window ) return nullptr;
	return window->DrawList;
}

void menu::draw_rect( const ImVec2& pos, const ImVec2& size, const ImColor& color, float rounding, float border_size, const ImColor& border_color, bool border_inside )
{
	auto dl = get_current_draw_list( );
	if ( !dl ) return;
	
	if ( color.Value.w <= 0.0f )
		return;	
	dl->AddRectFilled( pos, { pos.x + size.x, pos.y + size.y }, color, rounding, ImDrawCornerFlags_All );
	if ( border_size > 0.0f )
	{
		if ( !border_inside ) {
			dl->AddRectFilled(
				{ pos.x - border_size, pos.y - border_size },
				{ pos.x + size.x + border_size, pos.y + size.y + border_size },
				border_color, rounding, ImDrawCornerFlags_All
			);
		}
		else {
			dl->AddRectFilled(
				{ pos.x + border_size, pos.y + border_size },
				{ pos.x + size.x - border_size, pos.y + size.y - border_size },
				border_color, rounding, ImDrawCornerFlags_All
			);
		}
	}
}

void menu::draw_img( image_t* img, const ImVec2& pos, const ImVec2& size, const ImColor& color )
{
	auto dl = get_current_draw_list( );
	if ( !dl ) return;

	if ( color.Value.w <= 0.0f )
		return;
	
	if ( !img ) {
	failed:
		dl->AddRectFilled( pos, { pos.x + size.x, pos.y + size.y }
		, ImColor{ 1.f, 0.f, 1.f, color.Value.w } );
		return;
	}
	
	auto tex = img->get_texture( );
	if ( !tex || !img->is_valid( ) )
		goto failed;

	dl->AddImage( reinterpret_cast< void* >( tex ), pos, { pos.x + size.x, pos.y + size.y }, { 0.f, 0.f }, { 1.f, 1.f }, ImColor( color ) );
}

void menu::draw_text( const std::string& text, font_t* font, const ImVec2& pos, const ImColor& color )
{
	auto dl = get_current_draw_list( );
	if ( !dl ) return;
	if ( !font || !font->get_font( ) ) return;
	if ( color.Value.w <= 0.0f ) return;
	dl->AddText( font->get_font( ), font->get_size( ), pos, color, text.c_str( ) );
}
