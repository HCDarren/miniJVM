#include "runtime/BasicLock.hpp"
#include "oops/Oop.hpp"

namespace mini_jvm
{
    MarkWord BasicLock::displaced_header() const
    {
        return MarkWord(_metadata);
    }

    void BasicLock::set_displaced_header(MarkWord header)
    {
        _metadata = header.value();
    }
} // namespace name