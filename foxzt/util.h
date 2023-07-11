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
    pid_t GetThreadId();

    uint32_t GetFiberId();

    /**
     * @brief 获取当前时间的毫秒
     */
    uint64_t GetCurrentMS();

    /**
     * @brief 获取当前时间的微秒
     */
    uint64_t GetCurrentUS();
}

#endif //CPPPROJ_UTIL_H
