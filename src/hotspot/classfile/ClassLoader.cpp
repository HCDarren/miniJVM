#include "classfile/ClassLoader.hpp"
#include "classfile/ClassFileStream.hpp"
#include "classfile/ClassFileParser.hpp"
#include "oops/InstanceKClass.hpp"


namespace mini_jvm
{
    static std::unordered_map<std::string, InstanceKClass*> _classes;

    // 加载解析 class
    InstanceKClass *ClassLoader::load_class(const std::string &class_name)
    {
        auto cached_class_it = _classes.find(class_name);
        if (cached_class_it != _classes.end()) {
            return cached_class_it->second;
        }

        ClassFileStream* stream = new ClassFileStream(class_name);
        ClassFileParser parser;
        InstanceKClass* KClass = parser.create_parser_kclass(stream);
        KClass->set_class_name(class_name);
        // 可以考虑写一个跨平台的智能指针，本工程只是用来学习，就先简单处理了
        delete stream;
        _classes[class_name] = KClass;
        return KClass;
    }
} // namespace name