#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem> 
#include <mutex> 
#include <iterator>
#include <unordered_map>
#include <queue>

namespace Fluxus {
	std::string Compile(std::string const& script);
}