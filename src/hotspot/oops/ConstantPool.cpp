#include "oops/ConstantPool.hpp"
namespace mini_jvm
{
    ConstantPool::ConstantPool(u2 size){
        _tag = (u1*)malloc(size);
        _val = (u8*)malloc(size * sizeof(u8));
        _size = size;
    }
} // namespace name