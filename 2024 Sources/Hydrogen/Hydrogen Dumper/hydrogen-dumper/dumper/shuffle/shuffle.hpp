#ifndef _SHUFFLE_
#define _SHUFFLE_

#include <vector>

namespace dumper
{
	class c_shuffle
	{
		class c_order
		{
			using arg_t = std::pair
				< std::uint8_t, std::size_t >;

			std::size_t m_idx;
			std::vector< arg_t > m_args;

		public:
			void add( const std::uint8_t off );

			void dump( );
		};

		void shuffle_2( ) const;
		void shuffle_3( ) const;

	public:
		void dump( ) const;
	} const g_shuffle;
}

#endif