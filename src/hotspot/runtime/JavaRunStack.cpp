#include "runtime/JavaRunStack.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/Signature.hpp"
#include "oops/InstanceKClass.hpp"
#include <cassert>
#include <string>
#include <vector>

namespace mini_jvm
{

    JavaRunStack::JavaRunStack()
    {
        _current_stack_frame = new StackFrame(0, 0);
    }

    JavaRunStack::~JavaRunStack()
    {
    }

    void JavaRunStack::push_frame(StackFrame *stack_frame)
    {
        StackFrame *current_stack_frame = _current_stack_frame;
        stack_frame->_sender = current_stack_frame;
        _current_stack_frame = stack_frame;
    }

    void JavaRunStack::pop_frame()
    {
        assert(_current_stack_frame->_sender != NULL);
        _current_stack_frame = _current_stack_frame->_sender;
    }

    void JavaRunStack::pop_int_result()
    {
        StackValue *stack_top_value = &_current_stack_frame->_stack_slots[--_current_stack_frame->_stack_top_location];
        _current_stack_frame->_sender->push_int_to_stack(stack_top_value->_integer_value);
    }

    std::set<Oop *> JavaRunStack::get_all_statck_oops()
    {
        std::set<Oop *> stack_oops;
        StackFrame *stack_frame = _current_stack_frame;
        while (stack_frame)
        {
            stack_frame->print_stack_frame();
            size_t slots_size = stack_frame->_local_slots_size;
            for (size_t i = 0; i < slots_size; i++)
            {
                StackValue *stack_value = &stack_frame->_local_slots[i];
                if (stack_value->_type == T_OBJECT)
                {
                    stack_oops.insert((Oop *)stack_value->_integer_value);
                }
            }
            // 有可能有些被压到栈里面了，也得加进去
            for (int i = 0; i < stack_frame->_stack_top_location; i++)
            {
                StackValue *stack_value = &stack_frame->_stack_slots[i];
                if (stack_value->_type == T_OBJECT)
                {
                    stack_oops.insert((Oop *)stack_value->_integer_value);
                }
            }

            stack_frame = stack_frame->_sender;
        }
        return stack_oops;
    }

    void JavaRunStack::push_call_parameters(const MethodInfo *method, const InstanceKClass *kClass)
    {
        if (_current_stack_frame->_sender->_sender != NULL)
        {
            const char *method_signature = kClass->constants()->symbol_at(method->signature_index());
            std::tuple<int, std::vector<int>> parameterAndReturn = Signature::parseParameterAndReturn(method_signature);
            int parameter_size = std::get<1>(parameterAndReturn).size();
            // 如果不是静态方法，默认有一个参数在最前面
            if (!(method->flags() & ACC_STATIC))
            {
                parameter_size += 1;
            }
            // 方向要是反着的，不然执行命令后面就对不上了
            for (size_t i = 0; i < parameter_size; i++)
            {
                StackValue *src_stack_value = &_current_stack_frame->_sender->_stack_slots[--_current_stack_frame->_sender->_stack_top_location];
                StackValue *dst_local_value = &_current_stack_frame->_local_slots[parameter_size - i - 1];
                dst_local_value->_type = src_stack_value->_type;
                dst_local_value->_integer_value = src_stack_value->_integer_value;
            }
        }
    }

} // namespace name