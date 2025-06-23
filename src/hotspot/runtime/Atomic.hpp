#ifndef DAREEN_MINIJVM_RUNTIME_ATOMIC
#define DAREEN_MINIJVM_RUNTIME_ATOMIC
#include "oops/AllStatic.hpp"
#include <atomic>
#include <cassert>

namespace mini_jvm
{
    class Atomic : public AllStatic
    {
    public:
        // 本来打算用内联汇编，但是想了一下，我用的是 arm 大家可能用的是 x86 怕大家搞不定
        // 用 c++11 自带的 atomic 来实现，底层肯定原理是汇编，大家有概念就行，或者看我手写操作系统的代码
        static uint64_t cmpxchg(uintptr_t *addr, uint64_t old_val, uint64_t new_val)
        {
            // 将普通指针转换为原子指针（需确保内存地址满足原子操作对齐要求）
            std::atomic<uint64_t> *atomic_ptr = reinterpret_cast<std::atomic<uint64_t> *>(addr);
            // 使用 compare_exchange_strong 实现原子比较交换
            bool is_ok = atomic_ptr->compare_exchange_strong(old_val, new_val);
            return is_ok ? new_val : old_val;
        }
    };
} // namespace name

#endif