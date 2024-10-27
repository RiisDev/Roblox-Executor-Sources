#ifndef _INTEGRITY_
#define _INTEGRITY_

#include <udlib/ud.hpp>

namespace rbx::ac
{
	class c_integrity
	{
		std::pair< ud::segment_t, std::uint8_t* > 
			text{ ud::segment_t( ".text" ), nullptr };

		void* m_calc_second;
		static void __fastcall calc_second( 
			const void* const a1, const std::uint16_t a2, 
				const std::size_t key, const std::uint32_t& hash );

		std::uint8_t* m_jmp_calc_first, *shit;
		std::uintptr_t m_addr;
		static void calc_first( );

		static void second_check_bypass( );

	public:
		void init( );
	} inline g_integrity;
}

#endif