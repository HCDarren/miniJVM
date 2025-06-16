#include "java_native_DarrenThread.h"
#include <iostream>
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include <unistd.h>
#include "oops/Oop.hpp"
#include "runtime/JavaThread.hpp"
#include <pthread.h>
#include "jni/jni.h"

namespace mini_jvm
{
    void *thread_func(void *arg)
    {
        // 创建一个新的 java 线程跟当前 c 线程绑定
        jobject jobj = (jobject)arg;
        JavaThread *javaThread = new JavaThread();
        JavaVM::current()->attachThread(javaThread);

        // 找到 java Thread 的 run 方法执行
        JNIEnv* jniEnv = javaThread->jniEnv();
        jclass j_clazz = jniEnv->GetObjectClass(jobj);
        jmethodID j_mid = jniEnv->GetMethod(j_clazz, "run", "()V");
        jniEnv->CallVoidMethod(jobj, j_mid);

        delete javaThread;
        return (void *)0; // 返回值可被 pthread_join 获取
    }

    static JNINativeMethod DarrenThread_methods[] = {
        {(char *)"start", (char *)"()V", (void *)Java_DarrenThread_start},
        {(char *)"sleepNanos", (char *)"(J)V", (void *)Java_DarrenThread_sleepNanos},
    };

    void Java_native_DarrenThread::register_natives(JNIEnv *env)
    {
        InstanceKClass *k_Class = ClassLoader::load_class("DarrenThread");
        env->RegisterNatives((jclass)k_Class, DarrenThread_methods, sizeof(DarrenThread_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL Java_DarrenThread_start(JNIEnv *jniEnv, jobject jobj)
    {
        // 创建新的线程出来
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thread_func, jobj);
        if (ret != 0) {
            return;
        }
    }

    JNIEXPORT void JNICALL Java_DarrenThread_sleepNanos(JNIEnv *, jclass, jlong sleepNanos)
    {
        usleep(sleepNanos * 1000);
    }
} // namespace mini_jvm