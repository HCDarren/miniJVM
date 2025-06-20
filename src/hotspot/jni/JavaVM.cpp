#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include <unistd.h>
#include "runtime/ThreadLocalStorage.hpp"

namespace mini_jvm
{
    static JavaVM* javaVM;

    JavaThread* JavaVM::current_thread() {
        return ThreadLocalStorage::thread();
    }

    void JavaVM::attachThread(JavaThread* java_thread) {
         ThreadLocalStorage::set_thread(java_thread);
    }

    JavaVM* JavaVM::current() {
        if (javaVM == NULL) {
            javaVM = new JavaVM();
            ThreadLocalStorage::init();
        }
        return javaVM;
    }
}