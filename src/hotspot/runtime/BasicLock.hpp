#ifndef DAREEN_MINIJVM_RUNTIME_BASICLOCK
#define DAREEN_MINIJVM_RUNTIME_BASICLOCK
#include "oops/Oop.hpp"

namespace mini_jvm
{
    class BasicLock
    {
    private:
        volatile uintptr_t _metadata;
    public:
        BasicLock() {

        }

        inline MarkWord displaced_header() const {
            return MarkWord(_metadata);
        }

        inline void set_displaced_header(MarkWord header) {
           _metadata = header.value();
        }
    };
} // namespace name

#endif