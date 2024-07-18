#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <array>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include "../sdk/vec.h"

using json = nlohmann::json;

class IJsonSerializable
{
public:
	virtual ~IJsonSerializable(void) {};
	virtual void Serialize(json& root) = 0;
	virtual void Deserialize(json& root) = 0;
}; 

class CJsonSerializer
{
public:
	static bool Serialize(IJsonSerializable* pObj, std::ofstream& output);
	static bool Deserialize(IJsonSerializable* pObj, std::ifstream& input);

private:
	CJsonSerializer(void) {};
};

class Color : IJsonSerializable
{
public:
	Color(vec3 c) : m_Color(c) {};

	virtual void Serialize(json& root);
	virtual void Deserialize(json& root);

	vec3 m_Color{ 1.f, 1.f, 1.f };
	bool m_Rainbow{ false };
	float m_Speed{ .1f };
};

class Config : IJsonSerializable {
public:
	explicit Config(const char*) noexcept;
	void load(size_t);
	void save(size_t);
	void add(const char*);
	void remove(size_t);
	void rename(size_t, const char*);
	void reset();

	virtual void Serialize(json& root);
	virtual void Deserialize(json& root);

	std::vector<::std::string>& get_configs()
	{
		return configs;
	}

	struct esp_t
	{
		bool m_Enabled = true;
		bool m_Names = true;
		bool m_Distance = true;
		bool m_Box = true;
		bool m_Line = true;
		bool m_TeamCheck = true;
		bool m_HentaiESP = false;
		bool m_ShowScreenCords = false;
		bool m_ShowInventory = false;
		bool m_ShowInventory_Filter_Parts = true;
		bool m_ShowDebugInfo = false;
		bool m_LabelAllParts = false;

		int m_MaxDistance = 1000;
		int m_LabelDistance = 100;

		Color c_TargetDistance{ vec3{1.f, 1.f, .4f} };
		Color c_TargetName{ vec3{1.f, 1.f, .4f} };
		Color c_TargetLine{ vec3{1.f, 1.f, .4f} };
		Color c_TargetBox{ vec3{1.f, 1.f, .4f} };
		Color c_TargetInventory{ vec3{1.f, 1.f, .4f} };

		Color c_EnemyDistance{ vec3{ 1.f, .54f, .01f } };
		Color c_EnemyName{ vec3{ 1.f, .54f, .01f } };
		Color c_EnemyLine{ vec3{ 1.f, .54f, .01f } };
		Color c_EnemyBox{ vec3{ 1.f, .54f, .01f } };
		Color c_EnemyInventory{ vec3{ 1.f, .54f, .01f } };
	} esp;

   
    struct aim_t
	{
		bool m_Enabled = true;
		bool m_Head = true;
		bool m_TeamCheck = true;
		bool m_DrawFov = true;
		bool m_Prediciton = false;

		int m_AimKey = 2;
		int m_AimFov = 100;
		int m_AimSmooth = 3;
		int m_MaxDistance = 1000;
		int m_AimMethod = 1;
		int m_DeadZone = 2;

		float m_GravityAcceleration = 500.0f;
		float m_BulletVelocity = 30000.0f;
		float m_YOffset = 0.0f;
		float m_XOffset = 0.0f;

		Color c_FovColor{ vec3{ 1.f, .54f, .01f } };
	} aim;

    struct exploits_t
	{
        struct {
            bool m_Enabled = false;
			bool m_TeamCheck = true;

            int m_Distance = 25;
			int m_key = 66;
        } m_Telekill;

		struct {
			bool m_Enabled = false;
			int m_Amount = 5;

			int m_Left = 100;
			int m_Back = 101;
			int m_Right = 102;
			int m_Front = 104;

			int m_Up = 103;
			int m_Down = 105;
        } m_Telemove;

		struct {
			bool m_Enabled = true;
			int m_Height = 0;

			int m_Increase = 107;
			int m_Decrease = 109;
        } m_Elevator;

		struct {
			bool m_Enabled = true;
			int m_Speed = 0;

			int m_Increase = 107;
			int m_Decrease = 109;
		} m_WalkSpeed;

		struct {
			bool m_Enabled = false;
			int State = 8;
		} m_ChangeState;
    } exploits;

	struct security
	{
		struct
		{
			bool m_AutoLogin = true;
		} m_LoginSecurity;
	} security;

	int menu_key = 45;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern Config config;
#endif // SETTINGS_H
