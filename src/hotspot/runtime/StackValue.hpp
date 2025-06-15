#ifndef DAREEN_MINIJVM_RUNTIME_STATCKVALUE
#define DAREEN_MINIJVM_RUNTIME_STATCKVALUE

#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{
    class StackValue
    {
        friend class StackFrame;
        friend class JavaRunStack;

    public:
        StackValue()
        {
        }

        intptr_t value() {
            return _integer_value;
        } 

        BasicType type() {
            return _type;
        } 
    private:
        BasicType _type;
        intptr_t  _integer_value; // java 的 short char int 都用 int
    };
} // namespace name

#endif