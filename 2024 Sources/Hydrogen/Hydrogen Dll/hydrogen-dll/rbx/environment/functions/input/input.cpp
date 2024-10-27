#include "input.hpp"

bool rbx::environment::c_input::iswindowactive( sol::this_state L )
{
	return GetForegroundWindow( ) == game_window;
}

void rbx::environment::c_input::keypress( std::uint32_t keycode, sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = keycode;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::keyrelease( std::uint32_t keycode, sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = keycode;
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse1click( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		SendInput( 1, &input, sizeof( input ) );

		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse1press( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse1release( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse2click( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

		SendInput( 1, &input, sizeof( input ) );

		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse2press( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mouse2release( sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mousescroll( double px, sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.mouseData = px;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mousemoverel( double x, double y, sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::mousemoveabs( double x, double y, sol::this_state L )
{
	sol::state_view sv{ L };

	if ( sv[ "iswindowactive" ].call< bool >( ) )
	{
		INPUT input{ };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		input.mi.dx = x;
		input.mi.dy = y;

		SendInput( 1, &input, sizeof( input ) );
	}
}

void rbx::environment::c_input::init( lua_State* L ) const //TODO: test this
{
	game_window = FindWindowW( nullptr, L"Roblox" );

	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( iswindowactive, { "isrbxactive", "isgameactive", "iswindowactive" } );
	register_fn( keypress, { "keypress" } );
	register_fn( keyrelease, { "keyrelease" } );
	register_fn( mouse1click, { "mouse1click" } );
	register_fn( mouse1press, { "mouse1press" } );
	register_fn( mouse1release, { "mouse1release" } );
	register_fn( mouse2click, { "mouse2click" } );
	register_fn( mouse2press, { "mouse2press" } );
	register_fn( mouse2release, { "mouse2release" } );
	register_fn( mousescroll, { "mousescroll" } );
	register_fn( mousemoverel, { "mousemoverel" } );
	register_fn( mousemoveabs, { "mousemoveabs" } );
}
