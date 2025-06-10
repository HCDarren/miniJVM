#include "utilities/GlobalDefinitions.hpp"
namespace mini_jvm
{

class Bytes
{
public:
    static inline u2 get_Java_u2(address p)
    {
        return (u2(p[0]) << 8) | u2(p[1]);
    }

    static inline u4 get_Java_u4(address p)
    {
        return u4(p[0]) << 24 |
               u4(p[1]) << 16 |
               u4(p[2]) << 8 |
               u4(p[3]);
    }

    static inline u8 get_Java_u8(address p)
    {
        return u8(p[0]) << 56 |
               u8(p[1]) << 48 |
               u8(p[2]) << 40 |
               u8(p[3]) << 32 |
               u8(p[4]) << 24 |
               u8(p[5]) << 16 |
               u8(p[6]) << 8 |
               u8(p[7]);
    }
};

} // namespace name