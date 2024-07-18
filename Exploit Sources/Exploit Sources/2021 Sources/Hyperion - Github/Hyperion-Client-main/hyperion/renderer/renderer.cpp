#include "renderer.hpp"
#include <mutex>
#include "../roblox/vmthook/vmthook.hpp"
#include <iostream>
#include "../imgui/texteditor.hpp"
#include "../roblox/execution/execution.hpp"
#include "../roblox/globals/globals.hpp"
#include "../compiler/Parser/parser.hpp"
#include "../compiler/CodeGen/codegen.hpp"
#include "../compiler/Serializer/serializer.hpp"

vmthook* dx_vt_hook;

bool window_selected = true;

int s_w, s_h;
RECT rc;
RECT client_rect;
ImFont* IFont;
ImFont* SFont;

bool render_interface = false;

HWND window;
DXGI_SWAP_CHAIN_DESC sd;
ID3D11RenderTargetView* main_render_target_view;
WNDPROC ownd_proc;

HWND global_raw_hwnd = nullptr; // Roblox HWND
HWND global_hwnd = nullptr; // Global "protected" HWND

ID3D11Device* global_device = nullptr;
ID3D11DeviceContext* global_context = nullptr;
IDXGISwapChain* global_swapchain = nullptr;
std::once_flag is_init;

ID3D11Texture2D* pback_buffer = nullptr;
D3D11PresentHook pd3d11_present = nullptr;
ResizeBuffersHook pd3d11_resize_buffers = nullptr;

static bool main_window_enabled = true;

uintptr_t* swapchain_vtable = nullptr;

void init_imgui(ID3D11Device* device, ID3D11DeviceContext* ctx)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	IFont = io.Fonts->AddFontDefault();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = NULL;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, ctx);
}

//handle windows input
LRESULT WINAPI wnd_proc(const HWND hWnd, UINT Msg, WPARAM w_param, LPARAM l_param)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, w_param, l_param);

	if(render_interface) ImGui::GetIO().MouseDrawCursor = true;
	else ImGui::GetIO().MouseDrawCursor = false;

	switch (Msg)
	{
	case WM_KILLFOCUS:
		window_selected = false;
		break;
	case WM_SETFOCUS:
		window_selected = true;
		break;
	case WH_CBT:
		window = sd.OutputWindow;
		global_hwnd = hWnd;
		break;
	case WM_KEYDOWN:
		if (w_param == VK_INSERT) render_interface = !render_interface;
		break;
	case WM_MOUSEMOVE:
		if (render_interface && window_selected)
			return TRUE;
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
		if (render_interface) return TRUE; //block basically all messages we don't want roblox to receive
		break;
	}

	return CallWindowProc(ownd_proc, hWnd, Msg, w_param, l_param);
}

TextEditor editor;

//our d3d11 rendering hook
HRESULT WINAPI present_hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	std::call_once(is_init, [&]()
	{
		pSwapChain->GetDesc(&sd);
		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&global_device));
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pback_buffer));
		window = sd.OutputWindow;
		ownd_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc)));
		global_device->GetImmediateContext(&global_context);
		global_device->CreateRenderTargetView(pback_buffer, nullptr, &main_render_target_view); pback_buffer->Release();
		init_imgui(global_device, global_context);
		editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	});

	//if RTV becomes nullptr, which happens on resize because of our resize_buffers hook, we create new one
	if (main_render_target_view == nullptr)
	{
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pback_buffer));
		global_device->CreateRenderTargetView(pback_buffer, nullptr, &main_render_target_view);
		pback_buffer->Release();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (render_interface)
	{
		ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
		bgDrawList->AddRectFilled(ImVec2(0, 0), ImVec2(s_w, s_h), ImColor(0.0f, 0.0f, 0.0f, 0.5f));

//		ImGui::ShowStyleEditor();
		ImGui::SetNextWindowSize(ImVec2(700, 375), ImGuiCond_Once);
		ImGui::Begin("Hyperion", &main_window_enabled, ImGuiWindowFlags_NoResize);
		{

			if (ImGui::BeginTabBar("Menu"))
			{
				if (ImGui::TabItemButton("Executor"))
				{
				}

				if (ImGui::TabItemButton("Script Hub"))
				{

				}

				if (ImGui::TabItemButton("Settings"))
				{

				}

				ImGui::EndTabBar();
			}

			if (ImGui::BeginChild(1))
			{
				editor.Render("TextEditor", { 670, 260 });

				ImGui::Separator();
				if (ImGui::Button("Execute", { 75,20 }))
				{
					srcs.push(editor.GetText());
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear", { 75,20 }))
				{
					editor.SetText("");
				}
				ImGui::SameLine();
				ImGui::Button("Open", { 75,20 });

				ImGui::EndChild();
			}
		}

		ImGui::End();
	}
	ImGui::Render();
	global_context->OMSetRenderTargets(1, &main_render_target_view, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return pd3d11_present(pSwapChain, SyncInterval, Flags);
}

//handle resizing and resets RTV
HRESULT WINAPI resize_buffers_hook(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (main_render_target_view)
	{
		main_render_target_view->Release();
		main_render_target_view = nullptr;
	}

	window = sd.OutputWindow;
	GetWindowRect(global_hwnd, &rc);
	GetClientRect(global_hwnd, &client_rect);
	s_w = rc.right - rc.left;
	s_h = rc.bottom - rc.top;

	return pd3d11_resize_buffers(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

void start()
{
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = global_hwnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = ((GetWindowLongPtrA(global_hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.BufferDesc.Width = 1;
		sd.BufferDesc.Height = 1;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, sizeof(levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &sd, &global_swapchain, &global_device, &obtainedLevel, &global_context);

	uintptr_t* vt_swapchain;
	memcpy(&vt_swapchain, reinterpret_cast<LPVOID>(global_swapchain), sizeof(uintptr_t));

	pd3d11_present = reinterpret_cast<D3D11PresentHook>(dx_vt_hook->replace_vtable_funcpointer(vt_swapchain, 8, reinterpret_cast<unsigned int>(&present_hook)));
	pd3d11_resize_buffers = reinterpret_cast<ResizeBuffersHook>(dx_vt_hook->replace_vtable_funcpointer(vt_swapchain, 13, reinterpret_cast<unsigned int>(&resize_buffers_hook)));
	
	std::cout << "DX11 Hooked" << std::endl;
}

void hyperion::renderer::dx_hook::hook_init()
{
	global_raw_hwnd = FindWindowW(NULL, L"Roblox");
	SetForegroundWindow(global_raw_hwnd);
	global_hwnd = reinterpret_cast<HWND>(CreateMenu());
	GetWindowRect(global_raw_hwnd, &rc);
	GetClientRect(global_raw_hwnd, &client_rect);
	s_w = rc.right - rc.left;
	s_h = rc.bottom - rc.top;
	start();
}
