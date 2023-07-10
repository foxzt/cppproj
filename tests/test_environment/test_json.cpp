//
// Created by fox on 2023/7/10.
//
#include <iostream>
#include <json/json.h>

int main() {
    // 创建一个包含 JSON 数据的字符串
    std::string jsonString = R"(
        {
            "name": "John",
            "age": 30,
            "city": "New York"
        }
    )";

    // 解析 JSON 字符串
    Json::Value root;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(jsonString, root);
    if (!parsingSuccessful) {
        std::cout << "Failed to parse JSON string." << std::endl;
        return 1;
    }

    // 从 JSON 对象中获取值
    std::string name = root["name"].asString();
    int age = root["age"].asInt();
    std::string city = root["city"].asString();

    // 打印获取到的值
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "City: " << city << std::endl;

    // 构建 JSON 对象
    Json::Value newRoot;
    newRoot["name"] = "Alice";
    newRoot["age"] = 25;
    newRoot["city"] = "London";

    // 将 JSON 对象转换为字符串
    Json::FastWriter writer;
    std::string newJsonString = writer.write(newRoot);

    // 打印新的 JSON 字符串
    std::cout << "New JSON string: " << newJsonString << std::endl;

    return 0;
}
