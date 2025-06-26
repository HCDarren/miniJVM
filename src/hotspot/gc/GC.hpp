#ifndef DAREEN_MINIJVM_HOTSPOT_GC_GC
#define DAREEN_MINIJVM_HOTSPOT_GC_GC
#include <set>

namespace mini_jvm
{
    class Oop;
    class GC
    {
    public:
        static void gc();
        static std::set<Oop*> findAllRoots();
    };
} // namespace name

#endif
