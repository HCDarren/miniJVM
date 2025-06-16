#include <iostream>
#include "runtime/JavaThread.hpp"
#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"

using namespace mini_jvm;

int main(void) {
    JavaVM* javaVM = JavaVM::current();
    JavaThread* javaThread = new JavaThread();
    javaVM->attachThread(javaThread);
    JNIEnv* jniEnv = javaThread->jniEnv();
    jclass main_class = jniEnv->FindClass("MiniJVM");
    jmethodID j_mid = jniEnv->GetMethod(main_class, "main", "([Ljava/lang/String;)V");
    jniEnv->CallStaticVoidMethod(main_class, j_mid);
    return 0;
}