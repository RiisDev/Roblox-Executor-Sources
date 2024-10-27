#include "font.hpp"

#include <unordered_map>
#include <utility>
#include <cmath>

#include <dependencies/imgui/imgui_impl_opengl3.h>
static std::unordered_map<const char*, menu::font_t*> font_map;

menu::font_t::font_t( ImFont* font, float size ): font( font ), size( size )
{}

menu::font_t* menu::font_t::get( const char* const font_data, float font_size )
{
	auto it = font_map.find( font_data );
	font_t* result = nullptr;
	if ( it != font_map.end( ) ) {
		it->second->size = font_size;
		result = it->second;
	}
	else {
		result = new font_t( ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedBase85TTF( font_data, get_base_font_size( ) ), font_size );
		ImGui_ImplOpenGL3_CreateFontsTexture( );
		font_map.emplace( font_data, result );
	}

	result->get_font( )->Scale = font_size / get_base_font_size( );

	return result;
}

float menu::font_t::get_size( ) const
{
	return size;
}

ImFont* menu::font_t::get_font( ) const
{
	return font;
}
