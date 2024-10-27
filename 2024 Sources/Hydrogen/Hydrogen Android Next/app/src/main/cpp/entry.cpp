#include <jni.h>
#include <thread>
#include <chrono>

JavaVM* g_jvm = nullptr;

void start()
{
    while (!g_jvm) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    JNIEnv* env;
    if ( g_jvm->GetEnv( reinterpret_cast< void** >( &env ), JNI_VERSION_1_6 ) != JNI_OK )
        g_jvm->AttachCurrentThread( &env, nullptr );

    jclass cls = env->FindClass("com/versatile/hydrogen/Utils");
    jmethodID sendtoast = env->GetStaticMethodID(cls, "SendToast", "(Ljava/lang/String;I)V");
    jstring message = env->NewStringUTF("Hello from C");
    env->CallStaticVoidMethod(cls, sendtoast, message, 3500);
}

__attribute__((constructor))
void lib_main()
{
    std::thread{ start }.detach();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_jvm = vm;
    return JNI_VERSION_1_6;
}