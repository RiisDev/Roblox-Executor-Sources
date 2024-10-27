#pragma once

#include <dependencies/luau/lua.h>
#include <sol/sol.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <imgui/imgui.h>
#include "imgui/imgui_internal.h"

#include "../utilities.hpp"

namespace rbx::environment::utilities
{
	struct color_t
	{
		float r, g, b;

		operator ImVec4( ) { return { r, g, b, 0.f }; }

		operator ImVec4( ) const { return { r, g, b, 0.f }; }

		operator ImColor( ) const { return { r, g, b, 0.f }; }
	};

#define VEC2_GET_SET(prop) \
	sol::object get_ ## prop ##( sol::this_state L ) \
	{ \
		sol::state_view sv{ L };\
		const auto [x, y] = prop;\
		return sv[ "Vector2" ][ "new" ]( x, y );\
	}; \
	void set_ ## prop ##( sol::object vector, sol::this_state L ) \
	{ \
		c_utilities::e_is_typeof( 1, vector, "Vector2" ); \
		prop = *reinterpret_cast< const ImVec2* >( vector.pointer( ) ); \
	};

#define COL_GET_SET(prop) \
	sol::object get_ ## prop ##( sol::this_state L ) \
	{ \
		sol::state_view sv{ L }; \
		const auto [r, g, b] = prop; \
		return sv[ "Color3" ][ "new" ]( r, g, b ); \
	}; \
	void set_ ## prop ##( sol::object c, sol::this_state L ) \
	{ \
		c_utilities::e_is_typeof( 1, c, "Color3" ); \
		prop = *reinterpret_cast< const color_t* >( c.pointer( ) ); \
	};

	struct base_t
	{
		bool visible;
		int zindex;
		double transparency = 1;
		color_t color;

		COL_GET_SET( color )

		virtual void render( ) = 0;

		void remove( );

		virtual ~base_t( ) { }
	};

	struct line_t : public base_t
	{
		double thickness;
		ImVec2 from;
		ImVec2 to;

		VEC2_GET_SET( from )
		VEC2_GET_SET( to )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				dl->AddLine( this->from, this->to, c, this->thickness );
			}
		};
	};

	struct text_t : public base_t
	{
		std::string text;
		double size;
		bool center;
		bool outline;
		color_t outline_color;
		ImVec2 position;
		ImVec2 text_bounds;
		std::uint32_t font;

		COL_GET_SET( outline_color )

		VEC2_GET_SET( position )

		ImVec2 calc_text_sz( );

		sol::object get_text_bounds( sol::this_state L );

		void render( ) override;
	};

	struct image_t : public base_t
	{
		ID3D11ShaderResourceView* srv = nullptr;
		ImVec2 size;
		ImVec2 position;
		double rounding;
		bool loaded;

		void set_data( std::string_view img_data );

		VEC2_GET_SET( size )
		VEC2_GET_SET( position )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				dl->AddImageRounded( reinterpret_cast< void* >( srv ), position, position + size, ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImGui::GetColorU32( ImVec4( 1, 1, 1, 1 ) ), rounding );
			}
		};
	};

	struct circle_t : public base_t
	{
		double thickness;
		double numsides;
		double radius;
		bool filled;
		ImVec2 position;

		VEC2_GET_SET( position )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				if ( this->filled )
					dl->AddCircleFilled( position, radius, c, numsides );
				else
					dl->AddCircle( position, radius, c, numsides, thickness );
			}
		};
	};

	struct square_t : public base_t //TODO: rounding might be a nice addition
	{
		double thickness;
		ImVec2 size;
		ImVec2 position;
		bool filled;

		VEC2_GET_SET( size )
		VEC2_GET_SET( position )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				if ( this->filled )
					dl->AddRectFilled( this->position, this->position + this->size, c );
				else
					dl->AddRect( this->position, this->position + this->size, c, 0, 0, this->thickness );
			}
		};
	};

	struct quad_t : public base_t 
	{
		double thickness;
		ImVec2 pointa;
		ImVec2 pointb;
		ImVec2 pointc;
		ImVec2 pointd;
		bool filled;

		VEC2_GET_SET( pointa )
		VEC2_GET_SET( pointb )
		VEC2_GET_SET( pointc )
		VEC2_GET_SET( pointd )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				if ( this->filled )
					dl->AddQuadFilled( pointa, pointb, pointc, pointd, c );
				else
					dl->AddQuad( pointa, pointb, pointc, pointd, c, thickness );
			}
		};
	};

	struct triangle_t : public base_t
	{
		double thickness;
		ImVec2 pointa;
		ImVec2 pointb;
		ImVec2 pointc;
		bool filled;

		VEC2_GET_SET( pointa )
		VEC2_GET_SET( pointb )
		VEC2_GET_SET( pointc )

		void render( ) override
		{
			const auto dl = ImGui::GetBackgroundDrawList( );

			if ( this->visible )
			{
				ImColor c{ this->color };

				c.Value.w = this->transparency;

				if ( this->filled )
					dl->AddTriangleFilled( pointa, pointb, pointc, c );
				else
					dl->AddTriangle( pointa, pointb, pointc, c, thickness );
			}
		};
	};
}