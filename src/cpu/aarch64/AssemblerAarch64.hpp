#ifndef DAREEN_MINIJVM_CPU_AARCH64_ASSEMBLERAARCH64
#define DAREEN_MINIJVM_CPU_AARCH64_ASSEMBLERAARCH64

inline void tmit_address_param0(const void* address) {              
    __asm__ volatile (                                              
        "ldr x0, %[val]\n"                                 
        :                                                           
        : [val] "m"(address)                                        
    );                                                              
}

inline void tmit_address_param1(const void* address) {              
    __asm__ volatile (                                              
        "ldr x1, %[val]\n"                                 
        :                                                           
        : [val] "m"(address)                                        
    );                                                              
}

inline void tmit_address_param2(const void* address) {              
    __asm__ volatile (                                              
        "ldr x2, %[val]\n"                                 
        :                                                           
        : [val] "m"(address)                                        
    );                                                              
}

inline void tmit_int_param2(const int value) {              
    __asm__ volatile (                                              
        "ldr w2, %[val]\n"                                 
        :                                                           
        : [val] "m"(value)                                        
    );                                                              
}

inline void tmit_int_param3(const int value) {              
    __asm__ volatile (                                              
        "ldr w3, %[val]\n"                                 
        :                                                           
        : [val] "m"(value)                                        
    );                                                              
}

inline void tmit_int_param4(const int value) {              
    __asm__ volatile (                                              
        "ldr w4, %[val]\n"                                 
        :                                                           
        : [val] "m"(value)                                        
    );                                                              
}

#endif

