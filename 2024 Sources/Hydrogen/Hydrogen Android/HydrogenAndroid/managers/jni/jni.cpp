#include "jni.hpp"

#include <managers/managers.hpp>

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <stdio.h>

void managers::jni_manager_t::initiate(JavaVM* jvm)
{
	if (jvm)
	{
		managers::utility_mgr.log("jni_manager Initiated: %p", jvm);

		this->jvm = jvm;
	}
}

JavaVM* managers::jni_manager_t::get_jvm( ) const
{
	return this->jvm;
}

JNIEnv* managers::jni_manager_t::get_env( ) const
{
	auto _jvm = this->get_jvm( );
	JNIEnv* env;
	if ( _jvm->GetEnv( reinterpret_cast< void** >( &env ), JNI_VERSION_1_6 ) != JNI_OK )
		_jvm->AttachCurrentThread( &env, NULL );

	return env;
}

//float managers::jni_manager_t::get_display_density( )
//{
//	static float display_density = 0.0f;
//	if ( display_density > 0.0f ) return display_density;
//	
//	auto env = get_env( );
//	jclass resourcesClass = env->FindClass( "android/content/res/Resources" );
//	jmethodID getSystem = env->GetStaticMethodID( resourcesClass, "getSystem", "()Landroid/content/res/Resources;" );
//	jobject systemClassObject = env->CallStaticObjectMethod( resourcesClass, getSystem );
//	jclass systemClass = env->GetObjectClass( systemClassObject );
//	jmethodID getDisplayMetricsFromGetSystem = env->GetMethodID( systemClass, "getDisplayMetrics", "()Landroid/util/DisplayMetrics;" );
//	jobject displayMetricsClassObject = env->CallObjectMethod( systemClassObject, getDisplayMetricsFromGetSystem );
//	jclass displayMetricsClass = env->GetObjectClass( displayMetricsClassObject );
//	jfieldID density = env->GetFieldID( displayMetricsClass, "density", "F" );
//	display_density = env->GetFloatField( displayMetricsClassObject, density );
//	
//	return display_density;
//}

jobject managers::jni_manager_t::get_clipboard_manager(JNIEnv* env)
{
	static bool looper_called = [](JNIEnv* env) // In each new thread prepare
	{
		jclass looperClass = env->FindClass("android/os/Looper");
		jmethodID prepareMethodID = env->GetStaticMethodID(looperClass, "prepare", "()V");
		env->CallStaticVoidMethod(looperClass, prepareMethodID);

		return true;
	}(env);

	jclass cls = env->FindClass("android/content/Context");

	jmethodID mid = env->GetMethodID(cls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	jfieldID fid = env->GetStaticFieldID(cls, "CLIPBOARD_SERVICE", "Ljava/lang/String;");
	jobject str = env->GetStaticObjectField(cls, fid);
	const auto cbm = env->CallObjectMethod(get_global_context(env), mid, str);

	__android_log_print(ANDROID_LOG_INFO, "[Hydrogen]", "cbm %p", cbm);

	return cbm;
}

jobject managers::jni_manager_t::get_global_context(JNIEnv* env)
{
	jclass activityThread = env->FindClass("android/app/ActivityThread");
	jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
	jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
	jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
	jobject context = env->CallObjectMethod(at, getApplication);
	return context;
}

void managers::jni_manager_t::set_clipboard(const char* const text)
{
	/*
	* android api >= 11
	ClipboardManager _clipboard = (ClipboardManager) _activity.getSystemService(Context.CLIPBOARD_SERVICE);
	_clipboard.setText(YOUR TEXT);
	*/

	auto env = get_env();
	const auto cbm = get_clipboard_manager(env);
	
	jclass ClipboardManagerClass = env->FindClass("android/text/ClipboardManager");

	jmethodID setText = env->GetMethodID(ClipboardManagerClass, "setText", "(Ljava/lang/CharSequence;)V");

	if (jstring jtext = env->NewStringUTF(text))
		env->CallVoidMethod(cbm, setText, jtext);
}

std::string managers::jni_manager_t::get_clipboard()
{
	auto env = get_env();
	const auto cbm = get_clipboard_manager(env);

	jclass ClipboardManagerClass = env->FindClass("android/text/ClipboardManager");

	jmethodID getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");
	jstring text = (jstring)env->CallObjectMethod(cbm, getText);


	if (text)
	{
		const char* utf = env->GetStringUTFChars(text, NULL);
		if (utf)
		{
			//env->ReleaseStringUTFChars(text, utf);

			return utf;
		}
	}

	return "";
}

std::string getPublicStaticString(JNIEnv* env, const char* className, const char* fieldName) {
	jclass clazz = env->FindClass(className);
	if (clazz != nullptr) {
		jfieldID fid = env->GetStaticFieldID(clazz, fieldName, "Ljava/lang/String;");
		if (fid != nullptr) {
			jstring GladioReceiver = (jstring)env->GetStaticObjectField(clazz, fid);
			jboolean blnIsCopy;
			std::string mystr = env->GetStringUTFChars(GladioReceiver, &blnIsCopy);
			return mystr;
		}
	}
	return "ERROR";
}

std::string managers::jni_manager_t::get_hwid()
{
	auto env = get_env();


	jclass cls = env->FindClass("android/content/Context");
	jmethodID getContentResolverMID = env->GetMethodID(cls, "getContentResolver", "()Landroid/content/ContentResolver;");

	jobject contentResolverObj = env->CallObjectMethod(get_global_context(env), getContentResolverMID);
	jstring idStr = (jstring)env->NewStringUTF("android_id");
	jclass settingsSecureClass = env->FindClass("android/provider/Settings$Secure");
	jmethodID getStringMID = env->GetStaticMethodID(settingsSecureClass, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
	// Do relevant error checking, and then:
	jstring androidId = (jstring)env->CallStaticObjectMethod(settingsSecureClass, getStringMID, contentResolverObj, idStr);

	if (androidId)
	{
		const char* utf = env->GetStringUTFChars(androidId, NULL);
		if (utf)
		{
			env->ReleaseStringUTFChars(androidId, utf);

			return utf;
		}
	}

	return "";
}

std::string managers::jni_manager_t::get_external_file_dir( )
{

	auto env = get_env( );
	// getExternalFilesDir() - java
	jclass cls_Env = env->FindClass( "android/app/NativeActivity" );
	jmethodID mid = env->GetMethodID( cls_Env, "getExternalFilesDir",
		"(Ljava/lang/String;)Ljava/io/File;" );
	jobject obj_File = env->CallObjectMethod( get_global_context( env ), mid, NULL );
	jclass cls_File = env->FindClass( "java/io/File" );
	jmethodID mid_getPath = env->GetMethodID( cls_File, "getPath",
		"()Ljava/lang/String;" );
	jstring obj_Path = ( jstring )env->CallObjectMethod( obj_File, mid_getPath );

	const char* utf = env->GetStringUTFChars( obj_Path, NULL );
	if ( utf )
	{
		env->ReleaseStringUTFChars( obj_Path, utf );

		return utf;
	}

	return "";
}
