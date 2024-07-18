#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <optional>
#include <sstream>
#include <fstream>

extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/luaconf.h"
#include "Lua/lopcodes.h"
#include "Lua/lstate.h"
#include "Lua/lobject.h"
#include "Lua/lauxlib.h"
}

namespace script_ware
{
	class WriteUtilities
	{
	public:
		static void writeByte(std::stringstream& ss, unsigned char value)
		{
			ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		static void writeInt(std::stringstream& ss, int value)
		{
			ss.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
		static const void writeCompressedInt(std::stringstream& ss, unsigned int value)
		{
			do
			{
				int v = value & 0x7F;
				value >>= 7;

				if (value)
					v |= 0x80;

				writeByte(ss, v);
			} while (value);
		}
	};
}

script_ware::WriteUtilities stream_write;