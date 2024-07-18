#ifndef rlua_h
#define rlua_h
#include <iostream>
#include <vector>

typedef struct Vector3 { float x, y, z; };

typedef struct CFrame {
	float rotationmatrix[9];
	Vector3 position;
};

typedef struct Color3 {
	float r;
	float g;
	float b;
};

typedef struct UDim {
	float Scale;
	signed short Offset;
};

typedef struct UDim2 {
	UDim X;
	UDim Y;
};

typedef struct RBXInstance {
	int instance;
	int unk_0;
};

typedef struct InstanceTable {
	void* begin;
	void* end;
};

class RLua {
	int gameInstance;
	int gameInstance_raw;
public:
	RLua(int game);

	int GetGameInstance();
	int Clone(int instance);
	std::vector<int> GetChildren(int instance);
	int GetChildByName(int instance, const char* name);
	int GetChildByClass(int instance, const char* className);
	int GetInstanceInHierarchy(char** names, int size);
	std::vector<int> GetPlayers();
	int GetPlayerByName(const char* name);
	void Destroy(int instance); // naked
	int GetLocalPlayer();
	int ClearInstanceByName(int instance, const char* name);
	int ClearInstanceByClass(int instance, const char* cname);
	int GetParent(int instance);
	std::string GetFullName(int instance);
	std::string* GetName(int instance);
	std::string* GetClass(int instance);
	std::vector<int> GetInstanceTable(int ptr);
	int GetPlaceId();
};

#endif