#include "runtime/JavaRunStack.hpp"
#include "runtime/StackFrame.hpp"
#include <cassert>

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

} // namespace name