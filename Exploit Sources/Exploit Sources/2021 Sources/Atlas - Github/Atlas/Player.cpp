#include "RBXClasses.h"
#include "Memory.h"

__int64 Player::GetUserID() {
	return Memory::Read<__int64>((LPCVOID)(this->Address + 0xF8));
}

Instance Player::GetTeam() {
	return Instance(Memory::GetPointerAddress(this->Address + 0x98));
}

Instance Player::GetCharacter() {
	return Instance(Memory::GetPointerAddress(this->Address + 0x64));
}