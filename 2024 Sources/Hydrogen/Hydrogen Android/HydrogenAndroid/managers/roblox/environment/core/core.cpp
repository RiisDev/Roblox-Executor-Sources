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

#include <dependencies/curl/http.hpp>
#include <dependencies/lz4/lz4.h>
#include <dependencies/sha384/sha384.h>
#include <dependencies/zstd/zstd.h>
#include <dependencies/zstd/common/xxhash.h>

#include <structures/instance/instance.hpp>
#include <dependencies/Luau/lstring.h>

#include <algorithm>

lua_State* get_script_state(const std::uintptr_t script)
{
	auto node = *reinterpret_cast<std::uintptr_t*>(script + offsets::thread_node);
	auto weakthreadref = node ? *reinterpret_cast<std::uintptr_t*>(node + offsets::node_first) : 0;

	for (; weakthreadref; weakthreadref = *reinterpret_cast<std::uintptr_t*>(weakthreadref + offsets::node_next))
	{
		auto ref_state = *reinterpret_cast<std::uintptr_t*>(weakthreadref + offsets::state_off1);

		if (ref_state)
			ref_state = *reinterpret_cast<std::uintptr_t*>(ref_state + offsets::state_off2);
		else
			continue;

		if (ref_state)
		{
			const auto ref_script = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(reinterpret_cast<lua_State*>(ref_state)->userdata) + offsets::extraspace_script);

			if (ref_script)
			{
				if (ref_script == script)
					return reinterpret_cast<lua_State*>(ref_state);
			}
		}
	}

	return nullptr;
}

std::int32_t roblox::environment::core::loadstring( lua_State* state )
{
	const char* src = luaL_checklstring(state, 1, nullptr );

	const char* name = luaL_optlstring( state, 2, "hydrogen", nullptr );

	lua_pushnil( state );

	return managers::roblox_mgr.push_script( state, src, name ) ? 1 : 2;
}

std::int32_t roblox::environment::core::getgenv(lua_State* state)
{
	lua_pushvalue(managers::roblox_mgr.thread, LUA_GLOBALSINDEX);
	
	lua_xmove(managers::roblox_mgr.thread, state, 1);

	return 1;
}

std::int32_t roblox::environment::core::getrenv(lua_State* state)
{
	lua_pushvalue(managers::roblox_mgr.state, LUA_GLOBALSINDEX);
	
	lua_xmove(managers::roblox_mgr.state, state, 1);

	return 1;
}

std::int32_t roblox::environment::core::getrawmetatable( lua_State* state )
{
	// check if arg 1 type is TABLE or USERDATA
	if (!managers::roblox_mgr.check_types<2>(state, 1, { LUA_TTABLE, LUA_TUSERDATA }))
		luaL_argerrorL( state, 1, "table or userdata expected" );

	lua_getmetatable( state, 1 );
	
	return 1;
}

std::int32_t roblox::environment::core::setrawmetatable( lua_State* state )
{
	// check if arg 1 type is TABLE or USERDATA
	if (!managers::roblox_mgr.check_types<2>(state, 1, { LUA_TTABLE, LUA_TUSERDATA }))
		luaL_argerrorL( state, 1, "table or userdata expected" );

	luaL_checktype( state, 2, LUA_TTABLE );

	lua_setmetatable( state, -2 );

	return 1;
}

std::int32_t roblox::environment::core::getreg(lua_State* state)
{
	lua_pushvalue(state, LUA_REGISTRYINDEX);

	return 1;
}

std::int32_t roblox::environment::core::setreadonly(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);

	const auto val = lua_isboolean(state, 2) ? lua_toboolean(state, 2) : lua_isnumber(state, 2) == 1;

	lua_setreadonly(state, 1, val);

	return 0;
}

std::int32_t roblox::environment::core::make_writeable(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);

	lua_setreadonly(state, 1, false);

	return 0;
}

std::int32_t roblox::environment::core::make_readonly(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);

	lua_setreadonly(state, 1, true);

	return 0;
}

std::int32_t roblox::environment::core::isreadonly(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);

	lua_pushboolean(state, lua_getreadonly(state, 1));

	return 1;
}

std::int32_t roblox::environment::core::is_writeable(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTABLE);

	lua_pushboolean(state, lua_getreadonly(state, 1) == 0);

	return 1;
}

std::int32_t roblox::environment::core::setclipboard(lua_State* state)
{
	managers::jni_mgr.set_clipboard(luaL_checkstring(state, 1));

	return 0;
}

std::int32_t roblox::environment::core::islclosure( lua_State* state )
{
	luaL_checktype( state, 1, LUA_TFUNCTION );

	lua_pushboolean( state, lua_iscfunction( state, 1 ) == 0 );

	return 1;
}

std::int32_t roblox::environment::core::iscclosure( lua_State* state )
{
	luaL_checktype( state, 1, LUA_TFUNCTION );

	lua_pushboolean( state, lua_iscfunction( state, 1 ) == 1 );

	return 1;
}

std::int32_t roblox::environment::core::identifyexecutor(lua_State* state)
{
	lua_pushstring(state, "Hydrogen");
	lua_pushstring(state, HYDROGEN_VERSION);
	return 2;
}

std::int32_t roblox::environment::core::get_namecall_method(lua_State* state)
{
	if (const auto method = lua_namecallatom(state, nullptr))
	{
		lua_pushstring(state, method);

		return 1;
	}

	return 0;
}

std::int32_t roblox::environment::core::set_namecall_method(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TSTRING);

	state->namecall = tsvalue(luaA_toobject(state, 1));

	return 0;
}

std::int32_t roblox::environment::core::get_thread_identity(lua_State* state)
{
	lua_pushinteger(state, managers::roblox_mgr.get_thread_identity(state));

	return 1;
}

std::int32_t roblox::environment::core::set_thread_identity(lua_State* state)
{
	managers::roblox_mgr.set_thread_identity(state, luaL_checkinteger(state, 1));

	return 0;
}

std::int32_t roblox::environment::core::checkcaller(lua_State* state)
{
	const auto script_ptr = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script);
	lua_pushboolean(state, !script_ptr);

	return 1;
}

std::int32_t roblox::environment::core::getobjects(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);
	luaL_checktype(state, 2, LUA_TSTRING);

	lua_getglobal(state, "game");
	lua_getfield(state, -1, "GetService");

	lua_pushvalue(state, -2);
	lua_pushstring(state, "InsertService");
	lua_pcall(state, 2, 1, 0);

	lua_getfield(state, -1, "LoadLocalAsset");

	lua_pushvalue(state, -2);
	lua_pushvalue(state, 2);
	lua_pcall(state, 2, 1, 0);

	if (lua_type(state, -1) == LUA_TSTRING)
	{
		luaL_error(state, lua_tostring(state, -1));
	}

	lua_createtable(state, 0, 0);

	lua_pushvalue(state, -2);

	lua_rawseti(state, -2, 1);

	return 1;
}

void* roblox::environment::core::datamodel;

lua_CFunction roblox::environment::core::namecall_original;
std::int32_t roblox::environment::core::namecall_hook(lua_State* state)
{
	if (*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script))
		return namecall_original(state);

	const std::string method = lua_namecallatom(state, nullptr);

	if ((lua_isuserdata(state, 1) && lua_touserdata(state, 1) == datamodel))
	{
		if (method == "GetObjects")
			return getobjects(state);
		if (method == "HttpGet")
			return httpget_async(state);

		//std::string str = method;

		//std::transform(str.begin(),
		//	str.end(),
		//	str.begin(),
		//	tolower);

		//if (str.find("purchase") != std::string::npos || str.find("prompt") != std::string::npos)
		//	return 0;
	}

	return namecall_original(state);
}

lua_CFunction roblox::environment::core::index_original;
std::int32_t roblox::environment::core::index_hook(lua_State* state)
{
	if (*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script))
		return index_original(state);

	if (ttype(managers::function_mgr.index2addr(state, 2)) == LUA_TSTRING) //isstring go brr for some reason
	{
		const std::string method = lua_tostring(state, 2);
		if (method == "GetObjects")
		{
			managers::function_mgr.lua_pushcclosurek(state, &getobjects, nullptr, 0, nullptr);
			return 1;
		}
		if (method == "HttpGet")
		{
			managers::function_mgr.lua_pushcclosurek(state, &httpget_async, nullptr, 0, nullptr);
			return 1;
		}

		std::string str = method;

		std::transform(str.begin(),
			str.end(),
			str.begin(),
			tolower);

		if (str.find("purchase") != std::string::npos || str.find("prompt") != std::string::npos)
			return 0;
	}

	return index_original(state);
}

std::int32_t newcclosure_helper(lua_State* state)
{
	const auto arg_count = lua_gettop(state);

	const auto closure = managers::roblox_mgr.get_closure(clvalue(state->ci->func));

	if (!closure)
		return 0;

	setclvalue(state, state->top, *closure);
	state->top++;

	lua_insert(state, 1);

	state->baseCcalls++;
	const auto status = lua_pcall(state, arg_count, LUA_MULTRET, 0); //must be swapped so not invoke at our vm
	state->baseCcalls--;

	expandstacklimit(state, state->top);

	if (status == 0 && (state->status == LUA_YIELD || state->status == LUA_BREAK))
		return -1;

	return lua_gettop(state);
}


std::int32_t non_yielding_newcclosure_helper(lua_State* state)
{
	const auto arg_count = lua_gettop(state);

	const auto closure = managers::roblox_mgr.get_closure(clvalue(state->ci->func));

	if (!closure)
		return 0;

	setclvalue(state, state->top, *closure);
	state->top++;

	lua_insert(state, 1);

	const auto status = lua_pcall(state, arg_count, LUA_MULTRET, 0);
	if (status && status != LUA_YIELD)
	{
		if (lua_isstring(state, -1) && !std::strcmp(lua_tostring(state, -1), "attempt to yield across metamethod/C-call boundary"))
		{
			return lua_yield(state, 0);
		}
	}

	return lua_gettop(state);
}

std::int32_t roblox::environment::core::read_file(lua_State* state)
{
	const auto file = luaL_checkstring(state, 1);

	std::ifstream stream(workspace_path / file);

	if (!stream.is_open())
		luaL_error(state, "failed to open file");

	std::string output;
	std::copy(std::istreambuf_iterator< char >(stream), std::istreambuf_iterator< char >(), std::back_inserter(output));

	lua_pushlstring(state, output.data(), output.size());

	return 1;
}

std::int32_t roblox::environment::core::write_file(lua_State* state)
{
	const auto file = luaL_checkstring(state, 1);
	const auto data = luaL_checkstring(state, 2);

	std::ofstream stream(workspace_path / file, std::ios::beg | std::ios::binary);

	if (!stream.is_open())
		luaL_error(state, "failed to open file");

	stream.write(data, std::strlen(data));

	return 0;
}

std::int32_t roblox::environment::core::append_file(lua_State* state)
{
	const auto file = luaL_checkstring(state, 1);

	const auto data = luaL_checkstring(state, 2);

	std::ofstream stream(workspace_path / file, std::ios::end | std::ios::binary | std::ios::app);

	if (!stream.is_open())
		luaL_error(state, "failed to open file");

	stream.write(data, std::strlen(data));

	return 0;
}

std::int32_t roblox::environment::core::list_files(lua_State* state)
{
	const auto dir = lua_isstring(state, 1) ? lua_tostring(state, 1) : "";

	const auto path = workspace_path / dir;
	if (!fs::exists(path))
		luaL_error(state, "directory does not exist");

	lua_newtable(state);
	auto i = 1;

	for (const auto& entry : fs::directory_iterator(path))
	{
		lua_pushstring(state, (std::string{ dir }.append("/") + entry.path().filename().string()).c_str());
		lua_rawseti(state, -2, i++);
	}

	return 1;
}

std::int32_t roblox::environment::core::is_file(lua_State* state)
{
	const auto file = luaL_checkstring(state, 1);
	lua_pushboolean(state, fs::exists(workspace_path / file) && !fs::is_directory(workspace_path / file));

	return 1;
}

std::int32_t roblox::environment::core::is_folder(lua_State* state)
{
	const auto dir = luaL_checkstring(state, 1);
	lua_pushboolean(state, fs::exists(workspace_path / dir) && fs::is_directory(workspace_path / dir));

	return 1;
}

std::int32_t roblox::environment::core::newcclosure(lua_State* state)
{
	luaL_argcheck(state, lua_isLfunction(state, 1), 1, "Expected lclosure");

	const auto yieldable = lua_gettop(state) > 1 && lua_isboolean(state, 2) && lua_toboolean(state, 2);

	lua_ref(state, 1);

	if (yieldable)
	{
		managers::function_mgr.lua_pushcclosurek(
			state,
			newcclosure_helper,
			nullptr,
			0,
			[](lua_State* state, int status) -> std::int32_t
			{
				if (!status)
					return lua_gettop(state);

				return 0;
			}
		);
	}
	else
	{
		managers::function_mgr.lua_pushcclosurek(
			state,
			non_yielding_newcclosure_helper,
			nullptr,
			0,
			nullptr
		);
	}

	managers::roblox_mgr.add_newcclosure(reinterpret_cast<Closure*>(const_cast<void*>(lua_topointer(state, -1))), reinterpret_cast<Closure*>(const_cast<void*>(lua_topointer(state, 1))));

	lua_ref(state, -1);

	return 1;
}

std::int32_t roblox::environment::core::getgc(lua_State* L )
{
	const auto tables = lua_gettop( L ) ? luaL_optboolean( L, -1, 0 ) : false;

	lua_newtable( L );

	struct ctx_t
	{
		lua_State* L;
		const int tables;
		std::size_t count;
	} ctx{ L, tables, 0 };

	luaM_visitgco( L, &ctx, [ ]( void* context, lua_Page* page, GCObject* gco )
	{
		const auto ctx = static_cast< ctx_t* >( context );

		if ( !( ( gco->gch.marked ^ WHITEBITS ) & otherwhite( ctx->L->global ) ) )
			return false;

		const auto tt = gco->gch.tt;
		if ( tt == LUA_TFUNCTION || tt == LUA_TUSERDATA
			|| ( ctx->tables && tt == LUA_TTABLE ) )
		{
			ctx->L->top->value.gc = gco;
			ctx->L->top->tt = gco->gch.tt;
			ctx->L->top++;

			lua_rawseti( ctx->L, -2, ++ctx->count );
		}

		return false;
	} );

	return 1;
}

std::int32_t roblox::environment::core::request( lua_State* state )
{
	luaL_checktype( state, 1, LUA_TTABLE );
	auto arg1 = managers::function_mgr.index2addr( state, 1 );

	lua_getfield( state, -1, "Url" );

	if ( lua_isnil( state, -1 ) )
		luaL_error( state, "Url was not passed to request." );

	std::string uri = lua_tostring( state, -1 );

	std::size_t start{};
	for ( const auto& c : uri )
	{
		if ( c != ' ' )
			break;

		start++;
	}
	std::string protocol = uri.substr( start );
	std::transform( protocol.begin( ), protocol.end( ), protocol.begin( ),
		[ ]( unsigned char c ) { return std::tolower( c ); } );

	if ( ( protocol.size( ) >= 5 && protocol.substr( 0, 5 ) == "file:" ) || ( protocol.size( ) >= 7 && protocol.substr( 0, 7 ) == "telnet:" ) )
		luaL_error( state, "Fail" );

	lua_pop( state, 1 );

	std::string method = "GET";
	std::string body = "";
	std::vector<std::string> headers;

	headers.push_back( std::string{ "User-Identifier: " } + managers::jni_mgr.get_hwid( ) );
	headers.push_back( std::string{ "Fingerprint: " } + managers::jni_mgr.get_hwid( ) );
	headers.push_back( ( "User-Agent: Hydrogen" ) );

	lua_getfield( state, -1, "Method" );

	if ( !lua_isnil( state, -1 ) )
	{
		method = lua_tostring( state, -1 );

		std::transform( method.begin( ), method.end( ), method.begin( ),
			[ ]( unsigned char c ) { return std::toupper( c ); } );
	}
	lua_pop( state, 1 );

	lua_getfield( state, -1, "Headers" );

	if ( !lua_isnil( state, -1 ) )
	{
		lua_pushnil( state );

		while ( lua_next( state, -2 ) )
		{
			if ( lua_isstring( state, -2 ) )
			{
				auto header = std::string( lua_tostring( state, -2 ) );
				std::string value = "";

				auto type_val = lua_type( state, -1 );

				if ( type_val == LUA_TBOOLEAN )
				{
					if ( lua_toboolean( state, -1 ) )
						value = "true";
					else
						value = "false";
				}
				else if ( type_val == LUA_TNUMBER )
				{
					value = std::to_string( lua_tonumber( state, -1 ) );
				}
				else if ( type_val == LUA_TSTRING )
				{
					value = lua_tostring( state, -1 );
				}
				//else if (type_val == LUA_TUSERDATA)
				//{
				//	auto ud = *reinterpret_cast<rbx::instance**>(lua_touserdata(state, -1));

				//	value = ud->name;
				//}

				headers.push_back( header + ": " + value );

			}

			lua_pop( state, 1 );
		}
	}
	lua_pop( state, 1 );

	lua_getfield( state, -1, "Body" );

	if ( !lua_isnil( state, -1 ) )
	{
		std::size_t len;
		const auto data = lua_tolstring( state, -1, &len );

		auto str = managers::function_mgr.index2addr( state, -1 );

		body = std::string( data, len );
	}
	lua_pop( state, 1 );

	auto result = http::send_request( uri, method, headers, body );

	lua_createtable( state, 0, 0 );

	auto success = false;
	std::string response_body = "";

	if ( result.has_value( ) )
	{
		success = true;
		response_body = *result;
	}

	long statuscode = http::http_code;
	std::string successmessage = http::status_message[ statuscode ];

	lua_pushboolean( state, success );
	lua_setfield( state, -2, "Success" );

	lua_pushstring( state, successmessage.c_str( ) );
	lua_setfield( state, -2, "StatusMessage" );

	lua_pushnumber( state, statuscode );
	lua_setfield( state, -2, "StatusCode" );

	lua_pushstring( state, response_body.c_str( ) );
	lua_setfield( state, -2, "Body" );

	auto response_headers = http::last_headers;

	lua_createtable( state, 0, 0 );

	for ( const auto& str : response_headers )
	{
		auto colon = str.find( ":" );

		if ( !colon )
		{
			lua_pushnil( state );
			lua_setfield( state, -2, str.c_str( ) );
		}
		else
		{
			auto header = str.substr( 0, colon );
			auto value = str.substr( colon + 2 );

			lua_pushstring( state, value.c_str( ) );
			lua_setfield( state, -2, header.c_str( ) );
		}
	}

	lua_setfield( state, -2, "Headers" );

	return 1;
}

//print(isscriptable(game.Players.LocalPlayer, "SimulationRadius"))
//print(isscriptable(game.Players.LocalPlayer.Character.Humanoid, "WalkSpeed"))
std::int32_t roblox::environment::core::is_scriptable(lua_State* state)
{
	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto instance = *reinterpret_cast<structures::instance_t**>(lua_touserdata(state, 1));

	luaL_checktype(state, 2, LUA_TSTRING);

	int atom;
	lua_tostringatom(state, 2, &atom);

	const auto prop_descriptor = managers::memory_mgr.rebase<std::uintptr_t*>(offsets::property_table)[atom];

	if (!prop_descriptor)
		return 0;

	const auto prop = managers::roblox_mgr.get_property(instance->get_class_descriptor()->address(), prop_descriptor);

	if (!prop)
		return 0;

	lua_pushboolean(state, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) >> 5 & 1);

	return 1;
}
//print(setscriptable(game.Players.LocalPlayer, "SimulationRadius", true))
std::int32_t roblox::environment::core::set_scriptable(lua_State* state)
{
	const auto sz = 479;

	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto instance = *reinterpret_cast<structures::instance_t**>(lua_touserdata(state, 1));

	luaL_checktype(state, 2, LUA_TSTRING);

	const auto enabled = luaL_checkboolean(state, 3);

	int atom;
	lua_tostringatom(state, 2, &atom);
	const auto prop_descriptor = managers::memory_mgr.rebase<std::uintptr_t*>(offsets::property_table)[atom];

	if (!prop_descriptor)
		luaL_error(state, "Invalid property name");

	//ik what ur gonna say but stfu im too lazy

	const auto clone = new std::uint8_t[sz];
	std::memcpy(clone, instance->get_class_descriptor(), sz);

	auto end = *(std::uintptr_t*)(clone + 0x1CC);
	auto start = *(std::uintptr_t*)(clone + 0x1C8);

	const auto prop_array_clone = new std::uint8_t[end - start];
	std::memcpy(prop_array_clone, reinterpret_cast<void*>(start), end - start);

	*(std::uint8_t**)(clone + 0x1C8) = prop_array_clone;
	*(std::uint8_t**)(clone + 0x1CC) = prop_array_clone + (end - start);

	*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uintptr_t>(instance) + 0xC) = clone;

	const auto prop = managers::roblox_mgr.get_property(instance->get_class_descriptor()->address(), prop_descriptor);

	if (!prop)
		luaL_error(state, "Invalid property name");

	switch (*reinterpret_cast<std::uintptr_t*>(prop + 4))
	{
		case 0:
		{
			const auto c = new std::uint8_t[0x50];
			std::memcpy(c, *reinterpret_cast<std::uint8_t**>(prop), 0x50);

			*reinterpret_cast<std::uint8_t**>(prop) = c;

			const auto old = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) >> 5 & 1;

			if (enabled)
				*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) |= 1 << 5;
			else
				*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) ^= 1 << 5;

			lua_pushboolean(state, old);

			return 1;
		}
	}

	return 0;
}

std::int32_t roblox::environment::core::set_hidden_property(lua_State* state)
{
	if (lua_gettop(state) != 3)
		luaL_error(state, "Invalid args");

	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto prop = luaL_checkstring(state, 2);

	lua_getglobal(state, "setscriptable");
	lua_pushvalue(state, 1);
	lua_pushvalue(state, 2);
	lua_pushboolean(state, true);

	lua_call(state, 3, 1);

	const auto old = lua_toboolean(state, -1);

	lua_pushvalue(state, 3);
	lua_setfield(state, 1, prop);

	lua_getglobal(state, "setscriptable");
	lua_pushvalue(state, 1);
	lua_pushvalue(state, 2);
	lua_pushboolean(state, old);

	lua_call(state, 3, 1);

	lua_pushboolean(state, old == 0);

	return 1;
}

std::int32_t roblox::environment::core::get_hidden_property(lua_State* state)
{
	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto prop = luaL_checkstring(state, 2);

	lua_getglobal(state, "setscriptable");
	lua_pushvalue(state, 1);
	lua_pushvalue(state, 2);
	lua_pushboolean(state, true);

	lua_call(state, 3, 1);

	const auto old = lua_toboolean(state, -1);

	lua_getfield(state, 1, prop);

	lua_getglobal(state, "setscriptable");
	lua_pushvalue(state, 1);
	lua_pushvalue(state, 2);
	lua_pushboolean(state, old);

	lua_call(state, 3, 1);

	lua_pushvalue(state, -2);

	lua_pushboolean(state, old == 0);

	return 2;
}

std::int32_t roblox::environment::core::get_properties(lua_State* state)
{
	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto instance = *reinterpret_cast<structures::instance_t**>(lua_touserdata(state, 1));

	const auto class_descriptor = (std::uintptr_t)instance->get_class_descriptor();

	lua_createtable(state, 0, 0);

	const auto start = *reinterpret_cast<std::uintptr_t*>(class_descriptor + 0x24);
	const auto end = *reinterpret_cast<std::uintptr_t*>(class_descriptor + 0x28);

	auto iter = 0u;

	for (auto i = start; i < end; i += 4)
	{
		const auto str = *reinterpret_cast<const char**>(i);

		if (!str)
			continue;

		const auto str_len = strlen(str);

		lua_pushstring(state, str);

		int atom;
		lua_tostringatom(state, -1, &atom);
		const auto prop_descriptor = managers::memory_mgr.rebase<std::uintptr_t*>(offsets::property_table)[atom];

		lua_pop(state, 1);

		if (auto prop = managers::roblox_mgr.get_property(class_descriptor, prop_descriptor))
		{
			if (*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) >> 5 & 1)
			{
				lua_pushnumber(state, ++iter);
				lua_pushstring(state, *reinterpret_cast<const char**>(i));
				lua_settable(state, -3);
			}
		}

	}

	return 1;
}

std::int32_t roblox::environment::core::get_hidden_properties(lua_State* state)
{
	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	const auto instance = *reinterpret_cast<structures::instance_t**>(lua_touserdata(state, 1));

	const auto class_descriptor = (std::uintptr_t)instance->get_class_descriptor();

	lua_createtable(state, 0, 0);

	const auto start = *reinterpret_cast<std::uintptr_t*>(class_descriptor + 0x24);
	const auto end = *reinterpret_cast<std::uintptr_t*>(class_descriptor + 0x28);

	auto iter = 0u;

	for (auto i = start; i < end; i += 4)
	{
		const auto str = *reinterpret_cast<const char**>(i);

		if (!str)
			continue;

		const auto str_len = strlen(str);

		lua_pushstring(state, str);

		int atom;
		lua_tostringatom(state, -1, &atom);
		const auto prop_descriptor = managers::memory_mgr.rebase<std::uintptr_t*>(offsets::property_table)[atom];

		lua_pop(state, 1);

		if (auto prop = managers::roblox_mgr.get_property(class_descriptor, prop_descriptor))
		{
			if (!(*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(prop) + 0x20) >> 5 & 1))
			{
				lua_pushnumber(state, ++iter);
				lua_pushstring(state, *reinterpret_cast<const char**>(i));
				lua_settable(state, -3);
			}
		}

	}

	return 1;
}

std::int32_t(*roblox::environment::core::old_require)(lua_State* state);
std::int32_t roblox::environment::core::require(lua_State* state)
{
	const auto script = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1));

	*reinterpret_cast<std::uint8_t*>(script + offsets::require_flag) = 1;

	const auto res = core::old_require(state);

	*reinterpret_cast<std::uint8_t*>(script + offsets::require_flag) = 0;

	return res;
}

std::int32_t roblox::environment::core::test(lua_State* state)
{
	return 0;
}

std::int32_t roblox::environment::core::queue_on_teleport(lua_State* state)
{
	const auto script = luaL_checkstring(state, 1);

	managers::roblox_mgr.push_teleport_script(script);

	return 0;
}

std::int32_t roblox::environment::core::get_instances(lua_State* state)
{
	//std::vector< void* > instances;

	//static const auto func = managers::memory_mgr.rebase(offsets::push_instance);

	//lua_pushlightuserdata(state, reinterpret_cast<void*>(func));
	//lua_rawget(state, LUA_REGISTRYINDEX);

	//lua_pushnil(state);

	//while (lua_next(state, -2))
	//{
	//	lua_pushvalue(state, -2);

	//	if (lua_isuserdata(state, -2))
	//	{
	//		instances.push_back(lua_touserdata(state, -2));
	//	}

	//	lua_pop(state, 2);
	//}

	//lua_newtable(state);

	//for (auto i = 0u; i < instances.size(); ++i)
	//{
	//	managers::function_mgr.push_instance(state, instances[i]);
	//	lua_rawseti(state, -2, i + 1);
	//}

	//return 1;


	static const auto func = managers::memory_mgr.rebase(offsets::push_instance);

	lua_pushlightuserdata(state, reinterpret_cast<void*>(func));
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_newtable( state );

	lua_pushnil( state );

	auto c = 0u;
	while ( lua_next( state, -3 ) )
	{
		if ( lua_isuserdata( state, -1 ) )
		{
			managers::function_mgr.push_instance( state, 
				lua_touserdata( state, -1 ) );

			lua_rawseti( state, -4, ++c );
		}

		lua_pop( state, 1 );
	}

	return 1;
}

std::int32_t roblox::environment::core::get_nil_instances(lua_State* state)
{
	std::vector< void* > instances;

	static const auto func = managers::memory_mgr.rebase(offsets::push_instance);

	lua_pushlightuserdata(state, reinterpret_cast<void*>(func));
	lua_rawget(state, LUA_REGISTRYINDEX);

	lua_pushnil(state);

	while (lua_next(state, -2))
	{
		lua_pushvalue(state, -2);

		if (lua_isuserdata(state, -2))
		{
			instances.push_back(lua_touserdata(state, -2));
		}

		lua_pop(state, 2);
	}

	lua_newtable(state);

	int count = 1;
	for (auto i = 0u; i < instances.size(); ++i)
	{
		const auto instance = *static_cast<roblox::structures::instance_t**>(instances[i]);

		if (!instance->get_parent())
		{
			managers::function_mgr.push_instance(state, instances[i]);
			lua_rawseti(state, -2, count++);
		}
	}

	return 1;
}

std::int32_t roblox::environment::core::get_scripts(lua_State* state)
{
	std::vector< void* > instances;

	static const auto func = managers::memory_mgr.rebase(offsets::push_instance);

	lua_pushlightuserdata(state, reinterpret_cast<void*>(func));
	lua_rawget(state, LUA_REGISTRYINDEX);

	lua_pushnil(state);

	while (lua_next(state, -2))
	{
		lua_pushvalue(state, -2);

		if (lua_isuserdata(state, -2))
		{
			instances.push_back(lua_touserdata(state, -2));
		}

		lua_pop(state, 2);
	}

	lua_newtable(state);

	int count = 1;
	for (auto i = 0u; i < instances.size(); ++i)
	{
		const auto instance = *static_cast<roblox::structures::instance_t**>(instances[i]);

		if (std::strstr(instance->get_class_name().c_str(), "Script"))
		{
			managers::function_mgr.push_instance(state, instances[i]);
			lua_rawseti(state, -2, count++);
		}

	}

	return 1;
}

std::int32_t roblox::environment::core::get_running_scripts(lua_State* state)
{
	std::unordered_map< void*, void* > map;

	lua_pushvalue( state, LUA_REGISTRYINDEX );

	lua_newtable( state );

	lua_pushnil( state );

	auto c = 0u;
	while ( lua_next( state, -3 ) )
	{
		if ( lua_isthread( state, -1 ) )
		{
			const auto thread = lua_tothread( state, -1 );

			if ( thread )
			{
				if ( const auto script_ptr = reinterpret_cast< std::uintptr_t >( thread->userdata ) + offsets::extraspace_script; *reinterpret_cast< std::uintptr_t* >( script_ptr ) )
				{
					if ( map.find( reinterpret_cast< void* >( script_ptr ) ) == map.end( ) )
					{
						map.insert( { reinterpret_cast< void* >( script_ptr ), nullptr } );

						managers::function_mgr.push_instance( state, reinterpret_cast< void* >( script_ptr ) );

						lua_rawseti( state, -4, ++c );
					}
				}
			}
		}

		lua_pop( state, 1 );
	}

	return 1;
}

std::int32_t roblox::environment::core::get_loaded_modules(lua_State* state)
{
	lua_getglobal( state, "game" );
	lua_getfield( state, -1, "GetService" );

	lua_pushvalue( state, -2 );
	lua_pushstring( state, "ScriptContext" );
	lua_pcall( state, 2, 1, 0 );

	const auto scriptcontext = *reinterpret_cast< std::uintptr_t* >( lua_touserdata( state, -1 ) );

	const auto& loaded_modules
		= *reinterpret_cast< std::set< std::weak_ptr< void* > >* >
			( scriptcontext + offsets::loadedmodules );

	lua_newtable( state );

	auto i = 1;
	for ( const auto& loaded_module : loaded_modules )
	{
		if ( !loaded_module.expired( ) )
		{
			static const auto func = reinterpret_cast< void( * )( lua_State*, const std::shared_ptr< void* >& ) >( managers::memory_mgr.rebase( offsets::push_instance ) );

			func( state, loaded_module.lock( ) );

			lua_getfield( state, -1, "IsDescendantOf" );
			lua_pushvalue( state, -2 );
			
			lua_getglobal( state, "game" );
			lua_getfield( state, -1, "GetService" );

			lua_pushvalue( state, -2 );
			lua_pushstring( state, "CoreGui" );
			lua_call( state, 2, 1 );

			lua_remove( state, -2 ); //remove game

			lua_call( state, 2, 1 );

			const auto is_core_gui = lua_toboolean( state, -1 );

			lua_pop( state, 1 );

			lua_getfield( state, -1, "IsDescendantOf" );
			lua_pushvalue( state, -2 );

			lua_getglobal( state, "game" );
			lua_getfield( state, -1, "GetService" );

			lua_pushvalue( state, -2 );
			lua_pushstring( state, "CorePackages" );
			lua_call( state, 2, 1 );

			lua_remove( state, -2 ); //remove game

			lua_call( state, 2, 1 );

			const auto is_core_packages = lua_toboolean( state, -1 );

			lua_pop( state, 1 );

			if ( is_core_gui || is_core_packages )
			{
				lua_pop( state, 1 );
				continue;
			}

			lua_rawseti( state, -2, i++ );
		}
	}

	return 1;
}

std::int32_t roblox::environment::core::is_rbx_active(lua_State* state)
{
	lua_pushboolean(state, 1);

	return 1;
}

std::int32_t roblox::environment::core::cloneref(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);

	const auto userdata = lua_touserdata(state, 1);
	const auto instance = *reinterpret_cast<void**>(userdata);

	static const auto func = managers::memory_mgr.rebase(offsets::push_instance);

	lua_pushlightuserdata(state, reinterpret_cast<void*>(func));
	lua_rawget(state, LUA_REGISTRYINDEX);

	lua_pushlightuserdata(state, instance);
	lua_rawget(state, -2);

	lua_pushlightuserdata(state, instance);
	lua_pushnil(state);
	lua_rawset(state, -4);

	managers::function_mgr.push_instance(state, userdata);

	lua_pushlightuserdata(state, instance);
	lua_pushvalue(state, -3);
	lua_rawset(state, -5);

	return 1;
}

std::int32_t roblox::environment::core::compareinstances(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);
	luaL_checktype(state, 2, LUA_TUSERDATA);

	const auto userdata1 = lua_touserdata(state, 1);
	const auto userdata2 = lua_touserdata(state, 2);

	lua_pushboolean(state, *reinterpret_cast<void**>(userdata1) == *reinterpret_cast<void**>(userdata2));

	return 1;
}

std::int32_t roblox::environment::core::lz4compress(lua_State* state)
{
	std::size_t sz;
	const auto data = luaL_checklstring(state, 1, &sz);

	std::vector<char> buff;
	buff.reserve(LZ4_compressBound(sz));

	const auto size = LZ4_compress(data, buff.data(), sz);

	lua_pushlstring(state, buff.data(), size);
	return 1;
}

std::int32_t roblox::environment::core::lz4decompress(lua_State* state)
{
	std::size_t sz;
	const auto data = luaL_checklstring(state, 1, &sz);
	const auto size = luaL_checknumber(state, 2);

	std::vector<char> buff(size);
	LZ4_decompress_safe(data, buff.data(), sz, size);

	lua_pushlstring(state, buff.data(), size);
	return 1;
}

std::int32_t roblox::environment::core::setfpscap(lua_State* state)
{
	const auto input = luaL_checknumber(state, 1);

	static const auto task_scheduler = *reinterpret_cast<std::uintptr_t**>(managers::memory_mgr.rebase(offsets::task_scheduler));

	static const auto fps_cap_offset = []() -> std::uint16_t {
		for (auto i = 0u; i < 0x500; i += 0x4)
		{
			if (1.0 / *reinterpret_cast<double*>(task_scheduler + i) == 60.0f)
				return i;
		}
	}();

	*reinterpret_cast<double*>(task_scheduler + fps_cap_offset) = input > 0.f ? 1.0f / input : 1.0 / 10000.0;

	return 0;
}

//table.foreach(getsenv(game.Players.LocalPlayer.PlayerScripts.ChatScript), print)
std::int32_t roblox::environment::core::getsenv(lua_State* state)
{
	check_instance_type(state, 1, "LocalScript or ModuleScript", [](const char* class_name) {
		return !std::strcmp(class_name, "LocalScript") || !std::strcmp(class_name, "ModuleScript");
	});

	const auto script = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1));

	if (const auto script_state = get_script_state(script))
	{
		lua_pushvalue(script_state, LUA_GLOBALSINDEX);

		lua_xmove(script_state, state, 1);
	}

	return 1;
}

std::int32_t roblox::environment::core::is_executor_closure(lua_State* state) //might need using own vm to catch shit made by us
{
	luaL_checktype(state, 1, LUA_TFUNCTION);

	const auto closure = clvalue(managers::function_mgr.index2addr(state, 1));

	if (!closure->isC)
	{
		const auto iter = std::find(managers::roblox_mgr.protos.begin(), managers::roblox_mgr.protos.end(), closure->l.p); 

		lua_pushboolean(state, iter != managers::roblox_mgr.protos.end());
		return 1;
	}

	lua_pushboolean(state, managers::roblox_mgr.function_map.find(closure) != managers::roblox_mgr.function_map.end());
	return 1;
}

std::int32_t roblox::environment::core::get_script_from_thread(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TTHREAD);

	const auto userdata = reinterpret_cast<std::uintptr_t>(lua_tothread(state, 1)->userdata);

	if (!userdata)
		return 0;

	const auto ref_script = *reinterpret_cast<std::uintptr_t**>(userdata + offsets::extraspace_script);

	if (ref_script)
	{
		managers::function_mgr.push_instance(state, reinterpret_cast<void*>(userdata + offsets::extraspace_script));

		return 1;
	}

	return 0;
}

enum closure_type_t
{
	cclosure,
	lclosure,
	newcclosure,
};

inline closure_type_t get_closure_type(Closure* c)
{
	if (!c->isC)
		return closure_type_t::lclosure;
	else if (c->c.f == newcclosure_helper || c->c.f == non_yielding_newcclosure_helper)
		return closure_type_t::newcclosure;
	else
		return closure_type_t::cclosure;
}

inline void clone_function(lua_State* state, int index) //if its yieldable cclosure it NEEDS TO COPY CONT
{
	luaL_checktype(state, index, LUA_TFUNCTION);

	const auto orig = clvalue(managers::function_mgr.index2addr(state, index));

	const auto clone_c = [&]() -> Closure*
	{
		const auto clone = luaF_newCclosure(state, orig->nupvalues, orig->env);
		clone->c.f = orig->c.f;
		clone->c.cont = orig->c.cont;
		clone->c.debugname = orig->c.debugname;
		for (int i = 0; i < orig->nupvalues; i++)
			setobj(state, &clone->c.upvals[i], &orig->c.upvals[i]);
		setclvalue(state, state->top, clone);
		api_incr_top(state);

		managers::roblox_mgr.function_map.insert({ clone, nullptr });

		return clone;
	};

	switch (get_closure_type(orig))
	{
		case closure_type_t::cclosure:
		{
			clone_c();
			return;
		}
		case closure_type_t::newcclosure:
		{
			const auto orig_lclosure = managers::roblox_mgr.get_closure(orig);

			const auto cl = clone_c();
			cl->c.cont = [](lua_State* state, int status) -> std::int32_t
			{
				if (!status)
					return lua_gettop(state);

				return 0;
			};

			managers::roblox_mgr.add_newcclosure(cl, *orig_lclosure);
			return;
		}
		case closure_type_t::lclosure:
		{
			lua_clonefunction(state, index);

			const auto closure = clvalue(managers::function_mgr.index2addr(state, -1));
			managers::roblox_mgr.function_map.insert({ closure, nullptr });

			return;
		}
	}
}


std::int32_t roblox::environment::core::clonefunction(lua_State* state)
{
	clone_function(state, 1);

	return 1;
}

//TODO CLONE THE ENV

std::int32_t roblox::environment::core::hookfunction(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TFUNCTION);
	luaL_checktype(state, 2, LUA_TFUNCTION);

	const auto destination_argument = clvalue(managers::function_mgr.index2addr(state, 1));
	const auto source_argument = clvalue(managers::function_mgr.index2addr(state, 2));

	lua_ref(state, 2);
	lua_ref(state, 1);

	const auto destination_nups = destination_argument->nupvalues;
	const auto source_nups = source_argument->nupvalues;

	if ((get_closure_type(destination_argument) == closure_type_t::cclosure || get_closure_type(destination_argument) == closure_type_t::newcclosure) && get_closure_type(source_argument) == closure_type_t::cclosure)
	{
		if (destination_nups >= source_nups)
		{
			clone_function(state, 1);

			lua_ref(state, -1);

			destination_argument->c.f = source_argument->c.f;
			destination_argument->c.cont = source_argument->c.cont;

			for (int i = 0; i < source_nups; i++)
				setobj2n(state, &destination_argument->c.upvals[i], &source_argument->c.upvals[i]);

			return 1;
		}

		luaL_error(state, "Destination has an upvalue count of %i source has an upvalue count of %i", destination_nups, source_nups);
	}
	else if (get_closure_type(destination_argument) == closure_type_t::newcclosure && get_closure_type(source_argument) == closure_type_t::newcclosure)
	{
		clone_function(state, 1);

		lua_ref(state, -1);

		//handle the offchance that a yieldable one is hooked with nonyieldable and viceversa
		destination_argument->c.f = source_argument->c.f;
		destination_argument->c.cont = source_argument->c.cont;

		managers::roblox_mgr.swap_newcclosure(destination_argument, *managers::roblox_mgr.get_closure(source_argument));

		return 1;
	}
	else if (get_closure_type(destination_argument) == closure_type_t::cclosure && get_closure_type(source_argument) == closure_type_t::newcclosure)
	{
		clone_function(state, 1);

		lua_ref(state, -1);

		managers::roblox_mgr.swap_newcclosure(destination_argument, *managers::roblox_mgr.get_closure(source_argument));

		destination_argument->c.f = source_argument->c.f;
		destination_argument->c.cont = source_argument->c.cont;

		return 1;
	}
	if ((get_closure_type(destination_argument) == closure_type_t::cclosure || get_closure_type(destination_argument) == closure_type_t::newcclosure) && get_closure_type(source_argument) == closure_type_t::lclosure)
	{
		clone_function(state, 1);

		lua_ref(state, -1);

		destination_argument->c.f = non_yielding_newcclosure_helper;
		destination_argument->c.cont = nullptr;

		managers::roblox_mgr.swap_newcclosure(destination_argument, source_argument);

		return 1;
	}
	if (get_closure_type(destination_argument) == closure_type_t::lclosure && get_closure_type(source_argument) == closure_type_t::lclosure)
	{
		if (destination_nups >= source_nups)
		{
			clone_function(state, 1);

			lua_ref(state, -1);

			destination_argument->l.p = source_argument->l.p;

			for (int i = 0; i < source_nups; i++)
				setobj2n(state, &destination_argument->l.uprefs[i], &source_argument->l.uprefs[i]);

			return 1;
		}

		luaL_error(state, "Destination has an upvalue count of %i source has an upvalue count of %i", destination_nups, source_nups);
	}
	else
	{
		luaL_error(state, "Invalid hookfunction arguments: destination %i source %i", get_closure_type(destination_argument), get_closure_type(source_argument));
	}
}

std::int32_t roblox::environment::core::hookmetamethod(lua_State* state)
{
	luaL_argcheck(state, lua_isuserdata(state, 1) || lua_istable(state, 1), 1, "Userdata or Table expected at argument 1");

	const auto method = luaL_checkstring(state, 2);
	luaL_checktype(state, 3, LUA_TFUNCTION);

	lua_getglobal(state, "hookfunction");

	if (!luaL_getmetafield(state, 1, method))
		luaL_error(state, "Cannot find metatable for object");

	lua_pushvalue(state, 3);
	lua_call(state, 2, 1);

	return 1;
}

inline std::string get_bytecode(const std::string& prot_string_data)
{
	constexpr const char bytecode_magic[] = "RSB1";

	std::string input = prot_string_data;

	std::uint8_t hash_bytes[4];
	memcpy(hash_bytes, &input[0], 4);

	for (auto i = 0u; i < 4; ++i)
	{
		hash_bytes[i] ^= bytecode_magic[i];
		hash_bytes[i] -= i * 41;
	}

	for (size_t i = 0; i < input.size(); ++i)
		input[i] ^= hash_bytes[i % 4] + i * 41;

	XXH32(&input[0], input.size(), 42);

	std::uint32_t data_size;
	memcpy(&data_size, &input[4], 4);

	std::vector<std::uint8_t> data(data_size);

	ZSTD_decompress(&data[0], data_size, &input[8], input.size() - 8);

	return std::string(reinterpret_cast<char*>(&data[0]), data_size);
}

// /e print(getscriptbytecode(game.Players.LocalPlayer.PlayerScripts.ChatScript))
// /e print(getscriptbytecode(game:GetService("Players").LocalPlayer.Character.Animate))
inline std::string get_script_bytecode_data(lua_State* state)
{
	roblox::environment::core::check_instance_type(state, 1, "LocalScript or ModuleScript", [](const char* class_name) {
		return !std::strstr(class_name, "LocalScript") || !std::strstr(class_name, "ModuleScript");
	});

	const auto script = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1));
	const auto vftable = *reinterpret_cast<std::uintptr_t**>(script);
	const auto prot_string_getter = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t)>(vftable[51]);

	const auto bytecode_data = *reinterpret_cast<std::string*>(*reinterpret_cast<std::uintptr_t*>(prot_string_getter(script) + 0x8) + 0x10);

	return get_bytecode(bytecode_data);
}

std::int32_t roblox::environment::core::get_calling_script(lua_State* state)
{
	if (const auto script_ptr = reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script; *reinterpret_cast<std::uintptr_t*>(script_ptr))
		managers::function_mgr.push_instance(state, reinterpret_cast<void*>(script_ptr));
	else
		lua_pushnil(state);

	return 1;
}

std::int32_t roblox::environment::core::get_script_bytecode(lua_State* state)
{
	const auto bytecode = get_script_bytecode_data(state);

	lua_pushlstring(state, bytecode.c_str(), bytecode.size());
	return 1;
}

// /e print(getscriptclosure(game.Players.LocalPlayer.PlayerScripts.ChatScript))

std::int32_t roblox::environment::core::get_script_closure(lua_State* state)
{
	const auto bytecode = get_script_bytecode_data(state);
	if (!bytecode.front())
	{
		lua_pushnil(state);

		return 0;
	}

	luau_load(state, "@", bytecode.c_str(), bytecode.size(), 0);
	return 1;
}

std::int32_t roblox::environment::core::get_script_hash(lua_State* state)
{
	const auto bytecode = get_script_bytecode_data(state);
	const auto hash = sha384(bytecode);

	lua_pushlstring(state, hash.c_str(), hash.size());
	return 1;
}

std::int32_t roblox::environment::core::fire_touch_interest(lua_State* state)
{
	check_instance_type(state, 1, "Instance", [](const char*) {
		return true;
	});

	check_instance_type(state, 2, "BasePart", [](const char*) {
		return true;
	});

	const auto primitive = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1)) + offsets::primitive);

	const auto type = luaL_checkinteger(state, 3);

	if (type == 1)
		managers::function_mgr.touch_end(primitive, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 2)) + offsets::primitive));
	else if (!type)
		managers::function_mgr.touch_start(primitive, *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 2)) + offsets::primitive));

	return 0;
}

std::int32_t roblox::environment::core::fire_click_detector(lua_State* state)
{
	check_instance_type(state, 1, "ClickDetector", [](const char*) {
		return true;
	});

	const auto click_detector = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1));

	const auto distance = luaL_optnumber(state, 2, 0.f);

	lua_getglobal(state, "game");
	lua_getfield(state, -1, "GetService");
	lua_getglobal(state, "game");
	lua_pushstring(state, "Players");
	lua_pcall(state, 2, 1, 0);
	lua_getfield(state, -1, "LocalPlayer");
	
	if (const auto ud = lua_touserdata(state, -1))
		managers::function_mgr.fire_click_detector(click_detector, distance, *reinterpret_cast<std::uintptr_t*>(ud));

	return 0;
}

std::int32_t roblox::environment::core::fire_proximity_prompt(lua_State* state)
{
	check_instance_type(state, 1, "ProximityPrompt", [](const char*) {
		return true;
	});

	managers::function_mgr.fire_proximity_prompt(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1)));

	return 0;
}

std::int32_t roblox::environment::core::set_proximity_prompt_duration(lua_State* state)
{
	check_instance_type(state, 1, "ProximityPrompt", [](const char*) {
		return true;
	});

	*reinterpret_cast<float*>(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1)) + offsets::proximity_prompt_duration) = luaL_checknumber(state, 2);

	return 0;
}

std::int32_t roblox::environment::core::get_proximity_prompt_duration(lua_State* state)
{
	check_instance_type(state, 1, "ProximityPrompt", [](const char*) {
		return true;
	});

	lua_pushnumber(state, *reinterpret_cast<float*>(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, 1)) + offsets::proximity_prompt_duration));

	return 1;
}

std::int32_t roblox::environment::core::get_fflag(lua_State* state)
{
	std::string fflag = luaL_checkstring(state, 1);

	std::string res;

	if (managers::function_mgr.get_fflag(&fflag, &res, true))
	{
		managers::utility_mgr.log("Val %s\n", res.c_str());

		lua_pushstring(state, res.c_str());

		return 1;
	}

	managers::utility_mgr.log("not found\n");

	return 0;
}

std::int32_t roblox::environment::core::set_fflag(lua_State* state)
{
	std::string fflag = luaL_checkstring(state, 1);
	std::string value = luaL_checkstring(state, 2);

	std::string res;
	if (managers::function_mgr.get_fflag(&fflag, &res, true))
	{
		constexpr auto FASTVARTYPE_ANY = 0x3F;

		managers::function_mgr.set_fflag(&fflag, &value, FASTVARTYPE_ANY, false);

		return 0;
	}

	luaL_errorL(state, "Invalid fflag name");

	return 0;
}

void roblox::environment::core::check_instance_type(lua_State* state, int idx, const char* err, std::function<bool(const char*)> check)
{
	if (!lua_isuserdata(state, idx) || std::strcmp(luaT_objtypename(state, managers::function_mgr.index2addr(state, idx)), "Instance"))
		luaL_typeerror(state, idx, err);

	const auto instance = *reinterpret_cast<structures::instance_t**>(lua_touserdata(state, 1));
	const auto name = instance->get_class_name();

	if (!check(name.c_str()))
		luaL_typeerror(state, idx, err);
}

std::int32_t roblox::environment::core::httpget_async(lua_State* state)
{
	lua_getfield(state, LUA_GLOBALSINDEX, "game");
	lua_getfield(state, -1, "HttpGetAsync");
	lua_pushvalue(state, 1);
	lua_pushvalue(state, 2);
	lua_pushnumber(state, 0);

	if (lua_pcall(state, 3, 1, 0))
	{
		if (!std::strcmp("attempt to yield across metamethod/C-call boundary", lua_tostring(state, -1)))
			return lua_yield(state, 0);
	}
}

std::int32_t roblox::environment::core::getconnections(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);

	if (std::strcmp(luaT_objtypename(state, managers::function_mgr.index2addr(state, 1)), "RBXScriptSignal"))
		luaL_typeerror(state, 1, "RBXScriptSignal");

	lua_getfield(state, 1, "Connect");
	lua_pushvalue(state, 1);

	managers::function_mgr.lua_pushcclosurek(state, [](lua_State*) {
		return 0;
	}, nullptr, 0, nullptr);

	lua_call(state, 2, 1);

	const auto signal = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(state, -1));

	lua_getfield(state, -1, "Disconnect");
	lua_insert(state, -2);
	lua_call(state, 1, 0);

	lua_newtable(state);

	if (!signal)
		return 1;

	auto obj = *reinterpret_cast<std::uintptr_t*>(signal + 0x10);
	auto iter = 1u;

	static const auto enable_func = [](lua_State* state) -> std::int32_t {
		if (const auto connection = reinterpret_cast<std::uintptr_t>(lua_topointer(state, lua_upvalueindex(1))); managers::roblox_mgr.registered_connections[connection])
			*reinterpret_cast<std::uintptr_t*>(connection + 0x14) = managers::roblox_mgr.registered_connections[connection];

		return 0;
	};

	static const auto disable_func = [](lua_State* state) -> std::int32_t {
		const auto connection = reinterpret_cast<std::uintptr_t>(lua_topointer(state, lua_upvalueindex(1)));

		if (!managers::roblox_mgr.registered_connections[connection])
			managers::roblox_mgr.registered_connections[connection] = *reinterpret_cast<std::uintptr_t*>(connection + 0x14);

		*reinterpret_cast<std::uintptr_t*>(connection + 0x14) = 0;

		return 0;
	};

	static const auto fire_func = [](lua_State* state) -> std::int32_t {
		const auto argn = lua_gettop(state);

		lua_pushvalue(state, lua_upvalueindex(1));
		if (!lua_isfunction(state, -1))
			return 0;

		lua_insert(state, 2);
		lua_call(state, argn - 1, 0);

		return 0;
	};

	static const auto defer_func = [](lua_State* state) -> std::int32_t {
		const auto argn = lua_gettop(state);

		lua_getglobal(state, "task");
		lua_getfield(state, -1, "defer");
		lua_insert(state, 2);

		lua_pushvalue(state, lua_upvalueindex(1));
		if (!lua_isfunction(state, -1))
			return 0;

		lua_insert(state, 3);
		lua_pop(state, 1);
		lua_call(state, argn - 1, 0);

		return 0;
	};

	static const auto index_mt = [](lua_State* state) -> std::int32_t {
		const std::string key = luaL_checkstring(state, 2);

		if (key == "State" || key == "Enabled")
		{
			lua_getfield(state, 1, "__ENABLED");
			return 1;
		}

		else if (key == "Disconnect")
		{
			lua_getfield(state, 1, "__OBJECT");

			//FIX
			managers::function_mgr.lua_pushcclosurek(state, disable_func, nullptr, 1, nullptr);

			return 1;
		}

		else if (key == "Enable")
		{
			lua_getfield(state, 1, "__OBJECT");

			managers::function_mgr.lua_pushcclosurek(state, enable_func, nullptr, 1, nullptr);

			return 1;
		}

		else if (key == "LuaConnection")
		{
			lua_getfield(state, 1, "Function");

			if (lua_isfunction(state, -1))
				lua_pushboolean(state, !lua_iscfunction(state, -1));
			else
				lua_pushboolean(state, false);

			return 1;
		}

		else if (key == "Fire")
		{
			lua_getfield(state, 1, "Function");

			lua_pushcclosure(state, fire_func, nullptr, 1);

			return 1;
		}

		else if (key == "Defer")
		{
			lua_getfield(state, 1, "Function");

			lua_pushcclosure(state, defer_func, nullptr, 1);

			return 1;
		}


		else if (key == "Disable" || key == "Disconnect")
		{
			lua_getfield(state, 1, "__OBJECT");

			lua_pushcclosure(state, disable_func, nullptr, 1);

			return 1;
		}

		else if (key == "Function")
		{
			lua_getfield(state, 1, "__OBJECT");

			const auto wrapper = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(lua_tolightuserdata(state, -1)) + 0x1C);

			if (const auto udata = *reinterpret_cast<std::uintptr_t*>(wrapper + 0x3C))
			{
				const auto key = *reinterpret_cast<std::uintptr_t*>(udata + 0xC);

				lua_rawgeti(managers::roblox_mgr.state, LUA_REGISTRYINDEX, key);
				lua_xmove(managers::roblox_mgr.state, state, 1);

				if (!lua_isfunction(state, -1))
					lua_pushnil(state);

				return 1;
			}
		}

		return 0;
	};

	while (obj)
	{
		lua_pushinteger(state, iter++);

		lua_newtable(state);
		{
			lua_pushstring(state, "__OBJECT");
			lua_pushlightuserdata(state, reinterpret_cast<void*>(obj));
			lua_rawset(state, -3);

			const auto enabled = *reinterpret_cast<std::uint8_t*>(obj + 0x14);

			lua_pushstring(state, "__ENABLED");
			lua_pushboolean(state, enabled);
			lua_rawset(state, -3);

			lua_pushstring(state, "__OBJECT_ENABLED");
			lua_pushboolean(state, enabled);
			lua_rawset(state, -3);
		}

		lua_newtable(state);
		{
			// __index
			lua_pushcfunction(state, index_mt, nullptr);
		}

		lua_setfield(state, -2, "__index");
		lua_setmetatable(state, -2);
		lua_rawset(state, -3);

		obj = *reinterpret_cast<std::uintptr_t*>(obj + 0x10);
	}

	return 1;
}

std::int32_t roblox::environment::core::firesignal(lua_State* state)
{
	const auto argn = lua_gettop(state);

	getconnections(state);

	lua_pushnil(state);

	while (lua_next(state, -2))
	{
		lua_pushvalue(state, -2);

		lua_getfield(state, -2, "Fire");
		for (auto i = 1; i <= argn; i++)
			lua_pushvalue(state, i);

		lua_call(state, argn, 0);

		lua_pop(state, 2);
	}

	return 0;
}

std::int32_t roblox::environment::core::make_folder(lua_State* state)
{
	const auto dir = luaL_checkstring(state, 1);

	const auto path = workspace_path / dir;
	if (fs::exists(path))
		return 0;

	fs::create_directories(path);
	return 0;
}

std::int32_t roblox::environment::core::delete_folder(lua_State* state)
{
	const auto dir = luaL_checkstring(state, 1);

	if (!std::strlen(dir))
		luaL_error(state, "cannot delete workspace directory");

	const auto path = workspace_path / dir;
	if (!fs::exists(path))
		luaL_error(state, "directory does not exist");

	fs::remove_all(path);

	return 0;
}

std::int32_t roblox::environment::core::delete_file(lua_State* state)
{
	const auto file = luaL_checkstring(state, 1);

	const auto path = workspace_path / file;
	if (!fs::exists(path))
		luaL_error(state, "file does not exist");

	fs::remove(path);
	return 0;
}

std::int32_t roblox::environment::core::load_file(lua_State* state)
{
	const auto dir = luaL_checkstring(state, 1);
	const auto chunkname = lua_gettop(state) > 1 ? luaL_checkstring(state, 2) : nullptr;

	std::ifstream stream(workspace_path / dir);

	std::string output;
	std::copy(std::istreambuf_iterator< char >(stream), std::istreambuf_iterator< char >(), std::back_inserter(output));

	if (!managers::roblox_mgr.push_script(state, output, chunkname))
	{
		lua_pushnil(state);
		lua_pushvalue(state, -2);

		return 2;
	}

	return 1;
}
