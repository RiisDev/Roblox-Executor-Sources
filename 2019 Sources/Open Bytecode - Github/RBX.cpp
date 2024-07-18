#include "RBX.hpp"

syn::Instance syn::Instance::GetParent() const
{
	return *reinterpret_cast<DWORD*>(instance_ptr + 0x34);
}

std::vector<syn::Instance> syn::Instance::GetChildren() const
{
	std::vector<Instance> ChildrenVector;
	const DWORD ChildVector = *reinterpret_cast<DWORD*>(instance_ptr + 44);
	if (!ChildVector)
		return ChildrenVector;

	const DWORD ChildEnd = *reinterpret_cast<DWORD*>(ChildVector + 4);
	DWORD ChildStart = *reinterpret_cast<DWORD*>(ChildVector);
	const DWORD ChildMax = *reinterpret_cast<DWORD*>(ChildVector + 4) - (*reinterpret_cast<DWORD*>(ChildVector) >> 3);
	DWORD ChildCount = 0;
	for (; ChildStart != ChildEnd; ChildStart += 8)
	{
		if (ChildCount == ChildMax)
			break;
		ChildCount++;
		ChildrenVector.emplace_back(*reinterpret_cast<DWORD*>(ChildStart));
	}
	return ChildrenVector;
}

syn::Instance syn::Instance::GetChildFromName(const char* InstanceName) const
{
	
	std::vector<Instance> ChildrenVector = GetChildren();
	for (DWORD Child : ChildrenVector)
		if (!strcmp(GetInstanceName(Child).c_str(), InstanceName))
			return Child;
	return NULL;
}

syn::Instance syn::Instance::GetChildFromClassName(const char* ClassName) const
{
	
	std::vector<Instance> ChildrenVector = GetChildren();
	for (DWORD Child : ChildrenVector)
		if (!strcmp(GetInstanceClassName(Child).c_str(), ClassName))
			return Child;
	return NULL;
}

/* Use 0 to signify getting the current instance's name */
std::string syn::Instance::GetInstanceName(DWORD Inst) const
{
	if (Inst == 0)
	{
		
#ifdef _DEBUG
		return std::string(*(const char**)(instance_ptr + 0x28));
#else
		return **(std::string**)(instance_ptr + 0x28);
#endif
	}
#ifdef _DEBUG
	return std::string(*(const char**)(Inst + 0x28));
#else
	return **(std::string**)(Inst + 0x28);
#endif
}

/* Use 0 to signify getting the current instance's class name */
std::string syn::Instance::GetInstanceClassName(DWORD Inst) const
{
	if (Inst == 0)
	{
#ifdef _DEBUG
		return std::string(*(const char**)(*(DWORD*)(instance_ptr + 0xC) + 4));
#else
		return **(std::string**)(*(DWORD*)(instance_ptr + 0xC) + 4);
#endif
	}
#ifdef _DEBUG
	return std::string(*(const char**)(*(DWORD*)(Inst + 0xC) + 4));
#else
	return **(std::string**)(*(DWORD*)(Inst + 0xC) + 4);
#endif
}