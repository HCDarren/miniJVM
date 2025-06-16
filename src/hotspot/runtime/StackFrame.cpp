#include "runtime/StackFrame.hpp"
#include <cassert>
#include <iostream>
#include "oops/Oop.hpp"

namespace mini_jvm
{

    StackFrame::StackFrame(const u2 local_size, const u2 stack_size):_sender(nullptr)
    {
        _local_slots_size = local_size;
        _maxt_stack_size = stack_size;
        _stack_top_location = 0;
        _local_slots = new StackValue[local_size];
        _stack_slots = new StackValue[stack_size];
    }

    StackFrame::~StackFrame()
    {
        delete[] _local_slots;
        delete[] _stack_slots;
    }

    void StackFrame::push_to_stack(const StackValue* dststack_value) {
        assert(_stack_top_location < _maxt_stack_size);
        StackValue* dst = &_stack_slots[_stack_top_location++];
        dst->_type = dststack_value->_type;
        dst->_integer_value = dststack_value->_integer_value;
    }

    void StackFrame::push_int_to_stack(int value)
    {
        assert(_stack_top_location < _maxt_stack_size);
        StackValue* stack_value = &_stack_slots[_stack_top_location++];
        stack_value->_type = T_INT;
        stack_value->_integer_value = value;
    }

    StackValue* StackFrame::pop_value_from_stack() {
        assert(_stack_top_location >= 1);
        return &_stack_slots[--_stack_top_location];
    }
    
    void StackFrame::locals_load_to_stack(int location) {
        assert(_stack_top_location < _maxt_stack_size);
        assert(location < _local_slots_size);
        StackValue* src_value = &_local_slots[location];
        StackValue* dst_value = &_stack_slots[_stack_top_location++];
        dst_value->_type = src_value->_type;
        dst_value->_integer_value = src_value->_integer_value;
    }

    void StackFrame::stack_store_to_local(int location) {
        assert(_stack_top_location < _maxt_stack_size);
        assert(location < _local_slots_size);
        StackValue* src_value = &_stack_slots[--_stack_top_location];
        StackValue* dst_value = &_local_slots[location];
        dst_value->_type = src_value->_type;
        dst_value->_integer_value = src_value->_integer_value;
    }

    // 将局部变量槽里的数据 push 到栈里面
    void StackFrame::load_int(int location) {
        assert(_stack_top_location < _maxt_stack_size);
        assert(location < _local_slots_size);
        assert(_sender != NULL);
        StackValue load_stack_value = _local_slots[location];
        push_int_to_stack(load_stack_value._integer_value);
    }

    // 将栈顶的值取出放到 location 位置的局部变量槽里
    void StackFrame::store_int(int location) {
        assert(_stack_top_location < _maxt_stack_size);
        assert(location < _local_slots_size);
        StackValue* top_stack_value = &_stack_slots[--_stack_top_location];
        _local_slots[location]._type = top_stack_value->_type;
        _local_slots[location]._integer_value = top_stack_value->_integer_value;
    }

    StackValue* StackFrame::get_value_by(int location) {
        return &_local_slots[location];
    }

    void StackFrame::push_obj_to_stack(const Oop* oop) {
        assert(_stack_top_location < _maxt_stack_size);
        StackValue* top_stack_value = &_stack_slots[_stack_top_location++];
        top_stack_value->_type = T_OBJECT;
        top_stack_value->_integer_value = (intptr_t)oop;
    }

    void StackFrame::dup_stack_top() {
        assert(_stack_top_location < _maxt_stack_size);
        StackValue* top_stack_value = &_stack_slots[_stack_top_location - 1];
        StackValue* dup_stack_value = &_stack_slots[_stack_top_location++];
        dup_stack_value->_type = top_stack_value->_type;
        dup_stack_value->_integer_value = top_stack_value->_integer_value;
    }
    
    void StackFrame::print_stack_frame() {
#if STACK_DEBUG
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "local data: " << _local_slots_size << std::endl;
        for (int i = 0; i < _local_slots_size; i++)
        {
            std::cout << _local_slots[i]._integer_value << std::endl;
        }

        std::cout << "stack data:" << _stack_top_location << std::endl;
        for (int i = 0; i < _stack_top_location; i++)
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