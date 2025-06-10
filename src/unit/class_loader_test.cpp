#include "unit/class_loader_test.hpp"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include "oops/ConstantPool.hpp"
#include "classfile/constants.hpp"
#include <iostream>
#include <cassert>
#include <iomanip>
#include <sstream>

using namespace mini_jvm;

// 格式化字符串，不足三位前面补空格
std::string formatWithSpace(const std::string& str) {
    std::stringstream oss;
    oss  << str;  // 输出到字符串流
    return oss.str();
}

void test_print_constant_pool(InstanceKClass* kClass) {
    ConstantPool* cp =  kClass->constants();
    std::cout << "Constant pool:" << std::endl;
    for (size_t index = 1; index < cp->size(); index++)
    {
        u1 tag = cp->tag_at(index);
        std::string str = "#" + std::to_string(index);
        std::cout << std::setw(3) << str << " = ";
        switch (tag)
        {
            case JVM_CONSTANT_Utf8:
            {
                std::cout << "Utf8               ";
                const char* str = cp->symbol_at(index);
                std::cout << str << std::endl;
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
                std::cout << "Class              ";
                u8 class_index = cp->kclass_index_at(index);
                std::cout << "#" << class_index << std::endl;
                break;
            }
            case JVM_CONSTANT_String:
            {
                std::cout << "String             ";
                u8 string_index = cp->string_index_at(index);
                std::cout << "#" << string_index << std::endl;
                break;
            }
            
            case JVM_CONSTANT_Fieldref:
            {
                std::cout << "Fieldref           ";
                u8 filed = cp->filed_at(index);
                std::cout << "#" << (filed & 0xFF) << "." << "#" << (filed >> 16) << std::endl;
                break;
            }
            
            case JVM_CONSTANT_Methodref:
            {
                std::cout << "Methodref          ";
                u8 method = cp->method_at(index);
                std::cout << "#" << (method & 0xFF) << "." << "#" << (method >> 16) << std::endl;
                break;
            }
            case JVM_CONSTANT_InterfaceMethodref:

                break;
            case JVM_CONSTANT_NameAndType:
            {
                std::cout << "NameAndType        ";
                u8 name_and_type = cp->name_and_type_at(index);
                std::cout << "#" << (name_and_type & 0xFF) << ":" << "#" << (name_and_type >> 16) << std::endl;
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

void test_class_loader() {
    InstanceKClass* kClass = mini_jvm::ClassLoader::load_class("/Users/darrenzeng/Desktop/Darren/Study/miniJVM/src/MiniJVM.class");
    test_print_constant_pool(kClass);
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
}