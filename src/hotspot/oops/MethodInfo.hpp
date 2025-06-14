#ifndef DAREEN_MINIJVM_OOPS_METHODINFO
#define DAREEN_MINIJVM_OOPS_METHODINFO

#include "utilities/GlobalDefinitions.hpp"
#include "classfile/constants.hpp"

namespace mini_jvm
{
    class MethodInfo
    {
    public:
        MethodInfo() {

        }

        void set_flags(const u2 flags) {
            _flags = flags;
        }

        void set_name_index(const u2 name_index) {
            _name_index = name_index;
        }

        void set_signature_index(const u2 signature_index) {
            _signature_index = signature_index;
        }

        void set_attribute_count(const u2 attribute_count) {
            _attribute_count = attribute_count;
        }

        void set_max_stack(const u2 max_stack) {
            _max_stack = max_stack;
        }

        u2 max_stack() const{
            return _max_stack;
        }
        
        void set_max_locals(const u2 max_locals) {
            _max_locals = max_locals;
        }

        u2 max_locals() const{
            return _max_locals;
        }

        void set_code_length(const u4 code_length) {
            _code_length = code_length;
        }

        void set_code(const u1* code_start) {
            _code_start = (u1*)malloc(_code_length);
            memcpy(_code_start, code_start, _code_length);
        }

        void set_native_function (address native_function) {
            _native_function = native_function;
        }

        address native_function () const{
            return _native_function;
        }

        u1* code() const{
            return _code_start;
        }

        u4 code_length() const{
            return _code_length;
        }

        u2 flags() const{
            return _flags;
        }

        u2 name_index() const{
            return _name_index;
        }

        u2 signature_index() const{
            return _signature_index;
        }

        u2 attribute_count() {
            return _attribute_count;
        }
    private:
        u2 _flags;
        u2 _name_index;
        u2 _signature_index;
        u2 _attribute_count;
        u2 _max_stack;
        u2 _max_locals;
        u4 _code_length;
        u1* _code_start;
        address _native_function;
    };
} // namespace name

#endif