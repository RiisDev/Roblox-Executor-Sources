#pragma once
#include <fstream>
#include <ShlObj.h>

#include <iostream>

#include <nlohmann/json.hpp>

#include "config.h"
#include "../sdk/vec.h"


Config::Config(const char* name) noexcept
{
	if (PWSTR documents; SUCCEEDED(
		SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documents)))
	{
		path = documents, path /= name;
		CoTaskMemFree(documents);
	}

	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
		this->save(0);
	}

	std::transform(std::filesystem::directory_iterator{ path },
		std::filesystem::directory_iterator{ }, std::back_inserter(configs),
		[](const auto& entry) { return entry.path().filename().string(); });
}

void Config::load(size_t id)
{
	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ifstream in{ path / configs[id] };
	if (!in.good())
		return;

	CJsonSerializer::Deserialize(this, in);
	in.close();
}

void Config::save(size_t id)
{
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ofstream out{ path / configs[id] };
	if (!out.good())
		return;

	CJsonSerializer::Serialize(this, out);
	out.close();
}

void Config::add(const char* name)
{
	if (*name && strlen(name) > 2 &&
		std::find(this->configs.begin(), this->configs.end(), name) == this->configs.end())
	{
		this->configs.emplace_back(name);
		this->save(this->configs.size() - 1);
	}
}

void Config::remove(size_t id)
{
	std::filesystem::remove(path / configs[id]);
	configs.erase(configs.cbegin() + id);
}

void Config::rename(size_t id, const char* name)
{
	std::filesystem::rename(path / configs[id], path / name);
	configs[id] = (char*)name;
}

void Config::reset()
{
	this->aim = {};
	this->esp = {};
	this->exploits = {};
}

void Color::Serialize(json& root)
{
	root["Color"][0] = this->m_Color.data[0];
	root["Color"][1] = this->m_Color.data[1];
	root["Color"][2] = this->m_Color.data[2];
	root["Rainbow"] = this->m_Rainbow;
	root["RainbowSpeed"] = this->m_Speed;
}
void Color::Deserialize(json& root)
{
	this->m_Color = vec3{	root["Color"][0].get<float>(),
							root["Color"][1].get<float>(),
							root["Color"][2].get<float>() };

	this->m_Rainbow = root["Rainbow"].get<bool>();
	this->m_Speed = root["RainbowSpeed"].get<float>();
}

void Config::Serialize(json& root)
{
	/* ESP */
	auto& esp = root["esp"];
	esp["m_TeamCheck"] = this->esp.m_TeamCheck;
	esp["m_Distance"] = this->esp.m_Distance;
	esp["m_Enabled"] = this->esp.m_Enabled;
	esp["m_Names"] = this->esp.m_Names;
	esp["m_Line"] = this->esp.m_Line;
	esp["m_Box"] = this->esp.m_Box;

	esp["m_MaxDistance"] = this->esp.m_MaxDistance;

	//esp["m_ShowInventory"] = this->esp.m_ShowInventory;

	this->esp.c_TargetDistance.Serialize(esp["c_TargetDistance"]);
	this->esp.c_TargetName.Serialize(esp["c_TargetName"]);
	this->esp.c_TargetLine.Serialize(esp["c_TargetLine"]);
	this->esp.c_TargetBox.Serialize(esp["c_TargetBox"]);

	this->esp.c_EnemyDistance.Serialize(esp["c_EnemyDistance"]);
	this->esp.c_EnemyName.Serialize(esp["c_EnemyName"]);
	this->esp.c_EnemyLine.Serialize(esp["c_EnemyLine"]);
	this->esp.c_EnemyBox.Serialize(esp["c_EnemyBox"]);

	/* Aim */
	auto& aim = root["aim"];
	aim["m_TeamCheck"] = this->aim.m_TeamCheck;
	aim["m_Enabled"] = this->aim.m_Enabled;
	aim["m_DrawFov"] = this->aim.m_DrawFov;
	aim["m_Head"] = this->aim.m_Head;

	aim["m_MaxDistance"] = this->aim.m_MaxDistance;
	aim["m_AimSmooth"] = this->aim.m_AimSmooth;
	aim["m_AimKey"] = this->aim.m_AimKey;
	aim["m_AimFov"] = this->aim.m_AimFov;

	this->aim.c_FovColor.Serialize(aim["c_FovColor"]);

	/* Exploits */
	auto& telekill = root["exploits"]["m_Telekill"];
	telekill["m_Enabled"] = this->exploits.m_Telekill.m_Enabled;
	telekill["m_distance"] = this->exploits.m_Telekill.m_Distance;
	telekill["m_key"] = this->exploits.m_Telekill.m_key;

	auto& telemove = root["exploits"]["m_Telemove"];
	telemove["m_Enabled"] = this->exploits.m_Telemove.m_Enabled;
	telemove["m_Amount"] = this->exploits.m_Telemove.m_Amount;

	telemove["m_Left"] = this->exploits.m_Telemove.m_Left;
	telemove["m_Back"] = this->exploits.m_Telemove.m_Back;
	telemove["m_Right"] = this->exploits.m_Telemove.m_Right;
	telemove["m_Front"] = this->exploits.m_Telemove.m_Front;

	telemove["m_Up"] = this->exploits.m_Telemove.m_Up;
	telemove["m_Down"] = this->exploits.m_Telemove.m_Down;

	auto& elevator = root["exploits"]["m_Elevator"];
	elevator["m_Enabled"] = this->exploits.m_Elevator.m_Enabled;
	elevator["m_Height"] = this->exploits.m_Elevator.m_Height;

	elevator["m_Increase"] = this->exploits.m_Elevator.m_Increase;
	elevator["m_Decrease"] = this->exploits.m_Elevator.m_Decrease;
}
void Config::Deserialize(json& root)
{
	/* ESP */
	auto& esp = root["esp"];
	this->esp.m_TeamCheck = esp["m_TeamCheck"].get<bool>();
	this->esp.m_Distance = esp["m_Distance"].get<bool>();
	this->esp.m_Enabled = esp["m_Enabled"].get<bool>();
	this->esp.m_Names = esp["m_Names"].get<bool>();
	this->esp.m_Line = esp["m_Line"].get<bool>();
	this->esp.m_Box = esp["m_Box"].get<bool>();

	this->esp.m_MaxDistance = esp["m_MaxDistance"].get<int>();

	//this->esp.m_ShowInventory = esp["m_ShowInventory"].get<bool>();

	this->esp.c_TargetDistance.Deserialize(esp["c_TargetDistance"]);
	this->esp.c_TargetName.Deserialize(esp["c_TargetName"]);
	this->esp.c_TargetLine.Deserialize(esp["c_TargetLine"]);
	this->esp.c_TargetBox.Deserialize(esp["c_TargetBox"]);

	this->esp.c_EnemyDistance.Deserialize(esp["c_EnemyDistance"]);
	this->esp.c_EnemyName.Deserialize(esp["c_EnemyName"]);
	this->esp.c_EnemyLine.Deserialize(esp["c_EnemyLine"]);
	this->esp.c_EnemyBox.Deserialize(esp["c_EnemyBox"]);

	/* Aim */
	auto& aim = root["aim"];
	this->aim.m_TeamCheck = aim["m_TeamCheck"].get<bool>();
	this->aim.m_Enabled = aim["m_Enabled"].get<bool>();
	this->aim.m_DrawFov = aim["m_DrawFov"].get<bool>();
	this->aim.m_Head = aim["m_Head"].get<bool>();

	this->aim.m_MaxDistance = aim["m_MaxDistance"].get<int>();
	this->aim.m_AimSmooth = aim["m_AimSmooth"].get<int>();
	this->aim.m_AimKey = aim["m_AimKey"].get<int>();
	this->aim.m_AimFov = aim["m_AimFov"].get<int>();

	this->aim.c_FovColor.Deserialize(aim["c_FovColor"]);
	
	/* Exploits */
	auto& telekill = root["exploits"]["m_Telekill"];
	this->exploits.m_Telekill.m_Enabled = telekill["m_Enabled"].get<bool>();
	this->exploits.m_Telekill.m_Distance = telekill["m_distance"].get<int>();
	this->exploits.m_Telekill.m_key = telekill["m_key"].get<int>();

	auto& telemove = root["exploits"]["m_Telemove"];
	this->exploits.m_Telemove.m_Enabled = telemove["m_Enabled"].get<bool>();
	this->exploits.m_Telemove.m_Amount = telemove["m_Amount"].get<int>();

	this->exploits.m_Telemove.m_Left = telemove["m_Left"].get<int>();
	this->exploits.m_Telemove.m_Back = telemove["m_Back"].get<int>();
	this->exploits.m_Telemove.m_Right = telemove["m_Right"].get<int>();
	this->exploits.m_Telemove.m_Front = telemove["m_Front"].get<int>();

	this->exploits.m_Telemove.m_Up = telemove["m_Up"].get<int>();
	this->exploits.m_Telemove.m_Down = telemove["m_Down"].get<int>();

	auto& elevator = root["exploits"]["m_Elevator"];
	this->exploits.m_Elevator.m_Enabled = elevator["m_Enabled"].get<bool>();
	this->exploits.m_Elevator.m_Height = elevator["m_Height"].get<int>();

	this->exploits.m_Elevator.m_Increase = elevator["m_Increase"].get<int>();
	this->exploits.m_Elevator.m_Decrease = elevator["m_Decrease"].get<int>();
}

bool CJsonSerializer::Serialize(IJsonSerializable* pObj, std::ofstream& output)
{
	if (!pObj)
		return 0;

	json serializeRoot;
	pObj->Serialize(serializeRoot);

	output << serializeRoot.dump(4);
	return 1;
}
bool CJsonSerializer::Deserialize(IJsonSerializable* pObj, std::ifstream& input)
{
	if (!pObj)
		return 0;

	json deserializeRoot;
	input >> deserializeRoot;

	pObj->Deserialize(deserializeRoot);
	return 1;
}