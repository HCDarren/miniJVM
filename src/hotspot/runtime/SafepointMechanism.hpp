#ifndef DAREEN_MINIJVM_RUNTIME_SAFEPOINTMECHANISM
#define DAREEN_MINIJVM_RUNTIME_SAFEPOINTMECHANISM
#include "oops/AllStatic.hpp"

namespace mini_jvm
{
    class PlatformEvent;

    class SafepointMechanism
    {
    public:
        SafepointMechanism();

        static void initialize();

        static void begin();

        static void end();

        static void check();
    };
}

#endif