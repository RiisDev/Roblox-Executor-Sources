#pragma once
#ifndef COMMONS_H
#define COMMONS_H

#define SECURITY_WIN32 

#include <Windows.h>
#include <stdint.h>
#include <mutex>
#include <iostream>
#include <vector>
#include <exception>
#include <memory>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <Psapi.h>
#include <cstdint>

#define _XM_NO_INTRINSICS_
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <imgui/imgui.h>
#include <imgui/impl/imgui_impl_dx11.h>
#include <imgui/impl/imgui_impl_win32.h>

#include "Memory.h"
#include <discord_rpc.h>



#endif // COMMONS_H