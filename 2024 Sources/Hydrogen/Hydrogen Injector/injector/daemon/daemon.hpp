#pragma once

namespace daemon
{
	class c_daemon
	{

	public:
		static bool is_daemon( );

		void init( );
	} inline g_daemon;
}