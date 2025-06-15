#include "runtime/JavaRunStack.hpp"
#include "runtime/StackFrame.hpp"
#include "runtime/Signature.hpp"
#include <cassert>
#include <string>
#include <vector>

namespace mini_jvm
{

    JavaRunStack::JavaRunStack() {
        _current_stack_frame = new StackFrame(0, 0);
    }

    JavaRunStack::~JavaRunStack() {

    }

    void JavaRunStack::push_frame(StackFrame* stack_frame) {
        StackFrame* current_stack_frame = _current_stack_frame;
        stack_frame->_sender = current_stack_frame;
        _current_stack_frame = stack_frame;
    }

    void JavaRunStack::pop_frame() {
        assert(_current_stack_frame->_sender != NULL);
        _current_stack_frame = _current_stack_frame->_sender;
    }

    void JavaRunStack::pop_int_result() {
        StackValue* stack_top_value = &_current_stack_frame->_stack_slots[--_current_stack_frame->_stack_top_location];
        _current_stack_frame->_sender->push_int_to_stack(stack_top_value->_integer_value);
        _current_stack_frame->_sender->print_stack_frame();
    }

    void JavaRunStack::push_call_parameters(const std::string& method_signature) {
        if (_current_stack_frame->_sender != NULL) {
            std::tuple<int, std::vector<int>> parameterAndReturn = Signature::parseParameterAndReturn(method_signature);
            int parameter_size = std::get<1>(parameterAndReturn).size();
            for (size_t i = 0; i < parameter_size; i++)
            {
                StackValue* src_stack_value = &_current_stack_frame->_sender->_stack_slots[--_current_stack_frame->_sender->_stack_top_location];
                StackValue* dst_local_value = &_current_stack_frame->_local_slots[i];
                dst_local_value->_type = src_stack_value->_type;
                dst_local_value->_integer_value = src_stack_value->_integer_value;
            }
        }
    }

} // namespace name