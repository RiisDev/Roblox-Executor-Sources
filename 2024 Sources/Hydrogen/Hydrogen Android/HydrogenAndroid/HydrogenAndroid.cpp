#include "managers/managers.hpp"

void start()
{
	managers::utility_mgr.log("Injected");

	//managers::utility_mgr.log(managers::jni_mgr.get_hwid().c_str());

	managers::initiate();
}

__attribute__((constructor))
void lib_main() 
{
	managers::utility_mgr.log("lib_main");

	std::thread{ start }.detach();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	managers::jni_mgr.initiate(vm);

	return JNI_VERSION_1_6;
}