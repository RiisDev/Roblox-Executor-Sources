#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <cstdint>
#include <vector>

#include "Luau/lstate.h"


class D3D11Hook
{
public:
    using Present = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    using ResizeBuffers = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

    static HWND window;
    static DXGI_SWAP_CHAIN_DESC swapchain_desc;

    static Present o_present;
    static ResizeBuffers o_resize_buffers;
    static WNDPROC o_window_callback;

    static IDXGISwapChain* swap_chain;
    static ID3D11Device* d3d_device;
    static ID3D11DeviceContext* device_context;
    static ID3D11RenderTargetView* render_target_view;
    static ID3D11Texture2D* back_buffer;

    static std::int32_t width;
    static std::int32_t height;

    static bool render_scene;
    static bool render_interface;
    static bool render_initialized;

    static float dpi_scale;
    static std::vector < ImFont* > rendering_fonts;

    static ImFont* ui;
    static ImFont* system;
    static ImFont* plex;
    static ImFont* monospace;
    static ImFont* consolas;

    using render_vf_function_t = std::uintptr_t(__fastcall*)(std::uintptr_t, std::uintptr_t);
    static render_vf_function_t original_render_step;

    static bool is_render_hooked;

    static void Initialize();

    static void InitializeEnvironment(lua_State* L);

    static void ClearDrawings();

    static void Reinitialize();

    static void Unhook();
};