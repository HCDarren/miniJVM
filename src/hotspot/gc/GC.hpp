#ifndef DAREEN_MINIJVM_HOTSPOT_GC_GC
#define DAREEN_MINIJVM_HOTSPOT_GC_GC

#include <pthread.h>

namespace mini_jvm
{
    class GC
    {
    public:
        static void gc();
    };
} // namespace name

#endif
