#include "connection.hpp"

#include <sol/sol.hpp>

using c_connection
	= rbx::environment::utilities::c_connection;

//TODO: delete created bindable event
void c_connection::set_connection_tag( lua_State* L )
{
	sol::state_view sv{ L };

	const auto bindable_event
		= sv[ "Instance" ][ "new" ]( "BindableEvent" ).get< sol::userdata >( );

	bindable_event[ "Event" ][ "Connect" ]( bindable_event[ "Event" ], 
			sol::as_function( [ ]( lua_State* )->std::int32_t { return 0; } ) )
				.get< sol::object >( ).push( );

	connection_tag = lua_userdatatag( L, -1 );
}

std::int32_t c_connection::get_connection_tag( )
{
	return connection_tag;
}

std::uint8_t* c_connection::get_weak_function_ref( std::uint8_t* connection )
{
	const auto slot_wrapper
		= *reinterpret_cast< std::uint8_t** >( connection + 0x18 );

	if ( !slot_wrapper )
		return 0;

	const auto weak_function_ref
		= *reinterpret_cast< std::uint8_t** >( slot_wrapper + 0x30 );

	return weak_function_ref;
}

lua_State* c_connection::get_connections_thread( std::uint8_t* connection )
{
	const auto weak_function_ref
		= get_weak_function_ref( connection );

	if ( !weak_function_ref )
		return 0;

	const auto thread
		= *reinterpret_cast< lua_State** >( weak_function_ref + 0x4 );

	return thread;
}

sol::function c_connection::get_connections_function( std::uint8_t* connection )
{
	const auto weak_function_ref
		= get_weak_function_ref( connection );

	if ( !weak_function_ref )
		return sol::function{ };

	const auto function_id
		= *reinterpret_cast< std::uint32_t* >( weak_function_ref + 0xC );

	const auto thread
		= get_connections_thread( connection );

	sol::state_view sv{ thread };

	return sv.lua_registry( )
		[ function_id ].get< sol::function >( );
}
