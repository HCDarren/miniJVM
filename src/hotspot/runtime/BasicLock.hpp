#ifndef DAREEN_MINIJVM_RUNTIME_BASICLOCK
#define DAREEN_MINIJVM_RUNTIME_BASICLOCK
#include <cstdint>

namespace mini_jvm
{
    class MarkWord;
    class BasicLock
    {
    private:
        volatile uintptr_t _metadata;
    public:
        BasicLock() {

        }

        MarkWord displaced_header() const;

        void set_displaced_header(MarkWord header);
    };
} // namespace name

#endif