#ifndef DAREEN_MINIJVM_OOPS_FIELDINFO
#define DAREEN_MINIJVM_OOPS_FIELDINFO

#include "utilities/GlobalDefinitions.hpp"
#include "classfile/constants.hpp"

namespace mini_jvm
{
    class FieldInfo
    {
    public:
        FieldInfo() {

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

        u2 flags() {
            return _flags;
        }

        u2 name_index() {
            return _name_index;
        }

        u2 signature_index() {
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
    };
} // namespace name

#endif