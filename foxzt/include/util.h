//
// Created by fox on 2023/7/10.
//

#ifndef CPPPROJ_UTIL_H
#define CPPPROJ_UTIL_H

#include <cxxabi.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <iomanip>
#include <cstdint>
#include <chrono>

namespace foxzt {
    long GetThreadId();

    uint32_t GetFiberId();

    std::chrono::time_point<std::chrono::system_clock> getNowTime();

    template<class T>
    const char* TypeToName() {
        static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
        return s_name;
    }
}

#endif //CPPPROJ_UTIL_H
