#include "drawing_types.hpp"

#include <rbx/environment/utilities/storage/storage.hpp>

#include <renderer/renderer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <dependencies/imgui/stb_image.h>

// Simple helper function to load an image into a DX11 texture with common settings
static bool LoadTextureFromMemory( std::string_view bytes, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height )
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	const auto image_data = stbi_load_from_memory( (unsigned char*)bytes.data( ), bytes.size( ), &image_width, &image_height, NULL, 4 );

	if ( image_data == NULL )
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_renderer.get_device( )->CreateTexture2D( &desc, &subResource, &pTexture );

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( srvDesc ) );
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_renderer.get_device( )->CreateShaderResourceView( pTexture, &srvDesc, out_srv );
	pTexture->Release( );

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free( image_data );

	return true;
}

void rbx::environment::utilities::base_t::remove( )
{
	g_drawing_cache.remove( this ); //TODO: need to do some testing to see if this should free or not
}

void rbx::environment::utilities::image_t::set_data( std::string_view img_data ) // TODO: prolly a memleak here
{
	std::int32_t x, y;
	loaded = LoadTextureFromMemory( img_data, &srv, &x, &y );
}

ImVec2 rbx::environment::utilities::text_t::calc_text_sz( )
{
	ImGuiContext& g = *ImGui::GetCurrentContext( );

	ImFont* font = g_renderer.get_font( this->font );

	ImVec2 text_size = font->CalcTextSizeA( size, FLT_MAX, -1.0f, text.c_str( ), nullptr, nullptr );
	text_size.x = IM_FLOOR( text_size.x + 0.99999f );

	return text_size;
}

sol::object rbx::environment::utilities::text_t::get_text_bounds( sol::this_state L )
{
	sol::state_view sv{ L };

	const auto [x, y] = calc_text_sz( );

	return sv[ "Vector2" ][ "new" ]( x, y );
}

void rbx::environment::utilities::text_t::render( )
{
	const auto dl = ImGui::GetBackgroundDrawList( );

	if ( this->visible )
	{
		ImFont* font = g_renderer.get_font( this->font );

		const auto text_sz = calc_text_sz( );

		auto [pos_x, pos_y] = position;

		if ( this->center )
		{
			pos_x -= text_sz.x / 2.f;
		}

		ImColor c{ this->color };

		c.Value.w = this->transparency;

		//TODO: font, proper support for \n
		if ( outline )
		{
			ImColor col{ this->outline_color };

			col.Value.w = this->transparency;

			ImVec2 pos1( { pos_x, pos_y } );
			ImVec2 pos2( pos1 );
			ImVec2 pos3( pos1 );
			ImVec2 pos4( pos1 );

			pos1.x -= 1;
			pos1.y -= 1;

			dl->AddText( font, this->size, pos1, col, this->text.c_str( ), nullptr );
			pos2.x -= 1;
			pos2.y += 1;
			dl->AddText( font, this->size, pos2, col, this->text.c_str( ), nullptr );
			pos3.x += 1;
			pos3.y -= 1;
			dl->AddText( font, this->size, pos3, col, this->text.c_str( ), nullptr );
			pos4.x += 1;
			pos4.y += 1;
			dl->AddText( font, this->size, pos4, col, this->text.c_str( ), nullptr );
		}

		dl->AddText( font, this->size, ImVec2{ pos_x, pos_y }, c, this->text.c_str( ) );
	}
};
