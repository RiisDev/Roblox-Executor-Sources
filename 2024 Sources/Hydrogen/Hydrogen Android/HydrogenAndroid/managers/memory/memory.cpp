#include "memory.hpp"

#include <managers/managers.hpp>

#include <thread>

std::uintptr_t managers::memory_manager_t::get_module_base(const char* const name)
{
	const auto mod = dlopen(name, RTLD_NOW);

	if (mod)
	{
		const auto exp = dlsym(mod, "vpx_malloc");

		Dl_info info{};
		dladdr(exp, &info);

		return reinterpret_cast<std::uintptr_t>(info.dli_fbase);
	}

	return 0;
}

