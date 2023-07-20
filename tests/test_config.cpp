#include <ostream>
#include "foxzt/log.h"
#include "foxzt/config.h"

void test_log_config() {
    FOXZT_INFO("before: hello");

    YAML::Node node = YAML::LoadFile("log.yml");
    foxzt::Config::LoadFromYaml(node);

    FOXZT_INFO("after: hello");
    FOXZT_LOGGER_ERROR(LOGGER_NAME_RAW("system"), "after: hello");
    std::set<int> vec{1, 2, 3, 2};
    FOXZT_INFO("after: hello{}", vec);
}


int main() {
    test_log_config();

    auto out = fmt::output_file("guide.txt");
    out.print("Don't {}", "Panic");

    return 0;
}