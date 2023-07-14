#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <map>
#include "foxzt/log.h"
#include "foxzt/config.h"

// NOLINT(cppcoreguidelines-recursion)
void printYamlNodeType(const YAML::Node &node, int level = 0) {
    if (node.IsNull()) return;
    if (node.IsScalar()) {
        std::cout << node.as<std::string>() << std::endl;
    } else if (node.IsSequence()) {
        for (const auto &element: node) {
            printYamlNodeType(element, level + 1);
        }
    } else if (node.IsMap()) {
        for (const auto &kv: node) {
            std::cout << kv.first.as<std::string>() << ".";
            printYamlNodeType(kv.second);
        }
    }
}



int main() {

    return 0;
}
