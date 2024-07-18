#pragma once

#include <Windows.h>
#include <string>
#include <vector>

DWORD getParent(DWORD Instance);
std::vector<DWORD> getChildren(DWORD Instance);
DWORD getChildFromName(DWORD Instance, PCHAR InstanceName);
DWORD getChildFromClassName(DWORD Instance, PCHAR ClassName);
std::string* getInstanceName(DWORD Instance);
std::string* getInstanceClassName(DWORD Instance);

namespace RBX {
	class Instance
	{
	private:
		DWORD InstanceAddress;
	public:
		Instance();
		Instance operator[](PCHAR ChildName);
		Instance GetParent();
		std::vector<Instance> GetChildren();
		std::string* Name;
		std::string* ClassName;
		Instance FindFirstChild(PCHAR ChildName);
		Instance FindFirstChildOfClass(PCHAR ClassName);
		DWORD GetAddress();
		Instance(DWORD Address);
	};
}