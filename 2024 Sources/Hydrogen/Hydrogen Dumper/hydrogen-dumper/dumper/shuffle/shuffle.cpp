#include <functional>
#include <span>
#include "../dumper.hpp"
#include "shuffle.hpp"

void dumper::c_shuffle::c_order::add( const std::uint8_t off )
{
	m_args.emplace_back( off, m_idx++ );
}

void dumper::c_shuffle::c_order::dump( )
{
	std::printf( "#define LUAVM_SHUFFLE( sep, " );
	for ( auto idx = 0u; idx < m_args.size( ); ++idx )
		std::printf( "a%i%s", idx, 
			idx == m_args.size( ) - 1 ? " ) " : ", " );

	std::ranges::sort( m_args, {}, &arg_t::first );

	std::ranges::for_each( m_args, 
		std::bind( &std::printf, "a%i sep ", 
			std::placeholders::_1 ), &arg_t::second );

	std::printf( "\n" );
}

void dumper::c_shuffle::shuffle_2( ) const
{
	const auto scan = 
		*rbx.find_pattern< std::uint8_t* >
			( "8B ?? ?? 6A 00 ?? ?? FF ?? ?? FF D0" );

	c_order order;

	order.add( scan[ 2 ] );
	order.add( scan[ 9 ] );

	order.dump( );
}

void dumper::c_shuffle::shuffle_3( ) const
{
	const auto scan =
		*reinterpret_cast< const char*** >
			( *rbx.find_pattern
				( "B8 ?? ?? ?? ?? 83 FA FF 74 07" ) + 13 );

	std::span< const char* > types{ scan, 10 };
	const auto find = [ & ]( const char* const name )
	{
		using strstr_t = const char*( * )
			( const char*, const char* );

		const auto strstr = std::bind( 
			static_cast< strstr_t >( std::strstr ),
				name, std::placeholders::_1 );

		return std::distance( types.begin( ), 
			std::ranges::find_if( types, strstr ) );
	};
	
	c_order order;

	order.add( find( "userdata" ) );
	order.add( find( "number" ) );
	order.add( find( "vector" ) );

	order.dump( );
}

void dumper::c_shuffle::dump( ) const
{
#define SHUFFLE( fnc ) std::bind( &c_shuffle::fnc, this )

	const auto shuffles = 
	{ 
		SHUFFLE( shuffle_2 ), 
		SHUFFLE( shuffle_3 ) 
	};

	for ( const auto shuffle : shuffles )
		shuffle( );
}