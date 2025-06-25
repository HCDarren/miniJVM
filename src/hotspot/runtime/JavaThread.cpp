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
        JavaThread *current_thread = JavaVM::current()->current_thread();
        assert(current_thread != NULL);
        return current_thread;
    }

    JavaThread::JavaThread()
    {
        _run_java_statck = new JavaRunStack();
        _jniEnv = new JNIEnv();
        _interpreter = new BytecodeInterpreter(this);
        _ParkEvent = new PlatformEvent();
    }

    bool JavaThread::is_lock_owned(address adr)
    {
        // TODO：这个蛋疼了，adr 是不是当前栈上开辟的地址，目前由于没有做栈逻辑
        BasicLock *top_basicLock = _run_java_statck->top_frame()->top_basicLock();
        return (void *)adr == top_basicLock;
    }

    void JavaThread::park()
    {
        _ParkEvent->park();
    }

    void JavaThread::unpark()
    {
        _ParkEvent->unpark();
    }

    JavaThread::~JavaThread()
    {
        delete _run_java_statck;
        delete _jniEnv;
        delete _interpreter;
    }

} // namespace name