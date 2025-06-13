#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "utilities/GlobalDefinitions.hpp"

namespace mini_jvm
{

    // TODO: 后面还要改 jclass 的 handle 值转 class
    jint JNIEnv::RegisterNatives(jclass clazz, const JNINativeMethod *methods, jint nMethods)
    {
        InstanceKClass* kClass = (InstanceKClass*)clazz;
        // 遍历所有的 class 匹配方法，给 method 设置一个 native 指针函数
        for (size_t index = 0; index < nMethods; index++)
        {
            const char* native_method_name = methods[index].name;
            const char* native_method_signature = methods[index].signature;
            MethodInfo* method_info = kClass->findMethod(native_method_name, native_method_signature);
            method_info->set_native_function((address)methods[index].fnPtr);
        }
        return 0;
    }
}