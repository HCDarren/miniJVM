#include "runtime/ObjectSynchronizer.hpp"
#include <iostream>

namespace mini_jvm
{
    void ObjectSynchronizer::enter(Oop *obj, BasicLock *lock, JavaThread *current)
    {
        // 轻量级锁：发生任何竞争都升级为重量级锁
        MarkWord mark = obj->mark();
        // 没有锁
        if (mark.is_unlocked())
        {
            // Anticipate successful CAS -- the ST of the displaced mark must
            // be visible <= the ST performed by the CAS.
            lock->set_displaced_header(mark);
            if (mark == obj->cas_set_mark(MarkWord::from_pointer(lock), mark))
            {
                return;
            }
        }
        // 当前线程重入
        else if (mark.has_locker() && current->is_lock_owned((address)mark.locker()))
        {
            lock->set_displaced_header(MarkWord::from_pointer(nullptr));
            return;
        }

        // 重量级锁：先自旋锁，再做互斥锁
        
    }

    void ObjectSynchronizer::exit(Oop *obj, BasicLock *lock, JavaThread *current)
    {
        // 退出轻量级锁
        MarkWord mark = obj->mark();
        MarkWord dhw = lock->displaced_header();
        if (dhw.value() == 0)
        {   // 重入的
            return;
        }
        if (mark == MarkWord::from_pointer(lock))
        {
            if (obj->cas_set_mark(dhw, mark) == mark)
            {
                return;
            }
        }
        // 退出重量级锁

    }
} // namespace name