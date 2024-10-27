#pragma once

#include <dependencies/common_includes.hpp>

#include "../datamodel/datamodel.hpp"

#include <managers/offset/offset.hpp>

namespace roblox::structures
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

	struct task_scheduler_t : public base_t
	{
		struct job_container_t
		{
			job_t* job;
			void* reference;
		};

		[[nodiscard]] std::vector< job_container_t > get_jobs( ) const
		{
			return { *reinterpret_cast< job_container_t** >( address( ) + offsets::task_scheduler::jobs_start ), *reinterpret_cast< job_container_t** >( address( ) + offsets::task_scheduler::jobs_end ) };
		}

		job_t* get_job_by_name( std::string_view name ) const
		{
			for ( const auto children = get_jobs( ); const auto child : children )
			{
				if ( child.job->name == name )
					return child.job;
			}

			return nullptr;
		}

		[[nodiscard]] std::vector< job_t* > get_jobs_by_name( std::string_view name ) const
		{
			std::vector< job_t* > jobs;

			for ( const auto children = get_jobs( ); const auto child : children )
			{
				if ( child.job->name == name )
					jobs.push_back( child.job );
			}

			return jobs;
		}

		template< typename t >
		t get_job_by_name( std::string_view name ) const
		{
			for ( const auto children = get_jobs( ); const auto child : children )
			{
				if ( child.instance->name == name )
					return reinterpret_cast< t >( child.job );
			}

			return nullptr;
		}

		static task_scheduler_t* get( )
		{
			return *reinterpret_cast< task_scheduler_t** >( offsets::task_scheduler::task_scheduler_ptr );
		}
	};

}