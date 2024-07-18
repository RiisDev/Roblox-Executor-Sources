#include "datamodel.hpp"
#include "../globals/globals.hpp"

std::uintptr_t hyperion::datamodel::get_datamodel()
{
	int out[2];
	std::uintptr_t class_ptr = hyperion::datamodel::get_instance();
	hyperion::functions::getdatamodel(class_ptr, reinterpret_cast<int>(out));

	return out[0] + hyperion::offsets::inst;
}

void hyperion::datamodel::wait_for_gameloaded() //doesn't loop 24/7, only when tp handler triggers & finishes main stage then <- loops until new datamodel created
{
	while (hyperion::globals::datamodel == get_datamodel())
		Sleep(100);

	int out[2];
	std::uintptr_t class_ptr = hyperion::datamodel::get_instance();
	hyperion::functions::getdatamodel(class_ptr, reinterpret_cast<int>(out));

	while (*reinterpret_cast<bool*>(out[0] + hyperion::offsets::loaded) == 0) //requires weekly updates
		Sleep(100);

	hyperion::globals::datamodel = out[0] + hyperion::offsets::inst;
}

bool hyperion::datamodel::is_gameloaded()
{
	int out[2];
	std::uintptr_t class_ptr = hyperion::datamodel::get_instance();
	hyperion::functions::getdatamodel(class_ptr, reinterpret_cast<int>(out));

	return *reinterpret_cast<bool*>(out[0] + hyperion::offsets::loaded); //requires weekly updates
}

std::uintptr_t hyperion::datamodel::get_instance()
{
	return hyperion::functions::getdatamodel_instance();
}