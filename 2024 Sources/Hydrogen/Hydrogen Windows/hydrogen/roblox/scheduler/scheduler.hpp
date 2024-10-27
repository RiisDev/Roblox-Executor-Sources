#pragma once
#include <string_view>

#include <vector>

namespace roblox
{
	struct job_t
	{
		void* vftable;

		std::uint8_t pad_0 [ 0xC ];

		std::string name;

		std::uint8_t pad_1 [ 0x10 ];

		double time;

		std::uint8_t pad_2 [ 0x10 ];

		double time_spend;

		std::uint8_t pad_3 [ 0x8 ];

		std::uintptr_t state;
	};

	struct job_container_t
	{
		job_t* job;
		void* reference;
	};
    
    struct task_scheduler_t
    {
        [[nodiscard]] std::vector< job_t* > jobs( ) const;

		[[nodiscard]] job_t* get_job( const std::string_view name ) const;

		[[nodiscard]] std::vector< job_t* > find_jobs( const std::string_view name ) const;
		
		[[nodiscard]] static task_scheduler_t* get( );
    };
}