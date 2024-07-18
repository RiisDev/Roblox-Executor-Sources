#include <cstdint>
#include <vector>
#include <string>

__forceinline const char* DUMP_SHUFFLE_3(const char* tt2, const char* tt3, const char* tt4)
{
	std::string code = "#define SHUFFLE_3(a1, a2, a3) ";

	if (strcmp(tt2, "userdata") == 0)
		code += "a1; ";
	else if (strcmp(tt2, "number") == 0)
		code += "a2; ";
	else
		code += "a3; ";

	if (strcmp(tt3, "userdata") == 0)
		code += "a1; ";
	else if (strcmp(tt3, "number") == 0)
		code += "a2; ";
	else
		code += "a3; ";

	if (strcmp(tt4, "userdata") == 0)
		code += "a1";
	else if (strcmp(tt4, "number") == 0)
		code += "a2";
	else
		code += "a3";

	return code.data();
}

__forceinline const char* DUMP_SHUFFLE_4(const char* tt6, const char* tt7, const char* tt8, const char* tt9)
{
	std::string code = "#define SHUFFLE_4(a1, a2, a3, a4) ";

	if (strcmp(tt6, "table") == 0)
		code += "a1; ";
	else if (strcmp(tt6, "function") == 0)
		code += "a2; ";
	else if (strcmp(tt6, "userdata") == 0)
		code += "a3; ";
	else
		code += "a4; ";

	if (strcmp(tt7, "table") == 0)
		code += "a1; ";
	else if (strcmp(tt7, "function") == 0)
		code += "a2; ";
	else if (strcmp(tt7, "userdata") == 0)
		code += "a3; ";
	else
		code += "a4; ";

	if (strcmp(tt8, "table") == 0)
		code += "a1; ";
	else if (strcmp(tt8, "function") == 0)
		code += "a2; ";
	else if (strcmp(tt8, "userdata") == 0)
		code += "a3; ";
	else
		code += "a4; ";

	if (strcmp(tt9, "table") == 0)
		code += "a1";
	else if (strcmp(tt9, "function") == 0)
		code += "a2";
	else if (strcmp(tt9, "userdata") == 0)
		code += "a3";
	else
		code += "a4";

	return code.data();
}

__forceinline const char* DUMP_SHUFFLE_5(const char* tt25, const char* tt26, const char* tt27, const char* tt28, const char* tt29)
{
	std::string code = "#define SHUFFLE_5(a1, a2, a3, a4, a5) ";

	if (strcmp(tt25, "__lt") == 0)
		code += "a1; ";
	else if (strcmp(tt25, "__le") == 0)
		code += "a2; ";
	else if (strcmp(tt25, "__concat") == 0)
		code += "a3; ";
	else if (strcmp(tt25, "__type") == 0)
		code += "a4; ";
	else
		code += "a5; ";

	if (strcmp(tt26, "__lt") == 0)
		code += "a1; ";
	else if (strcmp(tt26, "__le") == 0)
		code += "a2; ";
	else if (strcmp(tt26, "__concat") == 0)
		code += "a3; ";
	else if (strcmp(tt26, "__type") == 0)
		code += "a4; ";
	else
		code += "a5; ";

	if (strcmp(tt27, "__lt") == 0)
		code += "a1; ";
	else if (strcmp(tt27, "__le") == 0)
		code += "a2; ";
	else if (strcmp(tt27, "__concat") == 0)
		code += "a3; ";
	else if (strcmp(tt27, "__type") == 0)
		code += "a4; ";
	else
		code += "a5; ";

	if (strcmp(tt28, "__lt") == 0)
		code += "a1; ";
	else if (strcmp(tt28, "__le") == 0)
		code += "a2; ";
	else if (strcmp(tt28, "__concat") == 0)
		code += "a3; ";
	else if (strcmp(tt28, "__type") == 0)
		code += "a4; ";
	else
		code += "a5; ";

	if (strcmp(tt29, "__lt") == 0)
		code += "a1";
	else if (strcmp(tt29, "__le") == 0)
		code += "a2";
	else if (strcmp(tt29, "__concat") == 0)
		code += "a3";
	else if (strcmp(tt29, "__type") == 0)
		code += "a4";
	else
		code += "a5";

	return code.data();
}

__forceinline const char* DUMP_SHUFFLE_7(const char* tt18, const char* tt19, const char* tt20, const char* tt21, const char* tt22, const char* tt23, const char* tt24)
{
	std::string code = "#define SHUFFLE_7(a1, a2, a3, a4, a5, a6, a7) ";

	if (strcmp(tt18, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt18, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt18, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt18, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt18, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt18, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt19, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt19, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt19, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt19, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt19, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt19, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt20, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt20, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt20, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt20, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt20, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt20, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt21, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt21, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt21, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt21, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt21, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt21, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt22, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt22, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt22, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt22, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt22, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt22, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt23, "__add") == 0)
		code += "a1; ";
	else if (strcmp(tt23, "__sub") == 0)
		code += "a2; ";
	else if (strcmp(tt23, "__mul") == 0)
		code += "a3; ";
	else if (strcmp(tt23, "__div") == 0)
		code += "a4; ";
	else if (strcmp(tt23, "__mod") == 0)
		code += "a5; ";
	else if (strcmp(tt23, "__pow") == 0)
		code += "a6; ";
	else
		code += "a7; ";

	if (strcmp(tt24, "__add") == 0)
		code += "a1";
	else if (strcmp(tt24, "__sub") == 0)
		code += "a2";
	else if (strcmp(tt24, "__mul") == 0)
		code += "a3";
	else if (strcmp(tt24, "__div") == 0)
		code += "a4";
	else if (strcmp(tt24, "__mod") == 0)
		code += "a5";
	else if (strcmp(tt24, "__pow") == 0)
		code += "a6";
	else
		code += "a7";

	return code.data();
}