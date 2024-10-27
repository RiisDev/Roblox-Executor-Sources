#include <Windows.h>
#include <vector>
#include <string>
#include <functional>
#include <winnt.h>
#include <iostream>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <thread>
#include <fstream>
#include <filesystem>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <chrono>
#include <string>
#include <regex>
#include <cmath>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <d3d9.h>
#include "..//ImGui/imgui.h"
#include "..//ImGUI/imgui_impl_dx11.h"
#include "..//ImGUI/imgui_impl_win32.h"
#include "..//ImGUI/imgui_impl_dx9.h"
#include "..//..//RBX.hpp"
#include "..//MinHook/MinHook.hpp"
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "libMinHook.x86.lib")

namespace Fluxus {
	namespace Drawing {
		void HookRenderer();
		void RegisterDrawing(DWORD rL);
		HWND GetRobloxHWND();
	}
}