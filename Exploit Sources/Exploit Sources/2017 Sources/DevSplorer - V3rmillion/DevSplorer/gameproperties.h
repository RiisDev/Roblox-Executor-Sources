#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <tchar.h>
#include <chrono>
#include <urlmon.h>
#include <Lmcons.h>
#include <direct.h>
#include <stdio.h>
#include <sys/types.h>
#include <shlobj.h>
#include <map>
#include "luaaddy.h"
#include <msclr\marshal_cppstd.h>
#include <algorithm>

using namespace System::ComponentModel;
using namespace System;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel::Design::Serialization;
using namespace std;


char* GlobalProperties[] = { "", "AllowClientInsertModels", "Reflectance", "CanCollide", "Name", "Enabled", "ClassName", "Parent", "WalkSpeed", "MaxHealth", "Health", "RigType", "MaxSlopeAngle", "HipHeight", "Character", "Transparency", "CharacterAppearanceId", "NameDisplayDistance", "Anchored", "Locked", "MembershipType", "RespawnLocation", "FollowUserId", "CanBeDropped", "Adornee", "Value", "FogEnd", "JumpPower", "AccountAge", "CharacterAutoLoads", "CharacterAutoLoads", "FilteringEnabled", "Archivable" };
void HandleProperty(string property, bool value);

namespace explorerval
{
	int FogEnd;
	int TimeOfDay;
	int FogStart;
	int GeographicLatitude;
	int CharacterAppearanceId;
	int AccountAge;
	int FollowUserId;

	float Gravity;
	float FallenPartsDestroyHeight;
	float Health;
	float JumpPower;
	float MaxHealth;
	float WalkSpeed;
	float Reflectance;
	float NameDisplayDistance;
	float MaxSlopeAngle;
	float HipHeight;
	float Transparency;

	float X = 0;
	float Y = 0;
	float Z = 0;

	double DistributedGameTime;

	bool FilteringEnabled;
	bool AllowThirdPartySales;
	bool Archivable = 0;
	bool StreamingEnabled;
	bool GlobalShadows = 0;

	string Name;
	string Parent;
	string ClassName;
	string Value;
	string Adornee;
	string Character;
	string RigType;
	string RespawnLocation;
	string MembershipType;
	
	bool Anchored;
	bool CanCollide;
	bool Enabled;
	bool CanBeDropped;
	bool AllowClientInsertModels;
	bool HttpEnabled;
	bool Locked;
	bool AutoJumpEnabled;
	bool CharacterAutoLoads;

}

namespace PropertyIndex
{
	bool FogEnd = false;
	bool TimeOfDay = false;
	bool FogStart = false;
	bool GeographicLatitude = false;
	bool GlobalShadows = false;

	bool Gravity = false;
	bool FilteringEnabled = false;
	bool DistributedGameTime = false;
	bool AllowThirdPartySales = false;
	bool Archivable = false;
	bool StreamingEnabled = false;
	bool Name = false;
	bool Parent = false;
	bool ClassName = false;
	bool Anchored = false;
	bool CanCollide = false;
	bool Value = false;
	bool Enabled = false;
	bool CanBeDropped = false;
	bool Adornee = false;
	bool FallenPartsDestroyHeight = false;
	bool AllowClientInsertModels = false;
	bool HttpEnabled = false;
	bool Health = false;
	bool JumpPower = false;
	bool MaxHealth = false;
	bool WalkSpeed = false;
	bool Reflectance = false;
	bool Character = false;
	bool CharacterAppearanceId = false;
	bool RigType = false;
	bool NameDisplayDistance = false;
	bool MaxSlopeAngle = false;
	bool HipHeight = false;
	bool Locked = false;
	bool FollowUserId = false;
	bool Transparency = false;
	bool AccountAge = false;
	bool RespawnLocation = false;
	bool MembershipType = false;
	bool AutoJumpEnabled = false;
	bool CharacterAutoLoads = false;
}

namespace ExplorerValues
{
	bool Archivable = false;
}

void GetCertainProperty()
{
	//MessageBoxA(0, "getfield", "e", MB_OK);
	r_getglobal(explorerLib::RState, "game");
	r_getfield(explorerLib::RState, -1, "Workspace");
	r_getfield(explorerLib::RState, -1, "FilteringEnabled");
	//(0, "tobool", "e", MB_OK);
	explorerval::FilteringEnabled = r_toboolean(explorerLib::RState, -1);
	r_emptystack(explorerLib::RState);
	//EmptyStack(explorerLib::RState);
}

string removeSpaces(string input)
{
	for (string::iterator i = input.begin(); i != input.end(); i++)
	{
		if (!isalnum(input.at(i - input.begin())))
		{
			input.erase(i);
			i--;
		}
	}

	for (string::iterator i = input.begin(); i != input.end(); i++)
	{
		if (!isalpha(input.at(i - input.begin())))
		{
			input.erase(i);
			i--;
		}
	}

	return input;
}

void ResetProperty()
{
	PropertyIndex::Adornee = false;
	PropertyIndex::AllowClientInsertModels = false;
	PropertyIndex::AllowThirdPartySales = false;
	PropertyIndex::Anchored = false;
	PropertyIndex::Archivable = false;
	PropertyIndex::CanBeDropped = false;
	PropertyIndex::Parent = false;
	PropertyIndex::Name = false;
	PropertyIndex::CanCollide = false;
	PropertyIndex::ClassName = false;
	PropertyIndex::Reflectance = false;
	PropertyIndex::Health = false;
	PropertyIndex::MaxHealth = false;
	PropertyIndex::WalkSpeed = false;
	PropertyIndex::Character = false;
	PropertyIndex::CharacterAppearanceId = false;
	PropertyIndex::NameDisplayDistance = false;
	PropertyIndex::RigType = false;
	PropertyIndex::MaxSlopeAngle = false;
	PropertyIndex::HipHeight = false;
	PropertyIndex::Enabled = false;
	PropertyIndex::Value = false;
	PropertyIndex::Locked = false;
	PropertyIndex::CanBeDropped = false;
	PropertyIndex::RespawnLocation = false;
	PropertyIndex::AccountAge = false;
	PropertyIndex::Transparency = false;
	PropertyIndex::MembershipType = false;
	PropertyIndex::CharacterAutoLoads = false;
	PropertyIndex::AutoJumpEnabled = false;
}

void IndexPropety()
{
	ResetProperty();
	//EmptyStack(explorerLib::RState);
	r_emptystack(explorerLib::RState);

	for (const string &newStack: IndexStack) {
			removeSpaces(newStack);
			//MessageBoxA(0, newStack.c_str(), newStack.c_str(), MB_OK);
			r_getfield_property(explorerLib::RState, -1, newStack);
	}
}

void DeleteExplorerObject(System::Windows::Forms::TreeView^ node, System::Windows::Forms::PropertyGrid^ propertys)
{
	if (MessageBoxA(0, "You won't be able to index it\nagain.", "Delete object?", MB_YESNO) == IDYES){
		try
		{
			MemoryWatch_Start();
			IndexPropety();
			//IndexStack.erase()
			r_getfield(explorerLib::RState, -1, "Destroy");
			r_pushvalue(explorerLib::RState, -2);
			r_pcall(explorerLib::RState, 1, 0, 0);
			node->SelectedNode->Remove();
			propertys->SelectedObject = nullptr;
			MemoryWatch_End();
		}
		catch (...)
		{
			//MessageBoxA(0, "Object is not available on the stack,\nor it was already deleted.", "Could not delete object.", MB_OK);
		}
	}
}

void CheckProperties(bool val)
{
	if (val == false)
	{
		IndexPropety();
		val = true;
	}
}

bool SetPropertyBool(bool value, string properties, bool setval)
{
	MemoryWatch_Start();
	IndexPropety();
	//MessageBoxA(0, "well", "just testing indexing", MB_OK);
	r_pushboolean(explorerLib::RState, value);
	r_setfield(explorerLib::RState, -2, properties.c_str());
	r_pcall(explorerLib::RState, 1, 0, 0);
	IndexPropety();
	MemoryWatch_End();
	setval = value;
	return setval;
}

System::String^ ReadString(const char* Name)
{
	MemoryWatch_Start();
	r_getfield(explorerLib::RState, -1, Name);
	explorerval::Value = r_tostring(explorerLib::RState, -1);
	MemoryWatch_End();
	System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Value);
	return memz;
}

System::String^ ReadPropertyValue(string className)
{
	if (className == "StringValue")
	{
		MemoryWatch_Start();
		CheckProperties(PropertyIndex::Value);
		r_getfield(explorerLib::RState, -1, "Value");
		explorerval::Value = r_tostring(explorerLib::RState, -1);
		MemoryWatch_End();
		System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Value);
		return memz;
	}

	if (className == "IntValue")
	{
		MemoryWatch_Start();
		CheckProperties(PropertyIndex::Value);
		r_getfield(explorerLib::RState, -1, "Value");
		explorerval::Value = r_tostring(explorerLib::RState, -1);
		MemoryWatch_End();
		System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Value);
		return memz;
	}
}


System::String^ SetPropertyValue_String(System::String^ value, string propertys)
{
	try
	{
		MemoryWatch_Start();
		IndexPropety();
		string val = msclr::interop::marshal_as<std::string>(value);
		r_pushstring(explorerLib::RState, val.c_str());
		r_setfield(explorerLib::RState, -2, propertys.c_str());
		r_pcall(explorerLib::RState, 1, 0, 0);
		IndexPropety();
		MemoryWatch_End();
		return value;
	}
	catch (...)
	{
		SetPropertyValue_String(value, propertys);
	}
}

int SetIntValue(const char* property, int value)
{
	MemoryWatch_Start();
	IndexPropety();
	r_pushnumber(explorerLib::RState, value);
	r_setfield(explorerLib::RState, -2, property);
	r_pcall(explorerLib::RState, 1, 0, 0);
	IndexPropety();
	MemoryWatch_End();
	return value;
}


System::String^ ValueSetType(System::String^ value, string className)
{
	if (className == "StringValue")
	{
		SetPropertyValue_String(value, "Value");
		return value;
	}

	else if (className == "IntValue")
	{
		string val = msclr::interop::marshal_as<std::string>(value);
		int newValue = atoi(val.c_str());
		newValue = SetIntValue("Value", newValue);
		string readOnlyValue = to_string(newValue);
		value = msclr::interop::marshal_as<String^>(readOnlyValue);
		return value;
	}
}

float SetNumberValue(const char* property, float value)
{
	MemoryWatch_Start();
	r_pushnumber(explorerLib::RState, value);
	r_setfield(explorerLib::RState, -2, property);
	r_pcall(explorerLib::RState, 1, 0, 0);
	IndexPropety();
	MemoryWatch_End();
	return value;
}


float HealthProperty()
{
	if (explorerval::ClassName != "Model")
	{
		MemoryWatch_Start();
		CheckProperties(PropertyIndex::Health);
		r_getfield(explorerLib::RState, -1, "Health");
		int newHealth = r_tonumber(explorerLib::RState, -1);
		explorerval::Health = newHealth;
		MemoryWatch_End();
		return explorerval::Health;
	}
	else
	{
		HandleProperty("Health", false);
		explorerval::Health = 0;
		HandleProperty("Health", false);
	}
}

float FloatProperty(const char* propertyName, float value)
{
	MemoryWatch_Start();
	r_getfield(explorerLib::RState, -1, propertyName);
	int newVal = r_tonumber(explorerLib::RState, -1);
	value = newVal;
	MemoryWatch_End();
	return value;
}

public value struct PropertyXYZ
{
	public:

	/*[SerializableAttribute]
	[TypeConverterAttribute((SizeConverter^::typeid))]
	[ComVisibleAttribute(true)]*/

	property int X {
		int get()
		{
			return 1;
		}
	}

	[Browsable(true)]
	property int Y {
		int get()
		{
			return 1;
		}
	}

	[Browsable(true)]
	property int Z {
		int get()
		{
			return 1;
		}
	}
};

ref class RProperty {
public:

	[Browsable(false)]
	[Category("Behavior")]

	property bool Archivable {
		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Archivable);
			r_getfield(explorerLib::RState, -1, "Archivable");
			explorerval::Archivable = r_toboolean(explorerLib::RState, -1);
			return explorerval::Archivable;
			MemoryWatch_End();
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "Archivable", explorerval::Archivable);
			explorerval::Archivable = value;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool Locked {
		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Locked);
			r_getfield(explorerLib::RState, -1, "Locked");
			explorerval::Locked = r_toboolean(explorerLib::RState, -1);
			return explorerval::Locked;
			MemoryWatch_End();
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "Locked", explorerval::Locked);
			explorerval::Locked = value;
		}
	}

	/*[Browsable(true)]
	[Category("Behavior")]

	property PropertyXYZ Snow
	{
		PropertyXYZ get() {
			PropertyXYZ me;
			return me;
		}
	}*/

	[Browsable(false)]
	[Category("Behavior")]

	property bool StreamingEnabled {
		bool get() {
			CheckProperties(PropertyIndex::StreamingEnabled);
			r_getfield(explorerLib::RState, -1, "StreamingEnabled");
			explorerval::StreamingEnabled = r_toboolean(explorerLib::RState, -1);
			return explorerval::StreamingEnabled;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool AllowThirdPartySales {
		bool get() {
			CheckProperties(PropertyIndex::AllowThirdPartySales);
			r_getfield(explorerLib::RState, -1, "AllowThirdPartySales");
			explorerval::StreamingEnabled = r_toboolean(explorerLib::RState, -1);
			return explorerval::AllowThirdPartySales;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property float FallenPartsDestroyHeight {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::FallenPartsDestroyHeight);
			MemoryWatch_End();
			explorerval::FallenPartsDestroyHeight = FloatProperty("FallenPartsDestroyHeight", explorerval::FallenPartsDestroyHeight);
			return explorerval::FallenPartsDestroyHeight;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::FallenPartsDestroyHeight = SetNumberValue("FallenPartsDestroyHeight", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float Health {
		float get() {
			HealthProperty();
			return explorerval::Health;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::Health = SetNumberValue("Health", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float JumpPower {
		float get() {
			CheckProperties(PropertyIndex::JumpPower);
			FloatProperty("JumpPower", PropertyIndex::JumpPower);
			return explorerval::JumpPower;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::JumpPower = SetNumberValue("JumpPower", value);
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property float NameDisplayDistance {
		float get() {
			CheckProperties(PropertyIndex::NameDisplayDistance);
			FloatProperty("NameDisplayDistance", PropertyIndex::NameDisplayDistance);
			return explorerval::NameDisplayDistance;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::NameDisplayDistance = SetNumberValue("NameDisplayDistance", value);
		}
	}

	[Browsable(false)]
	[Category("Appearance")]

	property float Transparency {
		float get() {
			CheckProperties(PropertyIndex::Transparency);
			FloatProperty("Transparency", PropertyIndex::Transparency);
			return explorerval::Transparency;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::Transparency = SetNumberValue("Transparency", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float MaxHealth {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::MaxHealth);
			MemoryWatch_End();
			FloatProperty("MaxHealth", PropertyIndex::MaxHealth);
			return explorerval::MaxHealth;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::MaxHealth = SetNumberValue("MaxHealth", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float HipHeight {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::HipHeight);
			MemoryWatch_End();
			FloatProperty("HipHeight", PropertyIndex::HipHeight);
			return explorerval::HipHeight;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::HipHeight = SetNumberValue("HipHeight", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float MaxSlopeAngle {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::MaxSlopeAngle);
			MemoryWatch_End();
			FloatProperty("MaxSlopeAngle", PropertyIndex::MaxSlopeAngle);
			return explorerval::MaxSlopeAngle;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::MaxSlopeAngle = SetNumberValue("MaxSlopeAngle", value);
		}
	}

	[Browsable(false)]
	[Category("Game")]

	property float WalkSpeed {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::WalkSpeed);
			MemoryWatch_End();
			explorerval::WalkSpeed = FloatProperty("WalkSpeed", explorerval::WalkSpeed);
			return explorerval::WalkSpeed;
		}

		void set(float value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::WalkSpeed = SetNumberValue("WalkSpeed", value);
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property int CharacterAppearanceId {
		int get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::CharacterAppearanceId);
			MemoryWatch_End();
			explorerval::CharacterAppearanceId = FloatProperty("CharacterAppearanceId", explorerval::CharacterAppearanceId);
			return explorerval::CharacterAppearanceId;
		}

		void set(int value)
		{
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::CharacterAppearanceId = SetNumberValue("CharacterAppearanceId", value);
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property int AccountAge {
		int get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::AccountAge);
			MemoryWatch_End();
			explorerval::AccountAge = FloatProperty("AccountAge", explorerval::AccountAge);
			return explorerval::AccountAge;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property int FollowUserId {
		int get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::FollowUserId);
			MemoryWatch_End();
			explorerval::FollowUserId = FloatProperty("FollowUserId", explorerval::FollowUserId);
			return explorerval::FollowUserId;
		}
	}



	[Browsable(false)]
	[Category("Behavior")]

	property bool Anchored {

		bool get() {
			CheckProperties(PropertyIndex::Anchored);
			r_getfield(explorerLib::RState, -1, "Anchored");
			explorerval::Anchored = r_toboolean(explorerLib::RState, -1);
			return explorerval::Anchored;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "Anchored", explorerval::Anchored);
			explorerval::Anchored = value;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool CanCollide {

		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::CanCollide);
			r_getfield(explorerLib::RState, -1, "CanCollide");
			explorerval::CanCollide = r_toboolean(explorerLib::RState, -1);
			MemoryWatch_End();
			return explorerval::CanCollide;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "CanCollide", explorerval::CanCollide);
			explorerval::CanCollide = value;
		}
	}

	[Browsable(false)]
	[Category("State")]

	property bool Enabled {

		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Enabled);
			r_getfield(explorerLib::RState, -1, "Enabled");
			explorerval::Enabled = r_toboolean(explorerLib::RState, -1);
			MemoryWatch_End();
			return explorerval::Enabled;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "Enabled", explorerval::Enabled);
			explorerval::Enabled = value;
		}

	}


	[Browsable(false)]
	[Category("Behavior")]

	property bool CanBeDropped {
		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::CanBeDropped);
			r_getfield(explorerLib::RState, -1, "CanBeDropped");
			explorerval::CanBeDropped = r_toboolean(explorerLib::RState, -1);
			MemoryWatch_End();
			return explorerval::CanBeDropped;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "CanBeDropped", explorerval::CanBeDropped);
			explorerval::CanBeDropped = value;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool CharacterAutoLoads {
		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::CharacterAutoLoads);
			r_getfield(explorerLib::RState, -1, "CharacterAutoLoads");
			explorerval::CharacterAutoLoads = r_toboolean(explorerLib::RState, -1);
			MemoryWatch_End();
			return explorerval::CharacterAutoLoads;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "CharacterAutoLoads", explorerval::CharacterAutoLoads);
			explorerval::CharacterAutoLoads = value;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property bool AutoJumpEnabled {
		bool get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::AutoJumpEnabled);
			r_getfield(explorerLib::RState, -1, "AutoJumpEnabled");
			explorerval::AutoJumpEnabled = r_toboolean(explorerLib::RState, -1);
			MemoryWatch_End();
			return explorerval::AutoJumpEnabled;
		}

		void set(bool value)
		{
			value = SetPropertyBool(value, "AutoJumpEnabled", explorerval::AutoJumpEnabled);
			explorerval::AutoJumpEnabled = value;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool AllowClientInsertModels {
		bool get() {
			CheckProperties(PropertyIndex::AllowClientInsertModels);
			r_getfield(explorerLib::RState, -1, "AllowClientInsertModels");
			explorerval::AllowClientInsertModels = r_toboolean(explorerLib::RState, -1);
			return explorerval::AllowClientInsertModels;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property bool HttpEnabled {
		bool get() {
			CheckProperties(PropertyIndex::HttpEnabled);
			r_getfield(explorerLib::RState, -1, "HttpEnabled");
			explorerval::HttpEnabled = r_toboolean(explorerLib::RState, -1);
			return explorerval::HttpEnabled;
		}
	}

	[Browsable(false)]
	[Category("Behavior")]

	property bool FilteringEnabled {
		bool get() {
			return explorerval::FilteringEnabled;
		}
	}

	[Browsable(false)]

	property float FogEnd {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::FogEnd);
			MemoryWatch_End();
			explorerval::FogEnd = FloatProperty("FogEnd", explorerval::FogEnd);
			return explorerval::FogEnd;
		}

		void set(float value) {
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::FogEnd = SetNumberValue("FogEnd", value);
		}
	}

	[Browsable(false)]
	[Category("Appearance")]

	property float Reflectance {
		float get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Reflectance);
			MemoryWatch_End();
			explorerval::Reflectance = FloatProperty("Reflectance", explorerval::Reflectance);
			return explorerval::Reflectance;
		}

		void set(float value) {
			MemoryWatch_Start();
			IndexPropety();
			MemoryWatch_End();
			explorerval::Reflectance = SetNumberValue("Reflectance", value);
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ Name
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Name);
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::Name = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Name);
			return memz;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ ClassName
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::ClassName);
			r_getfield(explorerLib::RState, -1, "ClassName");
			explorerval::ClassName = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::ClassName);
			return memz;
		}

		void set(System::String^ value) {
			explorerval::ClassName = msclr::interop::marshal_as<std::string>(value);
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ Adornee
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Adornee);
			r_getfield(explorerLib::RState, -1, "Adornee");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::Adornee = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();

			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Adornee);
			return memz;
		}

		void set(System::String^ value) {
			
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ Parent
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Parent);
			r_getfield(explorerLib::RState, -1, "Parent");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::Parent = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Parent);
			return memz;
			
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ Character
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::Character);
			r_getfield(explorerLib::RState, -1, "Character");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::Character = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::Character);
			return memz;

		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ RigType
	{
		System::String^ get() {

			MemoryWatch_Start();
			CheckProperties(PropertyIndex::RigType);
			r_getfield(explorerLib::RState, -1, "RigType");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::RigType = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::RigType);
			return memz;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ RespawnLocation
	{
		System::String^ get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::RespawnLocation);
			r_getfield(explorerLib::RState, -1, "RespawnLocation");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::RespawnLocation = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::RespawnLocation);
			return memz;
		}
	}

	[Browsable(false)]
	[Category("Data")]

	property System::String^ MembershipType
	{
		System::String^ get() {
			MemoryWatch_Start();
			CheckProperties(PropertyIndex::MembershipType);
			r_getfield(explorerLib::RState, -1, "MembershipType");
			r_getfield(explorerLib::RState, -1, "Name");
			explorerval::MembershipType = r_tostring(explorerLib::RState, -1);
			MemoryWatch_End();
			System::String^ memz = msclr::interop::marshal_as<System::String^>(explorerval::MembershipType);
			return memz;
		}
	}


	[Browsable(false)]
	[Category("Data")]

	property System::String^ Value
	{
		System::String^ get()
		{
			System::String^ newVal = "failed to get value";
			return newVal = ReadPropertyValue(explorerval::ClassName);
		}

		void set(System::String^ value)
		{
			value = ValueSetType(value, explorerval::ClassName);
		}
	}

};

namespace ExplorerIndex
{
	int Parent;
	int CurrentIndex;
	bool IndexedBefore = false;
	bool ClearingStack = true;
}

ref class GLobalXPlorer
{
public:
	TreeView^ globalxplorer = gcnew TreeView;
	PropertyGrid^ globalproperties = gcnew PropertyGrid;
};

void RightClickObject(Object^ sender, EventArgs^ e)
{
	Menu^ type = (Menu^)sender;
	GLobalXPlorer^ xplorers = gcnew GLobalXPlorer;
	if (type->MenuItems->Find("Destroy", false))
	{
		DeleteExplorerObject(xplorers->globalxplorer, xplorers->globalproperties);
	}
}

void DisplayRightMenu(Control^ thiss, ContextMenu^ menu, MouseEventArgs^ mouse, TreeView^ explorer)
{
	menu->MenuItems->Add("Destroy", gcnew EventHandler(&RightClickObject));
	menu->Show(thiss, explorer->PointToClient(mouse->Location));
	explorer->ContextMenu = menu;
}

void AddNode(string nodeName, int Index, System::Windows::Forms::TreeView^ node)
{
	ExplorerIndex::CurrentIndex = Index;
	r_getfield_explorer(explorerLib::RState, -1, nodeName.c_str());
	vector<DWORD> children = explorerLib::GetChildren(Index);
	for (int i = 0; i != children.size(); ++i) {
		if (!children.empty())
		{
			node->SelectedNode->Nodes->Add(msclr::interop::marshal_as< String^ >(*explorerLib::GetName(children[i])));
		}
		else
		{
			IndexPropety();
			ExplorerIndex::CurrentIndex = Index;
			r_getfield_explorer(explorerLib::RState, -1, nodeName.c_str());
		}
	}
}

void IndexExplorer(System::Windows::Forms::TreeView^ a)
{
	int count = a->SelectedNode->Nodes->Count;
	string nodeName = msclr::interop::marshal_as<std::string>(a->SelectedNode->Text->ToString());

	if (count == 0 && ExplorerIndex::IndexedBefore == false)
	{
		int Index = explorerLib::FindFirstChildes(explorerLib::DataModel, nodeName.c_str());
		AddNode(nodeName, Index, a);
		ExplorerIndex::IndexedBefore = true;
	}

	else if (count == 0 && !ExplorerIndex::IndexedBefore == false)
	{
		int Index = explorerLib::FindFirstChildes(ExplorerIndex::CurrentIndex, nodeName.c_str());
		AddNode(nodeName, Index, a);
	}
}

void HandleProperty(string properties, bool value)
{

	RProperty^ explorerProperties = gcnew RProperty;
	String^ newProperty = msclr::interop::marshal_as<String^>(properties);

	auto visibleproperty = dynamic_cast<BrowsableAttribute^>(TypeDescriptor::GetProperties(explorerProperties->GetType())[newProperty]->Attributes[BrowsableAttribute::typeid]);
	visibleproperty->GetType()->GetField("browsable", BindingFlags::NonPublic | BindingFlags::Instance)->SetValue(visibleproperty, value);

	if (properties == "Health" && explorerval::ClassName == "Model")
	{
		auto visibleproperty = dynamic_cast<BrowsableAttribute^>(TypeDescriptor::GetProperties(explorerProperties->GetType())["Health"]->Attributes[BrowsableAttribute::typeid]);
		visibleproperty->GetType()->GetField("browsable", BindingFlags::NonPublic | BindingFlags::Instance)->SetValue(visibleproperty, false);
	}
}

int FetchAllProperties(const char* property)
{
	try
	{
		//MessageBoxA(0, "rlly?", property, MB_OK);
		r_getfield(explorerLib::RState, -1, property);

		if (!rlua_isnil(explorerLib::RState, -1))
		{
			//MessageBoxA(0, "property existant", "", MB_OK);
			HandleProperty(property, true);
			IndexPropety();
			//EmptyStack(explorerLib::RState);
			return 0;
		}
		else
		{
			//MessageBoxA(0, "property non-existant", "", MB_OK);
			HandleProperty(property, false);
			IndexPropety();
			return 0;
		}
	}
	catch (std::exception error)
	{
		FetchAllProperties(property);
	}
}
