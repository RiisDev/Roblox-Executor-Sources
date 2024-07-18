#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"
#include "json/json.hpp"

#include <Windows.h>
#include <thread>

#include "roblox/roblox.hpp"

void open_console()
{
	const auto lib = LoadLibraryA("KERNEL32.dll");

	if (!lib)
		return;

	const auto free_console = reinterpret_cast<std::uintptr_t>(GetProcAddress(lib, "FreeConsole"));

	if (free_console)
	{
		static std::uintptr_t jmp = free_console + 0x6;

		DWORD old{};

		VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, PAGE_EXECUTE_READWRITE, &old);

		*reinterpret_cast<std::uintptr_t**>(free_console + 0x2) = &jmp;
		*reinterpret_cast<std::uint8_t*>(free_console + 0x6) = 0xC3;

		VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, old, &old);
	}

	AllocConsole();

	FILE* file_stream;

	freopen_s(&file_stream, "CONIN$", "r", stdin);
	freopen_s(&file_stream, "CONOUT$", "w", stdout);
	freopen_s(&file_stream, "CONOUT$", "w", stderr);

	fclose(file_stream);

	SetConsoleTitleA("GRU");
}

void d_main()
{
	open_console();

	sockpp::tcp_connector conn;

	const auto status = conn.connect(sockpp::inet_address("localhost", 8124));

	if (!status)
	{
		std::printf("Failed to connect\n");

		return;
	}

	while (conn.is_open())
	{
		char buf[100]{};
		conn.read(buf, sizeof(buf));

		const auto json = nlohmann::json::parse(buf);

		const auto evnt = json["event"].get<std::string>();

		nlohmann::json j;

		j["data"] = {};

		if (evnt == "updatescripts")
		{
			j["event"] = "scripts";

			const auto running_scripts = roblox::task_scheduler_t::get()->get_script_context()->get_running_scripts();

			for (const auto local_script : running_scripts)
				j["data"].push_back(local_script->name);

			const auto data = j.dump();

			conn.write_n(data.c_str(), data.size());
		}

		else if (evnt == "getscript")
		{
			j["event"] = "scriptdata";

			const auto running_scripts = roblox::task_scheduler_t::get()->get_script_context()->get_running_scripts();

			const auto script_index = json["data"].get<std::uint32_t>();

			const auto bytecode = running_scripts[script_index]->script.decompress_bytecode();

			for (const auto byte : bytecode)
				j["data"].push_back(byte);

			const auto data = j.dump();

			conn.write_n(data.c_str(), data.size());
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		std::thread{ d_main }.detach();

	return TRUE;
}