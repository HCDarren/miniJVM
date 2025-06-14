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

#endif