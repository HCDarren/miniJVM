#ifndef DAREEN_MINIJVM_JNI_JNI
#define DAREEN_MINIJVM_JNI_JNI

#define JNIEXPORT
#define JNICALL
#include <string>
#include "utilities/GlobalDefinitions.hpp"
#include <pthread.h>
#include <map>

class _jobject
{
};
class _jclass : public _jobject
{
};
class _string : public _jobject {};

typedef int jint;
typedef long jlong;
typedef unsigned char jboolean;
typedef unsigned short jchar;
typedef short jshort;
typedef float jfloat;
typedef double jdouble;
typedef jint jsize;

typedef _jobject* jobject;
typedef _jclass* jclass;
typedef _jclass* jstring;
struct _jfieldID;
typedef struct _jfieldID *jfieldID;

struct _jmethodID;
typedef struct _jmethodID *jmethodID;

namespace mini_jvm
{

    class InstanceKClass;
    class JavaThread;

    /*
     * used in RegisterNatives to describe native method name, signature,
     * and function pointer.
     */
    typedef struct
    {
        char *name;
        char *signature;
        void *fnPtr;
    } JNINativeMethod;

    class JNIEnv
    {
        public:
            jint RegisterNatives(const jclass clazz, const JNINativeMethod *methods, const jint nMethods);
            jclass GetObjectClass(const jobject jobj);
            jclass FindClass(const std::string class_name);
            jmethodID GetMethod(const jclass clazz, const std::string method_name, const std::string method_signature);
            void CallVoidMethod(const jobject jobj,const jmethodID mid);
            void CallStaticVoidMethod(jclass clazz, const jmethodID mid);
    };

    class JavaVM
    {
        public:
            JavaVM();
            JavaThread* current_thread();
            static JavaVM* current();
            void attachThread(JavaThread* java_thread);
            void detachThread(JavaThread* java_thread);
            std::map<pthread_t, JavaThread*> getAllThreads();
        private:
            std::map<pthread_t, JavaThread*> _thread_infos;
    };
}
#endif