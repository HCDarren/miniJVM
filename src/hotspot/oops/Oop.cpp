#include "oops/Oop.hpp"
#include "oops/InstanceKClass.hpp"
#include "runtime/StackValue.hpp"
#include <cassert>

namespace mini_jvm
{
    JavaFiled* Oop::find_java_field(const u2 filed_cp_index) {
        const InstanceKClass* kClass = _metadata;
        // 先从 cp 里面可以解析出，类名，属性名，属性签名
        const u8 filed_ref = kClass->constants()->filed_at(filed_cp_index);
        const u2 name_and_type_index = filed_ref >> 16;
        const u8 name_and_type = kClass->constants()->name_and_type_at(name_and_type_index);
        const u2 name_index = name_and_type & 0xFF;
        const u2 signature_index = name_and_type >> 16;
        // 计算到属性的位置在哪里
        int field_index = EOF;
        FieldInfo* fields = kClass->fields();
        const int field_size = kClass->field_size();
        for (size_t i = 0; i < field_size; i++)
        {
            FieldInfo* field = fields + i;
            if (field->signature_index() == signature_index && field->name_index() == name_index) {
                field_index = i;
            }
        }
        assert(field_index != EOF);
        // 按照规则往里面塞就可以了，不用搞太麻烦
        JavaFiled* java_filed = (JavaFiled*)this;
        return &java_filed[2 + field_index];
    }

    void Oop::set_filed_value(const u2 filed_cp_index, const intptr_t value) {
        // 按照规则往里面塞就可以了，不用搞太麻烦
        JavaFiled* java_filed = find_java_field(filed_cp_index);
        *java_filed = (JavaFiled)value;
    }

    StackValue Oop::get_filed_value(const u2 filed_cp_index) {
        JavaFiled* java_filed = find_java_field(filed_cp_index);
        StackValue res;
        // TODO 这里类型有问题，需要根据属性签名来设置 type 。只是目前没有问题。
        res._type = T_ILLEGAL;
        res._integer_value = (intptr_t)*java_filed;
        return res;
    }

    InstanceKClass* Oop::metadata_class() {
        return this->_metadata;
    }
    
} // namespace mini_jvm