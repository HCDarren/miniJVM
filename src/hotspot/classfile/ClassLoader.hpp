#ifndef DAREEN_MINIJVM_CLASSLOADER
#define DAREEN_MINIJVM_CLASSLOADER
#include <string>

namespace mini_jvm
{
    class InstanceKClass;
    class ClassFileStream;

    class ClassLoader
    {
    public:
        static InstanceKClass *load_class(const std::string &class_name);
        static ClassFileStream *open_stream(const std::string &class_name);
    };
} // namespace name

#endif