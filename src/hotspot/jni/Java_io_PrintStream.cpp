#include "jni/Java_io_PrintStream.h"
#include <iostream>
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"

namespace mini_jvm
{
    static JNINativeMethod PrintStream_methods[] = {
        {(char*)"println", (char*)"(Ljava/lang/String;)V", (void*)java_io_PrintStream_println_Ljava_lang_String},
        {(char*)"println", (char*)"(I)V", (void*)java_io_PrintStream_println_I},
    };

    void Java_io_PrintStream::register_natives(JNIEnv* env) {
        InstanceKClass* k_Class = ClassLoader::load_class("java/io/PrintStream");
        env->RegisterNatives((jclass)k_Class, PrintStream_methods, sizeof(PrintStream_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL java_io_PrintStream_println_Ljava_lang_String(JNIEnv *env, jobject obj, jstring j_str, jint a, jint b) {
        std::cout << (char*)j_str << std::endl;
    }

    JNIEXPORT void JNICALL java_io_PrintStream_println_I(JNIEnv *, jobject, jint j_int) {
        std::cout << j_int << std::endl;
    }
} // namespace mini_jvm