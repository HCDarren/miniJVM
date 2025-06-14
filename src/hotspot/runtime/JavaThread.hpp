#ifndef DAREEN_MINIJVM_OOPS_JAVATHREAD
#define DAREEN_MINIJVM_OOPS_JAVATHREAD

#include "runtime/Thread.hpp"

namespace mini_jvm
{
    class JavaRunStack;
    class InstanceKClass;
    class MethodInfo;

    class JavaThread : public Thread
    {
    public:
        JavaThread();
        ~JavaThread();
    public:
        JavaRunStack* run_java_statck() {
            return _run_java_statck;
        }

        void runJavaMethod(MethodInfo* method, const InstanceKClass *kClass);
    private:
        JavaRunStack* _run_java_statck;

    };
} // namespace name

#endif