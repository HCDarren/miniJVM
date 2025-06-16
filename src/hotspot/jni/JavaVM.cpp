#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include <unistd.h>

namespace mini_jvm
{
    static JavaVM* javaVM;

    JavaThread* JavaVM::current_thread() {
        pthread_t thread_id = pthread_self();
        return _thread_infos[thread_id];
    }

    void JavaVM::attachThread(JavaThread* java_thread) {
        pthread_t thread_id = pthread_self();
        _thread_infos[thread_id] = java_thread;
    }

    JavaVM* JavaVM::current() {
        if (javaVM == NULL) {
            javaVM = new JavaVM();
        }
        return javaVM;
    }
}