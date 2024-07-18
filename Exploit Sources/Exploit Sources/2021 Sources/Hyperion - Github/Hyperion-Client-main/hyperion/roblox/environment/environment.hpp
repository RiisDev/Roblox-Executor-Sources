#pragma once
#include "../state/state.hpp"
#include "../bypasses/callcheck/callcheck.hpp"

extern int hooked_index(uintptr_t ls);
extern int hooked_namecall(uintptr_t ls);

extern int oldnamecall_addr;

namespace hyperion::environment
{
	/*DEBUG LIB SHIT*/
	int getconstant(uintptr_t ls);
	int setconstant(uintptr_t ls);
	int getconstants(uintptr_t ls);
	int getupvalues(uintptr_t ls);
	int getupvalue(uintptr_t ls);
	int setupvalue(uintptr_t ls);
	/*DEBUG LIB SHIT*/

	void init_functions(state rstate);

	void init_tphandler(state rstate);

	void init_index_hook(state rstate);

	void init_namecall_hook(state rstate);

	int setfpscap(uintptr_t ls);

	int getfpscap(uintptr_t ls);

	int getfflag(uintptr_t ls);

	int setfflag(uintptr_t ls);

	int httpget(uintptr_t ls);

	int islclosure(uintptr_t ls);

	int iscclosure(uintptr_t ls);

	int getobjects(uintptr_t ls);

	int fireonesignal(uintptr_t ls);

	int getconnections(uintptr_t ls);

	int disableconnection(uintptr_t ls);

	int enableconnection(uintptr_t ls);

	int firesignal(uintptr_t ls);

	int fireproximityprompt(uintptr_t ls);

	int setproximityprompttime(uintptr_t ls);

	int fireclickdetector(uintptr_t ls);

	int firetouchtransmitter(uintptr_t ls);

	int gethui(uintptr_t ls);

	int getgenv(uintptr_t ls); //return -10002

	int getrenv(uintptr_t ls); //similar but at ur state

	int getreg(uintptr_t ls);

	int getgc(uintptr_t ls);

	int newcclosurehandler(uintptr_t ls); //gettop, push args, pcall, check for errors, if so warn() but in luac obv
	int newcclosure(uintptr_t ls);

	int setpropvisibility(uintptr_t ls); // for hidden prop

	int checkcaller(uintptr_t ls);

	int hookfunction(uintptr_t ls);

	int getnamecallmethod(uintptr_t ls);

	int stroketon(uintptr_t ls);

	int getminors(uintptr_t ls);

	int setreadonly(uintptr_t ls);

	int isreadonly(uintptr_t ls);

	int compilererror(uintptr_t ls);

	int getrawmetatable(uintptr_t ls);

	int runbytecode(uintptr_t ls);

	int setsimradiusflag(uintptr_t ls);

	int loadstring(uintptr_t ls);

	int setthreadidentity(uintptr_t ls);

	int test(uintptr_t ls);
}