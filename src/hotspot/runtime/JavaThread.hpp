#ifndef DAREEN_MINIJVM_OOPS_JAVATHREAD
#define DAREEN_MINIJVM_OOPS_JAVATHREAD

#include "runtime/Thread.hpp"

namespace mini_jvm
{
    class JavaRunStack;
    class InstanceKClass;
    class MethodInfo;
    class JNIEnv;

    class JavaThread : public Thread
    {
    public:
        JavaThread();
        ~JavaThread();
    public:
        static JavaThread* current();

        JavaRunStack* run_java_statck() {
            return _run_java_statck;
        }
        JNIEnv* jniEnv() {
            return _jniEnv;
        }
        void runJavaMethod(MethodInfo* method, InstanceKClass *kClass);
    private:
        JavaRunStack* _run_java_statck;
        JNIEnv* _jniEnv;
    };
} // namespace name

#endif