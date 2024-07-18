#pragma once

class Config {
	public:
		static void Save(char* configName);
		static void Load(char* configName);
};