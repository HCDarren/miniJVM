#ifndef DAREEN_MINIJVM_OOPS_OOPDESC
#define DAREEN_MINIJVM_OOPS_OOPDESC
#include "utilities/GlobalDefinitions.hpp"
typedef void *make_word;

typedef intptr_t* JavaFiled;

namespace mini_jvm
{

    class InstanceKClass;
    class StackValue;
    class Oop
    {
        friend InstanceKClass;

    public:
        JavaFiled* find_java_field(const u2 filed_cp_index);
        void set_filed_value(const u2 filed_cp_index, const intptr_t value);
        StackValue get_filed_value(const u2 filed_cp_index);
        InstanceKClass* metadata_class();

    private:
        // TODO: 后面 gc 的时候有用
        make_word _make_word;
        // 是哪个类创建出来的对象
        InstanceKClass *_metadata;
        // 后面是根据 kClaas 计算出来的类对象数据
    };
} // namespace name

#endif