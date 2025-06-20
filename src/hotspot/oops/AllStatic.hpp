#ifndef DAREEN_MINIJVM_OOPS_ALLSTATIC
#define DAREEN_MINIJVM_OOPS_ALLSTATIC

namespace mini_jvm
{
    struct AllStatic
    {
        AllStatic() = delete;
        ~AllStatic() = delete;
    };
}

#endif