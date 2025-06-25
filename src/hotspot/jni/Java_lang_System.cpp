#include "jni/Java_lang_System.h"
#include <iostream>
#include "runtime/SafepointMechanism.hpp"
#include "hotspot/gc/GC.hpp"

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
        GC::gc();
    }
} // namespace mini_jvm