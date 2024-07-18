#include "callcheck.hpp"
#include "../../environment/environment.hpp"
#include <iostream>

int hyperion::bypasses::callcheck::function_handler(std::uintptr_t rl)
{
	auto rstate = state(rl);

	auto closure = rstate.topointer(-hyperion::functions::gettop(rstate.return_state()) - 1);

	return reinterpret_cast<int(__cdecl*)(int a1)>(closure_map[ hyperion::obfuscation::get_closure_name(closure) ])(rstate.return_state()); // this offset doesnt change but the ptr decryption does lel, ican imagine auto update when
}

LONG __stdcall hyperion::bypasses::callcheck::exception_handler(PEXCEPTION_POINTERS exception)
{
	if (exception->ExceptionRecord->ExceptionCode == 0x80000003L)
	{
		if (exception->ContextRecord->Eip == hyperion::offsets::blankfunction_offset)
		{
			exception->ContextRecord->Eip = reinterpret_cast<int>(function_handler);
			//RemoveVectoredExceptionHandler(exception_handler);
		}
		
		if (exception->ContextRecord->Eip == hyperion::offsets::blankfunction2_offset)
		{
			exception->ContextRecord->Eip = reinterpret_cast<int>(hooked_index);
		}

		if (exception->ContextRecord->Eip == hyperion::offsets::blankfunction3_offset)
		{
			exception->ContextRecord->Eip = reinterpret_cast<int>(hooked_namecall);
		}

		if (exception->ContextRecord->Eip == hyperion::offsets::test)
		{
			exception->ContextRecord->Eip = reinterpret_cast<int>(environment::loadstring);
		}
		return -1;
	}
}

void hyperion::bypasses::callcheck::start_callcheck_bypass(state rstate)
{
	AddVectoredExceptionHandler(1, exception_handler);
}

void hyperion::bypasses::callcheck::push_function(state rstate, std::uintptr_t address, int upvals)
{
	hyperion::functions::pushcclosure(rstate.return_state(), hyperion::offsets::blankfunction_offset, ++key, upvals, 0);
	closure_map.insert(std::make_pair(key, address));
}
