#include "core.hpp"

#include <fstream>
#include <set>

#include <managers/managers.hpp>
#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>
#include <Luau/lmem.h>

#include <dependencies/curl/util.h>
#include <dependencies/xxhash/xxhash.hpp>
#include <dependencies/zstd/zstd.hpp>
#include <dependencies/sha384/sha384.h>
#include <utilities/io.hpp>
#include <managers/roblox/structures/instance/instance.hpp>
#include <dependencies/Luau/Luau/Bytecode.h>
#include <dependencies/lz4/lz4.h>
#include <managers/roblox/structures/task_scheduler/task_scheduler.hpp>
#include <dependencies/curl/http.hpp>

std::int32_t( *roblox::environment::core::old_require )( lua_State* state );

std::int32_t roblox::environment::core::loadstring( lua_State* state )
{
	l( );

	std::size_t sz;
	const auto script = luaL_checklstring( state, 1, &sz );

	const auto chunkname = lua_gettop( state ) > 1 ? luaL_checklstring( state, 2, nullptr ) : nullptr;

	if ( !managers::mgrs.roblox_mgr.push_script( state, { script, sz }, chunkname ) )
	{
		lua_pushnil( state );
		lua_pushvalue( state, -2 );

		return 2;
	}

	return 1;
}

std::int32_t roblox::environment::core::get_function_address( lua_State* state )
{
	const auto cfunc = lua_tocfunction( state, 1 );

	std::printf( "Address: %p\n", cfunc );

	return 0;
}

std::int32_t roblox::environment::core::getgenv( lua_State* state )
{
	l( );

	lua_pushvalue( managers::mgrs.roblox_mgr.globals.thread, LUA_GLOBALSINDEX );
	lua_xmove( managers::mgrs.roblox_mgr.globals.thread, state, 1 );

	return 1;
}

std::int32_t roblox::environment::core::getrenv( lua_State* state )
{
	l( );

	lua_pushvalue( managers::mgrs.roblox_mgr.globals.state, LUA_GLOBALSINDEX );
	lua_xmove( managers::mgrs.roblox_mgr.globals.state, state, 1 );

	return 1;
}

std::int32_t roblox::environment::core::getrawmetatable( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isuserdata( state, 1 ) || lua_istable( state, 1 ), 1, "Userdata or Table expected" );

	lua_getmetatable( state, 1 );

	return 1;
}

std::int32_t roblox::environment::core::setrawmetatable( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isuserdata( state, 1 ) || lua_istable( state, 1 ), 1, "Userdata or Table expected" );
	luaL_checktype( state, 2, LUA_TTABLE );

	lua_pushvalue( state, 1 );
	lua_pushvalue( state, 2 );
	lua_setmetatable( state, -2 );

	return 0;
}

std::int32_t roblox::environment::core::getreg( lua_State* state )
{
	l( );

	lua_pushvalue( state, LUA_REGISTRYINDEX );

	return 1;
}

std::int32_t roblox::environment::core::setreadonly( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );

	const auto val = lua_isboolean( state, 2 ) ? lua_toboolean( state, 2 ) : lua_isnumber( state, 2 ) == 1;

	lua_setreadonly( state, 1, val );

	return 0;
}

std::int32_t roblox::environment::core::isreadonly( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );

	lua_pushboolean( state, hvalue( index2addr( state, 1 ) )->readonly );

	return 1;
}

std::int32_t roblox::environment::core::is_executor_closure ( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TFUNCTION );

	const auto closure = clvalue( index2addr( state, 1 ) );

	lua_pushboolean( state, managers::mgrs.roblox_mgr.function_map.contains( closure ) );
	return 1;
}

std::int32_t roblox::environment::core::identifyexecutor( lua_State* state )
{
	l( );

	lua_pushstring( state, "Hydrogen" );
	return 1;
}

std::int32_t roblox::environment::core::islclosure( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TFUNCTION );

	lua_pushboolean( state, lua_iscfunction( state, 1 ) == 0 );

	return 1;
}

std::int32_t roblox::environment::core::iscclosure( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TFUNCTION );

	lua_pushboolean( state, lua_iscfunction( state, 1 ) );

	return 1;
}

std::int32_t roblox::environment::core::test( lua_State* state )
{
	static const auto func = reinterpret_cast< int( __thiscall* )( void*, bool ) >( base + 0x7ee370 );

	const auto userdata = *reinterpret_cast< void** >( lua_touserdata( state, 1 ) );

	func( userdata, true );
	return 0;
}

std::int32_t roblox::environment::core::messagebox( lua_State* state )
{
	l( );

	const auto text = luaL_checkstring( state, 1 );
	const auto caption = luaL_checkstring( state, 2 );

	const auto flags = luaL_checkinteger( state, 3 );

	lua_pushthread( state );
	lua_ref( state, -1 );

	std::thread {
		[ state, text, caption, flags ]( )
		{
			const auto result = MessageBoxA( nullptr, text, caption, flags );

			managers::mgrs.roblox_mgr.queue_yield_callback(
			[ state, result ] ( ) /* todo: impersonator of thread */
			{
				lua_pushinteger( state, result );

				managers::mgrs.roblox_mgr.resume(state, 1);
			} );
		}
	}.detach( );

	return lua_yield( state, 1 );
}

std::int32_t roblox::environment::core::getgc( lua_State* state )
{
	l( );

	const auto everything = lua_gettop( state ) && lua_isboolean( state, 1 ) ? lua_toboolean( state, 1 ) : false;

	lua_newtable( state );

	auto count = 1u;
	for ( auto current_page = state->global->allgcopages; current_page; current_page = current_page->gcolistnext )
	{
		char* start, * end;
		int busy_blocks, block_size;
		luaM_getpagewalkinfo( current_page, &start, &end, &busy_blocks, &block_size );

		for ( auto i = start; i != end; i += block_size )
		{
			if ( const auto gco = reinterpret_cast< GCObject* >( i ) )
			{
				if ( gco && gco->gch.tt == LUA_TFUNCTION || ( ( gco->gch.tt == LUA_TTABLE || gco->gch.tt == LUA_TUSERDATA ) && everything ) )
				{
					state->top->value.gc = gco;
					state->top->tt = gco->gch.tt;
					api_incr_top( state );

					lua_rawseti( state, -2, count++ );
				}
			}
		}
	}

	return 1;
}

lua_State* get_script_state( const std::uintptr_t script )
{
	const auto& offs_mgr = managers::mgrs.offset_mgr;

	auto node = *reinterpret_cast< std::uintptr_t* >( script + offs_mgr.offsets.thread_node );
	auto weakthreadref = node ? *reinterpret_cast< std::uintptr_t* >( node + offs_mgr.offsets.node_first ) : 0;

	for ( ; weakthreadref; weakthreadref = *reinterpret_cast< std::uintptr_t* >( weakthreadref + offs_mgr.offsets.node_next ) )
	{
		auto ref_state = *reinterpret_cast< std::uintptr_t* >( weakthreadref + offs_mgr.offsets.state_off1 );

		if ( ref_state )
			ref_state = *reinterpret_cast< std::uintptr_t* >( ref_state + offs_mgr.offsets.state_off2 );
		else
			continue;

		if ( ref_state )
		{
			const auto ref_script = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( reinterpret_cast< lua_State* >( ref_state )->userdata ) + offs_mgr.offsets.extraspace_script );

			if ( ref_script )
			{
				if ( ref_script == script )
					return reinterpret_cast< lua_State* >( ref_state );
			}
		}
	}

	return nullptr;
}

std::int32_t roblox::environment::core::httpget_async( lua_State* state )
{
	l( );

	lua_pushboolean( state, true );

	return httpget( state );
}

//table.foreach(getsenv(game.Players.LocalPlayer.PlayerScripts.ChatScript), print)
std::int32_t roblox::environment::core::getsenv( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "LocalScript or ModuleScript", [ ] ( const char* class_name ) {
		return !std::strstr( class_name, "LocalScript" ) || !std::strstr( class_name, "ModuleScript" );
		} );

	const auto script = *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, 1 ) );

	if ( const auto script_state = get_script_state( script ) )
	{
		lua_pushvalue( script_state, LUA_GLOBALSINDEX );

		lua_xmove( script_state, state, 1 );
	}

	return 1;
}

std::int32_t roblox::environment::core::get_namecall_method( lua_State* state )
{
	//l( );

	const auto method = state->namecall;

	if ( method )
	{
		setsvalue( state, state->top, method );
		api_incr_top( state );
	}
	else
		lua_pushnil( state );

	return 1;
}

std::int32_t roblox::environment::core::set_namecall_method( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TSTRING );

	state->namecall = tsvalue( index2addr( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::core::checkcaller( lua_State* state )
{
	l( );

	const auto script_ptr = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( state->userdata ) + managers::mgrs.offset_mgr.offsets.extraspace_script );
	lua_pushboolean( state, !script_ptr );

	return 1;
}

std::int32_t roblox::environment::core::hookmetamethod( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isuserdata( state, 1 ) || lua_istable( state, 1 ), 1, "Userdata or Table expected at argument 1" );

	const auto method = luaL_checkstring( state, 2 );
	luaL_checktype( state, 3, LUA_TFUNCTION );

	lua_getglobal( state, "hookfunction" );

	if ( !luaL_getmetafield( state, 1, method ) )
		luaL_error( state, "Cannot find metatable for object" );

	lua_pushvalue( state, 3 );
	lua_call( state, 2, 1 );

	return 1;
}

std::int32_t roblox::environment::core::lz4compress( lua_State* state )
{
	std::size_t sz;
	const auto data = luaL_checklstring( state, 1, &sz );

	std::vector<char> buff;
	buff.reserve( LZ4_compressBound( sz ) );

	const auto size = LZ4_compress( data, buff.data( ), sz );

	lua_pushlstring( state, buff.data( ), size );
	return 1;
}

std::int32_t roblox::environment::core::lz4decompress( lua_State* state )
{
	std::size_t sz;
	const auto data = luaL_checklstring( state, 1, &sz );
	const auto size = luaL_checknumber( state, 2 );

	std::vector<char> buff( size );
	LZ4_decompress_safe( data, buff.data( ), sz, size );

	lua_pushlstring( state, buff.data( ), size );
	return 1;
}

std::int32_t roblox::environment::core::setfpscap( lua_State* state )
{
	const auto input = luaL_checknumber( state, 1 );

	const auto task_scheduler = roblox::structures::task_scheduler_t::get( );

	static const auto fps_cap_offset = [ task_scheduler ] ( ) -> std::uint16_t {
		for ( auto i = 0u; i < 0x500; i += 0x4 )
		{
			if ( 1.0 / *reinterpret_cast< double* >( task_scheduler + i ) == 60.0f )
				return i;
		}
	}( );

	*reinterpret_cast< double* >( task_scheduler + fps_cap_offset ) = input > 0.f ? 1.0f / input : 1.0 / 10000.0;

	return 0;
}

std::int32_t roblox::environment::core::getconnections( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );

	if ( std::strcmp( luaT_objtypename( state, index2addr( state, 1 ) ), "RBXScriptSignal" ) )
		luaL_typeerror( state, 1, "RBXScriptSignal" );

	lua_getfield( state, 1, "Connect" );
	lua_pushvalue( state, 1 );

	lua_pushcfunction( state, [ ] ( lua_State* ) {
		return 0;
		}, nullptr );

	lua_pcall( state, 2, 1, 0 );

	const auto signal = *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, -1 ) );

	lua_getfield( state, -1, "Disconnect" );
	lua_insert( state, -2 );
	lua_pcall( state, 1, 0, 0 );

	lua_newtable( state );

	if ( !signal )
		return 1;

	auto obj = *reinterpret_cast< std::uintptr_t* >( signal + 0x10 );
	auto iter = 1u;

	static const auto enable_func = [ ] ( lua_State* state ) -> std::int32_t {
		if ( const auto connection = reinterpret_cast< std::uintptr_t >( lua_topointer( state, lua_upvalueindex( 1 ) ) ); managers::mgrs.roblox_mgr.registered_connections[ connection ] )
			*reinterpret_cast< std::uintptr_t* >( connection + 0x14 ) = managers::mgrs.roblox_mgr.registered_connections[ connection ];

		return 0;
	};

	static const auto disable_func = [ ] ( lua_State* state ) -> std::int32_t {
		const auto connection = reinterpret_cast< std::uintptr_t >( lua_topointer( state, lua_upvalueindex( 1 ) ) );

		if ( !managers::mgrs.roblox_mgr.registered_connections[ connection ] )
			managers::mgrs.roblox_mgr.registered_connections[ connection ] = *reinterpret_cast< std::uintptr_t* >( connection + 0x14 );

		*reinterpret_cast< std::uintptr_t* >( connection + 0x14 ) = 0;

		return 0;
	};

	static const auto fire_func = [ ] ( lua_State* state ) -> std::int32_t {
		const auto argn = lua_gettop( state );

		lua_pushvalue( state, lua_upvalueindex( 1 ) );
		if ( !lua_isfunction( state, -1 ) )
			return 0;

		lua_insert( state, 2 );
		lua_call( state, argn - 1, 0 );

		return 0;
	};

	static const auto defer_func = [ ] ( lua_State* state ) -> std::int32_t {
		const auto argn = lua_gettop( state );

		lua_getglobal( state, "task" );
		lua_getfield( state, -1, "defer" );
		lua_insert( state, 2 );

		lua_pushvalue( state, lua_upvalueindex( 1 ) );
		if ( !lua_isfunction( state, -1 ) )
			return 0;

		lua_insert( state, 3 );
		lua_pop( state, 1 );
		lua_call( state, argn - 1, 0 );

		return 0;
	};

	// static const auto disconnect_func = [ ] ( lua_State* state ) -> std::int32_t {
	// 	const auto connection = reinterpret_cast<std::uintptr_t>(lua_topointer(state, lua_upvalueindex(1)));

	// 	std::printf( "Disconnecting connection %p\n", connection );
	// 	//const auto head = *reinterpret_cast<std::uintptr_t*>(connection);
	// 	//const auto next = *reinterpret_cast<std::uintptr_t*>(connection + 0x10);

	// 	//*reinterpret_cast<std::uintptr_t*>(head + 0x10) = next;
	// 	//*reinterpret_cast<std::uintptr_t*>(next) = head;

	// 	return 0;
	// };

	static const auto index_mt = [ ] ( lua_State* state ) -> std::int32_t {
		const std::string key = luaL_checkstring( state, 2 );

		if ( key == "State" || key == "Enabled" )
		{
			lua_getfield( state, 1, "__ENABLED" );
			return 1;
		}

		else if ( key == "Disconnect" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			//FIX
			lua_pushcclosure( state, disable_func, nullptr, 1 );

			return 1;
		}

		else if ( key == "Enable" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			lua_pushcclosure( state, enable_func, nullptr, 1 );

			return 1;
		}

		else if ( key == "LuaConnection" )
		{
			lua_getfield( state, 1, "Function" );

			if ( lua_isfunction( state, -1 ) )
				lua_pushboolean( state, !lua_iscfunction( state, -1 ) );
			else
				lua_pushboolean( state, false );

			return 1;
		}

		else if ( key == "Fire" )
		{
			lua_getfield( state, 1, "Function" );

			lua_pushcclosure( state, fire_func, nullptr, 1 );

			return 1;
		}

		else if ( key == "Defer" )
		{
			lua_getfield( state, 1, "Function" );

			lua_pushcclosure( state, defer_func, nullptr, 1 );

			return 1;
		}


		else if ( key == "Disable" || key == "Disconnect" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			lua_pushcclosure( state, disable_func, nullptr, 1 );

			return 1;
		}

		else if ( key == "ForeignState" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			const auto wrapper = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( lua_tolightuserdata( state, -1 ) ) + 0x1C );
			if ( const auto udata = *reinterpret_cast< std::uintptr_t* >( wrapper + 0x3C ) )
				lua_pushboolean( state, *reinterpret_cast< std::uintptr_t* >( udata + 4 ) );
			else
				lua_pushboolean( state, false );

			return 1;
		}

		else if ( key == "Thread" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			const auto wrapper = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( lua_tolightuserdata( state, -1 ) ) + 0x1C );
			if ( const auto udata = *reinterpret_cast< std::uintptr_t* >( wrapper + 0x3C ) )
			{
				auto ref = *reinterpret_cast< lua_State** >( udata + 4 );
				lua_pushthread( ref );
				lua_xmove( ref, state, 1 );

				return 1;
			}

			lua_pushnil( state );
			return 1;
		}

		else if ( key == "Function" )
		{
			lua_getfield( state, 1, "__OBJECT" );

			const auto wrapper = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( lua_tolightuserdata( state, -1 ) ) + 0x1C );

			if ( const auto udata = *reinterpret_cast< std::uintptr_t* >( wrapper + 0x3C ) )
			{
				const auto key = *reinterpret_cast< std::uintptr_t* >( udata + 0xC );

				lua_rawgeti( managers::mgrs.roblox_mgr.globals.state, LUA_REGISTRYINDEX, key );
				lua_xmove( managers::mgrs.roblox_mgr.globals.state, state, 1 );

				if ( !lua_isfunction( state, -1 ) )
					lua_pushnil( state );

				return 1;
			}
		}

		return 0;
	};

	while ( obj )
	{
		lua_pushinteger( state, iter++ );

		lua_newtable( state );
		{
			lua_pushstring( state, "__OBJECT" );
			lua_pushlightuserdata( state, reinterpret_cast< void* >( obj ) );
			lua_rawset( state, -3 );

			const auto enabled = *reinterpret_cast< std::uint8_t* >( obj + 0x14 );

			lua_pushstring( state, "__ENABLED" );
			lua_pushboolean( state, enabled );
			lua_rawset( state, -3 );

			lua_pushstring( state, "__OBJECT_ENABLED" );
			lua_pushboolean( state, enabled );
			lua_rawset( state, -3 );
		}

		lua_newtable( state );
		{
			// __index
			lua_pushcfunction( state, index_mt, nullptr );
		}

		lua_setfield( state, -2, "__index" );
		lua_setmetatable( state, -2 );
		lua_rawset( state, -3 );

		obj = *reinterpret_cast< std::uintptr_t* >( obj + 0x10 );
	}

	return 1;
}

std::int32_t roblox::environment::core::firesignal( lua_State* state )
{
	const auto argn = lua_gettop( state );

	getconnections( state );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		lua_getfield( state, -2, "Fire" );
		for ( auto i = 1; i <= argn; i++ )
			lua_pushvalue( state, i );

		lua_call( state, argn, 0 );

		lua_pop( state, 2 );
	}

	return 0;
}

std::int32_t roblox::environment::core::request(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);
	auto arg1 = index2addr(state, 1);

	lua_getfield(state, -1, "Url");

	if (lua_isnil(state, -1))
		luaL_error(state, "Url was not passed to request.");

	std::string uri = lua_tostring(state, -1);

	std::size_t start{};
	for (const auto& c : uri)
	{
		if (c != ' ')
			break;

		start++;
	}
	std::string protocol = uri.substr(start);
	std::transform(protocol.begin(), protocol.end(), protocol.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if ((protocol.size() >= 5 && protocol.substr(0, 5) == "file:") || (protocol.size() >= 7 && protocol.substr(0, 7) == "telnet:"))
		luaL_error(state, "Fail");

	lua_pop(state, 1);

	std::string method = "GET";
	std::string body = "";
	std::vector<std::string> headers;

	headers.push_back("User-Identifier: ");
	headers.push_back("Fingerprint: ");
	headers.push_back(("User-Agent: Hydrogen"));

	lua_getfield(state, -1, "Method");

	if (!lua_isnil(state, -1))
	{
		method = lua_tostring(state, -1);

		std::transform(method.begin(), method.end(), method.begin(),
			[](unsigned char c) { return std::toupper(c); });
	}
	lua_pop(state, 1);

	lua_getfield(state, -1, "Headers");

	if (!lua_isnil(state, -1))
	{
		lua_pushnil(state);

		while (lua_next(state, -2))
		{
			if (lua_isstring(state, -2))
			{
				auto header = std::string(lua_tostring(state, -2));
				std::string value = "";

				auto type_val = lua_type(state, -1);

				if (type_val == LUA_TBOOLEAN)
				{
					if (lua_toboolean(state, -1))
						value = "true";
					else
						value = "false";
				}
				else if (type_val == LUA_TNUMBER)
				{
					value = std::to_string(lua_tonumber(state, -1));
				}
				else if (type_val == LUA_TSTRING)
				{
					value = lua_tostring(state, -1);
				}
				//else if (type_val == LUA_TUSERDATA)
				//{
				//	auto ud = *reinterpret_cast<rbx::instance**>(lua_touserdata(state, -1));

				//	value = ud->name;
				//}

				headers.push_back(header + ": " + value);

			}

			lua_pop(state, 1);
		}
	}
	lua_pop(state, 1);

	lua_getfield(state, -1, "Body");

	if (!lua_isnil(state, -1))
	{
		std::size_t len;
		const auto data = lua_tolstring(state, -1, &len);

		auto str = index2addr(state, -1);

		body = std::string(data, len);
	}
	lua_pop(state, 1);

	auto result = http::send_request(uri, method, headers, body);

	lua_createtable(state, 0, 0);

	auto success = false;
	std::string response_body = "";

	if (result.has_value())
	{
		success = true;
		response_body = *result;
	}

	long statuscode = http::http_code;
	std::string successmessage = http::status_message[statuscode];

	lua_pushboolean(state, success);
	lua_setfield(state, -2, "Success");

	lua_pushstring(state, successmessage.c_str());
	lua_setfield(state, -2, "StatusMessage");

	lua_pushnumber(state, statuscode);
	lua_setfield(state, -2, "StatusCode");

	lua_pushstring(state, response_body.c_str());
	lua_setfield(state, -2, "Body");

	auto response_headers = http::last_headers;

	lua_createtable(state, 0, 0);

	for (const auto& str : response_headers)
	{
		auto colon = str.find(":");

		if (!colon)
		{
			lua_pushnil(state);
			lua_setfield(state, -2, str.c_str());
		}
		else
		{
			auto header = str.substr(0, colon);
			auto value = str.substr(colon + 2);

			lua_pushstring(state, value.c_str());
			lua_setfield(state, -2, header.c_str());
		}
	}

	lua_setfield(state, -2, "Headers");

	return 1;
}

std::int32_t roblox::environment::core::getobjects( lua_State* state )
{
	l( );

	//static const auto content_provider = [ state ]
	//{
	//	lua_getglobal( state, "game" );
	//	lua_getfield( state, -1, "ContentProvider" );
	//	const auto result = *reinterpret_cast< void** >( lua_touserdata( state, -1 ) );
	//	lua_pop( state, 1 );

	//	return result;
	//}( );

	//std::string asset_id = luaL_checkstring( state, lua_isuserdata( state, 1 ) ? 2 : 1 );
	//std::vector< std::uint64_t > instances;
	//managers::mgrs.offset_mgr.roblox.blocking_load_instances( content_provider, &asset_id, &instances );

	//lua_newtable( state );

	//for ( auto i = 0u; i < instances.size( ); ++i )
	//{
	//	managers::mgrs.offset_mgr.roblox.push_instance( state, &instances[ i ] );
	//	lua_rawseti( state, -2, i + 1 );
	//}

	luaL_checktype(state, 1, LUA_TUSERDATA);

	lua_pushvalue(state, 1);
	lua_getfield(state, -1, "GetService");
	lua_pushvalue(state, 1);
	lua_pushstring(state, "InsertService");
	lua_pcall(state, 2, 1, 0);
	lua_getfield(state, -1, "LoadLocalAsset");
	lua_pushvalue(state, -2);
	lua_pushvalue(state, 2);

	lua_call(state, 2, 1);

	lua_newtable(state);

	lua_pushnumber(state, 1);

	lua_pushvalue(state, -3);

	lua_settable(state, -3);

	return 1;
}

std::int32_t newcclosure_helper( lua_State* state )
{
	//l( );

	const auto arg_count = lua_gettop( state );

	const auto closure = managers::mgrs.roblox_mgr.get_closure( clvalue( state->ci->func ) );

	if ( !closure )
		return 0;

	setclvalue( state, state->top, *closure );
	api_incr_top( state );

	lua_insert( state, 1 );

	state->baseCcalls++;
	const auto status = lua_pcall( state, arg_count, LUA_MULTRET, 0 );
	state->baseCcalls--;

	expandstacklimit( state, state->top );

	if ( status == 0 && ( state->status == LUA_YIELD || state->status == LUA_BREAK ) )
		return -1;

	return lua_gettop( state );
}


std::int32_t non_yielding_newcclosure_helper( lua_State* state )
{
	//l( );

	const auto arg_count = lua_gettop( state );

	const auto closure = managers::mgrs.roblox_mgr.get_closure( clvalue( state->ci->func ) );

	if ( !closure )
		return 0;

	setclvalue( state, state->top, *closure );
	api_incr_top( state );

	lua_insert( state, 1 );

	const auto status = lua_pcall( state, arg_count, LUA_MULTRET, 0 );
	if ( status && status != LUA_YIELD )
	{
		if ( lua_isstring( state, -1 ) && !std::strcmp( lua_tostring( state, -1 ), "attempt to yield across metamethod/C-call boundary" ) )
		{
			return lua_yield( state, 0 );
		}
	}

	return lua_gettop( state );
}

std::int32_t roblox::environment::core::newcclosure( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ), 1, "Expected lclosure" );

	const auto yieldable = lua_gettop( state ) > 1 && lua_isboolean( state, 2 ) && lua_toboolean( state, 2 );

	lua_ref( state, 1 );

	if ( yieldable )
	{
		managers::mgrs.roblox_mgr.pushcclosurek(
			state,
			newcclosure_helper,
			0,
			[ ] ( lua_State* state, int status ) -> std::int32_t
			{
				if ( !status )
					return lua_gettop( state );

				return 0;
			}
		);
	}
	else
	{
		managers::mgrs.roblox_mgr.pushcclosure(
			state,
			non_yielding_newcclosure_helper,
			0
		);
	}

	managers::mgrs.roblox_mgr.add_newcclosure(
		clvalue( index2addr( state, -1 ) ), clvalue( index2addr( state, 1 ) ) );

	lua_ref( state, -1 );

	return 1;
}

enum closure_type_t
{
	cclosure,
	lclosure,
	newcclosure,
};

inline closure_type_t get_closure_type( Closure* c )
{
	if ( !c->isC )
		return closure_type_t::lclosure;
	else if ( c->c.f == newcclosure_helper || c->c.f == non_yielding_newcclosure_helper )
		return closure_type_t::newcclosure;
	else
		return closure_type_t::cclosure;
}

inline void clone_function( lua_State* state, int index )
{
	luaL_checktype( state, index, LUA_TFUNCTION );

	const auto orig = clvalue( index2addr( state, index ) );

	const auto clone_c = [ & ] ( ) -> Closure*
	{
		const auto clone = luaF_newCclosure( state, orig->nupvalues, orig->env );
		clone->c.f = orig->c.f;
		clone->c.cont = orig->c.cont;
		clone->c.debugname = orig->c.debugname;
		for ( int i = 0; i < orig->nupvalues; i++ )
			setobj( state, &clone->c.upvals[ i ], &orig->c.upvals[ i ] );
		setclvalue( state, state->top, clone );
		api_incr_top( state );

		return clone;
	};

	switch ( get_closure_type( orig ) )
	{
		case closure_type_t::cclosure:
		{
			clone_c( );
			return;
		}
		case closure_type_t::newcclosure:
		{
			const auto orig_lclosure = managers::mgrs.roblox_mgr.get_closure( orig );

			managers::mgrs.roblox_mgr.add_newcclosure( clone_c( ), *orig_lclosure );
			return;
		}
		case closure_type_t::lclosure:
		{
			lua_clonefunction( state, index );
			return;
		}
	}
}


std::int32_t roblox::environment::core::clonefunction( lua_State* state )
{
	l( );

	clone_function( state, 1 );

	return 1;
}

std::int32_t roblox::environment::core::hookfunction( lua_State* state )
{
	l( );

	auto& rbx_m = managers::mgrs.roblox_mgr;

	luaL_checktype( state, 1, LUA_TFUNCTION );
	luaL_checktype( state, 2, LUA_TFUNCTION );

	const auto destination_argument = clvalue( index2addr( state, 1 ) );
	const auto source_argument = clvalue( index2addr( state, 2 ) );

	const auto offset = offsetof( Closure, isC );

	lua_ref( state, 2 );
	lua_ref( state, 1 );

	const auto destination_nups = destination_argument->nupvalues;
	const auto source_nups = source_argument->nupvalues;

	if ( ( get_closure_type( destination_argument ) == closure_type_t::cclosure || get_closure_type( destination_argument ) == closure_type_t::newcclosure ) && get_closure_type( source_argument ) == closure_type_t::cclosure )
	{
		if ( destination_nups >= source_nups )
		{
			clone_function( state, 1 );

			destination_argument->c.f = source_argument->c.f;
			destination_argument->c.cont = source_argument->c.cont;
			destination_argument->env = source_argument->env;

			for ( int i = 0; i < source_nups; i++ )
				setobj2n( state, &destination_argument->c.upvals[ i ], &source_argument->c.upvals[ i ] );

			return 1;
		}

		luaL_error( state, "Destination has an upvalue count of %i source has an upvalue count of %i", destination_nups, source_nups );
	}
	else if ( get_closure_type( destination_argument ) == closure_type_t::newcclosure && get_closure_type( source_argument ) == closure_type_t::newcclosure )
	{
		clone_function( state, 1 );

		rbx_m.swap_newcclosure( destination_argument, *managers::mgrs.roblox_mgr.get_closure( source_argument ) );

		return 1;
	}
	else if ( get_closure_type( destination_argument ) == closure_type_t::cclosure && get_closure_type( source_argument ) == closure_type_t::newcclosure )
	{
		clone_function( state, 1 );

		rbx_m.swap_newcclosure( destination_argument, *managers::mgrs.roblox_mgr.get_closure( source_argument ) );

		destination_argument->c.f = source_argument->c.f;
		destination_argument->c.cont = source_argument->c.cont;
		destination_argument->env = source_argument->env;

		return 1;
	}
	if ( ( get_closure_type( destination_argument ) == closure_type_t::cclosure || get_closure_type( destination_argument ) == closure_type_t::newcclosure ) && get_closure_type( source_argument ) == closure_type_t::lclosure )
	{
		clone_function( state, 1 );

		destination_argument->c.f = newcclosure_helper;

		rbx_m.swap_newcclosure( destination_argument, source_argument );

		return 1;
	}
	if ( get_closure_type( destination_argument ) == closure_type_t::lclosure && get_closure_type( source_argument ) == closure_type_t::lclosure )
	{
		if ( destination_nups >= source_nups )
		{
			clone_function( state, 1 );

			destination_argument->l.p = source_argument->l.p;
			destination_argument->env = source_argument->env;

			for ( int i = 0; i < source_nups; i++ )
				setobj2n( state, &destination_argument->l.uprefs[ i ], &source_argument->l.uprefs[ i ] );

			return 1;
		}

		luaL_error( state, "Destination has an upvalue count of %i source has an upvalue count of %i", destination_nups, source_nups );
	}
	else
	{
		luaL_error( state, "Invalid hookfunction arguments: destination %i source %i", get_closure_type( destination_argument ), get_closure_type( source_argument ) );
	}
}

//loadstring(game:HttpGet('https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source',true))()
std::int32_t roblox::environment::core::httpget( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isuserdata( state, 1 ), 1, "Game expected as self" );

	const auto link = luaL_checkstring( state, 2 );

	if ( lua_gettop( state ) < 3 || ( lua_isboolean( state, 3 ) && !lua_toboolean( state, 3 ) ) )
	{
		lua_pushstring( state, get( link ).c_str( ) );
		return 1;
	}

	lua_pushthread( state );
	lua_ref( state, -1 );

	std::thread {
		[ state, link ]( )
		{
			const auto result = get( link );

			managers::mgrs.roblox_mgr.queue_yield_callback(
			[ state, result ] ( )
			{
				lua_pushstring( state, result.c_str( ) );

				if ( managers::mgrs.roblox_mgr.resume( state, 1 ) == LUA_ERRRUN )
				{
					const auto err = lua_tostring( state, -1 );

					utilities::io::log( utilities::io::log_options_t::error, err );
				}
			} );
		}
	}.detach( );

	return lua_yield( state, 1 );
}

void* roblox::environment::core::datamodel;
HWND roblox::environment::core::hwnd;

lua_CFunction roblox::environment::core::namecall_original;
std::int32_t roblox::environment::core::namecall_hook( lua_State* state )
{
	if ( *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( state->userdata ) + managers::mgrs.offset_mgr.offsets.extraspace_script ) )
		return namecall_original( state );

	const std::string method = getstr( state->namecall );

	if ( ( lua_isuserdata( state, 1 ) && lua_touserdata( state, 1 ) == datamodel ) )
	{
		if ( method == "GetObjects" )
			return getobjects( state );
		else if ( method == "HttpGet" )
			return httpget( state );
		else if ( method == "HttpGetAsync" )
			return httpget_async( state );
	}

	return namecall_original( state );
}

lua_CFunction roblox::environment::core::index_original;
std::int32_t roblox::environment::core::index_hook( lua_State* state )
{
	if ( *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( state->userdata ) + managers::mgrs.offset_mgr.offsets.extraspace_script ) )
		return index_original( state );

	if ( lua_isstring( state, 2 ) && ( lua_isuserdata( state, 1 ) && lua_touserdata( state, 1 ) == datamodel ) )
	{
		const std::string key = lua_tostring( state, 2 );
		if ( key == "GetObjects" )
		{
			lua_pushcfunction( state, &httpget, nullptr );
			return 1;
		}
		else if ( key == "HttpGet" )
		{
			lua_pushcfunction( state, &httpget, nullptr );
			return 1;
		}
		else if ( key == "HttpGetAsync" )
		{
			lua_pushcfunction( state, &httpget_async, nullptr );
			return 1;
		}
		else if ( key == "Players" ) /* Need universal way for all services, prolly just checking if res is nil */
		{
			lua_getfield( state, 1, "GetService" );
			lua_pushvalue( state, 1 );
			lua_pushvalue( state, 2 );
			lua_pcall( state, 2, 1, 0 );

			return 1;
		}
	}

	return index_original( state );
}

static bool is_roblox_active( )
{
	return GetForegroundWindow( ) == roblox::environment::core::hwnd;
}

std::int32_t roblox::environment::core::iswindowactive( lua_State* state )
{
	l( );
	lua_pushboolean( state, is_roblox_active( ) );
	return 1;
}

std::int32_t roblox::environment::core::keypress( lua_State* state )
{
	l( );

	const auto key_code = static_cast< std::uint32_t >( luaL_checknumber( state, 1 ) );
	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = key_code;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::keyrelease( lua_State* state )
{
	l( );

	const auto key_code = static_cast< std::uint32_t >( luaL_checknumber( state, 1 ) );
	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = key_code;
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mousepress1( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mouserelease1( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mouse1click( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		SendInput( 1, &input, sizeof( input ) );

		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mousepress2( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mouserelease2( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mouse2click( lua_State* state )
{
	l( );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

		SendInput( 1, &input, sizeof( input ) );

		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mousescroll( lua_State* state )
{
	l( );

	const auto pixels = static_cast< int >( luaL_checknumber( state, 1 ) );
	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.mouseData = pixels;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mousemoverel( lua_State* state )
{
	l( );

	const auto x = static_cast< int >( luaL_checknumber( state, 1 ) );
	const auto y = static_cast< int >( luaL_checknumber( state, 2 ) );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::mousemoveabs( lua_State* state )
{
	l( );

	const auto x = static_cast< int >( luaL_checknumber( state, 1 ) );
	const auto y = static_cast< int >( luaL_checknumber( state, 2 ) );

	if ( is_roblox_active( ) )
	{
		INPUT input { };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;

		SendInput( 1, &input, sizeof( input ) );
	}

	return 0;
}

std::int32_t roblox::environment::core::make_writeable( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );

	hvalue( index2addr( state, 1 ) )->readonly = false;

	return 0;
}

std::int32_t roblox::environment::core::make_readonly( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );

	hvalue( index2addr( state, 1 ) )->readonly = true;

	return 0;
}

std::int32_t roblox::environment::core::is_readonly( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );
	lua_pushboolean( state, hvalue( index2addr( state, 1 ) )->readonly );

	return 1;
}

std::int32_t roblox::environment::core::is_writeable( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TTABLE );
	lua_pushboolean( state, !hvalue( index2addr( state, 1 ) )->readonly );

	return 1;
}

std::int32_t roblox::environment::core::setclipboard( lua_State* state )
{
	l( );

	const auto str = luaL_checkstring( state, 1 );

	if ( !OpenClipboard( nullptr ) )
		luaL_error( state, "failed to open clipboard" );

	EmptyClipboard( );

	const auto global_alloc = GlobalAlloc( GMEM_MOVEABLE, std::strlen( str ) + 1 );
	if ( global_alloc == nullptr )
		luaL_error( state, "failed to allocate clipboard memory" );

	const auto global_lock = GlobalLock( global_alloc );
	if ( global_lock == nullptr )
		luaL_error( state, "failed to lock clipboard memory" );

	std::memcpy( global_lock, str, std::strlen( str ) + 1 );

	GlobalUnlock( global_alloc );
	SetClipboardData( CF_TEXT, global_alloc );

	CloseClipboard( );
	GlobalFree( global_alloc );

	return 0;
}

std::int32_t roblox::environment::core::get_calling_script( lua_State* state )
{
	l( );

	if ( const auto script_ptr = reinterpret_cast< std::uintptr_t >( state->userdata ) + 0x30; *reinterpret_cast< std::uintptr_t* >( script_ptr ) )
		managers::mgrs.offset_mgr.roblox.push_instance( state, script_ptr );
	else
		lua_pushnil( state );

	return 1;
}

inline std::string get_bytecode( const std::string& prot_string_data )
{
	constexpr const char bytecode_magic[ ] = "RSB1";

	std::string input = prot_string_data;

	std::uint8_t hash_bytes[ 4 ];
	memcpy( hash_bytes, &input[ 0 ], 4 );

	for ( auto i = 0u; i < 4; ++i )
	{
		hash_bytes[ i ] ^= bytecode_magic[ i ];
		hash_bytes[ i ] -= i * 41;
	}

	for ( size_t i = 0; i < input.size( ); ++i )
		input[ i ] ^= hash_bytes[ i % 4 ] + i * 41;

	XXH32( &input[ 0 ], input.size( ), 42 );

	std::uint32_t data_size;
	memcpy( &data_size, &input[ 4 ], 4 );

	std::vector<std::uint8_t> data( data_size );

	ZSTD_decompress( &data[ 0 ], data_size, &input[ 8 ], input.size( ) - 8 );

	return std::string( reinterpret_cast< char* >( &data[ 0 ] ), data_size );
}

inline std::string get_script_bytecode_data( lua_State* state )
{
	roblox::environment::core::check_instance_type( state, 1, "LocalScript or ModuleScript", [ ] ( const char* class_name ) {
		return !std::strstr( class_name, "LocalScript" ) || !std::strstr( class_name, "ModuleScript" );
		} );

	const auto script = *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, 1 ) );
	const auto vftable = *reinterpret_cast< std::uintptr_t** >( script );
	const auto prot_string_getter = reinterpret_cast< std::uintptr_t( __thiscall* )( std::uintptr_t ) >( vftable[ 50 ] );
	const auto bytecode_data = *reinterpret_cast< std::string* >( *reinterpret_cast< std::uintptr_t* >( prot_string_getter( script ) + 0x8 ) + 0x10 );

	return get_bytecode( bytecode_data );
}

std::int32_t roblox::environment::core::get_script_bytecode( lua_State* state )
{
	l( );

	const auto bytecode = get_script_bytecode_data( state );

	lua_pushlstring( state, bytecode.c_str( ), bytecode.size( ) );
	return 1;
}

std::int32_t roblox::environment::core::get_script_closure( lua_State* state )
{
	l( );

	const auto bytecode = get_script_bytecode_data( state );
	if ( !bytecode.front( ) )
		luaL_error( state, "script has invalid bytecode" );

	luau_load( state, "@", bytecode.c_str( ), bytecode.size( ), 0 );
	return 1;

}

std::int32_t roblox::environment::core::get_script_hash( lua_State* state )
{
	l( );

	const auto bytecode = get_script_bytecode_data( state );
	const auto hash = sha384( bytecode );

	lua_pushlstring( state, hash.c_str( ), hash.size( ) );
	return 1;
}

std::int32_t roblox::environment::core::fire_touch_interest( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "Instance", [ ] ( const char* ) {
		return true;
		} );

	check_instance_type( state, 2, "BasePart", [ ] ( const char* ) {
		return true;
		} );

	const auto& offs_mgr = managers::mgrs.offset_mgr;

	const auto primitive = *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, 1 ) ) + offs_mgr.offsets.primitive );

	offs_mgr.roblox.report_touch
	(
		*reinterpret_cast< std::uintptr_t* >( primitive + offs_mgr.offsets.world ),
		primitive,
		*reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, 2 ) ) + offs_mgr.offsets.primitive ),
		luaL_checkinteger( state, 3 )
	);

	return 0;
}

std::int32_t roblox::environment::core::fire_click_detector( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );

	const auto dist = lua_gettop( state ) > 1 ? luaL_checknumber( state, 2 ) : 0.f;

	return 0;
}

std::int32_t roblox::environment::core::get_thread_identity( lua_State* state )
{
	l( );

	lua_pushinteger( state, managers::mgrs.roblox_mgr.get_thread_identity( state ) );

	return 1;
}

std::int32_t roblox::environment::core::set_thread_identity( lua_State* state )
{
	l( );

	managers::mgrs.roblox_mgr.set_thread_identity( state, luaL_checkinteger( state, 1 ) );

	return 0;
}

// list of file extensions that can be used maliciously
const auto bad_extensions = std::to_array< std::string >( {
	".exe", ".com", ".bat", ".cmd", ".vbs", ".vbe", ".js", ".jse", ".wsf", ".wsh", ".ps1", ".ps1_sys", ".ps2", ".ps2_sys", ".ps3", ".ps3_sys", ".ps4", ".ps4_sys", ".ps5", ".ps5_sys", ".ps6", ".ps6_sys", ".ps7", ".ps7_sys", ".ps8", ".ps8_sys", ".psm1", ".psm1_sys", ".psd1", ".psd1_sys", ".psh1", ".psh1_sys", ".msc", ".msc_sys", ".msh", ".msh_sys", ".msh1", ".msh1_sys", ".msh2", ".msh2_sys", ".mshxml", ".mshxml_sys", ".vshost", ".vshost_sys", ".vbscript", ".vbscript_sys", ".wsf", ".wsf_sys", ".wsh", ".wsh_sys", ".wsh1", ".wsh1_sys", ".wsh2", ".wsh2_sys", ".wshxml", ".wshxml_sys", ".ps1", ".ps1_sys", ".ps2", ".ps2_sys", ".ps3", ".ps3_sys", ".ps4", ".ps4_sys", ".ps5", ".ps5_sys", ".ps6", ".ps6_sys", ".ps7", ".ps7_sys", ".ps8", ".ps8_sys", ".psm1", ".psm1_sys", ".psd1", ".psd1_sys", ".psh1", ".psh1_sys", ".msc", ".msc_sys", ".msh", ".msh_sys", ".msh1", ".msh1_sys", ".msh2", ".msh2_sys", ".mshxml", ".mshxml_sys", ".vshost", ".vshost_sys", ".vbscript", ".vbscript_sys", ".wsf", ".wsf_sys", ".wsh", ".wsh_sys",
	} );

bool is_escaping( const std::string_view str )
{
	return str.starts_with( ".." ) || std::any_of( bad_extensions.begin( ), bad_extensions.end( ), [ &str ] ( const auto& ext ) {
		return str.find( ext ) != std::string::npos;
		} );
}

std::int32_t roblox::environment::core::read_file( lua_State* state )
{
	l( );

	const auto file = luaL_checkstring( state, 1 );
	if ( is_escaping( file ) )
		luaL_error( state, "possibly malicious file escape" );

	std::ifstream stream( workspace_path / file );

	if ( !stream.is_open( ) )
		luaL_error( state, "failed to open file" );

	std::string output;
	std::copy( std::istreambuf_iterator< char >( stream ), std::istreambuf_iterator< char >( ), std::back_inserter( output ) );

	lua_pushlstring( state, output.data( ), output.size( ) );

	return 1;
}

std::int32_t roblox::environment::core::write_file( lua_State* state )
{
	l( );

	const auto file = luaL_checkstring( state, 1 );
	if ( is_escaping( file ) )
		luaL_error( state, "possibly malicious file escape" );

	const auto data = luaL_checkstring( state, 2 );

	std::ofstream stream( workspace_path / file, std::ios::beg | std::ios::binary );

	if ( !stream.is_open( ) )
		luaL_error( state, "failed to open file" );

	stream.write( data, std::strlen( data ) );

	return 0;
}


std::int32_t roblox::environment::core::append_file( lua_State* state )
{
	l( );

	const auto file = luaL_checkstring( state, 1 );
	if ( is_escaping( file ) )
		luaL_error( state, "possibly malicious file escape" );

	const auto data = luaL_checkstring( state, 2 );

	std::ofstream stream( workspace_path / file, std::ios::end | std::ios::binary | std::ios::app );

	if ( !stream.is_open( ) )
		luaL_error( state, "failed to open file" );

	stream.write( data, std::strlen( data ) );

	return 0;
}

std::int32_t roblox::environment::core::list_files( lua_State* state )
{
	l( );

	const auto dir = lua_isstring( state, 1 ) ? lua_tostring( state, 1 ) : "";
	if ( is_escaping( dir ) )
		luaL_error( state, "possibly malicious directory escape" );

	const auto path = workspace_path / dir;
	if ( !std::filesystem::exists( path ) )
		luaL_error( state, "directory does not exist" );

	lua_newtable( state );
	auto i = 1;

	for ( const auto& entry : std::filesystem::directory_iterator( path ) )
	{
		lua_pushstring( state, ( std::string { dir }.append( "/" ) + entry.path( ).filename( ).string( ) ).c_str( ) );
		lua_rawseti( state, -2, i++ );
	}

	return 1;
}

std::int32_t roblox::environment::core::is_file( lua_State* state )
{
	l( );

	const auto file = luaL_checkstring( state, 1 );
	lua_pushboolean( state, std::filesystem::exists( workspace_path / file ) && !std::filesystem::is_directory( workspace_path / file ) );

	return 1;
}

std::int32_t roblox::environment::core::is_folder( lua_State* state )
{
	l( );

	const auto dir = luaL_checkstring( state, 1 );
	lua_pushboolean( state, std::filesystem::exists( workspace_path / dir ) && std::filesystem::is_directory( workspace_path / dir ) );

	return 1;
}

std::int32_t roblox::environment::core::make_folder( lua_State* state )
{
	l( );

	const auto dir = luaL_checkstring( state, 1 );
	if ( is_escaping( dir ) )
		luaL_error( state, "possibly malicious directory escape" );

	const auto path = workspace_path / dir;
	if ( std::filesystem::exists( path ) )
		return 0;

	std::filesystem::create_directories( path );
	return 0;
}

std::int32_t roblox::environment::core::delete_folder( lua_State* state )
{
	l( );

	const auto dir = luaL_checkstring( state, 1 );
	if ( is_escaping( dir ) )
		luaL_error( state, "possibly malicious directory escape" );

	if ( !std::strlen( dir ) )
		luaL_error( state, "cannot delete workspace directory" );

	const auto path = workspace_path / dir;
	if ( !std::filesystem::exists( path ) )
		luaL_error( state, "directory does not exist" );

	std::filesystem::remove_all( path );

	return 0;
}

std::int32_t roblox::environment::core::delete_file( lua_State* state )
{
	l( );

	const auto file = luaL_checkstring( state, 1 );
	if ( is_escaping( file ) )
		luaL_error( state, "possibly malicious file escape" );

	const auto path = workspace_path / file;
	if ( !std::filesystem::exists( path ) )
		luaL_error( state, "file does not exist" );

	std::filesystem::remove( path );
	return 0;
}

std::int32_t roblox::environment::core::load_file( lua_State* state )
{
	l( );

	const auto dir = luaL_checkstring( state, 1 );
	const auto chunkname = lua_gettop( state ) > 1 ? luaL_checkstring( state, 2 ) : nullptr;

	if ( is_escaping( dir ) )
		luaL_error( state, "possibly malicious directory escape" );

	std::ifstream stream( workspace_path / dir );

	std::string output;
	std::copy( std::istreambuf_iterator< char >( stream ), std::istreambuf_iterator< char >( ), std::back_inserter( output ) );

	if ( !managers::mgrs.roblox_mgr.push_script( state, output, chunkname ) )
	{
		lua_pushnil( state );
		lua_pushvalue( state, -2 );

		return 2;
	}

	return 1;
}

std::int32_t roblox::environment::core::cloneref( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );

	const auto userdata = lua_touserdata( state, 1 );
	const auto instance = *reinterpret_cast< void** >( userdata );

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushlightuserdata( state, instance );
	lua_rawget( state, -2 );

	lua_pushlightuserdata( state, instance );
	lua_pushnil( state );
	lua_rawset( state, -4 );

	managers::mgrs.offset_mgr.roblox.push_instance( state, userdata );

	lua_pushlightuserdata( state, instance );
	lua_pushvalue( state, -3 );
	lua_rawset( state, -5 );

	return 1;
}

std::int32_t roblox::environment::core::get_instances( lua_State* state )
{
	l( );

	std::vector< void* > instances;

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		if ( lua_isuserdata( state, -2 ) )
		{
			instances.push_back( lua_touserdata( state, -2 ) );
		}

		lua_pop( state, 2 );
	}

	lua_newtable( state );

	for ( auto i = 0u; i < instances.size( ); ++i )
	{
		managers::mgrs.offset_mgr.roblox.push_instance( state, instances[ i ] );
		lua_rawseti( state, -2, i + 1 );
	}

	return 1;
}

std::int32_t roblox::environment::core::get_nil_instances( lua_State* state )
{
	l( );

	std::vector< void* > instances;

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		if ( lua_isuserdata( state, -2 ) )
		{
			instances.push_back( lua_touserdata( state, -2 ) );
		}

		lua_pop( state, 2 );
	}

	lua_newtable( state );

	int count = 1;
	for ( auto i = 0u; i < instances.size( ); ++i )
	{
		const auto instance = *static_cast< roblox::structures::instance_t** >( instances[ i ] );

		if ( !instance->get_parent( ) )
		{
			managers::mgrs.offset_mgr.roblox.push_instance( state, instances[ i ] );
			lua_rawseti( state, -2, count++ );
		}
	}

	return 1;
}

std::int32_t roblox::environment::core::get_scripts( lua_State* state )
{
	l( );

	std::vector< void* > instances;

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		if ( lua_isuserdata( state, -2 ) )
		{
			instances.push_back( lua_touserdata( state, -2 ) );
		}

		lua_pop( state, 2 );
	}

	lua_newtable( state );

	int count = 1;
	for ( auto i = 0u; i < instances.size( ); ++i )
	{
		const auto instance = *static_cast< roblox::structures::instance_t** >( instances[ i ] );

		if ( std::strstr( instance->get_class_name( ).c_str( ), "Script" ) )
		{
			managers::mgrs.offset_mgr.roblox.push_instance( state, instances[ i ] );
			lua_rawseti( state, -2, count++ );
		}

	}

	return 1;
}

std::int32_t roblox::environment::core::get_running_scripts( lua_State* state )
{
	l( );

	std::vector< void* > instances;

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		if ( lua_isuserdata( state, -2 ) )
		{
			instances.push_back( lua_touserdata( state, -2 ) );
		}

		lua_pop( state, 2 );
	}

	lua_newtable( state );

	int count = 1;
	for ( auto i = 0u; i < instances.size( ); ++i )
	{
		const auto instance = *static_cast< roblox::structures::instance_t** >( instances[ i ] );

		if ( std::strstr( instance->get_class_name( ).c_str( ), "Script" ) )
		{
			if ( get_script_state( reinterpret_cast< std::uintptr_t >( instance ) ) );
			{
				managers::mgrs.offset_mgr.roblox.push_instance( state, instances[ i ] );
				lua_rawseti( state, -2, count++ );
			}
		}

	}

	return 1;
}

std::int32_t roblox::environment::core::get_loaded_modules( lua_State* state )
{
	l( );

	std::vector< void* > instances;

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushnil( state );

	while ( lua_next( state, -2 ) )
	{
		lua_pushvalue( state, -2 );

		if ( lua_isuserdata( state, -2 ) )
		{
			instances.push_back( lua_touserdata( state, -2 ) );
		}

		lua_pop( state, 2 );
	}

	lua_newtable( state );

	int count = 1;
	for ( auto i = 0u; i < instances.size( ); ++i )
	{
		const auto instance = *static_cast< roblox::structures::instance_t** >( instances[ i ] );

		if ( !std::strcmp( instance->get_class_name( ).c_str( ), "ModuleScript" ) )
		{
			if ( get_script_state( reinterpret_cast< std::uintptr_t >( instance ) ) );
			{
				managers::mgrs.offset_mgr.roblox.push_instance( state, instances[ i ] );
				lua_rawseti( state, -2, count++ );
			}
		}

	}

	return 1;
}

//print(isscriptable(game.Players.LocalPlayer, "SimulationRadius"))
std::int32_t roblox::environment::core::is_scriptable( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "Instance", [ ] ( const char* ) {
		return true;
		} );

	const auto instance = *reinterpret_cast< structures::instance_t** >( lua_touserdata( state, 1 ) );

	luaL_checktype( state, 2, LUA_TSTRING );

	int atom;
	lua_tostringatom( state, 2, &atom );

	const auto prop_descriptor = managers::mgrs.offset_mgr.roblox.property_table[ atom ];

	if ( !prop_descriptor )
		luaL_error( state, "Invalid property name" );

	const auto prop = managers::mgrs.offset_mgr.roblox.get_property( instance->get_class_descriptor( )->address( ), prop_descriptor );

	if ( !prop )
		luaL_error( state, "Invalid property name" );

	lua_pushboolean( state, *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( prop ) + 0x20 ) >> 5 & 1 );

	return 1;
}
//print(setscriptable(game.Players.LocalPlayer, "SimulationRadius", true))
std::int32_t roblox::environment::core::set_scriptable( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "Instance", [ ] ( const char* ) {
		return true;
		} );

	const auto instance = *reinterpret_cast< structures::instance_t** >( lua_touserdata( state, 1 ) );

	luaL_checktype( state, 2, LUA_TSTRING );

	const auto enabled = luaL_checkboolean( state, 3 );

	int atom;
	lua_tostringatom( state, 2, &atom );
	const auto prop_descriptor = managers::mgrs.offset_mgr.roblox.property_table[ atom ];

	if ( !prop_descriptor )
		luaL_error( state, "Invalid property name" );

	const auto prop = managers::mgrs.offset_mgr.roblox.get_property( instance->get_class_descriptor( )->address( ), prop_descriptor );

	if ( !prop )
		luaL_error( state, "Invalid property name" );

	switch ( *reinterpret_cast< std::uintptr_t* >( prop + 4 ) )
	{
	case 0:
	{
		const auto old = *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( prop ) + 0x20 ) >> 5 & 1;

		if ( enabled )
			*reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( prop ) + 0x20 ) |= 1 << 5;
		else
			*reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( prop ) + 0x20 ) ^= 1 << 5;

		lua_pushboolean( state, old );

		return 1;
	}
	}

	return 0;
}

std::int32_t roblox::environment::core::set_hidden_property( lua_State* state )
{
	l( );

	if ( lua_gettop( state ) != 3 )
		luaL_error( state, "Invalid args" );

	check_instance_type( state, 1, "Instance", [ ] ( const char* ) {
		return true;
		} );

	const auto prop = luaL_checkstring( state, 2 );

	lua_getglobal( state, "setscriptable" );
	lua_pushvalue( state, 1 );
	lua_pushvalue( state, 2 );
	lua_pushboolean( state, true );

	lua_call( state, 3, 1 );

	const auto old = lua_toboolean( state, -1 );

	lua_pushvalue( state, 3 );
	lua_setfield( state, 1, prop );

	lua_getglobal( state, "setscriptable" );
	lua_pushvalue( state, 1 );
	lua_pushvalue( state, 2 );
	lua_pushboolean( state, old );

	lua_call( state, 3, 1 );

	lua_pushboolean( state, old == 0 );

	return 1;
}

std::int32_t roblox::environment::core::get_hidden_property( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "Instance", [ ] ( const char* ) {
		return true;
		} );

	const auto prop = luaL_checkstring( state, 2 );

	lua_getglobal( state, "setscriptable" );
	lua_pushvalue( state, 1 );
	lua_pushvalue( state, 2 );
	lua_pushboolean( state, true );

	lua_call( state, 3, 1 );

	const auto old = lua_toboolean( state, -1 );

	lua_getfield( state, 1, prop );

	lua_getglobal( state, "setscriptable" );
	lua_pushvalue( state, 1 );
	lua_pushvalue( state, 2 );
	lua_pushboolean( state, old );

	lua_call( state, 3, 1 );

	lua_pushvalue( state, -2 );

	lua_pushboolean( state, old == 0 );

	return 2;
}

std::int32_t roblox::environment::core::require( lua_State* state )
{
	l( );

	check_instance_type( state, 1, "ModuleScript", [ ] ( const char* ) {
		return true;
		} );

	const auto script = *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, 1 ) );

	*reinterpret_cast< std::uint8_t* >( script + managers::mgrs.offset_mgr.offsets.require_flag ) = 1;

	const auto res = core::old_require( state );

	*reinterpret_cast< std::uint8_t* >( script + managers::mgrs.offset_mgr.offsets.require_flag ) = 0;

	return res;
}

void roblox::environment::core::check_instance_type( lua_State* state, int idx, const char* err, std::function<bool( const char* )> check )
{
	if ( !lua_isuserdata( state, idx ) || std::strcmp( luaT_objtypename( state, index2addr( state, idx ) ), "Instance" ) )
		luaL_typeerror( state, idx, err );

	const auto instance = *reinterpret_cast< structures::instance_t** >( lua_touserdata( state, 1 ) );
	const auto class_descriptor = instance->get_class_descriptor( );
	const auto name = class_descriptor->class_name;

	if ( !check( name.c_str( ) ) )
		luaL_typeerror( state, idx, err );
}
