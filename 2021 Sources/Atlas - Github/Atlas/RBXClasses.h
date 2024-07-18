#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "RBXStructs.h"

class Instance {
	public:
		Instance(DWORD address);

		DWORD Address;

		std::string GetName();

		std::string GetClassType(); // GetClassName.

		std::vector<Instance> GetChildren();

		Instance FindFirstChild(std::string name);
};

class VisualEngine {
	public:
		VisualEngine(DWORD address);

		DWORD Address;

		Vector2 GetClientDimensions();

		float* GetViewMatrix();

		Vector3 GetCameraPosition();
};

class DataModel : public Instance {
	public:
		using Instance::Instance;

		Instance GetService(std::string className);
};

class Player : public Instance {
	public:
		using Instance::Instance;

		__int64 GetUserID();

		Instance GetTeam();

		Instance GetCharacter();
};

class Players : public Instance {
	public:
		using Instance::Instance;

		Player GetLocalPlayer();

		std::vector<Player> GetPlayers();
};

class BasePart : public Instance {
	public:
		using Instance::Instance;

		Vector3 GetPosition();
};