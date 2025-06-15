#include "jni/java_native_DarrenThread.h"
#include <iostream>
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"

namespace mini_jvm
{
    static JNINativeMethod DarrenThread_methods[] = {
        {(char*)"start", (char*)"()V", (void*)Java_DarrenThread_start},
        {(char*)"sleepNanos", (char*)"(J)V", (void*)Java_DarrenThread_sleepNanos},
    };

    void Java_native_DarrenThread::register_natives(JNIEnv* env) {
        InstanceKClass* k_Class = ClassLoader::load_class("DarrenThread");
        env->RegisterNatives((jclass)k_Class, DarrenThread_methods, sizeof(DarrenThread_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL Java_DarrenThread_start(JNIEnv *, jobject) {

    }

    JNIEXPORT void JNICALL Java_DarrenThread_sleepNanos(JNIEnv *, jclass, jlong){

    }
} // namespace mini_jvm