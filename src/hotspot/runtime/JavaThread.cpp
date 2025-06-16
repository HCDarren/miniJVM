#include "runtime/JavaThread.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/JavaRunStack.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
#include "jni/jni.h"

namespace mini_jvm
{

    static JavaThread *_current = nullptr;

    JavaThread *JavaThread::current()
    {
        if (_current == nullptr)
        {
            _current = new JavaThread();
        }
        return _current;
    }

    void JavaThread::runJavaMethod(MethodInfo *method, InstanceKClass *kClass)
    {
        BytecodeInterpreter interpreter;
        interpreter.invoke(method, kClass);
    }

    JavaThread::JavaThread()
    {
        _run_java_statck = new JavaRunStack();
        _jniEnv = new JNIEnv();
    }

    JavaThread::~JavaThread()
    {
        delete _run_java_statck;
        delete _jniEnv;
    }

} // namespace name