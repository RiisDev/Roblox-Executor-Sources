#include "CallCheck.hpp"

int callcheck_jumpback(DWORD Rl);

DWORD jmpbak;
DWORD jmp_back;

DWORD ArCallCheck;

__declspec(naked) void ActualHook()
{
	__asm {
		mov eax, [esp]
		cmp eax, ArCallCheck
		jne normal_case
		add esp, 4
		push[esp + 4]
		call[jmpbak]
		add esp, 4
		retn
		normal_case :
		jmp[jmp_back]
	}
}

void SetAddress()
{
	ArCallCheck = Fluxus::ReturnCallCheck() + 6;
}

void BypassCallCheck()
{
	const auto kernel32 = GetModuleHandleA("kernel32.dll");
	SetAddress();

	if (kernel32) {
		const auto A_CreateEventA = GetProcAddress(kernel32, "CreateEventA");
		jmpbak = reinterpret_cast<DWORD>(&callcheck_jumpback);

		typedef void(WINAPI* _CreateEventA)(LPSECURITY_ATTRIBUTES lp_event_attributes, BOOL b_banual_reset, BOOL b_initial_state, LPCTSTR lp_name);
		_CreateEventA H_CreateEventA = NULL;

		MH_Initialize();
		MH_CreateHook(reinterpret_cast<LPVOID>(A_CreateEventA), &ActualHook, reinterpret_cast<LPVOID*>(&H_CreateEventA));
		MH_EnableHook(reinterpret_cast<LPVOID>(A_CreateEventA));

		jmp_back = (DWORD)H_CreateEventA;
	}
	else {
		PLOG_ERROR << "Failed to find kernel32.dll (How does your computer even run)";
		std::cout << "Failed to find kernel32.dll [FATAL]\r\n";
	}
}

int callcheck_jumpback(DWORD rL)
{
	Fluxus::RBX RBX(rL);
	auto upvals = 0;

	for (auto i = 1; i < MAX_INT; i++) {
		if (RBX.Type(lua_upvalueindex(i)) != -1) {
			++upvals;
		}
		else {
			break;
		}
	}

	int CallBack = RBX.ToNumber(lua_upvalueindex(upvals), 0);
	return ((int(*)(DWORD))CallBack)(rL);
}