//
// Created by fox on 2023/7/20.
//
#include "foxzt/include/log.h"
#include "foxzt/include/config.h"
#include "foxzt/include/thread.h"

int shared_int = 0;
std::mutex m;

void fun(int x = 0, int y = 1) {
    FOXZT_INFO("{},{}", foxzt::Thread::GetName(), foxzt::Thread::GetThis()->getMId());
    for (int i = 0; i < 10000000; i++) {
        ++shared_int;
    }
    FOXZT_INFO("{},{}", x, y);
}

void fun1() {
    for (int i = 0; i < 10000; i++) {
        std::string s = std::string(9999, 'a');
        FOXZT_INFO("hello");
    }
}

void fun2() {
    for (int i = 0; i < 10000; i++) {
        FOXZT_LOGGER_INFO(LOGGER_NAME_RAW("system"), "system");
        //FOXZT_INFO("default2");
    }
}

void test1() {
    foxzt::Thread t1("线程1", fun, 123, 234);
    foxzt::Thread t2("线程2", fun, 2, 2);
    t1.join();
    t2.join();
}


void test2() {
    foxzt::Thread t1("线程1", fun1);
    foxzt::Thread t2("线程2", fun2);
    t1.join();
    t2.join();
}

int main() {
    YAML::Node node = YAML::LoadFile("log.yml");
    foxzt::Config::LoadFromYaml(node);

    test2();

    return 0;
}
