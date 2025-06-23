#include "hotspot/os/posix/park_posix.hpp"
#include "runtime/Atomic.hpp"

namespace mini_jvm
{
    PlatformEvent::PlatformEvent()
    {
        int status = pthread_cond_init(_cond, NULL);
        status = pthread_mutex_init(_mutex, NULL);
    }

    void PlatformEvent::park()
    {
        // 有虚假唤醒问题，没写了
        int status = pthread_mutex_lock(_mutex);
        status = pthread_cond_wait(_cond, _mutex);
        status = pthread_mutex_unlock(_mutex);
    }

    void PlatformEvent::unpark()
    {
        int status = pthread_mutex_lock(_mutex);
        status = pthread_cond_signal(_cond);
        status = pthread_mutex_unlock(_mutex);
    }

} // namespace name
