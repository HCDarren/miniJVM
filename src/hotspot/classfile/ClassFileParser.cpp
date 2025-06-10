#include "classfile/ClassFileParser.hpp"
#include "classfile/ClassFileStream.hpp"
#include "classfile/constants.hpp"
#include "oops/InstanceKClass.hpp"
#include "oops/ConstantPool.hpp"
#include <cassert>
#include <iostream>

namespace mini_jvm
{
    ClassFileParser::ClassFileParser()
    {
    }

    // 加载解析 class
    InstanceKClass *ClassFileParser::create_parser_kclass(ClassFileStream *stream)
    {
        // 这个 InstanceKClass 好像是放到元空间或者方法区，暂时直接 new 不释放，后面再说吧
        InstanceKClass *kClass = new InstanceKClass();
        // 各种校验要抛异常，后面再说吧
        u4 magic = stream->get_u4();
        assert(magic == JAVA_CLASSFILE_MAGIC);
        u2 mnior_version = stream->get_u2();
        u2 major_version = stream->get_u2();
        // 解析和创建长量池
        u2 cp_size = stream->get_u2();
        assert(cp_size > 1);
        parse_constant_pool(cp_size, stream, kClass);

        return kClass;
    }

    // 解析常量池数据
    void ClassFileParser::parse_constant_pool(const u2 length, ClassFileStream *stream, InstanceKClass *kClass)
    {
        ConstantPool *cp = new ConstantPool(length);
        kClass->set_constants(cp);
        for (int index = 1; index < length; index++)
        {
            const u1 tag = stream->get_u1();
            switch (tag)
            {
            case JVM_CONSTANT_Utf8:
            {
                const u2 utf8_length = stream->get_u2();
                const u1 *utf8_buffer = stream->current();
                char *new_str = (char *)malloc(utf8_length + 1);
                stream->skip_u1(utf8_length);
                memcpy(new_str, utf8_buffer, utf8_length);
                cp->symbol_at_put(index, new_str);
                break;
            }
            case JVM_CONSTANT_Unicode:

                break;
            case JVM_CONSTANT_Integer:

                break;
            case JVM_CONSTANT_Float:

                break;
            case JVM_CONSTANT_Long:

                break;
            case JVM_CONSTANT_Double:

                break;
            case JVM_CONSTANT_Class:
            {
                const u2 name_index = stream->get_u2();
                cp->kclass_index_at_put(index, name_index);
                break;
            }
            case JVM_CONSTANT_String:
            {
                const u2 string_index = stream->get_u2();
                cp->string_index_at_put(index, string_index);
                break;
            }
            case JVM_CONSTANT_Fieldref:
            {
                const u2 class_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->filed_at_put(index, class_index, signature_index);
                break;
            }
            case JVM_CONSTANT_Methodref:
            {
                const u2 class_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->method_at_put(index, class_index, signature_index);
                break;
            }
            case JVM_CONSTANT_InterfaceMethodref:

                break;
            case JVM_CONSTANT_NameAndType:
            {
                const u2 name_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->name_and_type_at_put(index, name_index, signature_index);
                break;
            }
            break;
            case JVM_CONSTANT_MethodHandle:

                break;
            case JVM_CONSTANT_MethodType:

                break;
            case JVM_CONSTANT_Dynamic:

                break;
            case JVM_CONSTANT_InvokeDynamic:

                break;
            case JVM_CONSTANT_Module:

                break;
            case JVM_CONSTANT_Package:

                break;
            default:
                assert(false);
                break;
            }
        }
    }
} // namespace name