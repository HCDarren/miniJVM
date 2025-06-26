#include "hotspot/gc/CollectedHeap.hpp"
#include "oops/Oop.hpp"
#include <iostream>

namespace mini_jvm
{
    std::set<Oop *> all_oops;

    Oop *CollectedHeap::obj_allocate(size_t size)
    {
        Oop *new_oop = (Oop *)malloc(size);
        // TODO: 在堆上开辟内存，我先不管，等实现 region 的时候再说
        all_oops.insert(new_oop);
        return new_oop;
    }

    void CollectedHeap::clearOopsKeepAvaliable(const std::set<Oop *> avaliables)
    {
        std::cout << "gc clear before oops size: " << all_oops.size() << std::endl;
        // 标记清除整理，也可以单独拷贝走，这就是各种 GC 算法的演进，自己不写还真不能理解这一些，写了自然就会遇到这些问题
        // 1、标记清除整理：如果存活对象比较多
        // 2、拷贝复制：如果存活对象比较少
        // 3、分代：一般对象朝生夕死的比较多，多轮 GC 都没有回收换老年代里面去，因此青年代老年代算法都不一样才行
        // 4、各种其他演进算法：G1，ZGC，Shenandoah
        size_t removed = std::erase_if(all_oops, [&](Oop *oop) {
            bool contains =  avaliables.contains(oop);
            if (!contains) {
                free(oop); // 释放内存
            }
            return !contains;
        });
        std::cout << "gc clear after oops size: " << all_oops.size() << std::endl;
    }

} // namespace name
