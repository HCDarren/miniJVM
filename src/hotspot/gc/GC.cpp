#include "hotspot/gc/GC.hpp"
#include "hotspot/gc/CollectedHeap.hpp"
#include "runtime/SafepointMechanism.hpp"
#include <iostream>
#include <unistd.h>
#include "runtime/JavaThread.hpp"
#include "jni/jni.h"
#include "runtime/JavaRunStack.hpp"

namespace mini_jvm
{
    void *gc_func(void *arg)
    {
        SafepointMechanism::begin();
        // 回收垃圾
        std::cout << "stop the world-->" << std::endl;
        std::set<Oop*> all_roots = GC::findAllRoots();
        std::cout << "all_roots size: " << all_roots.size() << std::endl;
        // 所有根对象三色标记，最终找到所有需要存活的对象
        CollectedHeap::clearOopsKeepAvaliable(all_roots);
        SafepointMechanism::end();
        return (void *)0; 
    }

    std::set<Oop*> GC::findAllRoots() {
        // 找到所有根对象，那么需要遍历栈
        std::set<JavaThread*> all_threads= JavaVM::current()->getAllThreads();
        std::set<Oop*> all_gc_roots;
        
        for (JavaThread* java_thread : all_threads)
        {
           std::set<Oop*> java_statck_oops = java_thread->run_java_statck()->get_all_statck_oops();
           all_gc_roots.insert(java_statck_oops.begin(), java_statck_oops.end());
        }
        // 还有什么方法区还是静态区？名字不重要，不能回收的也要去加进来，
        // 知道就行我们不做，我们好像是放在 class 类对象里面了？
        return all_gc_roots;
    }

    void GC::gc()
    {
        // -------------------- 思考 ------------------------
        // 1、怎么找到垃圾
        // 2、怎么清理垃圾
        // 3、停止用户正常工作的线程，GC 时间，需要考虑并行问题缩短 GC 时间
        //--------------------- 实现 ------------------------
        // 1、先参考 jdk 的源码来实现所有进入安全点
        // 2、实现并非标记垃圾的三色标记算法
        // 3、先实现简单的标记清除回收垃圾，明年有时间直播了再实现 G1 的 region 
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, gc_func, NULL);
        pthread_detach(tid);
    }

} // namespace name
