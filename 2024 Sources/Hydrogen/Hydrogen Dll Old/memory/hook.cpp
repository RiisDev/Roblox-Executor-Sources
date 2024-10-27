#include "hook.hpp"

#include <windows.h>
#include <unordered_map>

static std::unordered_map<std::uintptr_t, memory::fake_vmt_t*> fake_vmts;

memory::fake_vmt_t::fake_vmt_t(std::uintptr_t obj, std::uintptr_t vt_size)
{
	this->original_obj = obj;
	this->original_vmt = *reinterpret_cast<std::uintptr_t*>(obj);
	this->length = vt_size;
	
	this->vmt = reinterpret_cast<std::uintptr_t*>(VirtualAlloc(nullptr, vt_size * sizeof(void*), MEM_COMMIT, PAGE_READWRITE));
	memcpy(this->vmt, *reinterpret_cast<void**>(obj), vt_size * sizeof(void*));
	*reinterpret_cast<std::uintptr_t*>(obj) = reinterpret_cast<std::uintptr_t>(this->vmt);
}

memory::fake_vmt_t::~fake_vmt_t()
{
	*reinterpret_cast<std::uintptr_t*>(this->original_obj) = this->original_vmt;
	VirtualFree(this->vmt, this->length * sizeof(void*), MEM_RELEASE);
}

memory::fake_vmt_t* memory::fake_vmt_t::from(std::uintptr_t obj)
{
	auto it = fake_vmts.find(obj);
	if (it != fake_vmts.end())
		return it->second;
	return nullptr;
}

memory::fake_vmt_t* memory::fake_vmt_t::create(std::uintptr_t obj, std::uintptr_t length)
{
	auto fvmt = from(obj);
	if (fvmt) return fvmt;
	fvmt = new fake_vmt_t(obj, length);
	return fvmt;
}

std::uintptr_t memory::fake_vmt_t::swap_vfunction(std::uintptr_t vf, std::uintptr_t idx)
{
	if (idx >= this->length) return 0;
	auto old_vf = this->vmt[idx];
	this->vmt[idx] = vf;
	return old_vf;
}

void memory::fake_vmt_t::free()
{
	for (auto& it : fake_vmts) {
		if (it.second == this) {
			fake_vmts.erase(it.first);
			break;
		}
	}
	delete this;
}

std::uintptr_t memory::fake_vmt_t::get_original_obj()
{
	return original_obj;
}

std::uintptr_t memory::fake_vmt_t::get_original_vmt()
{
	return original_vmt;
}

std::uintptr_t memory::fake_vmt_t::get_length()
{
	return length;
}
