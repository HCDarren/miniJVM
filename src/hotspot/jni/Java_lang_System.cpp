#include "jni/Java_lang_System.h"
#include <iostream>

namespace mini_jvm
{
    static JNINativeMethod System_methods[] = {
        {(char*)"gc", (char*)"()V", (void*)Java_lang_System_gc}
    };

    void Java_lang_System::register_natives(JNIEnv* env) {
        jclass j_class = env->FindClass("java/lang/System");
        env->RegisterNatives(j_class, System_methods, sizeof(System_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL Java_lang_System_gc(JNIEnv *env, jclass j_class) {
        std::cout << "System_gc: " << std::endl;
    }
} // namespace mini_jvm