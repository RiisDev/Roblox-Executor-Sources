#include "renderer.hpp"

#include <rbx/environment/utilities/storage/storage.hpp>

#include <rbx/environment/utilities/drawing_types/drawing_types.hpp>
#include <dependencies/fonts/karla.hpp>
#include <dependencies/fonts/poppins.hpp>
#include <dependencies/fonts/roboto.hpp>
#include <dependencies/fonts/inter.hpp>

HRESULT __stdcall c_renderer::on_present( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags )
{
	static std::once_flag flag;
	std::call_once( flag, [ & ]( )
	{
		if ( swap_chain->GetDesc( &g_renderer.sd ) != S_OK )
			MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 22", "Hydrogen Error", MB_OK | MB_ICONERROR );

		if ( swap_chain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &g_renderer.device ) ) != S_OK )
			MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 23", "Hydrogen Error", MB_OK | MB_ICONERROR );

		if ( swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &g_renderer.back_buffer ) ) != S_OK )
			MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 24", "Hydrogen Error", MB_OK | MB_ICONERROR );

		g_renderer.window = g_renderer.sd.OutputWindow;

		//TODO: add wndproc if we wanna do internal ui later

		g_renderer.device->GetImmediateContext( &g_renderer.context );
		if ( g_renderer.device->CreateRenderTargetView( g_renderer.back_buffer, nullptr, &g_renderer.rtv ) != S_OK )
			MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 25", "Hydrogen Error", MB_OK | MB_ICONERROR );

		g_renderer.back_buffer->Release( );

		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		ImGui_ImplWin32_Init( g_renderer.window );
		ImGui_ImplDX11_Init( g_renderer.device, g_renderer.context );

		static const ImWchar ranges[] = {
				0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
				0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
		};

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		
		g_renderer.Ui = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( karla_compressed_data, karla_compressed_size, 25, NULL, ranges );
		g_renderer.System = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( Poppins_compressed_data, Poppins_compressed_size, 25, NULL, ranges );
		g_renderer.Plex = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( roboto_compressed_data, roboto_compressed_size, 25, NULL, ranges );
		g_renderer.Monospace = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( inter_compressed_data, inter_compressed_size, 25, NULL, ranges );
	} );

	if ( g_renderer.rtv == nullptr )
	{
		swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &g_renderer.back_buffer ) );
		g_renderer.device->CreateRenderTargetView( g_renderer.back_buffer, nullptr, &g_renderer.rtv );

		g_renderer.back_buffer->Release( );
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );

	ImGuiIO& io = ImGui::GetIO( );

	ImGui::NewFrame( );

	const auto copy = rbx::environment::utilities::g_drawing_cache.copy( );

	for ( const auto& drawing_obj : copy )
		drawing_obj->render( );

	ImGui::Render( );

	ImDrawData* data = ImGui::GetDrawData( ); //TODO: dpi scaling

	g_renderer.context->OMSetRenderTargets( 1, &g_renderer.rtv, NULL );
	ImGui_ImplDX11_RenderDrawData( data );

	return reinterpret_cast< decltype( &on_present ) >( 
		g_renderer.orig_swapchain.at( 8 ) )( swap_chain, sync_interval, flags );
}

HRESULT __stdcall c_renderer::on_resize( IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags )
{
	if ( g_renderer.rtv )
	{
		g_renderer.rtv->Release( );
		g_renderer.rtv = nullptr;
	}

	g_renderer.window = g_renderer.sd.OutputWindow;

	GetWindowRect( g_renderer.roblox_window, &g_renderer.screen_bounds );

	return reinterpret_cast< decltype( &on_resize ) >(
		g_renderer.orig_swapchain.at( 13 ) )( swap_chain, buffer_count, width, height, new_format, swap_chain_flags );
}

void c_renderer::init( )
{
	roblox_window = FindWindowW( nullptr, L"Roblox" );

	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 };
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
		sd.OutputWindow = roblox_window;
		sd.SampleDesc.Count = 1;
		sd.Windowed = true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		sizeof( levels ) / sizeof( D3D_FEATURE_LEVEL ),
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&obtained_level,
		&context
	);

	if ( hr != S_OK )
		MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 20", "Hydrogen Error", MB_OK | MB_ICONERROR );

	const PLH::VFuncMap map = {
		{ 8, reinterpret_cast< uint64_t >( &on_present ) },
		{ 13, reinterpret_cast< uint64_t >( &on_resize ) }
	};

	swapchain_hook = std::make_unique< 
		PLH::VFuncSwapHook >( reinterpret_cast< uint64_t >( swap_chain ),
				map, &orig_swapchain );

	if( !swapchain_hook->hook( ) )
		MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 21", "Hydrogen Error", MB_OK | MB_ICONERROR );
}
