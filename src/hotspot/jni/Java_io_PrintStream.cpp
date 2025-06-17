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
        jclass j_class = env->FindClass("Print");
        env->RegisterNatives(j_class, PrintStream_methods, sizeof(PrintStream_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL java_io_PrintStream_println_Ljava_lang_String(JNIEnv *env, jobject obj, jstring j_str, jint a, jint b) {
        std::cout << (char*)j_str << std::endl;
    }

    JNIEXPORT void JNICALL java_io_PrintStream_println_I(JNIEnv *env, jobject obj, jint j_int) {
        std::cout << "printlnI: " <<j_int << std::endl;
    }
} // namespace mini_jvm