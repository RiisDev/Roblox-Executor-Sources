#pragma once

#include <dependencies/common_includes.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <imgui/imgui.h>
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace roblox::drawing
{
	struct base_t;
}

namespace managers
{
	class interface_manager_t
	{
	public:
		using present_t = HRESULT( __stdcall* )( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags );
		using resize_buffers_t = HRESULT( __stdcall* )( IDXGISwapChain* this_ptr, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags );

		/* Amoys job below xd */
		struct {
			ImFont* System;
			ImFont* Plex;
			ImFont* Monospace;
		} fonts;

		float dpi = 1.f;

		std::vector< roblox::drawing::base_t*> drawing_objects;

		std::once_flag is_init;

		bool render_interface = false;
		bool window_selected = true;
		bool main_window_enabled = true;

		HWND window, global_raw_hwnd, global_hwnd;

		int s_w, s_h;

		RECT rc, client_rect;

		WNDPROC original_wnd_proc;

		present_t d3d11_present;
		resize_buffers_t d3d11_resize_buffers;

		DXGI_SWAP_CHAIN_DESC sd;

		ID3D11Device* global_device;
		ID3D11DeviceContext* global_context;
		ID3D11RenderTargetView* main_render_target_view;
		ID3D11Texture2D* back_buffer;
		IDXGISwapChain* global_swapchain;

		void initiate( );
	};
}