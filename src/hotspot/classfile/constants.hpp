#ifndef DAREEN_MINIJVM_CLASSFILE_CONSTANTS
#define DAREEN_MINIJVM_CLASSFILE_CONSTANTS
#include <string>

namespace mini_jvm
{

    enum Constants{
        JVM_CONSTANT_Utf8                   = 1,
        JVM_CONSTANT_Unicode                = 2, /* unused */
        JVM_CONSTANT_Integer                = 3,
        JVM_CONSTANT_Float                  = 4,
        JVM_CONSTANT_Long                   = 5,
        JVM_CONSTANT_Double                 = 6,
        JVM_CONSTANT_Class                  = 7,
        JVM_CONSTANT_String                 = 8,
        JVM_CONSTANT_Fieldref               = 9,
        JVM_CONSTANT_Methodref              = 10,
        JVM_CONSTANT_InterfaceMethodref     = 11,
        JVM_CONSTANT_NameAndType            = 12,
        JVM_CONSTANT_MethodHandle           = 15,  // JSR 292
        JVM_CONSTANT_MethodType             = 16,  // JSR 292
        JVM_CONSTANT_Dynamic                = 17,
        JVM_CONSTANT_InvokeDynamic          = 18,
        JVM_CONSTANT_Module                 = 19,
        JVM_CONSTANT_Package                = 20,
        JVM_CONSTANT_ExternalMax            = 20 
    };

    enum MethodAccess {
        ACC_PUBLIC = 0X0001,
        // Declared public; may be accessed from outside its package.
        ACC_PRIVATE = 0X0002,
        // Declared private; accessible only within the defining class.
        ACC_PROTECTED = 0X0004,
        // Declared protected; may be accessed within subclasses.
        ACC_STATIC = 0X0008,
        // Declared static.
        ACC_FINAL = 0X0010,
        // Declared final; must not be overridden.
        ACC_SYNCHRONIZED = 0X0020,
        // Declared synchronized; invocation is wrapped by a monitor use.
        ACC_BRIDGE = 0X0040,
        // A bridge method, generated by the compiler.
        ACC_VARARGS = 0X0080,
        // Declared with variable number of arguments.
        ACC_NATIVE = 0X0100,
        // Declared native; implemented in a language other than Java.
        ACC_ABSTRACT = 0X0400,
        // Declared abstract; no implementation is provided.
        ACC_STRICT = 0X0800,
        // Declared strictfp; floating-point mode is FPstrict.
        ACC_SYNTHETIC =
            0X1000  // Declared synthetic; not present in the source code.
    };
} // namespace name

#endif