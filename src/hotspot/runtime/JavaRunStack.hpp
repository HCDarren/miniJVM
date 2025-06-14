#ifndef DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK
#define DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK

namespace mini_jvm
{
    class StackFrame;

    class JavaRunStack
    {
    public:
        JavaRunStack();
        ~JavaRunStack();

        void push_frame(StackFrame* stack_frame);
        void pop_frame();

        StackFrame* top_frame() {
            return _current_stack_frame;
        }
        
    private:
        StackFrame* _current_stack_frame;
    };
} // namespace name

#endif