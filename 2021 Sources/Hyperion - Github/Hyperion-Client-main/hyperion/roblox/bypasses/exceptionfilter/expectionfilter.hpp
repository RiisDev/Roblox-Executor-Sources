#pragma once
#include <Windows.h>

namespace hyperion::bypasses::suef
{
	void terminate_suef_hook();
	long WINAPI top_level_exception_handler(PEXCEPTION_POINTERS pExceptionInfo);
}