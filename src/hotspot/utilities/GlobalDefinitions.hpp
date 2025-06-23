#ifndef DAREEN_MINIJVM_UTILITIES_GLOBALDEFINITIONS
#define DAREEN_MINIJVM_UTILITIES_GLOBALDEFINITIONS

#include <type_traits>

#define JAVA_CLASSFILE_MAGIC 0xCAFEBABE

typedef uint8_t jubyte;
typedef uint16_t jushort;
typedef uint32_t juint;
typedef uint64_t julong;

typedef jubyte u1;
typedef jushort u2;
typedef juint u4;
typedef julong u8;

typedef unsigned char u_char;
typedef u_char *address;

typedef intptr_t  intx;
typedef uintptr_t uintx;

enum {
    JVM_T_BOOLEAN = 4,
    JVM_T_CHAR    = 5,
    JVM_T_FLOAT   = 6,
    JVM_T_DOUBLE  = 7,
    JVM_T_BYTE    = 8,
    JVM_T_SHORT   = 9,
    JVM_T_INT     = 10,
    JVM_T_LONG    = 11
};

// NOTE: replicated in SA in vm/agent/sun/jvm/hotspot/runtime/BasicType.java
enum BasicType : u1
{
    // The values T_BOOLEAN..T_LONG (4..11) are derived from the JVMS.
    T_BOOLEAN = JVM_T_BOOLEAN,
    T_CHAR = JVM_T_CHAR,
    T_FLOAT = JVM_T_FLOAT,
    T_DOUBLE = JVM_T_DOUBLE,
    T_BYTE = JVM_T_BYTE,
    T_SHORT = JVM_T_SHORT,
    T_INT = JVM_T_INT,
    T_LONG = JVM_T_LONG,
    // The remaining values are not part of any standard.
    // T_OBJECT and T_VOID denote two more semantic choices
    // for method return values.
    // T_OBJECT and T_ARRAY describe signature syntax.
    // T_ADDRESS, T_METADATA, T_NARROWOOP, T_NARROWKLASS describe
    // internal references within the JVM as if they were Java
    // types in their own right.
    T_OBJECT = 12,
    T_ARRAY = 13,
    T_VOID = 14,
    T_ADDRESS = 15,
    T_NARROWOOP = 16,
    T_METADATA = 17,
    T_NARROWKLASS = 18,
    T_CONFLICT = 19, // for stack value type with conflicting contents
    T_ILLEGAL = 99
};

const int LogBytesPerShort   = 1;
const int LogBytesPerInt     = 2;
#ifdef _LP64
constexpr int LogBytesPerWord    = 3;
#else
constexpr int LogBytesPerWord    = 2;
#endif
const int LogBytesPerLong    = 3;

constexpr int LogBitsPerByte     = 3;

const int LogBitsPerShort    = LogBitsPerByte + LogBytesPerShort;
const int LogBitsPerInt      = LogBitsPerByte + LogBytesPerInt;
constexpr int LogBitsPerWord     = LogBitsPerByte + LogBytesPerWord;
const int LogBitsPerLong     = LogBitsPerByte + LogBytesPerLong;
constexpr int BitsPerWord        = 1 << LogBitsPerWord;

const intptr_t OneBit     =  1; // only right_most bit set in a word

#define nth_bit(n)        (((n) >= BitsPerWord) ? 0 : (OneBit << (n)))
#define right_n_bits(n)   (nth_bit(n) - 1)

// bit-operations using a mask m
inline void   set_bits    (intptr_t& x, intptr_t m) { x |= m; }
inline void clear_bits    (intptr_t& x, intptr_t m) { x &= ~m; }
inline intptr_t mask_bits      (intptr_t  x, intptr_t m) { return x & m; }
inline bool mask_bits_are_true (intptr_t flags, intptr_t mask) { return (flags & mask) == mask; }


#endif