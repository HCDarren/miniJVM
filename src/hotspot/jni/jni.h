#ifndef DAREEN_MINIJVM_JNI_JNI
#define DAREEN_MINIJVM_JNI_JNI

#define JNIEXPORT
#define JNICALL
#include <string>
#include "utilities/GlobalDefinitions.hpp"

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

namespace mini_jvm
{

    class InstanceKClass;

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
            jint RegisterNatives(jclass clazz, const JNINativeMethod *methods, jint nMethods);
    };
}
#endif