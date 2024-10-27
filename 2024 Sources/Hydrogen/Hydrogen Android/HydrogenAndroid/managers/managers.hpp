#pragma once

#include "hook/hook.hpp"
#include "utility/utility.hpp"
#include "jni/jni.hpp"
#include "memory/memory.hpp"
#include "roblox/roblox.hpp"
#include "function/function.hpp"
#include "renderer/renderer.hpp"
#include "crash/crash.hpp"
#include "drawing/drawing.hpp"
#include "menu/menu.hpp"

enum class status_t
{
	skipped,
	failed,
	success
};

namespace managers
{
	inline status_t preauthed = status_t::skipped;

	void initiate();
}