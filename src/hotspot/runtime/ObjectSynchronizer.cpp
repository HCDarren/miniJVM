#include "runtime/ObjectSynchronizer.hpp"
#include <iostream>
#include "runtime/ObjectMonitor.hpp"

namespace mini_jvm
{

    ObjectMonitor *ObjectSynchronizer::inflate(JavaThread *current, Oop *obj)
    {
        // JDK 源码很清楚了：膨胀完毕，轻量级开始膨胀，膨胀中，无锁开始（我们不考虑这个）
        // The mark can be in one of the following states:
        // *  inflated     - If the ObjectMonitor owner is anonymous and the
        //                   locking_thread owns the object lock, then we
        //                   make the locking_thread the ObjectMonitor owner.
        // *  stack-locked - Coerce it to inflated from stack-locked.
        // *  INFLATING    - Busy wait for conversion from stack-locked to
        //                   inflated.
        // *  unlocked     - Aggressively inflate the object.

        for (;;)
        {
            // 膨胀完毕
            const MarkWord mark = obj->mark_acquire();
            if (mark.has_monitor())
            {
                ObjectMonitor *inf = mark.monitor();
                MarkWord dmw = inf->header();
                if (inf->has_anonymous_owner() && current != nullptr)
                {
                    if (current->is_lock_owned((address)inf->stack_locker()))
                    {
                        inf->set_stack_locker(nullptr);
                        inf->set_owner_from_anonymous(current);
                    }
                }
                return inf;
            }
            // 膨胀中
            if (mark == MarkWord::INFLATING()) {
                // 考虑等一会，我们不写
                continue;
            }

            // 轻量级开始膨胀
            if (mark.has_locker()) {
                ObjectMonitor* m = new ObjectMonitor(obj);

                MarkWord new_mark = MarkWord::INFLATING();
                if (obj->cas_set_mark(new_mark, mark) != new_mark) { // 多线程还有竞争
                    delete m;
                    continue;       // Interference -- just retry
                }
                
                // 下面开始比较复杂了，这里可以更简单实现，但为了更理解 c++ 的多线程，所以仔细学习下
                const MarkWord dmw = mark.displaced_mark_helper();

                // Setup monitor fields to proper values -- prepare the monitor
                m->set_header(dmw);

                if (current != nullptr && current->is_lock_owned((address)mark.locker())) {
                    m->set_owner(current);
                } else {
                    m->set_stack_locker(mark.locker());
                    m->set_anonymous_owner();
                }

                obj->release_set_mark(MarkWord::encode(m));
                return m;
            }

            assert(false);
        }
    }

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

        lock->set_displaced_header(MarkWord::unused_mark());
        // 重量级锁：先自旋锁，再做互斥锁
        while (true)
        {
            ObjectMonitor *monitor = inflate(current, obj);
            if (monitor->enter(current))
            {
                return;
            }
        }
    }

    void ObjectSynchronizer::exit(Oop *obj, BasicLock *lock, JavaThread *current)
    {
        // 退出轻量级锁
        MarkWord mark = obj->mark();
        MarkWord dhw = lock->displaced_header();
        if (dhw.value() == 0)
        { // 重入的
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
        ObjectMonitor *monitor = inflate(current, obj);
        monitor->exit(current);
    }
} // namespace name