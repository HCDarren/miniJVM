#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include <unistd.h>
#include "runtime/ThreadLocalStorage.hpp"
#include "runtime/SafepointMechanism.hpp"

namespace mini_jvm
{
    static JavaVM *javaVM;

    JavaThread *JavaVM::current_thread()
    {
        return ThreadLocalStorage::thread();
    }

    JavaVM::JavaVM()
    {
        SafepointMechanism::initialize();
    }

    void JavaVM::attachThread(JavaThread *java_thread)
    {
        ThreadLocalStorage::set_thread(java_thread);
        pthread_t thread_id = pthread_self();
        _thread_infos[thread_id] = java_thread;
    }

    void JavaVM::detachThread(JavaThread *java_thread)
    {
        pthread_t thread_id = pthread_self();
        _thread_infos.erase(thread_id);
    }

    JavaVM *JavaVM::current()
    {
        if (javaVM == NULL)
        {
            javaVM = new JavaVM();
            ThreadLocalStorage::init();
        }
        return javaVM;
    }

    std::set<JavaThread*> JavaVM::getAllThreads() {
        std::set<JavaThread*> all_threads;
        for (auto& it : _thread_infos)
        {
            all_threads.insert(it.second);
        }
        return all_threads;
    }

}