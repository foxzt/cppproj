#include <ostream>
#include "foxzt/include/log.h"
#include "foxzt/include/config.h"

void test_log_config() {
    FOXZT_INFO("before: hello");

    YAML::Node node = YAML::LoadFile("log.yml");
    foxzt::Config::LoadFromYaml(node);

    FOXZT_INFO("after: hello");
    FOXZT_LOGGER_ERROR(LOGGER_NAME_RAW("system"), "after: hello");
    std::set<int> vec{1, 2, 3, 2};
    FOXZT_INFO(fg(fmt::color::crimson) | fmt::emphasis::bold,
               "Hello, {}!\n", "world");
}


int main() {
    test_log_config();

    return 0;
}