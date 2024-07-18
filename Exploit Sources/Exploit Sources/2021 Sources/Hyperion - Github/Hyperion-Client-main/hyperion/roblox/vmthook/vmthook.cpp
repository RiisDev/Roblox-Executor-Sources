#include "vmthook.hpp"

//Copies the VTable & the RTTI which becomes 0 so all funcs are now + 1; When hooking, it just replace the func pointer on the new VTable copy which we wrote then when we need to call original we get it from our backup
vmthook::vmthook(void* instance)
{
	vt_backup = *reinterpret_cast<uintptr_t**>(instance);
	while (reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(instance))[vt_functions])
	{
		vt_functions++;
	}

	const int vt_size = ((vt_functions * 0x4) + 0x4);
	vt_new = std::make_unique<uintptr_t[]>(vt_functions + 1);

	memcpy(vt_new.get(), &vt_backup[-1], vt_size);
	*reinterpret_cast<uintptr_t**>(instance) = &vt_new.get()[1];
}

void* vmthook::get_old_function(const int index)
{
	return reinterpret_cast<void*>(vt_backup[index]);
}

void vmthook::hook_func(void* new_pointer, const int index)
{
	vt_new[index + 1] = reinterpret_cast<uintptr_t>(new_pointer);
}

void vmthook::unhook_func(const int index)
{
	vt_new[index + 1] = vt_backup[index];
}

//Todo: Make actual RestoreVMT Function
void vmthook::restore_vmt()
{
	vt_new.get()[1] = vt_backup[0];
	delete[] vt_backup;
}

//A more simple VMTHook method. Will just replace the vtable function pointer instead copying the whole vtable
void* vmthook::replace_vtable_funcpointer(uintptr_t* vtable, const int index, const uintptr_t new_pointer)
{
	DWORD old_protection;
	VirtualProtect(&vtable[index], sizeof(uintptr_t), 0x40, &old_protection);
	void* old_func = reinterpret_cast<void*>(vtable[index]);
	vtable[index] = new_pointer;
	VirtualProtect(&vtable[index], sizeof(uintptr_t), old_protection, &old_protection);
	return old_func;
}