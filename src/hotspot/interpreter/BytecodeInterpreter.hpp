#ifndef DAREEN_MINIJVM_INTERPRETER_BYTECODEINTERPRETER
#define DAREEN_MINIJVM_INTERPRETER_BYTECODEINTERPRETER

#include "oops/MethodInfo.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/JavaRunStack.hpp"
#include <map>

namespace mini_jvm
{
    class MethodInfo;
    class InstanceKClass;
    class JNIEnv;
    class JavaRunStack;

    class BytecodeInterpreter
    {
    public:
        void invoke(const MethodInfo *method, const InstanceKClass* kClass);
        void invokeJavaMethod(const MethodInfo *method, const InstanceKClass* kClass);
        void invokeNativeMethod(const MethodInfo *method, const InstanceKClass* kClass);
        void invokevirtualMethod(const u1 class_index, const u1 name_and_type_index, const InstanceKClass* kClass);
        void invokeStaticMethod(const u1 class_index, const u1 name_and_type_index, const InstanceKClass* kClass);
        
    public:
        BytecodeInterpreter();
        ~BytecodeInterpreter();
    private:
        JNIEnv* _jniEnv;
        JavaRunStack* java_run_stack;
    };    
} // namespace mini_jvm


#endif