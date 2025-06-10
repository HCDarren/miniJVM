#ifndef DAREEN_MINIJVM_CLASSFILE_CLASSFILESTREAM
#define DAREEN_MINIJVM_CLASSFILE_CLASSFILESTREAM

#include "utilities/GlobalDefinitions.hpp"
#include "utilities/bytes_arm.hpp"
#include <string>

namespace mini_jvm
{
    class ClassFileStream
    {
    public:
        ClassFileStream(const std::string &class_name);
        ~ClassFileStream();

    public:
        u1 get_u1()
        {
            return *_current++;
        }

        u2 get_u2()
        {
            u2 res = Bytes::get_Java_u2(_current);
            _current += 2;
            return res;
        }

        u4 get_u4()
        {
            u4 res = Bytes::get_Java_u4(_current);
            _current += 4;
            return res;
        }

        u8 get_u8()
        {
            u8 res = Bytes::get_Java_u8(_current);
            _current += 8;
            return res;
        }

        void skip_u1(int length)
        {
            _current += length;
        }

        void skip_u2(int length)
        {
            _current += 2 * length;
        }

        void skip_u4(int length)
        {
            _current += 4 * length;
        }

        u1* current() { return _current; }
    private:
        u1 *_current;
        u1 *buffer_start;
    };
} // namespace name

#endif