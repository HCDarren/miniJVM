#include "classfile/ClassLoader.hpp"
#include "classfile/ClassFileStream.hpp"
#include "classfile/ClassFileParser.hpp"
#include "oops/InstanceKClass.hpp"

namespace mini_jvm
{
    // 加载解析 class
    InstanceKClass *ClassLoader::load_class(const std::string &class_name)
    {
        ClassFileStream* stream = new ClassFileStream(class_name);
        ClassFileParser parser;
        InstanceKClass* KClass = parser.create_parser_kclass(stream);
        // 可以考虑写一个跨平台的智能指针，本工程只是用来学习，就先简单处理了
        delete stream;
        return KClass;
    }
} // namespace name