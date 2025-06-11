#ifndef DAREEN_MINIJVM_CLASSFILEPARSER
#define DAREEN_MINIJVM_CLASSFILEPARSER
#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{
    class InstanceKClass;
    class ClassFileStream;
    class MethodInfo;
    class ConstantPool;

    class ClassFileParser {
        public:
            ClassFileParser();

            InstanceKClass* create_parser_kclass(ClassFileStream* stream);

            void parse_constant_pool(const u2 cp_size, ClassFileStream* stream, InstanceKClass* kClass);

            void parse_interfaces(ClassFileStream* stream, InstanceKClass* kClass);

            void parse_fileds(ClassFileStream* stream, InstanceKClass* kClass);

            void parse_methods(ClassFileStream* stream, InstanceKClass* kClass);

            void parse_method(ClassFileStream* stream, MethodInfo* method_info, ConstantPool* cp);
        };
} // namespace name

#endif