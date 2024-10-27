#pragma once

#include <functional>

#include <managers/managers.hpp>
#include "font.hpp"

namespace menu {
	constexpr float k_max_press_duration = 0.25f;

	enum class button_result_t {
		none,
		down,
		press,
		long_press,
	};

	// utils
	ImVec2 aspect_ratio_constraint( const ImVec2& target, float scale, float ratio );

	// routines
	void window( const std::string& name, const ImVec2& pos, const ImVec2& size, std::function<void( )> callback );	
	button_result_t button_behavior( const ImVec2& pos, const ImVec2& size );
	ImVec2 calc_text_size( const std::string& text, font_t* font );

	// drawing
	void draw_rect( const ImVec2& pos, const ImVec2& size, const ImColor& color, float rounding = 0.f, float border_size = 0.0f, const ImColor& border_color = ImColor( 0, 0, 0, 0 ), bool border_inside = false );
	void draw_img( image_t* img, const ImVec2& pos, const ImVec2& size, const ImColor& color );
	void draw_text( const std::string& text, font_t* font, const ImVec2& pos, const ImColor& color );
}