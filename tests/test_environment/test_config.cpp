//
// Created by fox on 2023/7/13.
//
#include "foxzt/foxzt.h"

foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup("system.port", (int) 100, "system port");
foxzt::ConfigVar<std::string>::ptr g_string = foxzt::Config::Lookup("system.pattern", std::string(), "system pattern");


int main() {

    FOXZT_WARN("before val: {} type: {}", g_int->getMVal(), g_int->getTypeName());
    FOXZT_WARN("before val: {} type: {}", g_string->getMVal(), g_int->getTypeName());

    YAML::Node node = YAML::LoadFile("../tests/test_environment/log.yml");
    foxzt::Config::LoadFromYaml(node);

    FOXZT_WARN("after val: {} type: {}", g_int->getMVal(), g_int->getTypeName());
    FOXZT_WARN("after val: {} type: {}", g_string->getMVal(), g_string->getTypeName());
    return 0;
}