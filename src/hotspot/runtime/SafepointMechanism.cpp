#include "runtime/SafepointMechanism.hpp"
#include <atomic>
#include <unistd.h>
#include "os/posix/park_posix.hpp"
#include "jni/jni.h"
#include "runtime/JavaThread.hpp"
#include <semaphore.h>
#include <iostream>

static address _polling_page;

namespace mini_jvm
{
    static std::atomic<int> enter_safe_point_thread_number;
    static std::atomic<bool> is_enter_safe_point;
    static SafepointMechanism *_SafepointMechanism = NULL;
    static sem_t _semaphore;

    SafepointMechanism::SafepointMechanism()
    {
        enter_safe_point_thread_number.store(0);
        is_enter_safe_point.store(false);
    }

    void SafepointMechanism::initialize()
    {
        if (_SafepointMechanism == NULL)
        {
            _SafepointMechanism = new SafepointMechanism();
        }
    }

    void SafepointMechanism::begin()
    {
        is_enter_safe_point.store(true);
        // 用系统自带的信号量来做
        unsigned int count = JavaVM::current()->getAllThreads().size();
        int ret = sem_init(&_semaphore, 0, count);
        do {
            ret = sem_wait(&_semaphore);
        } while (ret != 0 && errno == EINTR);
    }

    void SafepointMechanism::end()
    {
        // 唤醒所有线程退出安全点
        sem_destroy(&_semaphore);
        enter_safe_point_thread_number.store(0);
        is_enter_safe_point.store(false);
        std::set<JavaThread*> all_threads = JavaVM::current()->getAllThreads();
        for (const auto &item : all_threads)
        { 
            item->unpark();
        }
    }

    void SafepointMechanism::check()
    {
        if (is_enter_safe_point.load() == true)
        {
            enter_safe_point_thread_number.fetch_add(1, std::memory_order_seq_cst);
            sem_post(&_semaphore);
            JavaThread *current_thread = JavaThread::current();
            current_thread->park();
        }
    }
}