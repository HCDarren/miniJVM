#ifndef DAREEN_MINIJVM_OOPS_INSTANCEKCLASS
#define DAREEN_MINIJVM_OOPS_INSTANCEKCLASS

#include "oops/KClass.hpp"
#include "oops/ConstantPool.hpp"
#include "oops/FieldInfo.hpp"
#include "oops/MethodInfo.hpp"
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/StackValue.hpp"
#include <map>

namespace mini_jvm
{
    class Oop;

    class InstanceKClass : public KClass
    {
    protected:
        ConstantPool *_constants;
        u2 _field_size;
        u2 _method_size;
        FieldInfo* _fields;
        MethodInfo* _methods;
        std::string _class_name;
        bool _is_initialized = false;
    private:
        std::map<u4, StackValue*> _static_values;
    public:
        ConstantPool *constants() const
        {
            return _constants;
        }

        StackValue* get_static_value(const u2 name_index, const u2 name_and_type_index);
        void set_static_value(const u2 name_index, const u2 name_and_type_index, StackValue* value);

        void set_constants(ConstantPool *c)
        {
            _constants = c;
        }

        FieldInfo* fields() const{
            return _fields;
        }

        u2 field_size() const{
            return _field_size;
        }

        void set_fields(FieldInfo* fields) {
            _fields = fields;
        }

        void set_field_size(const u2 field_size) {
            _field_size = field_size;
        }

        void set_methods(MethodInfo* methods) {
            _methods = methods;
        }

        void set_method_size(const u2 method_size) {
            _method_size = method_size;
        }

        void set_class_name(std::string class_name) {
            _class_name = class_name;
        }

        std::string class_name() const {
            return _class_name;
        }

        u2 method_size() const{
            return _method_size;
        }

        MethodInfo* methods() const{
            return _methods;
        }

        void initialize();
        
        size_t size_helper();

        Oop* allocate_instance();

        MethodInfo* findMethod(const char *method_name, const char *method_signature);
    };
} // namespace name

#endif