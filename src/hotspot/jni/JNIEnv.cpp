#include "jni/jni.h"
#include "oops/InstanceKClass.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/JavaThread.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
#include "classfile/ClassLoader.hpp"
#include "oops/Oop.hpp"
#include "runtime/StackFrame.hpp"

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

    jclass JNIEnv::GetObjectClass(const jobject jobj) {
        Oop* oop = (Oop*)jobj;
        jclass clazz = (jclass)oop->metadata_class();
        return clazz;
    }

    jclass JNIEnv::FindClass(const std::string class_name) {
        InstanceKClass* kClass = mini_jvm::ClassLoader::load_class(class_name);
        kClass->initialize();
        return (jclass)kClass;
    }
    
    jmethodID JNIEnv::GetMethod(const jclass clazz, const std::string method_name, const std::string method_signature) {
        InstanceKClass* kClass = (InstanceKClass*)clazz;
        return (jmethodID)kClass->findMethod(method_name.c_str(), method_signature.c_str());
    }

    void JNIEnv::CallVoidMethod(const jobject jobj, const jmethodID mid) {
        JavaThread* javaThread = JavaThread::current();
        // 非静态方法要把 jobj 传递过去，不然跑不起来，创建新栈帧
        StackFrame* native_stack_frame = new StackFrame(0, 1);
        javaThread->run_java_statck()->push_frame(native_stack_frame);
        javaThread->run_java_statck()->top_frame()->push_obj_to_stack((Oop*)jobj);
        BytecodeInterpreter* interpreter = javaThread->interpreter();
        interpreter->invoke((MethodInfo*)mid, (InstanceKClass*)GetObjectClass(jobj));
        javaThread->run_java_statck()->pop_frame();
        delete native_stack_frame;
    }

    void JNIEnv::CallStaticVoidMethod(jclass clazz, const jmethodID mid) {
        JavaThread* javaThread = JavaThread::current();
        BytecodeInterpreter* interpreter = javaThread->interpreter();
        interpreter->invoke((MethodInfo*)mid, (InstanceKClass*)clazz);
    }
}