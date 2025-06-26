#include "oops/InstanceKClass.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
#include "oops/Oop.hpp"
#include "runtime/JavaThread.hpp"
#include "jni/jni.h"
#include <assert.h>
#include "hotspot/gc/CollectedHeap.hpp"

namespace mini_jvm
{

    MethodInfo *InstanceKClass::findMethod(const char *method_name, const char *method_signature)
    {
        for (size_t i = 0; i < _method_size; i++)
        {
            MethodInfo* method_info = _methods + i;
            int name_index = method_info->name_index();
            int signature_index = method_info->signature_index();
            const char *method_name_str = _constants->symbol_at(name_index);
            const char *signature_name_str = _constants->symbol_at(signature_index);
            if (strcmp(method_name_str, method_name) == 0 && strcmp(signature_name_str, method_signature) == 0)
            {
                return method_info;
            }
        }
        return nullptr;
    }

    size_t InstanceKClass::size_helper() {
        // 前面两个用来存 markword 和类对象地址
        return (2 + _field_size) * sizeof(void*);
    }

    void InstanceKClass::initialize() {
        // TODO 多线程要加锁，类只能被初始化一次，后面再说吧
        if (!_is_initialized) {
            _is_initialized = true;
            // 父类好像也要初始化，先也不管
            const MethodInfo* clinit = findMethod("<clinit>", "()V");
            if (clinit != NULL) {
                JavaThread* javaThread = JavaThread::current();
                JNIEnv* jniEnv = javaThread->jniEnv();
                jniEnv->CallStaticVoidMethod((jclass)this, (jmethodID)clinit);
            }
        }
    }

    StackValue* InstanceKClass::get_static_value(const u2 name_index, const u2 name_and_type_index) {
        // TODO 要做 filed 查找匹配，如果自己这里找不到，递归触发父类的设置，全部匹配不到就报错，后面再看吧
        // 简单做直接存了，也有内存泄漏
        u4 key = (name_index << 16) | name_and_type_index;
        return _static_values[key];
    }

    void InstanceKClass::set_static_value(const u2 name_index, const u2 name_and_type_index, StackValue* value) {
        // TODO 要做 filed 查找匹配，如果自己这里找不到，递归触发父类的设置，全部匹配不到就报错，后面再看吧
        // 简单做直接存了，也有内存泄漏
        u4 key = (name_index << 16) | name_and_type_index;
        StackValue* new_value = new StackValue();
        new_value->_type = value->_type;
        new_value->_integer_value = value->_integer_value;
        _static_values[key] = new_value;
    }

    Oop* InstanceKClass::allocate_instance() {
        size_t size = size_helper();
        // TODO 后面进行堆管理，等写 GC 的时候再说吧
        Oop* new_oop = CollectedHeap::obj_allocate(size);
        memset(new_oop, 0, size); // 全部都设置为默认值 0
        new_oop->_make_word._value = MarkWord::unlocked_value;
        new_oop->_metadata = this;
        return new_oop;
    }
}
