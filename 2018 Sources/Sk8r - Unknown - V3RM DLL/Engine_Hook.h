#ifndef __HOOKS
#define __HOOKS
#pragma once
#include <Windows.h>
#include <fstream>
#include <d3d11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

#include "FW1FontWrapper\FW1FontWrapper.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "util.h"
#include "MinHook\include\MinHook.h"

ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView;
IDXGISwapChain* pSwapChain;

typedef HRESULT(__stdcall *present_hook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

present_hook hooks_present;

DWORD_PTR* pSwapChain_VTable;
DWORD_PTR* pDevice_VTable;
DWORD_PTR* pContext_VTable;

UINT pssrStartSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  Descr;
ID3D11ShaderResourceView* ShaderResourceView;

IFW1Factory* pFW1Factory;
IFW1FontWrapper* pFontWrapper;

long __RGBAL(int iR, int iG, int iB, int iA) {

	return ((iA * 256 + iR) * 256 + iG) * 256 + iB;
}

#define RGB_TD(iR, iG, iB, iA) (__RGBAL(iR, iG, iB, iA))

#define RGB_TD(iR, iG, iB) (__RGBAL(iR, iG, iB, 255))

LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

/* UI */

ImGuiWindowFlags window_flags;

WNDPROC oldproc;
bool show_ui = false;
bool resized = false;

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX11_InvalidateDeviceObjects();
			RenderTargetView->Release();
			pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			resized = true;
			ImGui_ImplDX11_CreateDeviceObjects();
			return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
		}
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	if (show_ui) {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	else {
		::SetCursor(NULL);
		return CallWindowProc(oldproc, hwnd, msg, wParam, lParam);
	}
}

/* -- */

bool first_time = true;
char script[999999];

int tabindex = 0;

HRESULT __stdcall fhooks_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (first_time) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice))) {
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
			printf("<< SUCCESS >> [D3D Hook]: Device hooked succesfully.\n");
		}
		else {
			printf("<< FAILURE >> [D3D Hook]: Device hook failed.\n");
		}

		HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
		hResult = pFW1Factory->CreateFontWrapper(pDevice, L"Tahoma", &pFontWrapper);
		pFW1Factory->Release();

		printf("<< EVENT >> [FW1FontWrapper]: Fully loaded.\n");

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture))) {
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
		}

		ImGui_ImplDX11_Init(FindWindowA(NULL, "Roblox"), pDevice, pContext);
		ImGui_ImplDX11_CreateDeviceObjects();

		first_time = false;
	}

	if (resized) {
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture))) {
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
			printf("<< EVENT >> [Engine Hook]: RenderTargetView reinitialized successfully after resize.\n");
			resized = false;
		}
	}

	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	pFontWrapper->DrawString(pContext, L"Syndec v1.0.0 | By Cyberhound#5672", 14, 150.0f, 8.0f, RGB_TD(50, 50, 50, 255), FW1_RESTORESTATE);

	if ((GetAsyncKeyState(VK_F1) & 1))
		show_ui = !show_ui;

	if (show_ui) {
		ImGui_ImplDX11_NewFrame();

		ImGui::Begin("Syndec v1.0.0", 0, window_flags);
		ImGui::SetWindowSize(ImVec2(545, 351));
		ImGui::BeginChildFrame(1, ImVec2(83, 296));
		{
			if (ImGui::Button("Execution", ImVec2(75, 45))) { tabindex = 0; }
			if (ImGui::Button("", ImVec2(75, 45))) { tabindex = 1; }
			if (ImGui::Button("", ImVec2(75, 45))) { tabindex = 2; }
			if (ImGui::Button("", ImVec2(75, 45))) { tabindex = 3; }
			if (ImGui::Button("", ImVec2(75, 45))) { tabindex = 4; }
			if (ImGui::Button("", ImVec2(75, 45))) { tabindex = 5; }
		}
		ImGui::EndChildFrame();
		ImGui::SameLine();
		ImGui::BeginChildFrame(2, ImVec2(436, 296));
		{
			if (tabindex == 0) {
				ImGui::InputTextMultiline("input text", script, IM_ARRAYSIZE(script), ImVec2(431, 266));
				if (ImGui::Button("Execute", ImVec2(102, 20))) {

				}
				ImGui::SameLine();
				if (ImGui::Button("Open", ImVec2(92, 20))) {
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

					if (GetOpenFileNameA(&ofn)) {
						std::string buffer = util::read_file(file_name);
						strncpy(script, buffer.c_str(), sizeof(script));
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Save", ImVec2(92, 20))) {
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
						std::string buffer = script;
						util::write_file(file_name, buffer);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Clear", ImVec2(102, 20))) {
					strncpy(script, "", sizeof(script));
				}
			}
			if (tabindex == 1) {

			}
			if (tabindex == 2) {

			}
			if (tabindex == 3) {

			}
			if (tabindex == 4) {

			}
		}
		ImGui::EndChildFrame();
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	ImGui::GetIO().MouseDrawCursor = show_ui;

	return hooks_present(pSwapChain, SyncInterval, Flags);
}

const int MultisampleCount = 1;

namespace Hook {
	void Init() {
		util::thread([](void*)->DWORD {
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

			if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pContext))) {
				printf("<< FAILURE >> [Engine Hook]: Failed to create device and swap chain.\n");
				return NULL;
			}
			else {
				printf("<< SUCCESS >> [Engine Hook]: Created device and swapped chain successfully.\n");
			}

			oldproc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "Roblox"), GWL_WNDPROC, (LONG)hWndProc);

			ImGui::CreateContext();

			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;

			window_flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

			style->WindowRounding = 0;
			style->WindowTitleAlign = ImVec2(0.01, 0.5);
			style->Alpha = .8;
			style->GrabRounding = 1;
			style->GrabMinSize = 20;
			style->FrameRounding = 0;

			colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
			colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.9f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.18f, 0.21f, 0.9f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
			colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
			colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
			colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
			colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
			colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
			colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);

			pSwapChain_VTable = (DWORD_PTR*)pSwapChain;
			pSwapChain_VTable = (DWORD_PTR*)pSwapChain_VTable[0];

			pContext_VTable = (DWORD_PTR*)pContext;
			pContext_VTable = (DWORD_PTR*)pContext_VTable[0];

			pDevice_VTable = (DWORD_PTR*)pDevice;
			pDevice_VTable = (DWORD_PTR*)pDevice_VTable[0];

			if (MH_Initialize() != MH_OK) { return 0; }

			//pSwapChain_VTable[8] : present
			if (MH_CreateHook((DWORD_PTR*)pSwapChain_VTable[8], fhooks_present, reinterpret_cast<void**>(&hooks_present)) != MH_OK) { printf("<< FAILURE >> [Engine Hook]: Present hook creation failed.\n"); return 0; }
			if (MH_EnableHook((DWORD_PTR*)pSwapChain_VTable[8]) != MH_OK) { printf("<< FAILURE >> [Engine Hook]: Present hook failed.\n"); return 0; }
			else { printf("<< EVENT >> [Engine Hook]: Present hook created successfully.\n"); }

			while (true) {
				Sleep(50);
			}

			pDevice->Release();
			pContext->Release();
			pSwapChain->Release();
			return 0;
		});
	}
}
#endif