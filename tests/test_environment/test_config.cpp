#include "foxzt/foxzt.h"
#include <fmt/ranges.h>

foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup<int>("system.port", (int) 8080, "system port");

foxzt::ConfigVar<float>::ptr g_int_float_config =
        foxzt::Config::Lookup("system.port", (float) 8080, "system port");

foxzt::ConfigVar<std::string>::ptr g_string = foxzt::Config::Lookup("system.pattern", std::string(), "system pattern");

foxzt::ConfigVar<std::vector<int>>::ptr g_vec = foxzt::Config::Lookup("system.int_vec", std::vector<int>{1, 2},
                                                                      "system int vec");

foxzt::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config =
        foxzt::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system str int map");

int main() {
    FOXZT_INFO("before name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("before name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_int->getTypeName());
    FOXZT_INFO("before yaml: {}", g_int->toString());
    auto v = g_str_int_map_value_config->getMVal();
    for (auto item: v) {
        FOXZT_INFO("before string_int_map: {} ", item);
    }

    YAML::Node node = YAML::LoadFile("../tests/test_environment/log.yml");
    foxzt::Config::LoadFromYaml(node);

    v = g_str_int_map_value_config->getMVal();
    for (auto item: v) {
        FOXZT_INFO("after string_int_map: {} ", item);
    }
    FOXZT_INFO("after yaml: {}", g_str_int_map_value_config->toString());

    FOXZT_INFO("after name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("after name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_string->getTypeName());
    FOXZT_INFO("after yaml: {}", g_int->toString());

    std::stringstream ss;
    ss << node;
    YAML::Node node1(YAML::NodeType::Map);
    node1["aaa"] = "sdf";
    std::cout << node1 << std::endl;


    return 0;
}