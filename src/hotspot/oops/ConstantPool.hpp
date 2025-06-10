#ifndef DAREEN_MINIJVM_OOPS_CONSTANTPOOL
#define DAREEN_MINIJVM_OOPS_CONSTANTPOOL

#include "oops/Metadata.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include "classfile/constants.hpp"

namespace mini_jvm
{
    class ConstantPool : public Metadata
    {
    public:
        ConstantPool(u2 size);

        // 存储
        void method_at_put(const u2 cp_index, const u2 name_index, const u2 signature_index)
        {
            _tag[cp_index] = JVM_CONSTANT_Methodref;
            _val[cp_index] = ((u8)signature_index << 16) | name_index;
        }

        void filed_at_put(const u2 cp_index, const u2 name_index, const u2 signature_index)
        {
            _tag[cp_index] = JVM_CONSTANT_Fieldref;
            _val[cp_index] = ((u8)signature_index << 16) | name_index;
        }

        void kclass_index_at_put(const u2 cp_index, const u2 name_index)
        {
            _tag[cp_index] = JVM_CONSTANT_Class;
            _val[cp_index] = name_index;
        }

        void string_index_at_put(const u2 cp_index, const u2 string_index)
        {
            _tag[cp_index] = JVM_CONSTANT_String;
            _val[cp_index] = string_index;
        }

        void symbol_at_put(const u2 cp_index, const char* str) 
        {
            _tag[cp_index] = JVM_CONSTANT_Utf8;
            _val[cp_index] = (u8)str;
        }

        void name_and_type_at_put(const u2 cp_index, const u2 name_index, const u2 signature_index) {
            _tag[cp_index] = JVM_CONSTANT_NameAndType;
            _val[cp_index] = ((u8)signature_index << 16) | name_index;
        }

        // 获取
        u8 method_at(const u2 cp_index)
        {
            return _val[cp_index];
        }

        u8 filed_at(const u2 cp_index)
        {
            return _val[cp_index];
        }

        u8 kclass_index_at(const u2 cp_index)
        {
            return _val[cp_index];
        }

        u8 string_index_at(const u2 cp_index)
        {
            return _val[cp_index];
        }

        const char* symbol_at(const u2 cp_index) 
        {
            return (const char*)_val[cp_index];
        }

        u8 name_and_type_at(const u2 cp_index) {
            return _val[cp_index];
        }

        u1 tag_at(const u2 cp_index) {
            return _tag[cp_index];
        }

        u2 size() {
            return _size;
        }
    private:
        u1* _tag;
        u8* _val;
        u2 _size;
    };
} // namespace name

#endif