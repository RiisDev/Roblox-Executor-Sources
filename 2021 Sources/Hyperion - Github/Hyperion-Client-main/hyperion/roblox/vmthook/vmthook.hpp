#include <Windows.h>
#include <fstream>

class vmthook
{
	std::unique_ptr<uintptr_t[]> vt_new = nullptr;
	uintptr_t* vt_backup = nullptr;
	uintptr_t vt_functions = 0;

public:
	vmthook(void* instance);

	void* get_old_function(const int index);
	void hook_func(void* new_pointer, const int index);
	void unhook_func(const int index);
	void restore_vmt();
	void* replace_vtable_funcpointer(uintptr_t* vtable, const int index, const uintptr_t new_pointer);
};