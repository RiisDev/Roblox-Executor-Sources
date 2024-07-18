#include "hwid.h"

std::string hwid;

void update_hwid() {
	HW_PROFILE_INFOA hardware;
	GetCurrentHwProfileA(&hardware);
	hwid = hash::md5(std::string(hardware.szHwProfileGuid));
}