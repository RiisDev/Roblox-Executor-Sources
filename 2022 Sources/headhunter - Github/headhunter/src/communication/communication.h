#pragma once
#include <Windows.h>
#include <string>

class communication_t
{
public:
	explicit communication_t() {};
	std::string read_pipe() const;
};