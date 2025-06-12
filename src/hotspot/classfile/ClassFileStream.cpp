#include "classfile/ClassFileStream.hpp"
#include <stdio.h>
#include <filesystem>

namespace mini_jvm
{
    ClassFileStream::ClassFileStream(const std::string& class_name){
        std::string absolute_path = "/Users/darrenzeng/Desktop/Darren/Study/miniJVM/src/";
        absolute_path += class_name.c_str();
        absolute_path += ".class";
        // 打开文件，一次性把文件读出来
        FILE *file = fopen(absolute_path.c_str(), "r");
        fseek(file, 0, SEEK_END); // 将文件指针移动到文件末尾
        long file_size = ftell(file); // 获取文件大小
        fseek(file, 0, SEEK_SET); // 将文件指针移回文件开始处
        buffer_start = _current = (u1*)malloc(file_size);
        fread(_current, 1, file_size, file);
        fclose(file);
    }

    ClassFileStream::~ClassFileStream() {
         free(buffer_start);
    }
} // namespace name