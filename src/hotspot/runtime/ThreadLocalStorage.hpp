#ifndef DAREEN_MINIJVM_RUNTIME_TTREADLOCALSTORAGE
#define DAREEN_MINIJVM_RUNTIME_TTREADLOCALSTORAGE

#include "oops/AllStatic.hpp"
#include "runtime/JavaThread.hpp"

namespace mini_jvm
{
    class ThreadLocalStorage : AllStatic {

 // Exported API
 public:
  static JavaThread* thread(); // return current thread, if attached
  static void    set_thread(JavaThread* thread); // set current thread
  static void    init();
  static bool    is_initialized(); // can't use TLS prior to initialization
};
} // namespace name

#endif