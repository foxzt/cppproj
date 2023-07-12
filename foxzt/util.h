//
// Created by fox on 2023/7/10.
//

#ifndef CPPPROJ_UTIL_H
#define CPPPROJ_UTIL_H

#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <json/json.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <google/protobuf/message.h>

namespace foxzt {
    long GetThreadId();

    uint32_t GetFiberId();

    std::chrono::time_point<std::chrono::system_clock> getNowTime();
}

#endif //CPPPROJ_UTIL_H
