#pragma once

#include <string>

namespace Execution {
	void execute_bytecode(std::string_view bytecode);
	
	void execute_script(const std::string& source);
}
