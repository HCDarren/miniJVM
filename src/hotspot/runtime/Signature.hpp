#ifndef DAREEN_MINIJVM_RUNTIME_SIGNATURE
#define DAREEN_MINIJVM_RUNTIME_SIGNATURE

#include <string>

namespace mini_jvm
{
    class Signature
    {
    public:
    Signature() {}

    static std::tuple<int, std::vector<int>> parseParameterAndReturn(const std::string& methodSignature);
    };
} // namespace name

#endif