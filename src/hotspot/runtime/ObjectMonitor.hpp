#ifndef DAREEN_MINIJVM_RUNTIME_OBJECTMONITOR
#define DAREEN_MINIJVM_RUNTIME_OBJECTMONITOR

#include <atomic>
#include "runtime/JavaThread.hpp"


namespace mini_jvm
{
    static const int64_t NO_OWNER = 0;
    static const int64_t ANONYMOUS_OWNER = 1;
    static const int64_t DEFLATER_MARKER = 2;

    class Oop;
    class BasicLock;
    class MarkWord;

    class ObjectMonitor
    {
    private:
        std::atomic<uintptr_t> _metadata;
        std::atomic<int64_t> _owner;
        std::atomic<BasicLock*> _stack_locker;

    public:
        ObjectMonitor(Oop* oop);

        void set_anonymous_owner() {
            set_owner_from_raw(NO_OWNER, ANONYMOUS_OWNER);
        }

        void set_owner(JavaThread* thread) { set_owner_from(NO_OWNER, thread); }

        inline int64_t owner_raw() const {
            return _owner.load();
        }

        inline BasicLock* stack_locker() const {
            return _stack_locker.load();
        }

        inline void set_stack_locker(BasicLock* locker) {
            _stack_locker.store(locker);
        }

        inline void set_metadata(uintptr_t value) {
            _metadata.store(value);
        }

        void set_header(MarkWord hdr);

        bool has_anonymous_owner() const { return owner_raw() == ANONYMOUS_OWNER; }


        inline void set_owner_from_raw(int64_t old_value, int64_t new_value) {
            _owner.store(new_value);
        }

        int64_t owner_id_from(JavaThread* thread);

        inline void set_owner_from(int64_t old_value, JavaThread* current) {
            set_owner_from_raw(old_value, owner_id_from(current));
        }

        void set_owner_from_anonymous(JavaThread* owner) {
            set_owner_from(ANONYMOUS_OWNER, owner);
        }

        bool enter(JavaThread *current);

        inline uintptr_t metadata() const
        {
            return _metadata.load();
        }

        // Returns NO_OWNER if DEFLATER_MARKER is observed.
        inline int64_t owner() const {
            int64_t owner = owner_raw();
            return owner != DEFLATER_MARKER ? owner : NO_OWNER;
        }

        bool has_owner(JavaThread* thread);

        MarkWord header() const;

        void exit(JavaThread *current);
    };
} // namespace name

#endif