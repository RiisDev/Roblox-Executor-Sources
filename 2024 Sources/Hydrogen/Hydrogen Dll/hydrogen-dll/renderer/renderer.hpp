#pragma once

#include <dependencies/polyhook2/Virtuals/VFuncSwapHook.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <imgui/imgui.h>
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"

class c_renderer
{
	HWND roblox_window, window;
	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;

	ID3D11Texture2D* back_buffer;

	DXGI_SWAP_CHAIN_DESC sd;
	RECT screen_bounds;

	static HRESULT __stdcall on_present( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags );
	static HRESULT __stdcall on_resize( IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags );

	PLH::VFuncMap orig_swapchain{ };

	std::unique_ptr< PLH::VFuncSwapHook > swapchain_hook;

	ImFont* Ui, * System, * Plex, * Monospace;
public:
	ID3D11Device* get_device( )
	{
		return device;
	}

	ImFont* get_font( int id )
	{
		switch ( id )
		{
			case 0:
				return Ui;
			case 1:
				return System;
			case 2:
				return Plex;
			case 3:
				return Monospace;
			default:
				return Ui;
		}
	}

	void init( );
} inline g_renderer;