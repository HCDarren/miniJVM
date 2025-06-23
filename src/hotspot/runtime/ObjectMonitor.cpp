#include "runtime/ObjectMonitor.hpp"
#include "oops/Oop.hpp"
#include <iostream>

namespace mini_jvm
{

    bool ObjectMonitor::enter(JavaThread *current)
    {
        // 将当前线程加入等待队列，park 起来
        current->_ParkEvent->park();
        return true;
    }

    ObjectMonitor::ObjectMonitor(Oop *oop)
    {
    }

    void ObjectMonitor::exit(JavaThread *current)
    {
        if (!has_owner(current)) {
            assert(false);
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

    int64_t ObjectMonitor::owner_id_from(JavaThread *thread)
    {
        return thread->monitor_owner_id();
    }

    bool ObjectMonitor::has_owner(JavaThread* thread) { return owner() == owner_id_from(thread); }
} // namespace name