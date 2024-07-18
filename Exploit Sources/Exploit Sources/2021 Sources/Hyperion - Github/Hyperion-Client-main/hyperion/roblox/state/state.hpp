#pragma once
#include "../offsets/offsets.hpp"
#include "../taskscheduler/taskscheduler.hpp"

namespace hyperion
{
	class state {
		uintptr_t rbxstate;
	public:
		uintptr_t return_state() {
			return rbxstate;
		}

		void set_identity(std::size_t identity_level) {
			*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(rbxstate + hyperion::offsets::extraspace) + hyperion::offsets::identity) = identity_level;
			*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(rbxstate + hyperion::offsets::extraspace) + 0x20) = 1; //for checkcaller, literally a flag
		}

		void set_state(uintptr_t rstate) {
			rstate = rstate;
		}

		void getfield(std::int32_t index, const char* string)
		{
			hyperion::functions::getfield(rbxstate, index, string);
		}

		void setfield(std::int32_t index, const char* string)
		{
			hyperion::functions::setfield(rbxstate, index, string);
		}

		void getglobal(const char* string)
		{
			getfield(-10002, string);
		}

		void setglobal(const char* string)
		{
			setfield(-10002, string);
		}

		std::int32_t pcall(std::int32_t args, std::int32_t ret, std::int32_t c)
		{
			return hyperion::functions::pcall(rbxstate, args, ret, c);
		}

		std::int32_t topointer(std::int32_t index)
		{
			return hyperion::functions::topointer(rbxstate, index);
		}

		void push_string(const std::string& string)
		{
			hyperion::functions::pushlstring(rbxstate, string.c_str(), string.size());
		}

		void new_thread()
		{
			rbxstate = hyperion::functions::newthread(rbxstate);
		}

		void luau_load(const char* chunkname, const std::string& bytecode)
		{
			hyperion::functions::luau_load(rbxstate, chunkname, bytecode.c_str(), bytecode.size(), 0);
		}

		void spawn()
		{
			hyperion::functions::spawn(rbxstate);
		}

		void push_value(int value)
		{
			hyperion::functions::push_value(rbxstate, value);
		}

		void* touserdata(int index)
		{
			return hyperion::functions::touserdata(rbxstate, index);
		}

		void push_bool(bool bl)
		{
			hyperion::functions::push_bool(rbxstate, bl);
		}

		bool to_bool(int index)
		{
			return hyperion::functions::tobool(rbxstate, index);
		}

		void push_string(const char* str)
		{
			hyperion::functions::push_string(rbxstate, str, strlen(str));
		}

		void settop(int index)
		{
			hyperion::functions::settop(rbxstate, index);
		}

		void setreadonly(int index, bool readonly)
		{
			hyperion::functions::readreadonly(rbxstate, index, readonly);
		}

		void remove(int index)
		{
			hyperion::functions::remove(rbxstate, index);
		}

		int getmetatable(int index)
		{
			return hyperion::functions::getmetatable(rbxstate, index);
		}

		void push_nil()
		{
			hyperion::functions::push_nil(rbxstate);
		}

		void push_number(double n)
		{
			hyperion::functions::pushnumber(rbxstate, n);
		}

		int index2addr(int index)
		{
			return hyperion::functions::index2addr(rbxstate, index);
		}

		const char* tostring(int index)
		{
			return hyperion::functions::tostring(rbxstate, index, 0);
		}

		void sandboxthread()
		{
			hyperion::functions::sandboxthread(rbxstate);
		}

		void xmove(uintptr_t oldstate, uintptr_t newstate, int index)
		{
			hyperion::functions::xmove(oldstate, newstate, index);
		}

		void call(int args, int results)
		{
			hyperion::functions::call(rbxstate, args, results);
		}

		void rawgeti(int index, int n)
		{
			hyperion::functions::rawgeti(rbxstate, index, n);
		}

		double to_number(int index)
		{
			return hyperion::functions::tonumber(rbxstate, index, 0);
		}

		void error(std::string error)
		{
			throw std::exception(error.c_str());
		}

		void push_object(uintptr_t object, int data_type)
		{
			TValue* top = *reinterpret_cast<TValue**>(rbxstate + 24);
			top->value.p = reinterpret_cast<void*>(object);
			top->tt = data_type;
			*reinterpret_cast<uintptr_t*>(rbxstate + 24) += 16;
		}

		state() : rbxstate(hyperion::functions::get_state(hyperion::taskscheduler::get_script_context())) { }
		state(uintptr_t rbxstate) : rbxstate(rbxstate) {}
	};
}
