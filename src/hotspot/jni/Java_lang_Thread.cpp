#include "jni/Java_lang_Thread.h"
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
        JavaVM::current()->detachThread(javaThread);
        delete javaThread;
        return (void *)0; // 返回值可被 pthread_join 获取
    }

    static JNINativeMethod Thread_methods[] = {
        {(char *)"start", (char *)"()V", (void *)Java_lang_Thread_start},
        {(char *)"sleep", (char *)"(J)V", (void *)Java_lang_Thread_sleepNanos},
    };

    void Java_lang_Thread::register_natives(JNIEnv *env)
    {
        jclass j_class = env->FindClass("java/lang/Thread");
        env->RegisterNatives(j_class, Thread_methods, sizeof(Thread_methods) / sizeof(JNINativeMethod));
    }

    JNIEXPORT void JNICALL Java_lang_Thread_start(JNIEnv *jniEnv, jobject jobj)
    {
        // 创建新的线程出来
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thread_func, jobj);
        if (ret != 0) {
            return;
        }
    }

    JNIEXPORT void JNICALL Java_lang_Thread_sleepNanos(JNIEnv *, jclass, jlong sleepNanos)
    {
        usleep(sleepNanos * 1000);
    }
} // namespace mini_jvm