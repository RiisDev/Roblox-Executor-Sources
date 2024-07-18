#include "RBXClasses.h"
#include "Memory.h"

VisualEngine::VisualEngine(DWORD address) {
	this->Address = address;
}

Vector2 VisualEngine::GetClientDimensions() {
	Vector2 clientDimensions = { 0, 0 };

	DWORD offset = 0x4D8;
	int width = Memory::Read<int>((LPCVOID)(this->Address + offset));
	int height = Memory::Read<int>((LPCVOID)(this->Address + offset + sizeof(width)));

	clientDimensions.X = width;
	clientDimensions.Y = height;

	return clientDimensions;
}

float* VisualEngine::GetViewMatrix() {
	float viewMatrix[16];

	ReadProcessMemory(Globals::Handle, (LPCVOID)(this->Address + 0x120), &viewMatrix, sizeof(viewMatrix), 0);

	return viewMatrix;
}

Vector3 VisualEngine::GetCameraPosition() {
	Vector3 position = { 0, 0, 0 };

	position = Memory::Read<Vector3>((LPCVOID)(this->Address + 0x44));

	return position;
}