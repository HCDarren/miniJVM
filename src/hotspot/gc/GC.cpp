#include "hotspot/gc/GC.hpp"
#include "runtime/SafepointMechanism.hpp"
#include <iostream>
#include <unistd.h>

namespace mini_jvm
{
    void *gc_func(void *arg)
    {
        SafepointMechanism::begin();
        // 回收垃圾
        std::cout << "stop the world-->" << std::endl;
        usleep(5000000);
        SafepointMechanism::end();
        return (void *)0; 
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
