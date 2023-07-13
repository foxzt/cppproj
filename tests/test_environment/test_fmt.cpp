//
// Created by fox on 2023/7/13.
//
#include <vector>
#include <fmt/ranges.h>
#include <iostream>
#include "foxzt/foxzt.h"

int main() {
    std::vector<int> v = {1, 2, 3};
    fmt::print("{}\n", v);
    std::string s = fmt::format("The answer is {}.", 42);
    std::cout << s << std::endl;
    //DEFAULT_LOGGER_RAW()->setMLevel(foxzt::LogLevel::WARN);
    auto file = std::shared_ptr<foxzt::LogAppender>(new foxzt::FileLogAppender("./log.txt"));

    DEFAULT_LOGGER_RAW()->addAppender(file);
    DEFAULT_LOGGER_RAW()->setFormatter("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %]] [%@]: %v");
    FOXZT_LOGGER_CALL(DEFAULT_LOGGER_RAW(), foxzt::LogLevel::WARN, "as{}d%d", 12);
    FOXZT_LOGGER_CALL(DEFAULT_LOGGER_RAW(), foxzt::LogLevel::WARN, "hello");

    FOXZT_LOGGER_INFO(DEFAULT_LOGGER_RAW(), "asd{}", 123);
    FOXZT_LOGGER_INFO(DEFAULT_LOGGER_RAW(), "asd");

    FOXZT_INFO("zxc{}", 123);
    FOXZT_INFO("zxc");

    FOXZT_DEBUG("asd");


    return 0;
}