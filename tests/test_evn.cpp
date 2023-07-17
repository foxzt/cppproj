//
// Created by fox on 2023/7/10.
//

#include <iostream>
#include <yaml-cpp/yaml.h>
#include "foxzt/log.h"

// 获取编译器名称
std::string getCompilerName() {
#ifdef __clang__
    return "Clang";
#elif defined(__GNUC__)
    return "GCC";
#elif defined(_MSC_VER)
    return "MSVC";
#else
    return "Unknown Compiler";
#endif
}

// 获取编译器版本
std::string getCompilerVersion() {
#if defined(__clang__)
    return __VERSION__;
#elif defined(__GNUC__)
    return std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    return std::to_string(_MSC_VER);
#else
    return "Unknown Version";
#endif
}

// 获取编译日期和时间
std::string getCompileDateTime() {
    return __DATE__ " " __TIME__;
}

int main() {
    // 输出编译工具信息
    std::cout << "Compiler: " << getCompilerName() << std::endl;
    std::cout << "Version: " << getCompilerVersion() << std::endl;
    std::cout << "Compile DateTime: " << getCompileDateTime() << std::endl;

    FOXZT_INFO("hello");

    return 0;
}
