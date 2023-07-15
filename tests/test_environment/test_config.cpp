#include "foxzt/foxzt.h"
#include <fmt/ranges.h>

foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup("system.port", (int) 100, "system port");
foxzt::ConfigVar<std::string>::ptr g_string = foxzt::Config::Lookup("system.pattern", std::string(), "system pattern");

foxzt::ConfigVar<std::vector<int>>::ptr g_vec = foxzt::Config::Lookup("system.int_vec", std::vector<int>{1, 2},
                                                                      "system int vec");

int main() {
    FOXZT_INFO("before name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("before name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_int->getTypeName());
    auto v = g_vec->getMVal();
    for (auto item: v) {
        FOXZT_INFO("int_vec: {} ", item);
    }

    YAML::Node node = YAML::LoadFile("../tests/test_environment/log.yml");
    foxzt::Config::LoadFromYaml(node);

    v = g_vec->getMVal();
    for (auto item: v) {
        FOXZT_INFO("int_vec: {} ", item);
    }
    FOXZT_INFO("after name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("after name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_string->getTypeName());

    FOXZT_INFO("中文");

    YAML::Node node1 = YAML::Load("[1, 2]");
    std::stringstream ss;
    std::vector<int> vec;
    for (auto i: node1) {
        ss.str("");
        ss << i;
        vec.push_back(foxzt::LexicalCast<std::string, int>()(ss.str()));
    }
    fmt::print("{}\n", vec);

    g_vec->fromString("[100, 200]");
    v = g_vec->getMVal();
    for (auto item: v) {
        FOXZT_INFO("int_vec: {} ", item);
    }

    return 0;
}