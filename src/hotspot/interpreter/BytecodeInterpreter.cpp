
#include "interpreter/BytecodeInterpreter.hpp"
#include "oops/MethodInfo.hpp"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include "classfile/constants.hpp"
#include "utilities/bytes_arm.hpp"
#include <iostream>
#include <cassert>

#define ldc 18
#define return 177
#define getstatic 178
#define invokevirtual 182

namespace mini_jvm
{

    void BytecodeInterpreter::invoke(const MethodInfo *method, const InstanceKClass* kClass) {
        if (method->flags() & ACC_NATIVE) {
            invokeNativeMethod(method, kClass);
        } else {
            invokeJavaMethod(method, kClass);
        }
    }
    
    // 执行方法，用解释器，模板机器码太复杂也不利于学习
    void BytecodeInterpreter::invokeJavaMethod(const MethodInfo *method, const InstanceKClass* kClass)
    {
        u1 *code = method->code();
        u4 code_length = method->code_length();
        for (size_t op = 0; op < code_length; op++)
        {
            // 遍历指令集操作码，先简单实现再说，后面再做代码优化
            int instruct_code = code[op];
            std::cout << "exec instruct_code: ";
            switch (instruct_code)
            {
            case getstatic:
            {
                std::cout << "getstatic" << std::endl;
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                break;
            }
            case ldc:
            {
                std::cout << "ldc" << std::endl;
                const u1 index = *(code + op + 1);
                // 将该常量的值（或引用）压入当前线程的操作数栈顶部。
                break;
            }
            case invokevirtual:
            {
                std::cout << "invokevirtual" << std::endl;
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                assert(kClass->constants()->tag_at(index) == JVM_CONSTANT_Methodref);
                u8 method_ref = kClass->constants()->method_at(index);
                u1 class_index = method_ref & 0xFF;
                u1 name_and_type_index = method_ref >> 16;
                invokevirtualMethod(class_index, name_and_type_index, kClass);
                break;
            }
            case return:
            {
                std::cout << "return" << std::endl;
                op += 1;
                break;
            }
            }
        }
    }

    void BytecodeInterpreter::invokeNativeMethod(const MethodInfo *method, const InstanceKClass* kClass) {
        // 执行 jni 方法打印字符串，需要搞 jni 了
    }
    
    // 这里后面的代码要搬走
    void BytecodeInterpreter::invokevirtualMethod(const u1 class_index, const u1 name_and_type_index, const InstanceKClass* kClass) {
        int class_name_index = kClass->constants()->kclass_index_at(class_index);
        const char* class_name = kClass->constants()->symbol_at(class_name_index);
        // 这里要用 appClassLoader 去加载，还有双亲委派，jni 这些，也后面再看吧
        ClassLoader cl;
        InstanceKClass* newClass = cl.load_class(class_name);
        u8 name_and_type = kClass->constants()->name_and_type_at(name_and_type_index);
        int target_name_index = (name_and_type & 0xFF);
        int target_signature_index = (name_and_type >> 16);
        const char *target_method_name_str = kClass->constants()->symbol_at(target_name_index);
        const char *target_signature_name_str = kClass->constants()->symbol_at(target_signature_index);
        MethodInfo * method = newClass->findMethod(target_method_name_str, target_signature_name_str);
        invoke(method, kClass);
    }

    BytecodeInterpreter::BytecodeInterpreter()
    {
    }

    BytecodeInterpreter::~BytecodeInterpreter()
    {
    }
}