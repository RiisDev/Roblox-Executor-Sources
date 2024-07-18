#pragma once

/*Includes*/
#include "clvm.h"
#include <Windows.h>
#include <fstream>
#include <d3d11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

#include "..\TestLuaU\TestFolder\FW1FontWrapper\FW1FontWrapper.h"
#include "..\TestLuaU\TestFolder\ImGUI\imgui.h"
#include "..\TestLuaU\TestFolder\ImGUI\imgui_impl_dx11.h"
#include "..\TestLuaU\TestFolder\MinHook\MinHook.h"
#include "../TestLuaU/TestFolder/TextEditor.h"





/*Defines*/

ID3D11Device* D3D11Device;
ID3D11DeviceContext* D3D11Context;
ID3D11Texture2D* D3D11TextureTarget;
ID3D11RenderTargetView* D3D11RenderView;
IDXGISwapChain* D3D11SwapChain;


typedef HRESULT(__stdcall* HookPresent) (IDXGISwapChain* D3D11SwapChain, UINT SyncInterval, UINT Flags);
HookPresent PresentD3DHooks;


DWORD_PTR* D3D11SwapChain_VTABLE;
DWORD_PTR* D3D11Device_VTABLE;
DWORD_PTR* D3D11Context_VTABLE;

UINT StartingSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  D3D11ViewDescription;
ID3D11ShaderResourceView* D3D11ShaderResourceView;

IFW1Factory* AeoX_FW1FontFactory;
IFW1FontWrapper* AeoX_FW1FontWrapper;

/*Misc Functions*/

long ReturnRGBValue(int iR, int iG, int iB, int iA) {

	return ((iA * 256 + iR) * 256 + iG) * 256 + iB;
}

#define RGB_GET(iR, iG, iB, iA) (ReturnRGBValue(iR, iG, iB, iA))
#define RGB_GET(iR, iG, iB) (ReturnRGBValue(iR, iG, iB, 255))


/*CallBacks*/

LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

/*UI Definitions*/

ImGuiWindowFlags InternalWindowFlags;

WNDPROC OldWindowProc;
bool UICurrentlyShown = false;
bool IsBeingResized = false;

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


/*Window Proc Handler*/

LRESULT CALLBACK InternalWindowProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return true;
	}

	switch (msg) {
	case WM_SIZE: {
		if (wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX11_InvalidateDeviceObjects();
			D3D11RenderView->Release();
			D3D11SwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			IsBeingResized = true;
			ImGui_ImplDX11_CreateDeviceObjects();
			return CallWindowProc(OldWindowProc, hwnd, msg, wParam, lParam);
		}
	}
	case WM_SYSCOMMAND: {
		if ((wParam & 0xfff0) == SC_KEYMENU) {
			return 0;
		}
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}

	if (UICurrentlyShown) {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	else {
		SetCursor(NULL);
		return CallWindowProc(OldWindowProc, hwnd, msg, wParam, lParam);
	}
}

/*UI*/

bool FirstInit = true;

TextEditor Editor;

HRESULT CALLBACK OnHookPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (FirstInit) {

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& D3D11Device))) {
			pSwapChain->GetDevice(__uuidof(D3D11Device), (void**)& D3D11Device);
			D3D11Device->GetImmediateContext(&D3D11Context);
		}
		else
		{

			throw std::exception("Internal UI Init Failure!");
		}

		HRESULT HResult = FW1CreateFactory(FW1_VERSION, &AeoX_FW1FontFactory);
		HResult = AeoX_FW1FontFactory->CreateFontWrapper(D3D11Device, L"Arial", &AeoX_FW1FontWrapper);
		AeoX_FW1FontFactory->Release();

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& D3D11TextureTarget))) {
			D3D11Device->CreateRenderTargetView(D3D11TextureTarget, NULL, &D3D11RenderView);
			D3D11TextureTarget->Release();
		}
		else
		{

			throw std::exception("Internal UI Init Failure!");
		}

		ImGui_ImplDX11_Init(FindWindowA(NULL, "Roblox"), D3D11Device, D3D11Context); //Modified ImGUI Implementation for Roblox (Retards Make Bypassing FindWindowA Easy As Fuck)
		ImGui_ImplDX11_CreateDeviceObjects();

		FirstInit = false;
	}


	if (IsBeingResized) {
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& D3D11TextureTarget))) {
			D3D11Device->CreateRenderTargetView(D3D11TextureTarget, NULL, &D3D11RenderView);
			D3D11TextureTarget->Release();

			IsBeingResized = false;
		}
	}



	D3D11Context->OMSetRenderTargets(1, &D3D11RenderView, NULL);
	
	if ((GetAsyncKeyState(VK_INSERT) & 1)) {
		UICurrentlyShown = !UICurrentlyShown;
	}


	if (UICurrentlyShown) {
		//Begin Actual ImGUI UI

		ImGui_ImplDX11_NewFrame();

		ImGui::Begin("Netex - LuaU Project", 0, InternalWindowFlags);
		//ImGui::SetWindowSize(ImVec2(545, 351));

		Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
		Editor.Render("Script Editor", ImVec2(580, 265));
		if (ImGui::Button("Execute", ImVec2(70, 20))) {
			lua_State* L = lua_open();
			RunScript(RL, L, Editor.GetText());

		}
		ImGui::SameLine(60, 60);
		if (ImGui::Button("Clear", ImVec2(70, 20))) {
			Editor.SetText("");
		}
		ImGui::SameLine(180, 60);
		if (ImGui::Button("Open", ImVec2(70, 20))) {
			char file_name[MAX_PATH];

			OPENFILENAMEA ofn;
			ZeroMemory(&file_name, sizeof(file_name));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "Script (*.txt;*.lua)|*.txt;*.lua|All files (*.*)|*.*";
			ofn.lpstrFile = file_name;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			std::string Buffer;

			if (GetOpenFileNameA(&ofn)) {
				//Buffer = Files::ReadFile(file_name);
			}

			Editor.SetText(Buffer);
		}

		ImGui::SameLine(300, 60);
		if (ImGui::Button("Save", ImVec2(70, 20))) {
			char file_name[MAX_PATH];

			OPENFILENAMEA ofn;
			ZeroMemory(&file_name, sizeof(file_name));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "Script (*.txt;*.lua)|*.txt;*.lua|All files (*.*)|*.*";
			ofn.lpstrFile = file_name;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Save File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetSaveFileNameA(&ofn)) {
				std::string buffer = Editor.GetText();
				//Files::WriteFile(file_name, buffer);
			}
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}


	ImGui::GetIO().MouseDrawCursor = UICurrentlyShown;

	return PresentD3DHooks(pSwapChain, SyncInterval, Flags);
}


const int MultiSampleCount = 1;


namespace ImGUI {

	int Begin() {

		//Sloppeys FindWindowA Bypass
		DWORD FindWindowAProtect;
		VirtualProtect((LPVOID)& FindWindowA, 1, PAGE_EXECUTE_READWRITE, &FindWindowAProtect);
		*(char*)& FindWindowA = 0x90;
		VirtualProtect((LPVOID)& FindWindowA, 1, FindWindowAProtect, &FindWindowAProtect);


		HMODULE hDXGIDLL = 0;
		do {
			hDXGIDLL = GetModuleHandleA("dxgi.dll");
			Sleep(500);
		} while (!hDXGIDLL);

		WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
		RegisterClassExA(&wc);
		HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

		D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
		D3D_FEATURE_LEVEL obtainedLevel;
		ID3D11Device* d3dDevice = nullptr;
		ID3D11DeviceContext* d3dContext = nullptr;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };


		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &D3D11SwapChain, &D3D11Device, &featureLevel, &D3D11Context))) {
			//		CrashBoxText = "Internal UI Init Failed: D3D Device Creation And Chain Swap Failed!";
			throw std::exception("Internal UI Init Failure!");
		}


		OldWindowProc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "Roblox"), GWL_WNDPROC, (LONG)InternalWindowProcHandler);

		ImGui::CreateContext();

		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;


		InternalWindowFlags |= ImGuiWindowFlags_NoResize;

		style->WindowRounding = 0;
		style->WindowTitleAlign = ImVec2(0.01, 0.5);
		//style->Alpha = 0.9;
		style->GrabRounding = 1;
		style->GrabMinSize = 20;
		style->FrameRounding = 0;


		D3D11SwapChain_VTABLE = (DWORD_PTR*)D3D11SwapChain;
		D3D11SwapChain_VTABLE = (DWORD_PTR*)D3D11SwapChain_VTABLE[0];

		D3D11Context_VTABLE = (DWORD_PTR*)D3D11Context;
		D3D11Context_VTABLE = (DWORD_PTR*)D3D11Context_VTABLE[0];

		D3D11Device_VTABLE = (DWORD_PTR*)D3D11Device;
		D3D11Device_VTABLE = (DWORD_PTR*)D3D11Device_VTABLE[0];


		if (MH_Initialize() != MH_OK) {
			//	CrashBoxText = "Internal UI Init Failed: MinHook Init Failed!";
			throw std::exception("Internal UI Init Failure!");
		}

		if (MH_CreateHook((DWORD_PTR*)D3D11SwapChain_VTABLE[8], OnHookPresent, reinterpret_cast<void**>(&PresentD3DHooks)) != MH_OK) {
			//CrashBoxText = "Internal UI Init Failed: Hook Creation Failed!";
			throw std::exception("Internal UI Init Failure!");
		}


		if (MH_EnableHook((DWORD_PTR*)D3D11SwapChain_VTABLE[8]) != MH_OK) {
			//	CrashBoxText = "Internal UI Init Failed: Hook Enable Failed!";
			throw std::exception("Internal UI Init Failure!");
		}

		//D3D11Device->Release();
		//D3D11Context->Release();
		//D3D11SwapChain->Release();
		return 0;
	}
}