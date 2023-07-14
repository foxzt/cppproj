//
// Created by fox on 2023/7/10.
//
#include <yaml-cpp/yaml.h>
#include "foxzt/log.h"

void printYamlNode(const YAML::Node &node, int level = 0) {
    // 打印节点的键和值
    if (!node.IsNull()) {
        for (int i = 0; i < level; ++i) {
            std::cout << "  ";  // 缩进
        }

        if (node.IsScalar()) {
            std::cout << "- " << node.Scalar() << std::endl;
        } else if (node.IsSequence()) {
            std::cout << "- [Sequence]" << std::endl;
            for (const auto &element: node) {
                printYamlNode(element, level + 1);
            }
        } else if (node.IsMap()) {
            std::cout << "- {Map}" << std::endl;
            for (const auto &pair: node) {
                const auto &key = pair.first;
                const auto &value = pair.second;
                for (int i = 0; i < level + 1; ++i) {
                    std::cout << "  ";  // 缩进
                }
                std::cout << key << ":";
                printYamlNode(value, level + 1);
            }
        }
    }
}

int main() {

    YAML::Node root = YAML::LoadFile("log.yml");
    printYamlNode(root);
    FOXZT_INFO("hello {}","world !");
    return 0;
}