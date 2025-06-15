// The markWord describes the header of an object.
//
// Bit-format of an object header (most significant first, big endian layout below):
//
//  32 bits:
//  --------
//             hash:25 ------------>| age:4  self-fwd:1  lock:2 (normal object)
//
//  64 bits:
//  --------
//  unused:22 hash:31 -->| unused_gap:4  age:4  self-fwd:1  lock:2 (normal object)
//
//  64 bits (with compact headers):
//  -------------------------------
//  klass:22  hash:31 -->| unused_gap:4  age:4  self-fwd:1  lock:2 (normal object)
//
//  - hash contains the identity hash value: largest value is
//    31 bits, see os::random().  Also, 64-bit vm's require
//    a hash value no bigger than 32 bits because they will not
//    properly generate a mask larger than that: see library_call.cpp
//
//  - the two lock bits are used to describe three states: locked/unlocked and monitor.
//
//    [ptr             | 00]  locked             ptr points to real header on stack (stack-locking in use)
//    [header          | 00]  locked             locked regular object header (fast-locking in use)
//    [header          | 01]  unlocked           regular object header
//    [ptr             | 10]  monitor            inflated lock (header is swapped out, UseObjectMonitorTable == false)
//    [header          | 10]  monitor            inflated lock (UseObjectMonitorTable == true)
//    [ptr             | 11]  marked             used to mark an object
//    [0 ............ 0| 00]  inflating          inflation in progress (stack-locking in use)
//
//    We assume that stack/thread pointers have the lowest two bits cleared.
//
//  - INFLATING() is a distinguished markword value of all zeros that is
//    used when inflating an existing stack-lock into an ObjectMonitor.
//    See below for is_being_inflated() and INFLATING().
#ifndef DAREEN_MINIJVM_OOPS_MARKWOED
#define DAREEN_MINIJVM_OOPS_MARKWOED

namespace mini_jvm
{
    class MarkWord {
       
    };
} // namespace name

#endif