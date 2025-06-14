#ifndef DAREEN_MINIJVM_RUNTIME_STATCKVALUE
#define DAREEN_MINIJVM_RUNTIME_STATCKVALUE

#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{
    class StackValue
    {
        friend class StackFrame;

    public:
        StackValue()
        {
        }
    private:
        BasicType _type;
        intptr_t  _integer_value; // java 的 short char int 都用 int
    };
} // namespace name

#endif