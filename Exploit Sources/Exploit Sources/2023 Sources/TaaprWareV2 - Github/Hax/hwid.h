// for user's hashed hwid
// indended to create keys that roblox scripts can't create

#pragma once

#include <string>
#include <Windows.h>

#include "md5.h"

extern std::string hwid;

void update_hwid();