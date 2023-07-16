#include "foxzt/foxzt.h"
#include <fmt/ranges.h>
#include <ostream>

foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup<int>("system.port", (int) 8080, "system port");

//foxzt::ConfigVar<float>::ptr g_int_float_config = foxzt::Config::Lookup("system.port", (float) 8080, "system port");

foxzt::ConfigVar<std::string>::ptr g_string = foxzt::Config::Lookup("system.pattern", std::string(), "system pattern");

foxzt::ConfigVar<std::vector<int>>::ptr g_vec = foxzt::Config::Lookup("system.int_vec", std::vector<int>{1, 2},
                                                                      "system int vec");

foxzt::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config =
        foxzt::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system str int map");

void test_config() {
    FOXZT_INFO("before name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("before name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_int->getTypeName());
    FOXZT_INFO("before yaml: {}", g_int->toString());
    auto v = g_str_int_map_value_config->getMVal();
    for (auto item: v) {
        FOXZT_INFO("before string_int_map: {} ", item);
    }

    YAML::Node node = YAML::LoadFile("../tests/log.yml");
    foxzt::Config::LoadFromYaml(node);

    v = g_str_int_map_value_config->getMVal();
    for (auto item: v) {
        FOXZT_INFO("after string_int_map: {} ", item);
    }
    FOXZT_INFO("after yaml: {}", g_str_int_map_value_config->toString());

    FOXZT_INFO("after name: {} val: {} type: {}", g_int->getMName(), g_int->getMVal(), g_int->getTypeName());
    FOXZT_INFO("after name: {} val: {} type: {}", g_string->getMName(), g_string->getMVal(), g_string->getTypeName());
    FOXZT_INFO("after yaml: {}", g_int->toString());
}

class Person {
public:
    Person() = default;

    std::string m_name;
    int m_age = 0;
    bool m_sex = false;

    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           << "]";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const Person &person) {
        os << "m_name: " << person.m_name << " m_age: " << person.m_age << " m_sex: " << person.m_sex;
        return os;
    }

    bool operator==(const Person &oth) const {
        return m_name == oth.m_name
               && m_age == oth.m_age
               && m_sex == oth.m_sex;
    }
};

namespace foxzt {
    template<>
    class LexicalCast<std::string, Person> {
    public:
        Person operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            Person p;
            p.m_name = node["name"].as<std::string>();
            p.m_age = node["age"].as<int>();
            p.m_sex = node["sex"].as<bool>();
            return p;
        }
    };

    template<>
    class LexicalCast<Person, std::string> {
    public:
        std::string operator()(const Person &p) {
            YAML::Node node;
            node["name"] = p.m_name;
            node["age"] = p.m_age;
            node["sex"] = p.m_sex;
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };
};

foxzt::ConfigVar<Person>::ptr g_person =
        foxzt::Config::Lookup("class.person", Person(), "system person");

foxzt::ConfigVar<std::map<std::string, Person> >::ptr g_person_map =
        foxzt::Config::Lookup("class.map", std::map<std::string, Person>{{"p", Person()}}, "system person");

foxzt::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map =
        foxzt::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person> >(), "system person");

void test_class() {
    g_person->addListener([](const Person &old_value, const Person &new_value) {
        std::cout << "old_value=" << old_value.toString()
                  << " new_value=" << new_value.toString() << std::endl;
    });
    std::cout << "before: " << g_person->getMVal().toString() << std::endl;

    const auto &ma = g_person_map->getMVal();
    std::cout << "g_person_vec_map size:" << g_person_vec_map->getMVal().size() << std::endl;
    for (const auto &i: ma) {
        std::cout << i.first << i.second.toString() << std::endl;
    }

    std::cout << std::endl;
    YAML::Node node = YAML::LoadFile("../tests/log.yml");
    foxzt::Config::LoadFromYaml(node);

    std::cout << "after: " << g_person->getMVal().toString() << std::endl;
    const auto &ma1 = g_person_map->getMVal();
    std::cout << "g_person_vec_map size:" << g_person_vec_map->getMVal().size() << std::endl;
    const auto &maa = g_person_vec_map->getMVal();
    for (const auto &i: maa) {
        std::cout << i.first << std::endl;
        for (const auto &ii: i.second) {
            std::cout << ii.toString() << std::endl;
        }
    }
}

int main() {

    test_class();

    return 0;
}