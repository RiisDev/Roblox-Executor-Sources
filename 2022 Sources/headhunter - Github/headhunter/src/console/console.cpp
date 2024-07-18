#include "console.h"

console::console(const std::string& title)
{
	DWORD old;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &old);
	*reinterpret_cast<byte*>(&FreeConsole) = 0xC3;
	VirtualProtect(&FreeConsole, 1, old, &old);

	AllocConsole();
	freopen_s(&this->f_ptr, "CONOUT$", "w", stdout);
	freopen_s(&this->f_ptr, "CONOUT$", "w", stderr);
	freopen_s(&this->f_ptr, "CONIN$", "r", stdin);

	SetConsoleTitleA(title.c_str());
}

console::~console()
{
	fclose(this->f_ptr);
	FreeConsole();
}

const console& console::operator<<(const std::string& str) const
{
	std::cout << str;
	return *this;
}

const console& console::operator<<(uintptr_t num) const
{
	std::cout << std::hex << num;
	return *this;
}

const console& console::operator<<(console::color col) const
{
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (col)
	{
	case color::red:
		SetConsoleTextAttribute(h_console, 12);
		break;
	case color::green:
		SetConsoleTextAttribute(h_console, 10);
		break;
	case color::blue:
		SetConsoleTextAttribute(h_console, 9);
		break;
	case color::pink:
		SetConsoleTextAttribute(h_console, 13);
		break;
	case color::cyan:
		SetConsoleTextAttribute(h_console, 11);
		break;
	case color::white:
		[[fallthrough]];
	default:
		SetConsoleTextAttribute(h_console, 7);
		break;
	}

	return *this;
}