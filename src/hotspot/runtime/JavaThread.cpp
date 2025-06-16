#include "runtime/JavaThread.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/JavaRunStack.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
#include "jni/jni.h"
#include <cassert>

namespace mini_jvm
{
    JavaThread *JavaThread::current()
    {
        JavaThread* current_thread = JavaVM::current()->current_thread();
        assert(current_thread != NULL);
        return current_thread;
    }

    JavaThread::JavaThread()
    {
        _run_java_statck = new JavaRunStack();
        _jniEnv = new JNIEnv();
        _interpreter = new BytecodeInterpreter(this);
    }

    JavaThread::~JavaThread()
    {
        delete _run_java_statck;
        delete _jniEnv;
        delete _interpreter;
    }

} // namespace name