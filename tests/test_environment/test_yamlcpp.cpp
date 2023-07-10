//
// Created by fox on 2023/7/10.
//
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <fstream>

void fun() {
    // 创建一个新的 YAML 节点
    YAML::Node newConfig;
    newConfig["name"] = "Alice";
    newConfig["age"] = 25;
    newConfig["hobbies"].push_back("Reading");
    newConfig["hobbies"].push_back("Painting");

    // 将新的 YAML 节点写入文件
    YAML::Emitter emitter;
    emitter << newConfig;

    std::ofstream fout("new_config.yaml");
    fout << emitter.c_str();
    fout.close();

    std::cout << "New config file has been created." << std::endl;
}


int main() {
    // YAML 字符串
    std::string yamlString = R"(
        name: John
        age: 30
        hobbies:
          - Reading
          - Painting
    )";

    // 从字符串加载 YAML 数据
    YAML::Node config = YAML::Load(yamlString);

    // 从 YAML 节点中提取值
    auto name = config["name"].as<std::string>();
    int age = config["age"].as<int>();
    auto hobbies = config["hobbies"].as<std::vector<std::string>>();

    // 输出读取到的值
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Hobbies: ";
    for (const auto &hobby: hobbies) {
        std::cout << hobby << " ";
    }
    std::cout << std::endl;

    // 创建一个新的 YAML 节点
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << "Alice";
    emitter << YAML::Key << "age" << YAML::Value << 25;
    emitter << YAML::Key << "hobbies" << YAML::Value << YAML::BeginSeq << "Reading" << "Painting" << YAML::EndSeq;
    emitter << YAML::EndMap;

    // 获取生成的 YAML 字符串
    std::string newConfigString = emitter.c_str();

    // 输出新的 YAML 字符串
    std::cout << "New Config:\n" << newConfigString << std::endl;


    fun();

    return 0;
}
