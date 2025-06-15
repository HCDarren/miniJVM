#ifndef DAREEN_MINIJVM_RUNTIME_STACKFRAME
#define DAREEN_MINIJVM_RUNTIME_STACKFRAME
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/StackValue.hpp"

namespace mini_jvm
{
    class StackFrame
    {
    friend class JavaRunStack;
    public:
        StackFrame(const u2 local_size, const u2 stack_size);
        ~StackFrame();
        
        void push_int_to_stack(int value);
        int pop_int_from_stack();

        void push_float(float value);
        void push_double(double value);
        
        void load_int(int location);
        void store_int(int location);

        // 打印栈帧数据好调试一点
        void print_stack_frame();

        StackValue* get_data_by(int location);

    private:
        // 上一个栈帧，_sender 哪个方法调用的
        StackFrame* _sender;
        StackValue* _local_slots;
        StackValue* _stack_slots;
        int _local_slots_size;
        int _stack_top_location;
    };
} // namespace name

#endif