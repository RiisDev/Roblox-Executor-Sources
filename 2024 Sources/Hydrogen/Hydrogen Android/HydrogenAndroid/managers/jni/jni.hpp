#pragma once

#include <dependencies/common_includes.hpp>

namespace managers
{
    class jni_manager_t
    {
        std::atomic< JavaVM* > jvm;

    public:
        void initiate(JavaVM* jvm);

		JavaVM* get_jvm( ) const;
		JNIEnv* get_env( ) const;

        jobject get_global_context(JNIEnv* env);

        jobject get_clipboard_manager(JNIEnv* env);

        std::string get_clipboard();

        void set_clipboard(const char* const text);

        std::string get_hwid();

        std::string get_external_file_dir( );
    };

    extern jni_manager_t jni_mgr;
}

