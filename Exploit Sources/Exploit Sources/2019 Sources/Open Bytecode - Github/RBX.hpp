#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>


namespace syn
{
	class Instance
	{
	public:
		DWORD instance_ptr;
		Instance GetParent() const;
		std::vector<Instance> GetChildren() const;
		Instance GetChildFromName(const char* InstanceName) const;
		Instance GetChildFromClassName(const char* ClassName) const;
		std::string GetInstanceName(DWORD Inst = 0) const;
		std::string GetInstanceClassName(DWORD Inst = 0) const;
		bool IsEmpty() const { return !instance_ptr; }
		bool IsRobloxLocked() const { return *(BYTE*)(instance_ptr + 0x27); }
		Instance GetLocalPlayer() const { return *(DWORD*)(instance_ptr + 0xC8); }
		operator DWORD() const { return instance_ptr; }
		Instance(DWORD Address)
		{
			instance_ptr = Address;
		}
	};
}