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
#include <vector>
#include <execinfo.h>
#include <cstdlib>
#include <iostream>

#include "log.h"

namespace foxzt {
    long GetThreadId();

    uint32_t GetFiberId();

    std::chrono::time_point<std::chrono::system_clock> getNowTime();

    template<class T>
    const char *TypeToName() {
        static const char *s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
        return s_name;
    }

    void BackTrace(std::vector<std::string> &bt, int size, int skip);

    std::string BackTrace(int size, int skip = 2, const std::string &prefix = "");
}

#endif //CPPPROJ_UTIL_H
