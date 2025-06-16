#include "classfile/ClassFileParser.hpp"
#include "classfile/ClassFileStream.hpp"
#include "classfile/constants.hpp"
#include "oops/InstanceKClass.hpp"
#include "oops/ConstantPool.hpp"
#include <cassert>
#include <iostream>
#include "oops/FieldInfo.hpp"
#include "oops/MethodInfo.hpp"
#include "classfile/VmSymbols.hpp"

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

        u2 flags = stream->get_u2();
        u2 this_class_index = stream->get_u2();
        u2 super_class_index = stream->get_u2();
        parse_interfaces(stream, kClass);

        parse_fileds(stream, kClass);

        parse_methods(stream, kClass);
        return kClass;
    }

    void ClassFileParser::parse_interfaces(ClassFileStream *stream, InstanceKClass *kClass)
    {
        u2 interfaces_length = stream->get_u2();
        for (size_t i = 0; i < interfaces_length; i++)
        {
            const u2 interfaces_cp_index = stream->get_u2();
        }
    }

    void ClassFileParser::parse_fileds(ClassFileStream *stream, InstanceKClass *kClass)
    {
        const u2 field_size = stream->get_u2();
        FieldInfo *fields = new FieldInfo[field_size];
        kClass->set_fields(fields);
        kClass->set_field_size(field_size);
        for (size_t i = 0; i < field_size; i++)
        {
            const u2 flags = stream->get_u2();
            const u2 name_index = stream->get_u2();
            const u2 signature_index = stream->get_u2();
            const u2 attribute_count = stream->get_u2();
            // 解析字段属性，不写
            fields[i].set_flags(flags);
            fields[i].set_name_index(name_index);
            fields[i].set_signature_index(signature_index);
            fields[i].set_attribute_count(attribute_count);
        }
    }

    void ClassFileParser::parse_methods(ClassFileStream *stream, InstanceKClass *kClass)
    {
        const u2 method_size = stream->get_u2();
        MethodInfo *methods = new MethodInfo[method_size];
        kClass->set_methods(methods);
        kClass->set_method_size(method_size);
        for (size_t i = 0; i < method_size; i++)
        {
            parse_method(stream, methods + i, kClass->constants());
        }
    }

    void ClassFileParser::parse_method(ClassFileStream *stream, MethodInfo *m, ConstantPool *cp)
    {
        const u2 flags = stream->get_u2();
        const u2 name_index = stream->get_u2();
        const u2 signature_index = stream->get_u2();
        u2 method_attributes_count = stream->get_u2();

        u2 max_stack;
        u2 max_locals;
        u4 code_length;
        u1 *code_start;
        u2 exception_table_length;

        // 解析代码、异常表等信息，这个需要耐心，比较复杂一点
        while (method_attributes_count--)
        {
            const u2 method_attribute_name_index = stream->get_u2();
            const u4 method_attribute_length = stream->get_u4();
            const char *const method_attribute_name = cp->symbol_at(method_attribute_name_index);
            if (strcmp(method_attribute_name, tag_code) == 0)
            {
                max_stack = stream->get_u2();
                max_locals = stream->get_u2();
                code_length = stream->get_u4();
                code_start = stream->current();
                stream->skip_u1(code_length);
                exception_table_length = stream->get_u2();
                u2 code_attributes_count = stream->get_u2();
                while (code_attributes_count--)
                {
                    const u2 code_attribute_name_index = stream->get_u2();
                    const u4 code_attribute_length = stream->get_u4();
                    const char *const code_attribute_name = cp->symbol_at(code_attribute_name_index);
                    // code 属性都不做，直接往后跳
                    stream->skip_u1(code_attribute_length);
                }
            }
            else if (strcmp(method_attribute_name, tag_exceptions) == 0)
            {
                // 后面异常再写，看有没有时间了
            }
        }

        m->set_flags(flags);
        m->set_name_index(name_index);
        m->set_signature_index(signature_index);

        m->set_max_stack(max_stack);
        m->set_max_locals(max_locals);
        m->set_code_length(code_length);
        m->set_code(code_start);
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
            {
                const u8 value = stream->get_u8();
                cp->long_at_put(index, value);
                // long 占两个，由于我们的实现问题，直接要加 1
                index += 1;
                break;
            }
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
            {
                const u2 class_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->interface_method_at_put(index, class_index, signature_index);
                break;
            }
            case JVM_CONSTANT_NameAndType:
            {
                const u2 name_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->name_and_type_at_put(index, name_index, signature_index);
                break;
            }
            break;
            case JVM_CONSTANT_MethodHandle:
            {
                const u2 name_index = stream->get_u1();
                const u2 signature_index = stream->get_u2();
                cp->method_handle_at_put(index, name_index, signature_index);
                break;
            }
            case JVM_CONSTANT_MethodType:

                break;
            case JVM_CONSTANT_Dynamic:

                break;
            case JVM_CONSTANT_InvokeDynamic:
            {
                const u2 name_index = stream->get_u2();
                const u2 signature_index = stream->get_u2();
                cp->invoke_dynamic_at_put(index, name_index, signature_index);
                break;
            }
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