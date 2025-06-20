#ifndef DAREEN_MINIJVM_RUNTIME_OBJECTSYNCHRONIZER
#define DAREEN_MINIJVM_RUNTIME_OBJECTSYNCHRONIZER
#include "oops/AllStatic.hpp"
#include "oops/Oop.hpp"
#include "runtime/BasicLock.hpp"
#include "runtime/JavaThread.hpp"

namespace mini_jvm
{
    class ObjectSynchronizer : AllStatic
    {
    public:
        static void enter(Oop *obj, BasicLock *lock, JavaThread *current);
        static void exit(Oop *obj, BasicLock *lock, JavaThread *current);
    };
} // namespace name

#endif