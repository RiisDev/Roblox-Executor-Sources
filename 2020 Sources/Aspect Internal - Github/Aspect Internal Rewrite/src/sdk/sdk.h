#pragma once
#ifndef SDK_H
#define SDK_H

#include "../Memory.h"
#include "classes.h"

#define INSTANCE_CHECK(x) (!x || reinterpret_cast<uintptr_t>(x) == 0xF)
#define fm(a) (a - 0xB50000 + (DWORD)GetModuleHandleA(0))

typedef __int16(__thiscall* humanoid_sethipheight)(uintptr_t Humanoid,
    float value);
typedef __int16(__thiscall* humanoid_setwalkspeed)(uintptr_t Humanoid,
    float value);
typedef int (__thiscall* humanoid_changestate)(uintptr_t Humanoid, int state);
typedef void*(__cdecl* get_datamodel)(void* dm);
typedef uintptr_t*(__thiscall* body_velocity)(void* ptr);


struct SDK {
public:
    RBXInstance* data_model;
    RBXPlayers* players;
	RBXInstance* Workspace;

    ViewMatrix_t* view_matrix;
    uintptr_t view_matrix_offset;
    bool increase_view_matrix_offset = false;

public:
    humanoid_sethipheight sethipheight;
    humanoid_setwalkspeed setwalkspeed;
	humanoid_changestate changestate;
	body_velocity get_velocity;

public:
    inline void initialize()
    {
        //std::cout << "Sig1" << std::endl;
        sethipheight = (humanoid_sethipheight)Memory::Scan(
            "55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 "
            "04");

        setwalkspeed = (humanoid_setwalkspeed)Memory::Scan("55 8B EC 56 8B F1 0F 57  C0 F3 0F 5F 45 08 8B 96");
        //std::cout << "Sig2" << std::endl;
		changestate = (humanoid_changestate)Memory::unprotect(
			Memory::Scan("55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 08 56 6A 00 "));

		//"Recording Stopped"
        //std::cout << "Sig3" << std::endl;
		//https://i.gyazo.com/77ce467534530d9394e3d82a63a1e947.png
        auto get_dm = (void* (__cdecl *)(void*))Memory::Scan(
            "8D 45 D8 C7 45 FC 01 ? ? ? 50 E8 ? ? ? ? 8B 7D D8", CALL_REL_32, 11);
        unsigned char dm[8];
        std::cout << "Getting Addresses" << std::endl;
        data_model = (RBXInstance*)(*(uintptr_t*)get_dm(dm) + 0x44);
		//get_velocity = reinterpret_cast<body_velocity>(fm(0xF4A150));
        players = data_model->find_class<RBXPlayers>("Players");
		Workspace = data_model->find_class<RBXInstance>("Workspace");
		RBXInstance* Camera = Workspace->find_child<RBXInstance>("Camera");
		printf("data_model: 0x%p\n", reinterpret_cast<uintptr_t>(data_model));
		printf("Workspace: 0x%p\n", reinterpret_cast<uintptr_t>(Workspace));
		printf("players: 0x%p\n", reinterpret_cast<uintptr_t>(players));
		printf("Camera: 0x%p\n", reinterpret_cast<uintptr_t>(Camera));
		printf("LocalPlayer: 0x%p\n", reinterpret_cast<uintptr_t>(players->get_local_player()));
		printf("LocalPlayer Head: 0x%p\n", reinterpret_cast<uintptr_t>(players->get_local_player()->character->find_child<RBXInstance>("Head")));
		printf("LocalPlayer Humanoid: 0x%p\n", reinterpret_cast<uintptr_t>(players->get_local_player()->character->find_class("Humanoid")));
        uintptr_t render_view = *(uintptr_t*)(*(uintptr_t*)(reinterpret_cast<uintptr_t>(data_model) + 0x70) + 0x1C);
		uintptr_t visual_engine = *(uintptr_t*)(render_view + 0x8);
        view_matrix = (ViewMatrix_t*)(visual_engine + 0x14C);
        printf("0x%p, 0x%p, 0x%p\n", render_view, visual_engine, view_matrix);
		//std::cout << "Local WalkSpeed = " << players->get_local_player()->character->find_class("Humanoid")->GetWalkSpeed() << std::endl;
		//std::cout << "Setting up Aspect Lua." << std::endl;
    }

    template <class T>
    inline float distance_to(T from, T to)
    {
        return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
    }
};

extern SDK sdk;
#endif // SDK_H
