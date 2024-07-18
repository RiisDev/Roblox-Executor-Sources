#include "RBXClasses.h"
#include "Memory.h"

Vector3 BasePart::GetPosition() {
	Vector3 position = { 0, 0, 0 };

	DWORD primitive = Memory::Read<DWORD>((LPCVOID)(this->Address + 0xA8));
	position = Memory::Read<Vector3>((LPCVOID)(primitive + 0x11C));

	return position;
}