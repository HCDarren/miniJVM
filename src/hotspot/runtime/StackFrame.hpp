#ifndef DAREEN_MINIJVM_RUNTIME_STACKFRAME
#define DAREEN_MINIJVM_RUNTIME_STACKFRAME
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/StackValue.hpp"
#include "oops/Oop.hpp"

namespace mini_jvm
{
    class StackFrame
    {
    friend class JavaRunStack;
    public:
        StackFrame(const u2 local_size, const u2 stack_size);
        ~StackFrame();
        
        void push_int_to_stack(int value);
        void push_to_stack(const StackValue* stack_value);
        StackValue* pop_value_from_stack();

        void push_float(float value);
        void push_double(double value);
        
        // 将变量槽里面的数据记载到栈里面
        void locals_load_to_stack(int location);
        // 将栈顶数据加载到变量槽里面
        void stack_store_to_local(int location);

        void load_int(int location);
        void store_int(int location);

        void push_obj_to_stack(const Oop* oop);

        void dup_stack_top();

        // 打印栈帧数据好调试一点
        void print_stack_frame();

        StackValue* get_value_by(int location);

    private:
        // 上一个栈帧，_sender 哪个方法调用的
        StackFrame* _sender;
        StackValue* _local_slots;
        StackValue* _stack_slots;
        int _local_slots_size;
        int _max_stack_size;
        int _stack_top_location;
    };
} // namespace name

#endif