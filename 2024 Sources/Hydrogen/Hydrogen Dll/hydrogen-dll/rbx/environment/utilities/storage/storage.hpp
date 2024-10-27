#pragma once

#include <unordered_map>
#include <mutex>

struct Closure;

namespace rbx::environment::utilities
{
	struct base_t;

	template< class t >
	class c_storage
	{
		std::mutex m_l;

	protected:
		t m_container;
	public:

		auto safe_request( auto request, auto... args )
		{
			std::unique_lock l{ m_l };

			return request( args... );
		};

		void clear( )
		{
			safe_request( [ & ]( )
				{ m_container.clear( ); } );
		}
	};

	using block_cache_t
		= std::unordered_map< void*,
			bool >;

	class c_blocked : public c_storage< block_cache_t >
	{
	public:
		void toggle( void* connection, bool enabled )
		{
			safe_request( [ & ]( )
			{
				m_container[ connection ] = enabled;
			} );
		}

		bool should_block( void* connection )
		{
			return safe_request( [ & ]( )
			{
				return m_container.contains( connection ) 
					? !m_container[ connection ] : false;
			} );
		}

	} inline g_blocked;

	using instance_cache_t
		= std::unordered_map< void*,
			std::unordered_map< std::string, bool > >;

	class c_instance_cache : public c_storage< instance_cache_t >
	{
	public:
		void toggle( void* instance, const std::string& prop, bool enabled )
		{
			safe_request( [ & ]( )
			{
				m_container[ instance ][ prop ] = enabled;
			} );
		}

		std::optional< bool > is_scriptable( void* instance, const std::string& prop )
		{
			return safe_request( [ & ]( ) -> std::optional< bool >
			{
				if ( m_container.contains( instance ) )
				{
					const auto properties = m_container[ instance ];

					if ( properties.contains( prop ) )
						return properties.at( prop );
				}

				return std::nullopt;
			} );
		}
	} inline g_instance;

	using closure_cache_t
		= std::vector< void* >;

	class c_closure_cache : public c_storage< closure_cache_t >
	{
	public:
		void add( void* closure )
		{
			safe_request( [ & ]( )
			{
				m_container.push_back( closure );
			} );
		}

		bool contains( void* closure )
		{
			return safe_request( [ & ]( ) -> bool
			{
				return std::find( m_container.begin( ), m_container.end( ), closure ) != m_container.end( );
			} );
		}
	} inline g_closure_cache;

	using newcclosure_cache_t 
		= std::unordered_map< Closure*, Closure* >;

	class c_newcclosure_cache : public c_storage< newcclosure_cache_t >
	{
	public:
		void add( Closure* cclosure, Closure* lclosure )
		{
			safe_request( [ & ]( )
			{
				m_container[ cclosure ] = lclosure;
			} );
		}

		void remove( Closure* obj )
		{
			safe_request( [ & ] ( )
			{
				auto it = m_container.find( obj );
				if ( it != m_container.end( ) )
					m_container.erase( it );
			} );
		}

		std::optional< Closure* > get( Closure* closure )
		{
			return safe_request( [ & ]( ) -> std::optional< Closure* >
			{
				if ( m_container.contains( closure ) )
					return m_container.at( closure );

				return std::nullopt;
			} );
		}
	} inline g_newcclosure_cache;

	using drawing_cache_t
		= std::vector< base_t* >;

	class c_drawing_cache : public c_storage< drawing_cache_t >
	{
	public:
		void add( base_t* obj );

		void remove( base_t* obj )
		{
			safe_request( [ & ]( )
			{
				if ( const auto iter = std::find( m_container.begin( ), m_container.end( ), obj ); iter != m_container.end( ) )
					m_container.erase( iter );
			} );
		}

		bool contains( const void* obj )
		{
			return safe_request( [ & ]( ) -> bool
			{
				if ( const auto iter = std::find( m_container.begin( ), m_container.end( ), obj ); iter != m_container.end( ) )
					return true;

				return false;
			} );
		}

		drawing_cache_t copy( )
		{
			return safe_request( [ & ]( ) -> drawing_cache_t
			{
				return m_container;
			} );
		}
	} inline g_drawing_cache;
}