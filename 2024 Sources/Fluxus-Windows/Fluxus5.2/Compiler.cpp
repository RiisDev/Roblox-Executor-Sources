#include "Libs/HttpLib.h"
#include "Compiler.hpp"

namespace Fluxus {
	std::string Compile(std::string const& script) {
		return httplib::Client("51.222.19.123:85").Post("/compile", script, "text/plain").value().body;
	}
}