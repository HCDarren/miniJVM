#include "runtime/StackFrame.hpp"
#include <cassert>
#include <iostream>

namespace mini_jvm
{

    StackFrame::StackFrame(const u2 local_size, const u2 stack_size):_sender(nullptr)
    {
        _local_slots_size = local_size;
        _stack_top_location = 0;
        _local_slots = new StackValue[local_size];
        _stack_slots = new StackValue[stack_size];
    }

    StackFrame::~StackFrame()
    {
        delete[] _local_slots;
        delete[] _stack_slots;
    }

    void StackFrame::push_int_to_stack(int value)
    {
        StackValue* stack_value = &_stack_slots[_stack_top_location++];
        stack_value->_type = T_INT;
        stack_value->_integer_value = value;
    }

    int StackFrame::pop_int_from_stack() {
        return _stack_slots[--_stack_top_location]._integer_value;
    }
    
    // 将局部变量槽里的数据 push 到栈里面
    void StackFrame::load_int(int location) {
        assert(_sender != NULL);
        StackValue load_stack_value = _local_slots[location];
        push_int_to_stack(load_stack_value._integer_value);
    }

    // 将栈顶的值取出放到 location 位置的局部变量槽里
    void StackFrame::store_int(int location) {
        StackValue* top_stack_value = &_stack_slots[--_stack_top_location];

        _local_slots[location]._type = top_stack_value->_type;
        _local_slots[location]._integer_value = top_stack_value->_integer_value;
    }

    StackValue* StackFrame::get_data_by(int location) {
        return &_local_slots[location];
    }

    void StackFrame::print_stack_frame() {
#if STACK_DEBUG
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "local data:" << std::endl;
        for (size_t i = 0; i < _local_slots_size; i++)
        {
            std::cout << _local_slots[i]._integer_value << std::endl;
        }

        std::cout << "stack data:" << std::endl;
        for (size_t i = 0; i < _stack_top_location; i++)
        {
            std::cout << _stack_slots[i]._integer_value << std::endl;
        }
        
        std::cout << "===================================" << std::endl;
#endif
    }

    void StackFrame::push_float(float value)
    {

    }

    void StackFrame::push_double(double value)
    {

    }

} // namespace name