#pragma once
#include <string>

class http_t
{
public:
    http_t(JavaVM* jvm)
    {
        this->jvm = jvm;
        auto env = this->getEnv();
        url_class = env->FindClass("java/net/URL");
        http_url_class = env->FindClass("java/net/HttpURLConnection");
        input_stream_class = env->FindClass("java/io/InputStream");
        input_stream_reader_class = env->FindClass("java/io/InputStreamReader");
        buffered_input_reader_class = env->FindClass("java/io/BufferedInputStream");
        buffered_reader_class = env->FindClass("java/io/BufferedReader");
        url_constructor = env->GetMethodID(url_class, "<init>", "(Ljava/lang/String;)V");
        buffered_reader_constructor = env->GetMethodID(buffered_reader_class, "<init>",
            "(Ljava/io/Reader;)V");
        input_stream_reader_constructor = env->GetMethodID(input_stream_reader_class, "<init>",
            "(Ljava/io/InputStream;)V");
        buffered_input_reader_constructor = env->GetMethodID(buffered_input_reader_class, "<init>",
            "(Ljava/io/InputStream;)V");
        url_openConnection_method = env->GetMethodID(url_class, "openConnection",
            "()Ljava/net/URLConnection;");
        http_url_getResponseCode_method = env->GetMethodID(http_url_class, "getResponseCode",
            "()I");
        http_url_getInputStream_method = env->GetMethodID(http_url_class, "getInputStream",
            "()Ljava/io/InputStream;");
        buffered_reader_readLine_method = env->GetMethodID(buffered_reader_class, "readLine",
            "()Ljava/lang/String;");

        http_url_disconnect_method = env->GetMethodID(http_url_class, "disconnect", "()V");
    }

    std::pair<int, std::string> get(const char* url)
    {
        auto env = this->getEnv();
        jstring jurl = env->NewStringUTF(url);
        jobject url_object = env->NewObject(url_class, url_constructor, jurl);
        jobject url_connection = env->CallObjectMethod(url_object, url_openConnection_method);
        int response_code = env->CallIntMethod(url_connection, http_url_getResponseCode_method);
        if (response_code != 200)
        {
            return { response_code, "Failed" };
        }
        jobject url_input_stream = env->CallObjectMethod(url_connection,
            http_url_getInputStream_method);
        jobject buffered_input_stream = env->NewObject(buffered_input_reader_class,
            buffered_input_reader_constructor,
            url_input_stream);
        jobject input_stream_reader = env->NewObject(input_stream_reader_class,
            input_stream_reader_constructor,
            buffered_input_stream);
        jobject buffered_reader = env->NewObject(buffered_reader_class, buffered_reader_constructor,
            input_stream_reader);
        jobject jlineobject;
        std::string result;
        while ((jlineobject = env->CallObjectMethod(buffered_reader,
            buffered_reader_readLine_method)))
        {
            jstring jline = (jstring)jlineobject;
            //  LOGD("JLINE->%p", jlineobject);
            auto line = env->GetStringUTFChars(jline, 0);
            // LOGD("Line->%s", line);
            std::string linestring(line);
            linestring += "\n";
            result += linestring;
            env->DeleteLocalRef(jlineobject);
        }
        env->CallVoidMethod(url_connection, http_url_disconnect_method);
        //env->CallVoidMethod(buffered_input_stream, input_stream_close_method);
        env->DeleteLocalRef(jurl);
        env->DeleteLocalRef(url_object);
        env->DeleteLocalRef(url_connection);
        env->DeleteLocalRef(url_input_stream);
        env->DeleteLocalRef(buffered_input_stream);
        env->DeleteLocalRef(input_stream_reader);
        env->DeleteLocalRef(buffered_reader);
        return { 200, result };
    }
private:
    JavaVM* jvm;
    jclass url_class;
    jclass http_url_class;
    jclass input_stream_class;
    jclass input_stream_reader_class;
    jclass buffered_input_reader_class;
    jclass buffered_reader_class;
    jmethodID url_constructor;
    jmethodID buffered_input_reader_constructor;
    jmethodID buffered_reader_constructor;
    jmethodID input_stream_reader_constructor;
    jmethodID url_openConnection_method;
    jmethodID http_url_getResponseCode_method;
    jmethodID http_url_getInputStream_method;
    jmethodID buffered_reader_readLine_method;
    jmethodID input_stream_close_method;
    jmethodID http_url_disconnect_method;
    JNIEnv* getEnv() {
        JNIEnv* env;
        int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
        if (status < 0) {
            status = jvm->AttachCurrentThread(&env, NULL);
            if (status < 0) {
                return nullptr;
            }
        }
        return env;
    }
};
