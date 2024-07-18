#include <Windows.h>
#include <thread>
#include <format>
#include <array>
#include <vector>
#include <sstream>

#include "structs/structs.hpp"

#include "utilities/scan.hpp"
#include "utilities/io.hpp"
#include "utilities/hook.hpp"

const auto blacklisted_rulesets = std::to_array<std::string>({
	"mod_has_optional_header_magic32",
	"mod_has_optional_header_magic64",
	});

std::vector<std::string> rule_outputs;
void __stdcall sig_hook_stub(std::uintptr_t inst)
{
	const auto matching_string_size = *reinterpret_cast<std::uint32_t*>(inst + 0x8);
	const auto allocation_top = *reinterpret_cast<std::uint32_t*>(inst + 0x12C);

	const auto matched_string = allocation_top - matching_string_size;

	std::string_view str{ reinterpret_cast<const char*>(matched_string), matching_string_size };

	const auto possibly_nonstr = std::any_of(str.begin(), str.end(), [](auto c) {
		return c < ' ' || c > '~' && c != '\n' && c != '\t';
		});

	const auto sig_output = [&str]() -> std::string {
		std::stringstream ss;

		for (auto i = 0u; i < str.size(); i++)
		{
			const auto c = str[i];

			ss << std::format("{:02X}", static_cast<std::uint8_t>(c));

			if (i != str.size() - 1)
				ss << " ";
		}

		return ss.str();
	};

	const auto signature_out = sig_output();
	const auto& rule_set = *reinterpret_cast<std::string*>(0x58 * *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(inst + 4) + 8) + *reinterpret_cast<std::uintptr_t*>(inst + 0x164));

	if (std::find(blacklisted_rulesets.begin(), blacklisted_rulesets.end(), rule_set) != blacklisted_rulesets.end())
		return;

	rule_outputs.push_back(std::format("[RACLV2 : {}] -> Matched sig ({}): {} ({})\n", rule_set, possibly_nonstr ? "signature" : "string", possibly_nonstr ? signature_out : str, possibly_nonstr ? str : signature_out));
}

void __stdcall string_hook_stub(std::uintptr_t inst)
{
	const auto matching_string_size = *reinterpret_cast<std::uint32_t*>(inst + 0x78);
	const auto allocation_top = *reinterpret_cast<std::uint32_t*>(inst + 0x12C);

	const auto matched_string = allocation_top - matching_string_size;

	std::string_view str{ reinterpret_cast<const char*>(matched_string), matching_string_size };

	const auto& rule_set = *reinterpret_cast<std::string*>(0x58 * *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(inst + 4) + 8) + *reinterpret_cast<std::uintptr_t*>(inst + 0x164));

	if (std::find(blacklisted_rulesets.begin(), blacklisted_rulesets.end(), rule_set) != blacklisted_rulesets.end())
		return;

	rule_outputs.push_back(std::format("[RACLV2 : {}] -> Matched string: {}\n", rule_set, str));
}

void __stdcall reporter_hook_stub(std::uintptr_t inst)
{
	const auto rule_set_vector_start = *reinterpret_cast<std::uintptr_t*>(inst + 0x6C);
	const auto rule_set_vector_end = *reinterpret_cast<std::uintptr_t*>(inst + 0x6C + sizeof(std::uintptr_t));

	const auto size = rule_set_vector_end - rule_set_vector_start;

	const auto length = size / sizeof(rule_t);

	const auto dll_info_start = *reinterpret_cast<std::uintptr_t*>(inst + 0x18);

	const auto dll_info = reinterpret_cast<dll_info_t*>(dll_info_start + 0x21C);
	const auto allocation_info = reinterpret_cast<allocation_info_t*>(dll_info_start + 0x234);

	if (dll_info->base)
		std::printf("[RACLV2] -> Analyzing DLL: %ls | %x | %x\n", dll_info->path, dll_info->base, dll_info->size);
	else
		std::printf("[RACLV2] -> Analyzing Allocation: %x | %x\n", allocation_info->base, allocation_info->size);

	std::printf("\tSummary:\n");

	for (auto i = 0u; i < length; ++i)
	{
		const auto rule_struct = reinterpret_cast<rule_t*>(rule_set_vector_start + i * sizeof(rule_t));

		std::printf("\t\t[%s]\n", rule_struct->rule_name.c_str());
	}

	std::printf("\n");

	for (const auto& out : rule_outputs)
		std::cout << '\t' << out;

	std::printf("\n");

	rule_outputs.clear();
}

std::uintptr_t old_sg = 0;
__declspec(naked) void sig_match_stub()
{
	std::uintptr_t mf_ebx;

	__asm
	{
		mov mf_ebx, ebx

		pushad
	}

	if (*reinterpret_cast<std::uint32_t*>(mf_ebx + 0x7C) == 0)
		sig_hook_stub(mf_ebx);
	else
		string_hook_stub(mf_ebx);

	__asm
	{
		popad
		jmp old_sg
	}
}

std::uintptr_t old_ss = 0;
__declspec(naked) void string_match_stub()
{
	std::uintptr_t mf_ebx;

	__asm
	{
		mov mf_ebx, ebx

		pushad
	}

	if (*reinterpret_cast<std::uint32_t*>(mf_ebx + 0x7C) == 0)
		sig_hook_stub(mf_ebx);
	else
		string_hook_stub(mf_ebx);

	__asm
	{
		popad
		jmp old_ss
	}
}

std::uintptr_t old_reporter = 0;
__declspec(naked) void reporter_stub()
{
	std::uintptr_t mf_ecx;

	__asm
	{
		push ecx
		mov ecx, [ebp + 0x30]
		mov mf_ecx, ecx
		pop ecx

		pushad
	}

	reporter_hook_stub(mf_ecx);

	__asm
	{
		popad
		jmp old_reporter
	}
}

void entry()
{
	utilities::io::initiate("RACL V2 - gogo1000, 0x90");

	if (const auto ac = find_matching_funcs())
	{
		std::printf
		(
			"[RACLV2] -> match_stub #1: 0x%X\n"
			"[RACLV2] -> match_stub #2: 0x%X\n", 

			ac->front(), ac->back()
		);

		old_sg = tramp_hook(ac->front() + 0x3, reinterpret_cast<std::uintptr_t>(&sig_match_stub), 6);
		old_ss = tramp_hook(ac->back() + 0x3, reinterpret_cast<std::uintptr_t>(&string_match_stub), 6);
	}

	if (const auto ac = find_scan_reporter())
	{
		std::printf("[RACLV2] -> scan_module_for_known_signatures: 0x%X\n\n", ac);

		old_reporter = tramp_hook(ac, reinterpret_cast<std::uintptr_t>(&reporter_stub), 7);

		// lazy fix:
		std::uint8_t jump[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
		*reinterpret_cast<std::uintptr_t*>(jump + 1) = ac + 0x18;

		std::memcpy(reinterpret_cast<void*>(old_reporter + 0x18), jump, sizeof(jump));
	}
}

bool __stdcall DllMain(void*, DWORD reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH)
		std::thread{ entry }.detach();

	return true;
}