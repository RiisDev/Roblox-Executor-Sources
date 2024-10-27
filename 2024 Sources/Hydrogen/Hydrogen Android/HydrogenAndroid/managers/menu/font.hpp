#pragma once

#include <cstdint>
#include <dependencies/imgui/imgui.h>

namespace menu {
	class font_t {
	private:
		font_t( ImFont* font, float size );
		~font_t( ) = default;

		ImFont* font;
		float size;

	public:
		static font_t* get( const char* const font_data, float font_size );

		float get_size( ) const;
		ImFont* get_font( ) const;
	};

	constexpr float get_base_font_size( ) {
		return 50.f;
	}
}
