#ifndef DAREEN_MINIJVM_OOPS_OOP
#define DAREEN_MINIJVM_OOPS_OOP
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/Atomic.hpp"

typedef intptr_t *JavaFiled;

namespace mini_jvm
{
    class BasicLock;
    class MarkWord
    {
    public:
        uintptr_t _value;

    public:
        explicit MarkWord(uintptr_t value) : _value(value) {}

        static const uintptr_t locked_value = 0;
        static const uintptr_t unlocked_value = 1;
        static const uintptr_t monitor_value = 2;
        static const uintptr_t marked_value = 3;

        static const int age_bits = 4;
        static const int lock_bits = 2;
        static const int self_fwd_bits = 1;

        static const int lock_shift = 0;
        static const int self_fwd_shift = lock_shift + lock_bits;
        static const int age_shift = self_fwd_shift + self_fwd_bits;

        static const uintptr_t lock_mask = right_n_bits(lock_bits);

        static const uintptr_t lock_mask_in_place = lock_mask << lock_shift;

        bool operator==(const MarkWord &other) const
        {
            return _value == other._value;
        }

        bool operator!=(const MarkWord &other) const
        {
            return !operator==(other);
        }

        static MarkWord from_pointer(void *ptr)
        {
            return MarkWord((uintptr_t)ptr);
        }

        // Conversion
        uintptr_t value() const { return _value; }

        bool has_locker() const {
            return (value() & lock_mask_in_place) == locked_value;
        }

        BasicLock* locker() const {
            return (BasicLock*) value();
        }

        bool is_unlocked() const
        {
            return (mask_bits(value(), lock_mask_in_place) == unlocked_value);
        }
    };

    class InstanceKClass;
    class StackValue;
    class Oop
    {
        friend InstanceKClass;

    public:

        JavaFiled *find_java_field(const u2 filed_cp_index);
        void set_filed_value(const u2 filed_cp_index, const intptr_t value);
        StackValue get_filed_value(const u2 filed_cp_index);
        InstanceKClass *metadata_class();

        MarkWord cas_set_mark(MarkWord new_mark, MarkWord old_mark)
        {
            uint64_t value = Atomic::arm_cmpxchg(&_make_word._value, old_mark._value, new_mark._value);
            return MarkWord(value);
        }

        MarkWord mark();

    private:
        MarkWord _make_word;
        // 是哪个类创建出来的对象
        InstanceKClass *_metadata;
        // 后面是根据 kClaas 计算出来的类对象数据
    };
} // namespace name

#endif