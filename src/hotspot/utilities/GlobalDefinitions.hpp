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
typedef u_char* address;

#endif