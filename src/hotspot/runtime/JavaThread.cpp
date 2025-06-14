#include "runtime/JavaThread.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/JavaRunStack.hpp"
#include "interpreter/BytecodeInterpreter.hpp"

namespace mini_jvm
{
    void JavaThread::runJavaMethod(MethodInfo* method, const InstanceKClass *kClass) {
        BytecodeInterpreter interpreter;
        interpreter.invoke(method, kClass);
    }

    JavaThread::JavaThread() {
        _run_java_statck = new JavaRunStack();
    }

    JavaThread::~JavaThread() {
        delete _run_java_statck;
    }

    
} // namespace name