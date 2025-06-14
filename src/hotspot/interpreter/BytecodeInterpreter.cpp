
#include "interpreter/BytecodeInterpreter.hpp"
#include "oops/MethodInfo.hpp"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include "classfile/constants.hpp"
#include "utilities/bytes_arm.hpp"
#include <iostream>
#include <cassert>
#include "jni/Java_io_PrintStream.h"
#include "cpu/aarch64/AssemblerAarch64.hpp"
#include "runtime/JavaRunStack.hpp"
#include "runtime/JavaThread.hpp"
#include "runtime/StackFrame.hpp"

#define op_iconst_2 5
#define op_iconst_3 6
#define op_bipush 16
#define op_sipush 17
#define op_ldc 18
#define op_iload_0 26
#define op_iload_1 27
#define op_iload_2 28
#define op_istore_0 59
#define op_istore_1 60
#define op_istore_2 61
#define op_iadd 96
#define op_imul 104
#define op_ireturn 172
#define op_return 177
#define op_getstatic 178
#define op_invokevirtual 182
#define op_invokestatic 184

namespace mini_jvm
{
    BytecodeInterpreter::BytecodeInterpreter()
    {
        _jniEnv = new JNIEnv();
        Java_io_PrintStream::register_natives(_jniEnv);
        JavaThread *java_thread = new JavaThread();
        java_run_stack = java_thread->run_java_statck();
    }

    void BytecodeInterpreter::invoke(const MethodInfo *method, const InstanceKClass *kClass)
    {
        StackFrame *frame = new StackFrame(method->max_locals(), method->max_stack());
        java_run_stack->push_frame(frame);

        if (method->flags() & ACC_NATIVE)
        {
            invokeNativeMethod(method, kClass);
        }
        else
        {
            invokeJavaMethod(method, kClass);
        }
        // 要把返回结果拷贝到调用者的栈中
        java_run_stack->pop_frame();
        delete frame;
    }

    // 执行方法，用解释器，模板机器码太复杂也不利于学习
    void BytecodeInterpreter::invokeJavaMethod(const MethodInfo *method, const InstanceKClass *kClass)
    {
        u1 *code = method->code();
        u4 code_length = method->code_length();
        for (size_t op = 0; op < code_length; op++)
        {
            java_run_stack->top_frame()->print_stack_frame();
            // 遍历指令集操作码，先简单实现再说，后面再做代码优化
            int instruct_code = code[op];
            std::cout << "exec instruct_code: ";
            switch (instruct_code)
            {
            case op_bipush:
            {
                std::cout << "bipush" << std::endl;
                int value = code[++op];
                // 将 value 压入栈顶
                java_run_stack->top_frame()->push_int_to_stack(value);
                break;
            }
            case op_sipush:
            {
                std::cout << "sipush" << std::endl;
                const u2 value = Bytes::get_Java_u2(code + op + 1);
                // 将 value 压入栈顶
                java_run_stack->top_frame()->push_int_to_stack(value);
                break;
            }
            case op_istore_0:
            {
                std::cout << "istore_0" << std::endl;
                java_run_stack->top_frame()->store_int(0);
                break;
            }
            case op_istore_1:
            {
                std::cout << "istore_1" << std::endl;
                java_run_stack->top_frame()->store_int(1);
                break;
            }
            case op_istore_2:
            {
                std::cout << "istore_2" << std::endl;
                java_run_stack->top_frame()->store_int(2);
                break;
            }
            case op_getstatic:
            {
                std::cout << "getstatic" << std::endl;
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                break;
            }
            case op_ldc:
            {
                std::cout << "ldc" << std::endl;
                const u1 index = *(code + op + 1);
                // 将该常量的值（或引用）压入当前线程的操作数栈顶部。
                break;
            }
            case op_invokevirtual:
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
            case op_return:
            {
                std::cout << "return" << std::endl;
                op += 1;
                break;
            }
            case op_iconst_2:
            {
                std::cout << "op_iconst_2" << std::endl;
                java_run_stack->top_frame()->push_int_to_stack(2);
                break;
            }
            case op_iconst_3:
            {
                java_run_stack->top_frame()->push_int_to_stack(3);
                std::cout << "op_iconst_3" << std::endl;
                break;
            }
            case op_iload_0:
            {
                java_run_stack->top_frame()->load_int(0);
                std::cout << "iload_0" << std::endl;
                break;
            }
            case op_iload_1:
            {
                java_run_stack->top_frame()->load_int(1);
                std::cout << "op_iload_1" << std::endl;
                break;
            }
            case op_iload_2:
            {
                java_run_stack->top_frame()->load_int(2);
                std::cout << "op_iload_2" << std::endl;
                break;
            }
            case op_invokestatic:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                assert(kClass->constants()->tag_at(index) == JVM_CONSTANT_Methodref);
                u8 method_ref = kClass->constants()->method_at(index);
                u1 class_index = method_ref & 0xFF;
                u1 name_and_type_index = method_ref >> 16;
                invokeStaticMethod(class_index, name_and_type_index, kClass);
                std::cout << "op_invokestatic" << std::endl;
                break;
            }
            case op_iadd:
            {
                int number1 = java_run_stack->top_frame()->pop_int_from_stack();
                int number2 = java_run_stack->top_frame()->pop_int_from_stack();
                int sum = number1 + number2;
                java_run_stack->top_frame()->push_int_to_stack(sum);
                std::cout << "op_iadd" << std::endl;
                break;
            }
            case op_imul:
            {
                int number1 = java_run_stack->top_frame()->pop_int_from_stack();
                int number2 = java_run_stack->top_frame()->pop_int_from_stack();
                int imul = number1 * number2;
                java_run_stack->top_frame()->push_int_to_stack(imul);
                std::cout << "imul" << std::endl;
                break;
            }
            case op_ireturn:
            {
                java_run_stack->top_frame()->pop_int_result();
                std::cout << "op_ireturn" << std::endl;
                break;
            }
            }
        }
    }

    void BytecodeInterpreter::invokeNativeMethod(const MethodInfo *method, const InstanceKClass *kClass)
    {
        address native_function = method->native_function();
        // native 方法有了，怎么执行呢？尼玛, 找了一圈 jdk 源码也看不懂
        // 看样子只能用汇编调用的那一套来搞了，或者自己写一套规则来搞，还是突破一下用汇编吧
        // TODO 目前测试，后面要写成自动的
        tmit_address_param0(_jniEnv);
        tmit_address_param1(kClass);
        tmit_int_param2(5);
        __asm__ volatile(
            "ldr x20, %[func_ptr]\n"
            "blr x20\n"
            :
            : [func_ptr] "m"(native_function));
    }

    void BytecodeInterpreter::invokeStaticMethod(const u1 class_index, const u1 name_and_type_index, const InstanceKClass *kClass)
    {
        int class_name_index = kClass->constants()->kclass_index_at(class_index);
        const char *class_name = kClass->constants()->symbol_at(class_name_index);
        // TODO->这里要用 appClassLoader 去加载，还有双亲委派，jni 这些，也后面再看吧
        InstanceKClass *newClass = ClassLoader::load_class(class_name);
        u8 name_and_type = kClass->constants()->name_and_type_at(name_and_type_index);
        int target_name_index = (name_and_type & 0xFF);
        int target_signature_index = (name_and_type >> 16);
        const char *target_method_name_str = kClass->constants()->symbol_at(target_name_index);
        const char *target_signature_name_str = kClass->constants()->symbol_at(target_signature_index);
        MethodInfo *method = newClass->findMethod(target_method_name_str, target_signature_name_str);
        invoke(method, newClass);
    }

    // 这里后面的代码要搬走
    void BytecodeInterpreter::invokevirtualMethod(const u1 class_index, const u1 name_and_type_index, const InstanceKClass *kClass)
    {
        int class_name_index = kClass->constants()->kclass_index_at(class_index);
        const char *class_name = kClass->constants()->symbol_at(class_name_index);
        // TODO->这里要用 appClassLoader 去加载，还有双亲委派，jni 这些，也后面再看吧
        InstanceKClass *newClass = ClassLoader::load_class(class_name);
        u8 name_and_type = kClass->constants()->name_and_type_at(name_and_type_index);
        int target_name_index = (name_and_type & 0xFF);
        int target_signature_index = (name_and_type >> 16);
        const char *target_method_name_str = kClass->constants()->symbol_at(target_name_index);
        const char *target_signature_name_str = kClass->constants()->symbol_at(target_signature_index);
        MethodInfo *method = newClass->findMethod(target_method_name_str, target_signature_name_str);
        invoke(method, newClass);
    }

    BytecodeInterpreter::~BytecodeInterpreter()
    {
    }
}