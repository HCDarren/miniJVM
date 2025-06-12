#include "oops/InstanceKClass.hpp"
#include "interpreter/BytecodeInterpreter.hpp"
namespace mini_jvm
{
    MethodInfo *InstanceKClass::findMethod(const char *method_name, const char *method_signature)
    {
        for (size_t i = 0; i < _method_size; i++)
        {
            MethodInfo* method_info = _methods + i;
            int name_index = method_info->name_index();
            int signature_index = method_info->signature_index();
            const char *method_name_str = _constants->symbol_at(name_index);
            const char *signature_name_str = _constants->symbol_at(signature_index);
            if (strcmp(method_name_str, method_name) == 0 && strcmp(signature_name_str, method_signature) == 0)
            {
                return method_info;
            }
        }
        return nullptr;
    }
}
