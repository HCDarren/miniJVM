#ifndef DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK
#define DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK
#include <string>

namespace mini_jvm
{
    class StackFrame;
    class MethodInfo;

    class JavaRunStack
    {
    public:
        JavaRunStack();
        ~JavaRunStack();

        void push_frame(StackFrame* stack_frame);
        void pop_frame();

        void push_call_parameters(const std::string& method_signature);

        StackFrame* top_frame() {
            return _current_stack_frame;
        }
        
        void pop_int_result();

    private:
        StackFrame* _current_stack_frame;
    };
} // namespace name

#endif