#include "drawing.hpp"

#include <rbx/environment/utilities/drawing_types/drawing_types.hpp>
#include <rbx/environment/utilities/storage/storage.hpp>

sol::object rbx::environment::c_drawing::drawingnew( std::string type, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto obj_type = sv[ "Drawing" ][ type ];

	if ( obj_type == sol::nil )
		luaL_error( L, "Invalid drawing type" );

	return obj_type[ "new" ].call< sol::object >( );
}

void rbx::environment::c_drawing::cleardrawcache( sol::this_state L )
{
	rbx::environment::utilities::g_drawing_cache.clear( ); //TODO: invalidate reference
}

sol::object rbx::environment::c_drawing::getrenderproperty( sol::userdata obj, std::string_view prop, sol::this_state L )
{
	if ( !obj.is< sol::userdata >( ) )
		luaL_error( L, "Expected render object" );

	const auto in = *reinterpret_cast< void** >( const_cast< void* >( obj.pointer( ) ) ); 

	if ( !rbx::environment::utilities::g_drawing_cache.contains( in ) )
		luaL_error( L, "Expected render object" );

	return obj[ prop ];
}

void rbx::environment::c_drawing::setrenderproperty( sol::userdata obj, std::string_view prop, sol::object value, sol::this_state L )
{
	if ( !obj.is< sol::userdata >( ) )
		luaL_error( L, "Expected render object" );

	const auto in = *reinterpret_cast< void** >( const_cast< void* >( obj.pointer( ) ) );

	if ( !rbx::environment::utilities::g_drawing_cache.contains( in ) )
		luaL_error( L, "Expected render object" );

	obj[ prop ] = value;
}

bool rbx::environment::c_drawing::isrenderobj( sol::object obj )
{
	if ( !obj.is< sol::userdata >( ) )
		return false;

	const auto in = *reinterpret_cast< void** >( const_cast< void* >( obj.pointer( ) ) );

	return rbx::environment::utilities::g_drawing_cache.contains( in );
}

void rbx::environment::c_drawing::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	sol::table drawing = sv.create_named_table( "Drawing" );

	sol::table fonts = sv.create_table_with( "UI", 0, "System", 1, "Plex", 2, "Monospace", 3 );

	drawing[ "Fonts" ] = fonts;

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	const auto register_drawing_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			drawing.set_function( alias, function );
	};

	register_drawing_fn( drawingnew, { "new" } );

	register_fn( cleardrawcache, { "cleardrawcache" } );
	register_fn( getrenderproperty, { "getrenderproperty" } );
	register_fn( setrenderproperty, { "setrenderproperty" } );
	register_fn( isrenderobj, { "isrenderobj" } );

	using base_t = utilities::base_t;
	using line_t = utilities::line_t;
	using text_t = utilities::text_t;
	using image_t = utilities::image_t;
	using circle_t = utilities::circle_t;
	using square_t = utilities::square_t;
	using quad_t = utilities::quad_t;
	using triangle_t = utilities::triangle_t;

	drawing.new_usertype< line_t >( "Line",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Thickness", &line_t::thickness,
		"To", sol::property( &line_t::get_to, &line_t::set_to ),
		"From", sol::property( &line_t::get_from, &line_t::set_from ),

		sol::meta_function::construct, sol::factories( [ ]( )
		{
			line_t* obj = new line_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< text_t >( "Text",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Text", &text_t::text,
		"Size", &text_t::size,
		"Center", &text_t::center,
		"Outline", &text_t::outline,
		"OutlineColor", sol::property( &text_t::get_outline_color, &text_t::set_outline_color ),
		"Position", sol::property( &text_t::get_position, &text_t::set_position ),
		"TextBounds", sol::property( &text_t::get_text_bounds ),
		"Font", &text_t::font,

		sol::meta_function::construct, sol::factories( [ ]( )
		{
			text_t* obj = new text_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< image_t >( "Image",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Data", sol::property( &image_t::set_data ),
		"Size", sol::property( &image_t::get_size, &image_t::set_size ),
		"Position", sol::property( &image_t::get_position, &image_t::set_position ),
		"Rounding", &image_t::rounding,
		"Loaded", sol::readonly_property( &image_t::loaded ),

		sol::meta_function::construct, sol::factories( [ ]( )
		{
		image_t* obj = new image_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< circle_t >( "Circle",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Thickness", &circle_t::thickness,
		"NumSides", &circle_t::numsides,
		"Radius", &circle_t::radius,
		"Filled", &circle_t::filled,
		
		"Position", sol::property( &circle_t::get_position, &circle_t::set_position ),

		sol::meta_function::construct, sol::factories( [ ]( )
		{
			circle_t* obj = new circle_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< square_t >( "Square",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Thickness", &square_t::thickness,
		"Filled", &square_t::filled,
		"Size", sol::property( &square_t::get_size, &square_t::set_size ),
		"Position", sol::property( &square_t::get_position, &square_t::set_position ),

		sol::meta_function::construct, sol::factories( [ ]( ) { 
			square_t* obj = new square_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< quad_t >( "Quad",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Thickness", & quad_t::thickness,
		"PointA", sol::property( &quad_t::get_pointa, &quad_t::set_pointa ),
		"PointB", sol::property( &quad_t::get_pointb, &quad_t::set_pointb ),
		"PointC", sol::property( &quad_t::get_pointc, &quad_t::set_pointc ),
		"PointD", sol::property( &quad_t::get_pointd, &quad_t::set_pointd ),
		"Filled", & quad_t::filled,

		sol::meta_function::construct, sol::factories( [ ]( ) { 
			quad_t* obj = new quad_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);

	drawing.new_usertype< triangle_t >( "Triangle",
		"Visible", &base_t::visible,
		"ZIndex", &base_t::zindex,
		"Transparency", &base_t::transparency,
		"Color", sol::property( &base_t::get_color, &base_t::set_color ),

		"Thickness", &triangle_t::thickness,
		"PointA", sol::property( &triangle_t::get_pointa, &triangle_t::set_pointa ),
		"PointB", sol::property( &triangle_t::get_pointb, &triangle_t::set_pointb ),
		"PointC", sol::property( &triangle_t::get_pointc, &triangle_t::set_pointc ),
		"Filled", &triangle_t::filled,

		sol::meta_function::construct, sol::factories( [ ]( ) { 
			triangle_t* obj = new triangle_t{ };

			rbx::environment::utilities::g_drawing_cache.add( obj );

			return obj;
		} ),

		"Remove", &base_t::remove,
		"Destroy", &base_t::remove,
		sol::base_classes, sol::bases< base_t >( )
	);
}
