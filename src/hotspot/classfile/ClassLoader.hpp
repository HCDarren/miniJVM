#ifndef DAREEN_MINIJVM_CLASSLOADER
#define DAREEN_MINIJVM_CLASSLOADER
#include <string>

namespace mini_jvm
{
    class InstanceKClass;

    class ClassLoader {
        public:
             static InstanceKClass* load_class(const std::string& class_name);
        };
} // namespace name

#endif