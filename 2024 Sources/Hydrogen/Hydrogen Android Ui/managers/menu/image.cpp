#include "image.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

static std::vector<menu::image_t*> images;

menu::image_t::image_t( const char* const data, std::size_t size ): data{ data }, size{ size }
{
	images.push_back( this );
}

menu::image_t::~image_t( )
{
	auto it = std::find( images.begin( ), images.end( ), this );
	if ( it != images.end( ) )
		images.erase( it );
}

void menu::image_t::load( )
{
	if ( !data || tex )
		return;

	auto res = managers::renderer_mgr.load_texture( data, size, &tex, &width, &height );
	if ( !res ) {
		tex = 0, width = 0, height = 0, data = nullptr;
	}
}

void menu::image_t::reset_all( )
{
	for ( auto i : images ) {
		i->reset( );
	}
}

void menu::image_t::reset( )
{
	if ( !tex ) return;
	glDeleteTextures( 1, &tex );
	tex = NULL;
}

void menu::image_t::dispose( )
{
	delete this;
}

bool menu::image_t::is_valid( )
{
	return data != nullptr;
}

menu::image_t* menu::image_t::from_data( const std::uint8_t* data, std::size_t size )
{
	return new image_t( reinterpret_cast< const char* >( data ), size );
}

ImVec2 menu::image_t::get_size( )
{
	load( );
	return ImVec2( static_cast< float >( width ), static_cast< float >( height ) );
}

GLuint menu::image_t::get_texture( )
{
	load( );
	return tex;
}
