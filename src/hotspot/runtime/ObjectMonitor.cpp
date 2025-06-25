#include "runtime/ObjectMonitor.hpp"
#include "oops/Oop.hpp"
#include <iostream>

namespace mini_jvm
{

    bool ObjectMonitor::try_lock_or_add_to_entry_list(JavaThread *current, ObjectWaiter *node)
    {
        node->_prev = nullptr;

        for (;;)
        {
            ObjectWaiter *head = _entry_list.load();
            node->_next = head;
            if (_entry_list.compare_exchange_strong(head, node))
            {
                return false;
            }
            return true;
        }
    }

    bool ObjectMonitor::enter(JavaThread *current)
    {
        assert(current == JavaThread::current());

        int64_t own = owner_raw();
        // 1、有没有锁信息，没有设置为自己
        if (own == NO_OWNER)
        {
            own = try_set_owner_from(NO_OWNER, current);
            if (own != (int64_t)current)
            {
                // 有竞争
                return false;
            }
        }

        if (own == (int64_t)current)
        {
            _recursions++;
            return true;
        }

        // 3、设置为自己可以尝试自旋一定数量
        for (size_t i = 0; i < 1000; i++)
        {
            own = try_set_owner_from(own, current);
            if (own == (int64_t)current)
            {
                // 自旋竞争成功
                return true;
            }
        }
        // 4、如果不是自己，则加入等待列表，park 住
        ObjectWaiter node(current);

        if (!try_lock_or_add_to_entry_list(current, &node))
        {
            return false;
        }
        current->park();
        return true;
    }

    ObjectMonitor::ObjectMonitor(Oop *oop)
    {
        _recursions = 0;
    }

    void ObjectMonitor::exit(JavaThread *current)
    {
        assert(has_owner(current));
        if (_recursions != 0) {
            _recursions--;        // this is simple recursive enter
            return;
        }

        ObjectWaiter* w = _entry_list.load();
        if (w != nullptr) {
            current->_ParkEvent->unpark();
        }
    }

    void ObjectMonitor::set_header(MarkWord hdr)
    {
        set_metadata(hdr.value());
    }

    MarkWord ObjectMonitor::header() const
    {
        return MarkWord(metadata());
    }

    bool ObjectMonitor::has_owner(JavaThread *thread) { return owner() == (uint64_t)thread; }
} // namespace name