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
    FOXZT_LOGGER_CALL(DEFAULT_LOGGER_RAW(), foxzt::LogLevel::WARN, "as{}d%d", 12);
    FOXZT_LOGGER_CALL(DEFAULT_LOGGER_RAW(), foxzt::LogLevel::WARN, "hello");

    FOXZT_LOGGER_INFO(DEFAULT_LOGGER_RAW(), "asd{}", 123);
    FOXZT_LOGGER_INFO(DEFAULT_LOGGER_RAW(), "asd");

    FOXZT_INFO("zxc{}", 123);
    FOXZT_INFO("zxc");

    return 0;
}