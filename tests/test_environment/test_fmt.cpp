//
// Created by fox on 2023/7/13.
//
#include <vector>
#include <fmt/ranges.h>
#include <iostream>
#include "foxzt/foxzt.h"

#define FOXZT_LOG_LEVEL(level, fmt, ...) \
do { \
    auto logger = std::make_shared<foxzt::Logger>(); \
    auto event = std::make_shared<foxzt::LogEvent>( \
        level, __FILE__, __LINE__, foxzt::GetThreadId(), \
        foxzt::GetFiberId(), foxzt::getNowTime(), logger->getMName()); \
    event->format(fmt, ##__VA_ARGS__); \
    logger->log(level, event); \
} while(0)

int main() {
    std::vector<int> v = {1, 2, 3};
    fmt::print("{}\n", v);
    std::string s = fmt::format("The answer is {}.", 42);
// s == "The answer is 42."
    std::cout << s << std::endl;
    FOXZT_LOG_LEVEL(foxzt::LogLevel::WARN,"asd%d",12);
}