#pragma once

#include <managers/renderer/renderer.hpp>

namespace menu {
	class image_t {
	private:
		GLuint tex = NULL;
		const char* data;
		std::size_t size;
		int width, height;

		image_t( const char* const data, std::size_t size );
		~image_t( );

		void load( );
	public:
		static void reset_all( );
		void reset( );

		void dispose( );

		bool is_valid( );

		static image_t* from_data( const std::uint8_t* data, std::size_t size );

		ImVec2 get_size( );
		GLuint get_texture( );
	};
}