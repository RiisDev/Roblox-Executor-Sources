#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <detours/detours.h>
#include "imgui/custom/custom.h"

#ifdef _WIN64
#pragma comment(lib,"detours64")
#else
#pragma comment(lib,"detours")
#endif

struct input_t
{
	char key_pressed[256] = {};
} inline Input;

using present_fn = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain,
    UINT SyncInterval, UINT Flags);

class Renderer {
public:
    bool w2s(vec3 origin, vec2& screen);
    bool w2s(vec3 origin, vec2& screen, ViewMatrix_t matrix);

public:
    /* D3D11 */
    ID3D11Device* p_device;
    ID3D11DeviceContext* p_context;
    ID3D11Texture2D* p_render_target_texture;
    ID3D11RenderTargetView* p_render_target_view;

    uintptr_t vt;
    present_fn o_present;
    LONG o_wndproc;
    HWND hWnd;

    unsigned int s_w, s_h;
	int logo_w, logo_h;
	ID3D11ShaderResourceView* logo_texture;

public:
    union {
        bool active = true;
    } gui;
public:
    bool initialized = false;

    void initialize();
    void terminate();
	void labelpart(ImDrawList* list, std::shared_ptr<RBXInstance> part, int offset);
    void render();
	void handle_input(UINT msg, WPARAM w_param, LPARAM l_param);
};

extern Renderer renderer;

#endif // RENDERER_H
