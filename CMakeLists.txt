# cmake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置工程的名字，生成的可执行文件也将使用该名称
project(main)
set(CMAKE_C_STANDARD 14)

# 给visual studio编译器使用的宏，Enable GCC debug
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_compile_options(
        -g 
        -std=c++17
        -DSTACK_DEBUG
)

# 头文件搜索路径
include_directories(
        ${PROJECT_SOURCE_DIR}/src/hotspot
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/jni
)

# 参与编译的文件
file(GLOB_RECURSE SOURCE_LIST "src/main.cpp" "src/hotspot/*.cpp" "src/unit/*.cpp" "src/jni/*.cpp")

# 将源文件列表加入到工程中
add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} pthread pcap)


