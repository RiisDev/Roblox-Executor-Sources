#include "misc.hpp"

#include <rbx/environment/environment.hpp>
#include <rbx/environment/utilities/thread/thread.hpp>

void rbx::environment::c_misc::setclipboard( std::string value, sol::this_state L )
{
	if ( !OpenClipboard( nullptr ) )
		luaL_error( L, "failed to open clipboard" );

	EmptyClipboard( );

	const auto global_alloc = GlobalAlloc( GMEM_MOVEABLE, value.size( ) + 1 );
	if ( global_alloc == nullptr )
		luaL_error( L, "failed to allocate clipboard memory" );

	const auto global_lock = GlobalLock( global_alloc );
	if ( global_lock == nullptr )
		luaL_error( L, "failed to lock clipboard memory" );

	std::memcpy( global_lock, value.c_str( ), value.size( ) + 1 );

	GlobalUnlock( global_alloc );
	SetClipboardData( CF_TEXT, global_alloc );

	CloseClipboard( );
	GlobalFree( global_alloc );
}

sol::object rbx::environment::c_misc::getnamecallmethod( sol::this_state L )
{
	if ( const auto method = lua_namecallatom( L, nullptr ) )
		return sol::make_object( L, method );

	return sol::nil;
}

void rbx::environment::c_misc::setnamecallmethod( std::string method, sol::this_state L )
{
	lua_pushstring( L, method.c_str( ) );

	L->namecall = tsvalue( luaA_toobject( L, -1 ) );
}

std::int32_t rbx::environment::c_misc::messagebox( lua_State* L )
{
	const auto text = luaL_checkstring( L, 1 );
	const auto caption = luaL_checkstring( L, 2 );

	const auto flags = luaL_checkinteger( L, 3 );

	const auto identity
		= utilities::c_thread::get_thread_identity( L );

	lua_pushthread( L );
	const auto refid = lua_ref( L, -1 );

	struct c_storage
	{
		std::uint32_t identity;
		lua_State* L;
		int refid;
		int res;
		std::string text;
		std::string caption;
		int flags;
	};

	c_storage* storage =
		new c_storage{ .identity = identity,
			.L = L, .refid = refid, .text = text, 
				.caption = caption, .flags = flags }; // Prevent destruction of any objects due to exiting scope

	std::thread{
		[ storage ]( )
		{
			storage->res = MessageBoxA( nullptr, 
				storage->text.c_str( ), storage->caption.c_str( ), storage->flags );

			g_environment.m_yields.insert(
			[ storage ]( )
			{
				utilities::c_thread
					::set_thread_identity( storage->L, storage->identity );

				lua_pushinteger( storage->L, storage->res );

				g_environment.resume( storage->L, 1 );

				lua_unref( storage->L, storage->refid );

				delete storage;
			} );
		}
	}.detach( );

	return lua_yield( L, 1 );
}

void rbx::environment::c_misc::setfpscap( std::uint32_t fps, sol::this_state L )
{
	const auto task_scheduler = *task_scheduler::offs::self;

	static const auto fps_cap_offset = [ task_scheduler ]( ) -> std::uint16_t
	{
		for ( auto i = 0u; i < 0x500; i += 0x4 )
		{
			if ( 1.0 / *reinterpret_cast< double* >( task_scheduler + i ) == 60.0f )
				return i;
		}
	}( );

	*reinterpret_cast< double* >( task_scheduler + fps_cap_offset ) = fps > 0.f ? 1.0f / fps : 1.0 / 10000.0;
}

void rbx::environment::c_misc::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( setclipboard, { "setclipboard", "toclipboard" } );
	register_fn( getnamecallmethod, { "getnamecallmethod" } );
	register_fn( setnamecallmethod, { "setnamecallmethod" } );
	register_fn( messagebox, { "messagebox" } );
	register_fn( setfpscap, { "setfpscap" } );
}
