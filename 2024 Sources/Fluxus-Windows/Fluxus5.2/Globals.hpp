#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem> //cpp 17 superior filesystem wrapper
#include <mutex> //call_once & once_flag
#include <iterator>
#include <unordered_map>

/*Library Includes*/

#include "Libs/cURL/curl.h"
#include "Libs/cpr/cpr.h"
#include "Libs/plog/Init.h"
#include "Libs/plog/Log.h"
#include "Libs/plog/Initializers/RollingFileInitializer.h"
#include "Libs/VMProtect/VMProtectSDK32.h"

extern "C" {
#include "Libs/Lua/lua.h"
#include "Libs/Lua/ldo.h"
#include "Libs/Lua/lapi.h"
#include "Libs/Lua/lualib.h"
#include "Libs/Lua/lstate.h"
#include "Libs/Lua/lauxlib.h"
#include "Libs/Lua/luaconf.h"
#include "Libs/Lua/llimits.h"
#include "Libs/Lua/lapi.h"
#include "Libs/Lua/lfunc.h"
#include "Libs/Lua/lopcodes.h"
#include "Libs/Lua/lobject.h"
#include "Libs/Lua/lundump.h"
}

namespace Fluxus {
	namespace Globals { //don't need a class for this...
		static HMODULE DLLModule = 0;
		static bool ElegibleForSupport = true; //if the log service failed to initiate this'll be set to false so the log uploader won't attempt to upload a crash log...
		static bool Is64Bit = true; //assume it is
		static std::string LocalPath = "";
		static std::string RobloxPath = "";
		static std::string LogsPath = "";
		static std::once_flag MainInitFlag; //call once for main init

		HWND RobloxHWND();
		void SetWorkspacePath(std::string);
		std::string GetWorkspacePath();
		void SetAutoExec(std::string);
		std::string GetAutoExec();

		std::ostream& yellow(std::ostream& s);
		std::ostream& red(std::ostream& s);
		std::ostream& white(std::ostream& s);
		std::ostream& blue(std::ostream& s);
		std::ostream& green(std::ostream& s);
		std::ostream& purple(std::ostream& s);
		std::ostream& grey(std::ostream& s);
		std::ostream& pink(std::ostream& s);
		std::ostream& etc(std::ostream& s);
	}
}
