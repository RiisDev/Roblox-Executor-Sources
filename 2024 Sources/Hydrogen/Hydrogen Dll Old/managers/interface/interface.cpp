#include "interface.hpp"

#include <managers/managers.hpp>

#include "../roblox/environment/drawing/drawing.hpp"
#include <dependencies/fonts/poppins.hpp>

LRESULT __stdcall wnd_proc( const HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param )
{
	auto& if_mgr = managers::mgrs.interface_mgr;

	ImGuiIO& io = ImGui::GetIO( );

	io.MouseDrawCursor = if_mgr.render_interface;

	if ( ImGui_ImplWin32_WndProcHandler( hwnd, message, w_param, l_param ) )
		return true;

	switch ( message )
	{
	case WM_KILLFOCUS:
		if_mgr.window_selected = false;
		break;
	case WM_SETFOCUS:
		if_mgr.window_selected = true;
		break;
	case WH_CBT:
		if_mgr.window = if_mgr.sd.OutputWindow;
		if_mgr.global_hwnd = hwnd;
		break;
	case WM_KEYDOWN:
		if ( w_param == VK_INSERT )
			if_mgr.render_interface = !if_mgr.render_interface;
		break;
	case WM_MOUSEMOVE:
		if ( if_mgr.render_interface && if_mgr.window_selected )
			return TRUE;
		break;
	case WM_DPICHANGED:
		if_mgr.dpi = LOWORD( w_param ) / 96.0f;
		break;
	case 522:
	case 513:
	case 533:
	case 514:
	case 134:
	case 516:
	case 517:
	case 258:
	case 257:
	case 132:
	case 127:
	case 255:
	case 523:
	case 524:
	case 793:
		if ( if_mgr.render_interface ) return TRUE;
		break;
	}

	return CallWindowProc( if_mgr.original_wnd_proc, hwnd, message, w_param, l_param );
}

HRESULT __stdcall present_hook( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags )
{
	auto& if_mgr = managers::mgrs.interface_mgr;

	std::call_once( if_mgr.is_init, [ & ] ( )
		{
			swap_chain->GetDesc( &if_mgr.sd );
			swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &if_mgr.global_device ) );
			swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &if_mgr.back_buffer ) );

			if_mgr.window = if_mgr.sd.OutputWindow;
			if_mgr.original_wnd_proc = reinterpret_cast< WNDPROC >( SetWindowLongPtrA( if_mgr.window, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( wnd_proc ) ) );

			if_mgr.global_device->GetImmediateContext( &if_mgr.global_context );
			if_mgr.global_device->CreateRenderTargetView( if_mgr.back_buffer, nullptr, &if_mgr.main_render_target_view ); if_mgr.back_buffer->Release( );

			ImGui::CreateContext( );
			ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

			managers::mgrs.menu_mgr.set_style( );

			ImGui_ImplWin32_Init( if_mgr.window );
			ImGui_ImplDX11_Init( if_mgr.global_device, if_mgr.global_context );

			static const ImWchar ranges[ ] = {
				0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
				0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
			};

			ImFontConfig icons_config;
			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;

			ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( Poppins_compressed_data, Poppins_compressed_size, 14, NULL, ranges );
		} );

	if ( if_mgr.main_render_target_view == nullptr )
	{
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &if_mgr.back_buffer ) );
		if_mgr.global_device->CreateRenderTargetView( if_mgr.back_buffer, nullptr, &if_mgr.main_render_target_view );

		if_mgr.back_buffer->Release( );
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	ImGuiIO& io = ImGui::GetIO( );

	io.MousePos /= if_mgr.dpi;

	ImGui::NewFrame( );

	for ( auto obj : if_mgr.drawing_objects )
	{
		if ( obj )
			obj->draw( );
	}

	if ( if_mgr.render_interface )
		managers::mgrs.menu_mgr.render( );

	ImGui::Render( );

	ImDrawData* data = ImGui::GetDrawData( );

	for ( int i = 0; i < data->CmdListsCount; i++ )
	{
		ImDrawList* cmd_list = data->CmdLists[ i ];
		for ( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
		{
			ImDrawCmd* cmd = &cmd_list->CmdBuffer[ cmd_i ];
			cmd->ClipRect = cmd->ClipRect * if_mgr.dpi;
		}

		for ( auto& cmd : data->CmdLists[ i ]->VtxBuffer )
		{
			cmd.pos.x *= if_mgr.dpi;
			cmd.pos.y *= if_mgr.dpi;
		}
	}

	if_mgr.global_context->OMSetRenderTargets( 1, &if_mgr.main_render_target_view, NULL );
	ImGui_ImplDX11_RenderDrawData( data );

	return if_mgr.d3d11_present( swap_chain, sync_interval, flags );
}

HRESULT __stdcall resize_buffers_hook( IDXGISwapChain* this_ptr, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags )
{
	auto& if_mgr = managers::mgrs.interface_mgr;

	if ( if_mgr.main_render_target_view )
	{
		if_mgr.main_render_target_view->Release( );
		if_mgr.main_render_target_view = nullptr;
	}

	if_mgr.window = if_mgr.sd.OutputWindow;

	GetWindowRect( if_mgr.global_hwnd, &if_mgr.rc );
	GetClientRect( if_mgr.global_hwnd, &if_mgr.client_rect );

	if_mgr.s_w = if_mgr.rc.right - if_mgr.rc.left;
	if_mgr.s_h = if_mgr.rc.bottom - if_mgr.rc.top;

	return if_mgr.d3d11_resize_buffers( this_ptr, buffer_count, width, height, new_format, swap_chain_flags );
}


void managers::interface_manager_t::initiate( )
{
	global_raw_hwnd = FindWindowW( 0, L"Roblox" );
	global_hwnd = reinterpret_cast< HWND >( CreateMenu( ) );

	SetForegroundWindow( global_raw_hwnd );
	GetWindowRect( global_raw_hwnd, &rc );
	GetClientRect( global_raw_hwnd, &client_rect );

	dpi = ImGui_ImplWin32_GetDpiScaleForHwnd( global_raw_hwnd );

	s_w = rc.right - rc.left;
	s_h = rc.bottom - rc.top;

	D3D_FEATURE_LEVEL levels[ ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtained_level;

	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = global_hwnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = ( ( GetWindowLongPtrA( global_hwnd, GWL_STYLE ) & WS_POPUP ) != 0 ) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof( levels ) / sizeof( D3D_FEATURE_LEVEL ), D3D11_SDK_VERSION, &sd, &global_swapchain, &global_device, &obtained_level, &global_context );

	std::uintptr_t* vt_swapchain;
	std::memcpy( &vt_swapchain, reinterpret_cast< LPVOID >( global_swapchain ), sizeof( std::uintptr_t ) );

	DWORD old_protection;
	VirtualProtect( vt_swapchain, sizeof( std::uintptr_t ), PAGE_EXECUTE_READWRITE, &old_protection );

	d3d11_present = reinterpret_cast< decltype( d3d11_present ) >( vt_swapchain[ 8 ] );
	d3d11_resize_buffers = reinterpret_cast< decltype( d3d11_resize_buffers ) >( vt_swapchain[ 13 ] );

	vt_swapchain[ 8 ] = reinterpret_cast< std::uintptr_t >( &present_hook );
	vt_swapchain[ 13 ] = reinterpret_cast< std::uintptr_t >( &resize_buffers_hook );

	VirtualProtect( vt_swapchain, sizeof( std::uintptr_t ), old_protection, &old_protection );
}