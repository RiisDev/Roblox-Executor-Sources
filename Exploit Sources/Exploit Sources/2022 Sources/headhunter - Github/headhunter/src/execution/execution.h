#pragma once
#include <Windows.h>
#include <string>
#include <stack>

#include <Luau/Compiler.h>
#include <Luau/BytecodeBuilder.h>

#include "../addresses/addresses.h"
#include "../scheduler/scheduler.h"
#include "../console/console.h"

class execution_t
{
private:
	const scheduler_t* scheduler;
public:
	execution_t(const scheduler_t* scheduler);

	void hook_waiting_scripts_job() const;
	void run_script(const std::string& script) const;
	void set_identity(std::int8_t identity) const;

	friend int __fastcall scheduler_cycle(std::uintptr_t waiting_scripts_job, int fakearg, int a2);
};


extern const execution_t execution;