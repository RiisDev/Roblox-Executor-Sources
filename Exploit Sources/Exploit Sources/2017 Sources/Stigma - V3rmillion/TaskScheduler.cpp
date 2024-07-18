// We need a TaskScheduler because Lua is single threaded.
// If we start running stuff while Lua is, it might freak out and crash.
#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(lua_State *pL) {
	L = pL;
}

void TaskScheduler::QueueScript(char *scrip) {
	std::string script(scrip);
	this->queue.push_back("wait();" + script);
}