#pragma once

#include <string>
#include <type_traits>

#include <managers/managers.hpp>

namespace menu {
	namespace easings {
		float linear( float t );
		using type_t = decltype( linear );

		float in_sine( float t );
		float out_sine( float t );
		float inout_sine( float t );
		float in_quad( float t );
		float out_quad( float t );
		float inout_quad( float t );
		float in_cubic( float t );
		float out_cubic( float t );
		float inout_cubic( float t );
		float in_quart( float t );
		float out_quart( float t );
		float inout_quart( float t );
		float in_quint( float t );
		float out_quint( float t );
		float inout_quint( float t );
		float in_expo( float t );
		float out_expo( float t );
		float inout_expo( float t );
		float in_circ( float t );
		float out_circ( float t );
		float inout_circ( float t );
		float in_back( float t );
		float out_back( float t );
		float inout_back( float t );
		float in_elastic( float t );
		float out_elastic( float t );
		float inout_elastic( float t );
		float in_bounce( float t );
		float out_bounce( float t );
		float inout_bounce( float t );
	}

	class animator_t {
	private:
		float time_i = 0;

		animator_t( const animator_t& ) = delete;
		animator_t( );
		~animator_t( ) = default;
	public:
		void reset( );
		void timeout( float seconds );

		static animator_t& get( const std::string& id );
		void dispose( );

		/* lerping */
		template<class T>
		T lerp( T min, T max, float duration = 1.f, bool invert = false, easings::type_t easing = easings::linear ) {
			static_assert( std::is_floating_point<T>::value, "lerping must be a floating-point value." );

			if ( !time_i ) return invert ? min : max;

			float x = ( static_cast< float >( managers::renderer_mgr.epoch( ) ) - time_i ) / duration;
			x = x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x;
			x = invert ? 1.f - x : x;
			float t = easing( x );

			return min + ( max - min ) * t;
		}

		template <>
		ImVec2 lerp<ImVec2>( ImVec2 min, ImVec2 max, float duration, bool invert, easings::type_t easing ) {
			return ImVec2(
				lerp( min.x, max.x, duration, invert, easing ),
				lerp( min.y, max.y, duration, invert, easing )
			);
		}

		template <>
		ImVec4 lerp<ImVec4>( ImVec4 min, ImVec4 max, float duration, bool invert, easings::type_t easing ) {
			return ImVec4(
				lerp( min.x, max.x, duration, invert, easing ),
				lerp( min.y, max.y, duration, invert, easing ),
				lerp( min.z, max.z, duration, invert, easing ),
				lerp( min.w, max.w, duration, invert, easing )
			);
		}

		template <>
		ImColor lerp<ImColor>( ImColor min, ImColor max, float duration, bool invert, easings::type_t easing ) {
			return ImColor(
				lerp( min.Value.x, max.Value.x, duration, invert, easing ),
				lerp( min.Value.y, max.Value.y, duration, invert, easing ),
				lerp( min.Value.z, max.Value.z, duration, invert, easing ),
				lerp( min.Value.w, max.Value.w, duration, invert, easing )
			);
		}
	};
}