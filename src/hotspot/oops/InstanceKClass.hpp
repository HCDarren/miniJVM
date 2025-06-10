#ifndef DAREEN_MINIJVM_OOPS_INSTANCEKCLASS
#define DAREEN_MINIJVM_OOPS_INSTANCEKCLASS

#include "oops/KClass.hpp"
#include "oops/ConstantPool.hpp"

namespace mini_jvm
{
    class InstanceKClass : public KClass
    {
    protected:
        ConstantPool *_constants;

    public:
        ConstantPool *constants()
        {
            return _constants;
        }

        void set_constants(ConstantPool *c)
        {
            _constants = c;
        }
    };
} // namespace name

#endif