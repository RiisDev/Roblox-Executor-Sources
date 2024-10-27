#pragma once
#include <Windows.h>
#include <cstddef>

namespace ui
{
	class c_ui
	{
		const std::size_t  m_script_input = ( WM_APP + 500 );
		const std::size_t m_execute = ( WM_APP + 501 );

		HWND w_main_window = nullptr;
		HWND w_script_input = nullptr;
		HWND b_execute = nullptr;
	public:
		HWND get_main_window( );
		HWND get_script_input( );
		bool create( );
	} inline g_ui;
}