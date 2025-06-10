#ifndef DAREEN_MINIJVM_CLASSFILEPARSER
#define DAREEN_MINIJVM_CLASSFILEPARSER
#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{
    class InstanceKClass;
    class ClassFileStream;

    class ClassFileParser {
        public:
            ClassFileParser();

            InstanceKClass* create_parser_kclass(ClassFileStream* stream);

            void parse_constant_pool(const u2 cp_size, ClassFileStream* stream, InstanceKClass* kClass);
        };
} // namespace name

#endif