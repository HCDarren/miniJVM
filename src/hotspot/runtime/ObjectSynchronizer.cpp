#include "runtime/ObjectSynchronizer.hpp"
namespace mini_jvm
{
   void ObjectSynchronizer::enter(Oop* obj, BasicLock* lock, JavaThread* current) {
        // 轻量级锁


        // 重量级锁
   }

   void ObjectSynchronizer::exit(Oop* obj, BasicLock* lock, JavaThread* current) {
        
   }
} // namespace name