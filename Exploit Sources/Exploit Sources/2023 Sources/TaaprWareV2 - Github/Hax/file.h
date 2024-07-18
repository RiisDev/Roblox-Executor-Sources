#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <fileapi.h>

#include "xorstr.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

bool isfile(std::string path);
bool isfolder(std::string path);
const char* readfile(std::string path);
bool delfile(std::string path);
bool delfolder(std::string path);
bool writefile(std::string path, const char* data);
std::vector<std::string>* listfiles(std::string path);
std::vector<std::string>* listfolders(std::string path);