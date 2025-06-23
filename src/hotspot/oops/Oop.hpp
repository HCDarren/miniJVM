#ifndef DAREEN_MINIJVM_OOPS_OOP
#define DAREEN_MINIJVM_OOPS_OOP
#include "utilities/GlobalDefinitions.hpp"
#include "runtime/Atomic.hpp"
#include "runtime/ObjectMonitor.hpp"

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

        static MarkWord encode(ObjectMonitor* monitor) {
            uintptr_t tmp = (uintptr_t) monitor;
            return MarkWord(tmp | monitor_value);
        }

        MarkWord displaced_mark_helper() const;

        static MarkWord zero() { return MarkWord(uintptr_t(0)); }
        static MarkWord INFLATING() { return zero(); }

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

        ObjectMonitor* monitor() const {
            // Use xor instead of &~ to provide one extra tag-bit check.
            return (ObjectMonitor*) (value() ^ monitor_value);
        }

        static MarkWord unused_mark() {
            return MarkWord(marked_value);
        }

        BasicLock* locker() const {
            return (BasicLock*) value();
        }

        bool has_monitor() const {
            return ((value() & lock_mask_in_place) == monitor_value);
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
            uint64_t value = Atomic::cmpxchg(&_make_word._value, old_mark._value, new_mark._value);
            return MarkWord(value);
        }

        void release_set_mark(MarkWord m) {
            _make_word = m;
        }

        MarkWord mark_acquire() const {
            return _make_word;
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