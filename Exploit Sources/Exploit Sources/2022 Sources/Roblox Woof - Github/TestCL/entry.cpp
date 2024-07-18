#include <iostream>
#include <thread>

#include <Windows.h>
#include <winternl.h>

#include "utils.hpp"

// name of the module to abuse, must match name of the built proxy DLL
#define DLL_NAME "XInput1_4.dll"

// gets the WOW64 directory
std::wstring get_sys_dir()
{
	wchar_t buffer[MAX_PATH]{};

	const auto sz = GetSystemWow64Directory(buffer, MAX_PATH);

	return !sz ? L"" : std::wstring{ buffer, sz };
}

// writes an error to a message box then closes the program
void error(std::string&& msg)
{
	MessageBoxA(nullptr, (msg + "\nPlease disable auto launch.").c_str(), "Auto Launch", MB_OK);

	std::exit(0);
}

// debug logs a series of arguments and newlines after
template <typename... args_t>
void dbg(args_t... args)
{
#ifdef _DEBUG
	((std::cout << args), ...) << '\n';
#endif
}

template <typename to_t>
std::uintptr_t calculate_rel(const std::uintptr_t ptr, const std::uint8_t sz, const std::uint8_t offset)
{
	return ptr + *reinterpret_cast<to_t*>(ptr + offset) + sz;
}

// gets the entry and data table pointer in PEB from a module's handle
std::pair<LIST_ENTRY*, LDR_DATA_TABLE_ENTRY*> get_by_module(const void* const mod)
{
	const auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	const auto ldr = peb->Ldr;

	PLIST_ENTRY start = &ldr->InMemoryOrderModuleList, current = start;

	// iterate the list
	while ((current = current->Flink) != start)
	{
		const auto data = CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (!data || !data->DllBase)
			continue;

		if (data->DllBase == mod)
			return { current, data };
	}

	return {};
}

bool __stdcall DllMain(const void* const mod, const std::uint32_t reason, void*)
{
	using namespace std::chrono_literals;

	if (reason == DLL_PROCESS_ATTACH)
	{
	#ifdef _DEBUG
		// make freeconsole fail
		DWORD old{};

		static const auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
		static auto ptr_jmp = ptr + 0x6;

		VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);
		*reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
		*reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

		VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);

		FILE* file_stream{};

		AllocConsole();
		freopen_s(&file_stream, "CONIN$", "r", stdin);
		freopen_s(&file_stream, "CONOUT$", "w", stdout);
		freopen_s(&file_stream, "CONOUT$", "w", stderr);
	#endif

		dbg("gogo + iivillian + 0x90 proxy module loaded");

		// get PEB link to proxy module
		const auto [entry, link] = get_by_module(mod);
		if (!link)
			return error("can't acquire PEB link for proxy"), false;

		dbg("link: ", link, '\n', "checksum: ", link->CheckSum);

		// load genuine module
		const auto directory = get_sys_dir() + L"\\" + TEXT(DLL_NAME);
		const auto real_module = LoadLibraryW(directory.c_str());

		if (!real_module)
			return error("could not load real module"), false;

		dbg("loaded genuine module: ", real_module, ", proxy: ", mod);

		// get headers of genuine module
		const auto [_, real_link] = get_by_module(real_module);
		const auto dos_real = reinterpret_cast<PIMAGE_DOS_HEADER>(real_module);
		const auto nt_real = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(real_module) + dos_real->e_lfanew);

		dbg("preparing export rip");
		
		// set privellege so we can write to the headers
		LUID dbg_id{};
		TOKEN_PRIVILEGES privs{};
		TOKEN_PRIVILEGES prev{};
		DWORD cb{};
		HANDLE token{};

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token) && LookupPrivilegeValue(L"", SE_DEBUG_NAME, &dbg_id))
		{
			privs.PrivilegeCount = 1;
			privs.Privileges[0].Luid = dbg_id;
			privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (AdjustTokenPrivileges(token, FALSE, &privs, sizeof(TOKEN_PRIVILEGES), &prev, &cb) != ERROR_SUCCESS)
				return error("couldn't adjust privileges to write"), false;
		}
		else
			return error("couldn't lookup access privilege"), false;

		// set proxy headers to genuine module headers
		DWORD protection{};
		VirtualProtect(link->DllBase, nt_real->FileHeader.SizeOfOptionalHeader, PAGE_EXECUTE_READWRITE, &protection);
		std::memcpy(link->DllBase, real_module, nt_real->FileHeader.SizeOfOptionalHeader);
		VirtualProtect(link->DllBase, nt_real->FileHeader.SizeOfOptionalHeader, protection, &protection);

		// unlink the existing module from the list
		entry->Flink->Blink = entry->Blink;
		entry->Blink->Flink = entry->Flink;

		// reset so any existing connections to the link and its base point back to the genuine module
		link->DllBase = real_link->DllBase;
		link->FullDllName = {};

		// remove privellege escalation and close handle
		privs.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;
		if (AdjustTokenPrivileges(token, FALSE, &privs, sizeof(TOKEN_PRIVILEGES), &prev, &cb) != ERROR_SUCCESS)
			return error("couldn't reset privileges"), false;

		CloseHandle(token);

		dbg("woofing...");

		// set static tls flag for the function that query datas Roblox use. This way function the will not call and data will not be set

		const auto scan = hwid::utils::pattern_scan("\xE8\x00\x00\x00\x00\x83\x78\x10\x00\x75\x41", "x????xxxxxx"); //11
		if (scan.empty())
			return error("could not automatically resolve initialize function, sorry skid"), false;

		const auto& caller = scan[0];
		
		const auto function = calculate_rel<std::uint32_t>(caller, 5, 1);
		dbg("resolver: ", function);

		// offset 0x22 (mov instruction for flag + 1) may change, if it does too bad kid learn how to reverse 
		const auto flag = *reinterpret_cast<std::uintptr_t*>(function + 0x22);
		dbg("flag: ", flag);

		dbg("previous value: ", *reinterpret_cast<std::uint32_t*>(flag));
		dbg("writing backup flag");

		*reinterpret_cast<std::uint32_t*>(flag) = 0xEAC; // set this to any funny number above 0 that you like

		dbg("auto launch 2 godly./...");
	}

	return true;
}