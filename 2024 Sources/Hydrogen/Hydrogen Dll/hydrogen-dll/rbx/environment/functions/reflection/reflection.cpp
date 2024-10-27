#include "reflection.hpp"

#include <luau/bytecode/bytecode.hpp>
#include <rbx/environment/utilities/storage/storage.hpp>
#include <rbx/environment/utilities/thread/thread.hpp>
#include <rbx/environment/utilities/cclosure/cclosure.hpp>

std::tuple< sol::object, std::optional< std::string > > rbx::environment::c_reflection::loadstring( std::string chunk, std::optional< std::string > chunk_name, sol::this_state L )
{
	if ( ::luau::g_bytecode.load( L, chunk, chunk_name ) )
		return std::make_tuple( sol::stack::get< sol::function >( L, -1 ), std::nullopt );

	return std::make_tuple( sol::nil, sol::stack::get< std::string >( L, -1 ) );
}

bool rbx::environment::c_reflection::checkcaller( sol::this_state L )
{
	return !utilities::c_thread
		::get_threads_script< bool >( L );
}

bool rbx::environment::c_reflection::islclosure( sol::function f, sol::this_state L ) //TODO: check if this should error for non func type or just ret false
{
	if ( !f.is< sol::function >( ) )
		return false;

	return !reinterpret_cast< 
		const Closure* >( f.pointer( ) )->isC;
}

bool rbx::environment::c_reflection::iscclosure( sol::function f, sol::this_state L ) //TODO: error checking cuz apparently this doesnt already do it
{
	if ( !f.is< sol::function >( ) )
		return false;

	return reinterpret_cast<
		const Closure* >( f.pointer( ) )->isC;
}

//TODO: add decompile once 0x90 finishes it

bool rbx::environment::c_reflection::isexecutorclosure( sol::function f, sol::this_state L )
{
	const auto closure = reinterpret_cast<
		Closure* >( const_cast< void* >( f.pointer( ) ) );

	const auto closure_t 
		= utilities::g_cclosure.get_closure_type( closure );

	if ( closure_t == utilities::closure_type_t::lclosure )
		return std::uint8_t( ( closure->l.p->code[ closure->l.p->sizecode - 1 ] & 0xFF ) * 203u ) == LOP_SKID;
	else if ( closure_t == utilities::closure_type_t::newcclosure )
		return true;
	else
		return utilities::g_closure_cache.contains( closure );
}

void rbx::environment::c_reflection::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( loadstring, { "loadstring" } );
	register_fn( checkcaller, { "checkcaller" } );
	register_fn( islclosure, { "islclosure" } );
	register_fn( iscclosure, { "iscclosure" } );
	register_fn( isexecutorclosure, { "isexecutorclosure", "checkclosure", "isourclosure" } );
}
