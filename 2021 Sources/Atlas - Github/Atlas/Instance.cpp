#include <iostream>
#include "RBXClasses.h"
#include "Memory.h"

Instance::Instance(DWORD address) {
	this->Address = address;
}

std::string Instance::GetClassType() {
	std::string className;

	DWORD classDescriptor = Memory::GetPointerAddress(this->Address + 0xC);
	className = Memory::ReadStringOfUnknownLength(Memory::GetPointerAddress(classDescriptor + 0x4));

	return className;
}

std::string Instance::GetName() {
	DWORD nameAddress = Memory::GetPointerAddress(this->Address + 0x28);
	std::string name = Memory::ReadStringOfUnknownLength(nameAddress);

	int size = Memory::Read<int>((LPCVOID)(nameAddress + 0x14));

	if (size >= 16u) {
		DWORD newNameAddress = Memory::GetPointerAddress(nameAddress);
		return Memory::ReadStringOfUnknownLength(newNameAddress);
	} else {
		return name;
	}
}

std::vector<Instance> Instance::GetChildren() {
	std::vector<Instance> children = {};

	DWORD v4 = Memory::GetPointerAddress(this->Address + 0x2C);
	int v25 = (Memory::GetPointerAddress(v4 + 4) - Memory::GetPointerAddress(v4)) >> 3;
	if (!v25) {
		std::cout << "Couldn't get number of children of " << this->GetName() << "." << std::endl;
		return children;
	}

	DWORD v6 = Memory::GetPointerAddress(v4);
	for (int i = 0; i < v25; i++) {
		children.push_back(Instance(Memory::GetPointerAddress(v6)));
		v6 += 8;
	}

	return children;
}

Instance Instance::FindFirstChild(std::string name) {
	std::vector<Instance> children = GetChildren();

	for (Instance child : children) {
		if (child.GetName() == name) {
			return Instance(child);
		}
	}

	return Instance(0);
}