#ifndef DAREEN_MINIJVM_OS_POSIX_PARK
#define DAREEN_MINIJVM_OS_POSIX_PARK

#include <pthread.h>

namespace mini_jvm
{
    class PlatformEvent
    {
    private:
        pthread_mutex_t _mutex[1]; // Native mutex for locking
        pthread_cond_t _cond[1]; 
        uintptr_t _event;
    public:
        PlatformEvent();
        void park();
        void unpark();
    };
} // namespace name

#endif
