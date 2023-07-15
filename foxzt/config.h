//
// Created by fox on 2023/7/13.
//

#ifndef CPPPROJ_CONFIG_H
#define CPPPROJ_CONFIG_H

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "util.h"
#include "log.h"

namespace foxzt {
    class ConfigVarBase {
    public:
        using ptr = std::shared_ptr<ConfigVarBase>;

        explicit ConfigVarBase(std::string name, std::string description = "")
                : m_name(std::move(name)), m_description(std::move(description)) {
            std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
        }

        virtual ~ConfigVarBase() = default;

        const std::string &getMName() const {
            return m_name;
        }

        const std::string &getMDescription() const {
            return m_description;
        }

        virtual std::string toString() = 0;

        virtual bool fromString(const std::string &val) = 0;

        virtual std::string getTypeName() const = 0;

    protected:
        /// 配置参数的名称
        std::string m_name;
        /// 配置参数的描述
        std::string m_description;
    };

    template<class F, class T>
    class LexicalCast {
    public:
        T operator()(const F &v) {
            return boost::lexical_cast<T>(v);
        }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::vector<T>)
     */
    template<class T>
    class LexicalCast<std::string, std::vector<T> > {
    public:
        std::vector<T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::vector<T> vec;
            std::stringstream ss;
            for (size_t i = 0; i < node.size(); ++i) {
                ss.str("");
                ss << node[i];
                vec.push_back(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    /**
     * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::vector<T>, std::string> {
    public:
        std::string operator()(const std::vector<T> &v) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: v) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
    class ConfigVar : public ConfigVarBase {
    public:
        using ptr = std::shared_ptr<ConfigVar>;

        ConfigVar(const std::string &name, T mVal, const std::string &description = "") : ConfigVarBase(name,
                                                                                                        description),
                                                                                          m_val(mVal) {}

        std::string toString() override {
            try {
                //return boost::lexical_cast<std::string>(m_val);
                return ToStr()(m_val);
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::toString exception {} convert: {} to string", e.what(), typeid(m_val).name());
            }
            return "";
        }

        bool fromString(const std::string &val) override {
            try {
                //m_val = boost::lexical_cast<T>(val);
                setMVal(FromStr()(val));
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::fromString exception {} convert: string to {}", e.what(), typeid(m_val).name());
            }
            return false;
        }

        std::string getTypeName() const override {
            return TypeToName<T>();;
        }

        void setMVal(T mVal) {
            m_val = mVal;
        }

        T getMVal() const {
            return m_val;
        }

    private:
        T m_val;
    };

    class Config {
    public:
        using ConfigVarMap = std::map<std::string, ConfigVarBase::ptr>;

        template<class T>
        static typename ConfigVar<T>::ptr
        Lookup(const std::string &name, const T &default_value, const std::string &description = "") {
            auto tmp = Lookup<T>(name);
            if (tmp) {
                FOXZT_INFO("Lookup name = {} exists", name);
                return tmp;
            }

            if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
                != std::string::npos) {
                FOXZT_ERROR("Lookup name invalid {}", name);
                throw std::invalid_argument(name);
            }
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
            s_datas[name] = v;
            return v;
        }

        template<class T>
        static typename ConfigVar<T>::ptr
        Lookup(const std::string &name) {
            auto it = s_datas.find(name);
            if (it == s_datas.end()) {
                return nullptr;
            }
            return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
        }

        static ConfigVarBase::ptr LookupBase(const std::string &name);

        static void LoadFromYaml(const YAML::Node &root);

    private:
        static ConfigVarMap s_datas;

        static ConfigVarMap &GetDatas();
    };

    Config::ConfigVarMap &Config::GetDatas() {
        //static ConfigVarMap s_datas;
        return s_datas;
    }

    Config::ConfigVarMap Config::s_datas;

    ConfigVarBase::ptr Config::LookupBase(const std::string &name) {
        auto it = GetDatas().find(name);
        return it == GetDatas().end() ? nullptr : it->second;
    }

    static void ListAllMember(const std::string &prefix,
                              const YAML::Node &node,
                              std::list<std::pair<std::string, const YAML::Node>> &output) {
        if (prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
            != std::string::npos) {
            std::cerr << "Config invalid name: " << prefix << " : " << node;
            return;
        }
        output.emplace_back(prefix, node);
        if (node.IsMap()) {
            for (auto it = node.begin();
                 it != node.end(); ++it) {
                ListAllMember(prefix.empty() ? it->first.Scalar()
                                             : prefix + "." + it->first.Scalar(), it->second, output);
            }
        }
    }

    void Config::LoadFromYaml(const YAML::Node &root) {
        std::list<std::pair<std::string, const YAML::Node> > all_nodes;
        ListAllMember("", root, all_nodes);

        for (auto &i: all_nodes) {
            std::string key = i.first;
            if (key.empty()) {
                continue;
            }

            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            ConfigVarBase::ptr var = LookupBase(key);

            if (var) {
                if (i.second.IsScalar()) {
                    var->fromString(i.second.Scalar());
                } else {
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
    }
}

#endif //CPPPROJ_CONFIG_H
