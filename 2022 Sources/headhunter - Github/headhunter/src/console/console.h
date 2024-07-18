#pragma once
#include <Windows.h>
#include <iostream>

class console
{
private:
	FILE* f_ptr = nullptr;
public:
	enum class color
	{
		red,
		green,
		blue,
		cyan,
		white,
		pink
	};

	explicit console(const std::string& title);
	~console();

	const console& operator<<(const std::string& str) const;
	const console& operator<<(color col) const;
	const console& operator<<(uintptr_t num) const;
};


extern const console output;