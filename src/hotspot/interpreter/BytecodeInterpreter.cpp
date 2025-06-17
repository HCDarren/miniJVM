
#include "interpreter/BytecodeInterpreter.hpp"
#include "oops/MethodInfo.hpp"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include "classfile/constants.hpp"
#include "utilities/bytes_arm.hpp"
#include <iostream>
#include <cassert>
#include "jni/Java_io_PrintStream.h"
#include "jni/Java_lang_Thread.h"
#include "cpu/aarch64/AssemblerAarch64.hpp"
#include "runtime/JavaRunStack.hpp"
#include "runtime/JavaThread.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/Signature.hpp"
#include "oops/Oop.hpp"
#include "jni/jni.h"

#define op_iconst_0 3
#define op_iconst_2 5
#define op_iconst_3 6
#define op_bipush 16
#define op_sipush 17
#define op_ldc 18
#define op_ldc2_w 20
#define op_iload 21
#define op_aload 25
#define op_iload_0 26
#define op_iload_1 27
#define op_iload_2 28
#define op_iload_3 29
#define op_aload_0 42
#define op_aload_1 43
#define op_aload_2 44
#define op_istore 54
#define op_astore 58
#define op_istore_0 59
#define op_istore_1 60
#define op_istore_2 61
#define op_istore_3 62
#define op_astore_1 76
#define op_astore_2 77
#define op_dup 89
#define op_iadd 96
#define op_imul 104
#define op_iinc 132
#define op_if_icmpge 162
#define op_goto 167
#define op_ireturn 172
#define op_return 177
#define op_getstatic 178
#define op_putstatic 179
#define op_getfield 180
#define op_putfield 181
#define op_invokevirtual 182
#define op_invokespecial 183
#define op_invokestatic 184
#define invokeinterface 185
#define op_new 187
#define op_ifnull 198

namespace mini_jvm
{
    BytecodeInterpreter::BytecodeInterpreter(JavaThread *java_thread)
    {
        _jniEnv = java_thread->jniEnv();
        java_run_stack = java_thread->run_java_statck();
        // 注册 navtive 层的调用函数
        Java_io_PrintStream::register_natives(_jniEnv);
        Java_lang_Thread::register_natives(_jniEnv);
    }

    void BytecodeInterpreter::invoke(const MethodInfo *method, InstanceKClass *kClass)
    {
        
        StackFrame *frame = new StackFrame(method->max_locals(), method->max_stack());
        java_run_stack->push_frame(frame);

        // 把调用者的参数透传到目标方法的栈里面。栈拷贝
        // 栈 -> 栈，通过方法签名可以获取到参数
        java_run_stack->push_call_parameters(method, kClass);

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
    void BytecodeInterpreter::invokeJavaMethod(const MethodInfo *method, InstanceKClass *kClass)
    {
        u1 *code = method->code();
        u4 code_length = method->code_length();
        for (size_t op = 0; op < code_length; op++)
        {
            java_run_stack->top_frame()->print_stack_frame();
            // 遍历指令集操作码，先简单实现再说，后面再做代码优化
            int instruct_code = code[op];
#if EXEC_INSTRUCT_DEBUG
            std::cout << "exec instruct_code: " << instruct_code << std::endl;
#endif
            switch (instruct_code)
            {
            case op_astore:
            {
                const u1 index = code[++op];
                java_run_stack->top_frame()->stack_store_to_local(index);
                break;
            } 
            case op_astore_1:
            {
                java_run_stack->top_frame()->stack_store_to_local(1);
                break;
            } 
            case op_astore_2:
            {
                java_run_stack->top_frame()->stack_store_to_local(2);
                break;
            } 
            case op_aload:
            {
                const u1 index = code[++op];
                java_run_stack->top_frame()->locals_load_to_stack(index);
                break;
            }
            case op_aload_0:
            {
                java_run_stack->top_frame()->locals_load_to_stack(0);
                break;
            }
            case op_aload_1:
            {
                java_run_stack->top_frame()->locals_load_to_stack(1);
                break;
            }
            case op_aload_2:
            {
                java_run_stack->top_frame()->locals_load_to_stack(2);
                break;
            }
            case op_invokespecial:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                assert(kClass->constants()->tag_at(index) == JVM_CONSTANT_Methodref);
                u8 method_ref = kClass->constants()->method_at(index);
                u1 class_index = method_ref & 0xFF;
                u1 name_and_type_index = method_ref >> 16;
                invokespecialMethod(class_index, name_and_type_index, kClass);
                op += 2;
                break;
            }
            case op_putfield:
            {
                intptr_t value = java_run_stack->top_frame()->pop_value_from_stack()->value();
                Oop* obj = (Oop*)java_run_stack->top_frame()->pop_value_from_stack()->value();
                // TODO 给对象属性设置值，后面 gc 还要调整
                const u2 filed_index = Bytes::get_Java_u2(code + op + 1);
                obj->set_filed_value(filed_index, value);
                op += 2;
                break;
            }
            case op_getfield:
            {  
                Oop* obj = (Oop*)java_run_stack->top_frame()->pop_value_from_stack()->value();
                const u2 filed_index = Bytes::get_Java_u2(code + op + 1);
                StackValue value = obj->get_filed_value(filed_index);
                java_run_stack->top_frame()->push_to_stack(&value);
                op += 2;
                break;
            }
            case op_bipush:
            {
                int value = code[++op];
                // 将 value 压入栈顶
                java_run_stack->top_frame()->push_int_to_stack(value);
                break;
            }
            case op_sipush:
            {
                const u2 value = Bytes::get_Java_u2(code + op + 1);
                // 将 value 压入栈顶
                java_run_stack->top_frame()->push_int_to_stack(value);
                op += 2;
                break;
            }
            case op_istore:
            {
                const u1 index = code[++op];
                java_run_stack->top_frame()->store_int(index);
                break;
            }
            case op_istore_0:
            {
                java_run_stack->top_frame()->store_int(0);
                break;
            }
            case op_istore_1:
            {
                java_run_stack->top_frame()->store_int(1);
                break;
            }
            case op_istore_2:
            {
                java_run_stack->top_frame()->store_int(2);
                break;
            }
            case op_istore_3:
            {
                java_run_stack->top_frame()->store_int(3);
                break;
            }
            case op_getstatic:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                u8 filed_ref = kClass->constants()->method_at(index);
                u1 name_index = filed_ref & 0xFF;
                u1 name_and_type_index = filed_ref >> 16;
                StackValue* value = kClass->get_static_value(name_index, name_and_type_index);
                java_run_stack->top_frame()->push_to_stack(value);
                op += 2;
                break;
            }
            case op_putstatic:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                StackValue* value = java_run_stack->top_frame()->pop_value_from_stack();
                StackValue new_value = *value;
                u8 filed_ref = kClass->constants()->method_at(index);
                u1 name_index = filed_ref & 0xFF;
                u1 name_and_type_index = filed_ref >> 16;
                kClass->set_static_value(name_index, name_and_type_index, &new_value);
                op += 2;
                break;
            }
            case op_ldc:
            {
                const u1 index = code[++op];
                // 将该常量的值（或引用）压入当前线程的操作数栈顶部。
                break;
            }
            case op_ldc2_w:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                const u1 tag = kClass->constants()->tag_at(index);
                StackValue dststack_value;
                if (tag == JVM_CONSTANT_Long) { // long
                    dststack_value._type = T_LONG;
                } else if (tag == JVM_CONSTANT_Double) { // double
                    dststack_value._type = T_DOUBLE;
                } else {
                    assert(false);
                }
                dststack_value._integer_value = kClass->constants()->value_at(index);
                java_run_stack->top_frame()->push_to_stack(&dststack_value);
                op += 2;
                // 将该常量的值（或引用）压入当前线程的操作数栈顶部。
                break;
            }
            case op_invokevirtual:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                assert(kClass->constants()->tag_at(index) == JVM_CONSTANT_Methodref);
                u8 method_ref = kClass->constants()->method_at(index);
                u1 class_index = method_ref & 0xFF;
                u1 name_and_type_index = method_ref >> 16;
                invokevirtualMethod(class_index, name_and_type_index, kClass);
                op += 2;
                break;
            }
            case op_return:
            {
                break;
            }
            case op_iconst_0:
            {
                java_run_stack->top_frame()->push_int_to_stack(0);
                break;
            }
            case op_iconst_2:
            {
                java_run_stack->top_frame()->push_int_to_stack(2);
                break;
            }
            case op_iconst_3:
            {
                java_run_stack->top_frame()->push_int_to_stack(3);
                break;
            }
            case op_iload:
            {
                const u1 index = code[++op];
                java_run_stack->top_frame()->load_int(index);
                break;
            }
            case op_iload_0:
            {
                java_run_stack->top_frame()->load_int(0);
                break;
            }
            case op_iload_1:
            {
                java_run_stack->top_frame()->load_int(1);
                break;
            }
            case op_iload_2:
            {
                java_run_stack->top_frame()->load_int(2);
                break;
            }
            case op_iload_3:
            {
                java_run_stack->top_frame()->load_int(3);
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
                op += 2;
                break;
            }
            case invokeinterface:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                assert(kClass->constants()->tag_at(index) == JVM_CONSTANT_InterfaceMethodref);
                u8 method_ref = kClass->constants()->interface_method_at(index);
                u1 class_index = method_ref & 0xFF;
                u1 name_and_type_index = method_ref >> 16;
                invokeInterfaceMethod(class_index, name_and_type_index, kClass);
                op += 4; // 直接跳四个
                break;
            }
            case op_iadd:
            {
                int number1 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                int number2 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                int sum = number1 + number2;
                java_run_stack->top_frame()->push_int_to_stack(sum);
                break;
            }
            case op_imul:
            {
                int number1 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                int number2 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                int imul = number1 * number2;
                java_run_stack->top_frame()->push_int_to_stack(imul);
                break;
            }
            case op_iinc:
            {
                int index = code[++op];
                int inc = code[++op];
                StackValue* stack_value = java_run_stack->top_frame()->get_value_by(index);
                stack_value->_integer_value += inc;
                break;
            }
            case op_if_icmpge:
            {
                const u2 goto_index = Bytes::get_Java_u2(code + op + 1);
                int number2 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                int number1 = java_run_stack->top_frame()->pop_value_from_stack()->value();
                if (number1 >= number2) {
                    op += goto_index - 1;
                } else {
                    op += 2;
                }
                break;
            }
            case op_goto:
            {
                u2 goto_index = Bytes::get_Java_u2(code + op + 1);
                // 这是个坑... 查了很久
                goto_index += (u2)op;
                op = goto_index - 1;
                break;
            }
            case op_ireturn:
            {
                java_run_stack->pop_int_result();
                break;
            }
            case op_new:
            {
                const u2 index = Bytes::get_Java_u2(code + op + 1);
                const u2 class_index = kClass->constants()->kclass_index_at(index);
                const char* class_name = kClass->constants()->symbol_at(class_index);
                invokeNew(std::string(class_name), kClass);
                op += 2;
                break;
            }
            case op_ifnull:
            {
                const u2 offset = Bytes::get_Java_u2(code + op + 1);
                intptr_t value = java_run_stack->top_frame()->pop_value_from_stack()->value();
                if (value != 0) {
                    // 不等于 null 跳转到目标位置
                    op = offset - 1;
                } else {
                    op += 2;
                }
                break;
            }
            case op_dup:
            {
                java_run_stack->top_frame()->dup_stack_top();
                break;
            }
            default:
                assert(false);
                break;
            }
        }
    }

    void BytecodeInterpreter::invokeNew(const std::string &class_name, InstanceKClass *kClass)
    {
        InstanceKClass *newClass = ClassLoader::load_class(class_name);
        // new 指令触发类的初始化机制
        newClass->initialize();
        // TODO: 在堆上开辟内存，我先不管，后面 GC 再说
        const Oop* obj = newClass->allocate_instance();
        java_run_stack->top_frame()->push_obj_to_stack(obj);
    }

    void BytecodeInterpreter::invokeNativeMethod(const MethodInfo *method, InstanceKClass *kClass)
    {
        address native_function = method->native_function();
        assert(native_function != NULL);
        // native 方法有了，怎么执行呢？尼玛, 找了一圈 jdk 源码也看不懂
        // 看样子只能用汇编调用的那一套来搞了，或者自己写一套规则来搞，还是突破一下用汇编吧
        java_run_stack->top_frame()->print_stack_frame();
        const char *signature_str = kClass->constants()->symbol_at(method->signature_index());
        std::tuple<int, std::vector<int>> parameterAndReturn = Signature::parseParameterAndReturn(signature_str);
        int parameter_size = std::get<1>(parameterAndReturn).size();
    
        // JNIEnv, jclass/jobject 后面才跟参数
        int index = 0;
        if (method->flags() & ACC_STATIC) {
            tmit_address_param1(kClass);
        } else {
            StackValue* object = java_run_stack->top_frame()->get_value_by(index++);
            tmit_address_param1((void*)object->value());
        }
        
        for (int i = index; i < parameter_size; i++)
        {
            StackValue* object = java_run_stack->top_frame()->get_value_by(i);
            // TODO 后面开始第二个参数，后面想办法怎么写得通用，这是个 bug 代码
            tmit_int_param2(object->value());
        }

        // x0 传来传去必须放最后，不然一切白干
        tmit_address_param0(_jniEnv); 
        __asm__ volatile(
            "ldr x20, %[func_ptr]\n"
            "blr x20\n"
            :
            : [func_ptr] "m"(native_function));
    }

    void BytecodeInterpreter::invokeInterfaceMethod(const u1 class_index, const u1 name_and_type_index, InstanceKClass *kClass) {
        StackValue* stack_value = java_run_stack->top_frame()->pop_value_from_stack();
        // assert(stack_value->type() == T_OBJECT);
        Oop* oop = (Oop*)stack_value->value();
        java_run_stack->top_frame()->push_obj_to_stack(oop);
        // TODO 要递归去找方法，这里也没有做了
        InstanceKClass* tartget_Class = oop->metadata_class();
        u8 name_and_type = kClass->constants()->name_and_type_at(name_and_type_index);
        int target_name_index = (name_and_type & 0xFF);
        int target_signature_index = (name_and_type >> 16);
        const char *target_method_name_str = kClass->constants()->symbol_at(target_name_index);
        const char *target_signature_name_str = kClass->constants()->symbol_at(target_signature_index);
        MethodInfo *method = tartget_Class->findMethod(target_method_name_str, target_signature_name_str);
        invoke(method, tartget_Class);
    }

    void BytecodeInterpreter::invokeStaticMethod(const u1 class_index, const u1 name_and_type_index, InstanceKClass *kClass)
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

    void BytecodeInterpreter::invokespecialMethod(const u1 class_index, const u1 name_and_type_index, InstanceKClass* kClass) {
        int class_name_index = kClass->constants()->kclass_index_at(class_index);
        const char *class_name = kClass->constants()->symbol_at(class_name_index);
        if (strcmp(class_name, "java/lang/Object") == 0) {
            // TODO Object 的初始化不走，不然死循环了，后面再弄，但是会传一个参数，需要 pop
            java_run_stack->top_frame()->pop_value_from_stack();
            return;
        }
        // TODO->这里要用 appClassLoader 去加载，还有双亲委派，也后面再看吧
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
    void BytecodeInterpreter::invokevirtualMethod(const u1 class_index, const u1 name_and_type_index, InstanceKClass *kClass)
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