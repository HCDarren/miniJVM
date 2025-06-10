#include "unit/class_loader_test.hpp"
#include "oops/InstanceKClass.hpp"
#include "classfile/ClassLoader.hpp"
#include <iostream>

using namespace mini_jvm;

void test_class_loader() {
    InstanceKClass* kClass = ClassLoader::load_class("");
    std::cout << "HAHA" << std::endl;
}