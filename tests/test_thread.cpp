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
    std::lock_guard<std::mutex> lock(m);
    FOXZT_INFO("{},{}", foxzt::Thread::GetName(), foxzt::GetThreadId());
    for (int i = 0; i < 10000000; i++) {
        ++shared_int;
    }
}

void fun2() {
    std::lock_guard<std::mutex> lock(m);
    FOXZT_INFO("{},{}", foxzt::Thread::GetName(), foxzt::Thread::GetThis()->getMId());
    for (int i = 0; i < 10000000; i++) {
        ++shared_int;
    }
}

int main() {
    YAML::Node node = YAML::LoadFile("log.yml");
    foxzt::Config::LoadFromYaml(node);


    foxzt::Thread t1("线程1", fun, 123, 234);
    foxzt::Thread t2("线程2", fun, 2, 2);
    t1.join();
    t2.join();

    FOXZT_INFO("shared_int: {}", shared_int);

    return 0;
}
