#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <chrono>
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "wininet.lib")
#include <string>
#include <iostream>
#include <stdlib.h>
#include "LuaWrapper.h"
#include "Offsets.h"
#include <vector>
#include <intrin.h>
#include <TlHelp32.h>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include "XorS.h"
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#include <tchar.h>
#include "dwmapi.h"
#include "detours/detours.h"
#pragma comment(lib, "detours/detours.lib")
#include "TextEditor/TextEditor.h"
#pragma comment(lib, "Dwmapi.lib")
using namespace std;

bool firstTime = true;

UINT vps = 1;
D3D11_VIEWPORT viewport;

ID3D11RenderTargetView* RenderTargetView = NULL;

HWND window = nullptr;
bool ShowMenu = false;
static WNDPROC OriginalWndProcHandler = nullptr;

HRESULT hr;

typedef HRESULT(__stdcall* D3D11Hook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

D3D11Hook pHookD3D11 = NULL;

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

DWORD_PTR* pSwapChainVtable = NULL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(window, &mPos);
	ImGui::GetIO().MousePos.x = mPos.x;
	ImGui::GetIO().MousePos.y = mPos.y;

	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_INSERT)
		{
			if (ShowMenu)
				io.MouseDrawCursor = true;
			else
				io.MouseDrawCursor = false;
		}
	}

	if (ShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
}

std::string Rend2(int length) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string ppj;
	int pos = 0;
	while (pos != length) {
		int ppk = ((rand() % (str.size() - 1)));
		ppj = ppj + str[ppk];
		pos += 1;
	}
	return ppj.c_str();
}

int AdvanchedGC(lua_State* L) {
	void* UserObject = lua_touserdata(L, 1);
	if (L) {
		lua_pushvalue(L, 1);
		if (!lua_isnil(L, -1)) {
			int LRef = lua_tointeger(L, -1);

			r_lua_pushnil(RLS);
			r_lua_rawseti(RLS, LUA_REGISTRYINDEX, LRef);
			r_lua_settop(RLS, 0);
		}
		lua_pop(L, 1);
	}
	r_lua_settop(RLS, 0);
	return 0;
}


void ExecuteScript(const char* script)
{
	std::string buf;
	std::string newl = "\n";
	std::string random = Rend2(10);

	buf.append(R"(
spawn(function() 
script=Instance.new('LocalScript') 
script.Parent=nil 

local ServerSide_Token = "X_XXAHRPPARPPRXHPHHPER"
--server.push(ServerSide_Token, "ServerScriptStorage")
)");
	buf.append(script);
	buf.append("\r\nend)");
	//std::cout << igreen << buf.c_str() << endl;
	if (luaL_loadbuffer(LS, buf.c_str(), buf.size(), "@iYobel")) {
		//r_lua_singleton(error, lua_tostring(LS, -1));
	}
	else {
		lua_pcall(LS, 0, 0, 0);
	}
	//AdvanchedGC(LS);
}

HRESULT __stdcall HookD3D11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		ShowMenu = !ShowMenu;
	}

	if (firstTime)
	{
		firstTime = false;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		//imgui
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		window = sd.OutputWindow;

		//wndprochandler
		OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hWndProc);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
		ImGui::GetIO().ImeWindowHandle = window;
	}

	//create rendertarget
	if (RenderTargetView == NULL)
	{
		//viewport
		pContext->RSGetViewports(&vps, &viewport);

		//get backbuffer
		ID3D11Texture2D* backbuffer = NULL;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		if (FAILED(hr)) {
			return hr;
		}

		//create rendertargetview
		hr = pDevice->CreateRenderTargetView(backbuffer, NULL, &RenderTargetView);
		backbuffer->Release();
		if (FAILED(hr)) {
			return hr;
		}
	}
	else //call before you draw
		pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);


	//imgui
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	//mouse cursor on in menu
	if (ShowMenu)
		ImGui::GetIO().MouseDrawCursor = 1;
	else
		ImGui::GetIO().MouseDrawCursor = 0;

	//menu
	if (ShowMenu)
	{
		XorS(name, "Shiny Internal UI");
		XorS(exec, "Execute");
		XorS(clear, "Clear");
		XorS(open, "Open File");
		XorS(save, "Save File");
		XorS(executefil, "Execute File");
		ImGui::Begin(name.decrypt(), NULL, ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(450, 300), ImGuiCond_Always);
		static TextEditor Editor;
		static auto EditorInit = FALSE;
		if (!EditorInit)
		{
			//--TextEditor Init--//
			const auto Lang = TextEditor::LanguageDefinition::Lua();
			Editor.SetLanguageDefinition(Lang);
			Editor.SetPalette(TextEditor::GetDarkPalette());
			EditorInit = TRUE;
			//--Theme Init--//
			auto& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_Text] = ImVec4(0.860f, 0.930f, 0.890f, 0.78f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.860f, 0.930f, 0.890f, 0.28f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.58f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.9f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.200f, 0.220f, 0.270f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.232f, 0.201f, 0.271f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.502f, 0.075f, 0.256f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.200f, 0.220f, 0.270f, 0.75f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.200f, 0.220f, 0.270f, 0.47f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.200f, 0.220f, 0.270f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.455f, 0.198f, 0.301f, 0.76f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.86f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.502f, 0.075f, 0.256f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.455f, 0.198f, 0.301f, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.860f, 0.930f, 0.890f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.860f, 0.930f, 0.890f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.455f, 0.198f, 0.301f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.455f, 0.198f, 0.301f, 0.43f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.200f, 0.220f, 0.270f, 0.73f);
			style.WindowPadding = ImVec2(6, 4);
			style.WindowRounding = 0.0f;
			style.FramePadding = ImVec2(5, 2);
			style.FrameRounding = 3.0f;
			style.ItemSpacing = ImVec2(7, 1);
			style.ItemInnerSpacing = ImVec2(1, 1);
			style.TouchExtraPadding = ImVec2(0, 0);
			style.IndentSpacing = 6.0f;
			style.ScrollbarSize = 12.0f;
			style.ScrollbarRounding = 16.0f;
			style.GrabMinSize = 20.0f;
			style.GrabRounding = 2.0f;
			style.WindowTitleAlign.x = 0.50f;
			style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
			style.FrameBorderSize = 0.0f;
			style.WindowBorderSize = 1.0f;
		}
		if (ImGui::Button(exec.decrypt()))
		{
			ExecuteScript(Editor.GetText().c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button(clear.decrypt()))
		{
			Editor.SetText("");
		}
		ImGui::SameLine();
		if (ImGui::Button(open.decrypt()))
		{
			wchar_t FileName[MAX_PATH];
			wchar_t FileTitle[MAX_PATH];

			FileName[0] = '\0';
			FileTitle[0] = '\0';

			OPENFILENAMEW Ofn;
			SecureZeroMemory(&Ofn, sizeof Ofn);
			Ofn.lStructSize = sizeof Ofn;
			Ofn.hwndOwner = NULL;
			Ofn.lpstrFilter = L"Script Files (*.lua, *.txt)\0*.lua;*.txt\0";
			Ofn.lpstrTitle = L"Shiny Internal UI - Open File";
			Ofn.lpstrFileTitle = FileTitle;
			Ofn.lpstrFile = FileName;
			Ofn.nMaxFile = MAX_PATH;
			Ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			Ofn.lpstrDefExt = L"lua";

			if (GetOpenFileNameW(&Ofn))
			{
				std::ifstream Stream(Ofn.lpstrFile, std::ios_base::binary);
				std::string Final((std::istreambuf_iterator<char>(Stream)),
					std::istreambuf_iterator<char>());
				Editor.SetText(Final);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(executefil.decrypt())) 
		{
			wchar_t FileName[MAX_PATH];
			wchar_t FileTitle[MAX_PATH];

			FileName[0] = '\0';
			FileTitle[0] = '\0';

			OPENFILENAMEW Ofn;
			SecureZeroMemory(&Ofn, sizeof Ofn);
			Ofn.lStructSize = sizeof Ofn;
			Ofn.hwndOwner = NULL;
			Ofn.lpstrFilter = L"Script Files (*.lua, *.txt)\0*.lua;*.txt\0";
			Ofn.lpstrTitle = L"Shiny Internal UI - Execute File";
			Ofn.lpstrFileTitle = FileTitle;
			Ofn.lpstrFile = FileName;
			Ofn.nMaxFile = MAX_PATH;
			Ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			Ofn.lpstrDefExt = L"lua";

			if (GetOpenFileNameW(&Ofn))
			{
				std::ifstream Stream(Ofn.lpstrFile, std::ios_base::binary);
				std::string Final((std::istreambuf_iterator<char>(Stream)),
					std::istreambuf_iterator<char>());
				ExecuteScript(Final.c_str());
			}
		}
		ImGui::SameLine();
		if (ImGui::Button(save.decrypt()))
		{
			wchar_t FileName[MAX_PATH];
			wchar_t FileTitle[MAX_PATH];

			FileName[0] = L'\0';
			FileTitle[0] = L'\0';

			OPENFILENAMEW Ofn;
			SecureZeroMemory(&Ofn, sizeof Ofn);
			Ofn.lStructSize = sizeof Ofn;
			Ofn.hwndOwner = NULL;
			Ofn.lpstrFilter = L"Script Files (*.lua, *.txt)\0*.lua;*.txt\0";
			Ofn.lpstrTitle = L"Shiny Internal UI - Save File";
			Ofn.lpstrFileTitle = FileTitle;
			Ofn.lpstrFile = FileName;
			Ofn.nMaxFile = MAX_PATH;
			Ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
			Ofn.lpstrDefExt = L"lua";

			if (GetSaveFileNameW(&Ofn))
			{
				std::ofstream Out;
				Out.open(Ofn.lpstrFile, std::ios::out | std::ios::binary);
				auto Contents = Editor.GetText();
				Out.write(Contents.c_str(), Contents.size());
				Out.close();
			}
		}
		Editor.Render("TextEditor");
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return pHookD3D11(pSwapChain, SyncInterval, Flags);
}

const int MultisampleCount = 1; // Set to 1 to disable multisampling
LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }
DWORD __stdcall InitImGui(LPVOID)
{
	IDXGISwapChain* pSwapChain;

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = MultisampleCount;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;

	UINT createFlags = 0;

	IDXGISwapChain* d3dSwapChain = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		requestedLevels,
		sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		&obtainedLevel,
		&pContext)))
	{
		
		return NULL;
	}


	pSwapChainVtable = (DWORD_PTR*)pSwapChain;
	pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

	pHookD3D11 = (D3D11Hook)(DWORD_PTR*)pSwapChainVtable[8];

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)pHookD3D11, (PBYTE)HookD3D11);
	DetourTransactionCommit();

	DWORD dwOld;
	VirtualProtect(pHookD3D11, 2, PAGE_EXECUTE_READWRITE, &dwOld);

	while (true) {
		Sleep(10);
	}

	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

	return NULL;
}