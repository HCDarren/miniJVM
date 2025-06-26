#ifndef DAREEN_MINIJVM_HOTSPOT_GC_CollectedHeap
#define DAREEN_MINIJVM_HOTSPOT_GC_CollectedHeap
#include <set>
#include <pthread.h>

namespace mini_jvm
{
    class Oop;
    class CollectedHeap
    {
    public:
        static Oop* obj_allocate(size_t size);

        static void clearOopsKeepAvaliable(const std::set<Oop*> avaliables);
    };
} // namespace name

#endif
