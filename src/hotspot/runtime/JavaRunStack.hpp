#ifndef DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK
#define DAREEN_MINIJVM_RUNTIME_JAVARUNSTACK
#include <string>
#include <set>

namespace mini_jvm
{
    class StackFrame;
    class MethodInfo;
    class InstanceKClass;
    class Oop;

    class JavaRunStack
    {
    public:
        JavaRunStack();
        ~JavaRunStack();

        void push_frame(StackFrame* stack_frame);
        void pop_frame();

        void push_call_parameters(const MethodInfo *method, const InstanceKClass *kClass);

        StackFrame* top_frame() {
            return _current_stack_frame;
        }

        void pop_int_result();

        std::set<Oop*> get_all_statck_oops();

    private:
        StackFrame* _current_stack_frame;
    };
} // namespace name

#endif