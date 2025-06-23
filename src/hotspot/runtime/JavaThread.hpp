#ifndef DAREEN_MINIJVM_OOPS_JAVATHREAD
#define DAREEN_MINIJVM_OOPS_JAVATHREAD

#include <pthread.h>
#include "runtime/StackFrame.hpp"
#include "hotspot/os/posix/park_posix.hpp"

namespace mini_jvm
{
    class JavaRunStack;
    class InstanceKClass;
    class MethodInfo;
    class JNIEnv;
    class BytecodeInterpreter;

    class JavaThread
    {
        friend class ObjectMonitor;
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

        BytecodeInterpreter* interpreter() {
            return _interpreter;
        }

        bool is_lock_owned(address adr);

    private:
        JavaRunStack* _run_java_statck;
        JNIEnv* _jniEnv;
        pthread_t _tid;
        BytecodeInterpreter* _interpreter;
        PlatformEvent* _ParkEvent;
    };
} // namespace name

#endif