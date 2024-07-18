#include "execution.h"
#include <stack>
#include <mutex>

std::mutex mutex;
std::stack<std::string> script_queue;
std::uintptr_t original_func;

struct roblox_encoder_t : public Luau::BytecodeEncoder
{
	std::uint8_t encodeOp(const std::uint8_t opcode) override
	{
		return opcode * 227;
	}
};

int __fastcall scheduler_cycle(std::uintptr_t waiting_scripts_job, int fakearg, int a2)
{
	std::unique_lock<std::mutex> guard{ mutex };

	if (!script_queue.empty())
	{
		output << console::color::pink << "scheduler stepped!\n";
		const std::string bytecode = script_queue.top();
		script_queue.pop();
		guard.unlock();

		if (bytecode.at(0) == 0)
		{
			std::string error("[string \"headhunter.exe\"]");
			const char* msg = bytecode.c_str() + 1;
			rbx_output(1, (error + msg).c_str());
		}
		else
		{
			rbx_deserialize(execution.scheduler->get_global_luastate(), "headhunter.exe", bytecode.c_str(), bytecode.size());
			rbx_spawn(execution.scheduler->get_global_luastate());
			rbx_decrement_top(execution.scheduler->get_global_luastate(), 1);
		}
	}

	return reinterpret_cast<int(__thiscall*)(std::uintptr_t, int)>(original_func)(waiting_scripts_job, a2);
}

void execution_t::hook_waiting_scripts_job() const
{
	this->scheduler->hook_waiting_scripts_job(scheduler_cycle, original_func);
	output << console::color::red << "Original function: 0x" << original_func << "\n";
}

void execution_t::run_script(const std::string& script) const
{
	roblox_encoder_t roblox_encoder{};
	const std::string& compiled = Luau::compile(script, {}, {}, &roblox_encoder); // no clone here, is gonna be cloned on queue
	std::unique_lock<std::mutex> guard{ mutex };
	script_queue.push(compiled);
}

void execution_t::set_identity(std::int8_t identity) const
{
	rbx_setidentity(this->scheduler->get_global_luastate(), identity);
}

execution_t::execution_t(const scheduler_t* scheduler)
	: scheduler(scheduler) {};