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
#include <shared_mutex>
#include <iostream>

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

        [[nodiscard]] const std::string &getMName() const {
            return m_name;
        }

        [[nodiscard]] const std::string &getMDescription() const {
            return m_description;
        }

        virtual std::string toString() = 0;

        virtual bool fromString(const std::string &val) = 0;

        [[nodiscard]] virtual std::string getTypeName() const = 0;

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
            for (auto &&i: node) {
                ss.str("");
                ss << i;
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
            for (auto &&i: v) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    /**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::list<T>)
 */
    template<class T>
    class LexicalCast<std::string, std::list<T> > {
    public:
        std::list<T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::list<T> vec;
            std::stringstream ss;
            for (auto &&i: node) {
                ss.str("");
                ss << i;
                vec.push_back(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

/**
 * @brief 类型转换模板类片特化(std::list<T> 转换成 YAML String)
 */
    template<class T>
    class LexicalCast<std::list<T>, std::string> {
    public:
        std::string operator()(const std::list<T> &v) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: v) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::set<T>)
 */
    template<class T>
    class LexicalCast<std::string, std::set<T> > {
    public:
        std::set<T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::set<T> vec;
            std::stringstream ss;
            for (auto &&i: node) {
                ss.str("");
                ss << i;
                vec.insert(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

/**
 * @brief 类型转换模板类片特化(std::set<T> 转换成 YAML String)
 */
    template<class T>
    class LexicalCast<std::set<T>, std::string> {
    public:
        std::string operator()(const std::set<T> &v) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &&i: v) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_set<T>)
 */
    template<class T>
    class LexicalCast<std::string, std::unordered_set<T> > {
    public:
        std::unordered_set<T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::unordered_set<T> vec;
            std::stringstream ss;
            for (auto &&i: node) {
                ss.str("");
                ss << i;
                vec.insert(LexicalCast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

/**
 * @brief 类型转换模板类片特化(std::unordered_set<T> 转换成 YAML String)
 */
    template<class T>
    class LexicalCast<std::unordered_set<T>, std::string> {
    public:
        std::string operator()(const std::unordered_set<T> &v) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: v) {
                node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::map<std::string, T>)
 */
    template<class T>
    class LexicalCast<std::string, std::map<std::string, T> > {
    public:
        std::map<std::string, T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::map<std::string, T> vec;
            std::stringstream ss;
            for (auto it = node.begin();
                 it != node.end(); ++it) {
                ss.str("");
                ss << it->second;
                vec.insert(std::make_pair(it->first.Scalar(),
                                          LexicalCast<std::string, T>()(ss.str())));
            }
            return vec;
        }
    };

/**
 * @brief 类型转换模板类片特化(std::map<std::string, T> 转换成 YAML String)
 */
    template<class T>
    class LexicalCast<std::map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::map<std::string, T> &v) {
            YAML::Node node(YAML::NodeType::Map);
            for (auto &i: v) {
                node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_map<std::string, T>)
 */
    template<class T>
    class LexicalCast<std::string, std::unordered_map<std::string, T> > {
    public:
        std::unordered_map<std::string, T> operator()(const std::string &v) {
            YAML::Node node = YAML::Load(v);
            typename std::unordered_map<std::string, T> vec;
            std::stringstream ss;
            for (auto it = node.begin();
                 it != node.end(); ++it) {
                ss.str("");
                ss << it->second;
                vec.insert(std::make_pair(it->first.Scalar(),
                                          LexicalCast<std::string, T>()(ss.str())));
            }
            return vec;
        }
    };

/**
 * @brief 类型转换模板类片特化(std::unordered_map<std::string, T> 转换成 YAML String)
 */
    template<class T>
    class LexicalCast<std::unordered_map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::unordered_map<std::string, T> &v) {
            YAML::Node node(YAML::NodeType::Map);
            for (auto &i: v) {
                node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    template<>
    class LexicalCast<std::string, LogDefine> {
    public:
        LogDefine operator()(const std::string &v) {
            YAML::Node n = YAML::Load(v);
            LogDefine ld;
            if (!n["name"].IsDefined()) {
                std::cout << "log config error: name is null, " << n
                          << std::endl;
                std::stringstream ss;
                ss << n;
                FOXZT_ERROR("log config error: name is null, detailed information:\n{}", ss.str());
                throw std::logic_error("log config error: logger name is null");
            }
            ld.name = n["name"].as<std::string>();
            ld.level = logLevelFromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
            if (n["formatter"].IsDefined()) {
                ld.formatter = n["formatter"].as<std::string>();
            }

            if (n["appenders"].IsDefined()) {
                for (size_t x = 0; x < n["appenders"].size(); ++x) {
                    auto a = n["appenders"][x];
                    if (!a["type"].IsDefined()) {
                        std::stringstream ss;
                        ss << a;
                        FOXZT_ERROR(
                                "log config error: logger has a invalid appender which type is null, detailed information:\n{}",
                                ss.str());
                        throw std::logic_error("log config error: logger has a invalid appender which type is null");
                    }
                    auto type = a["type"].as<std::string>();
                    LogAppenderDefine lad;
                    if (type == "FileLogAppender") {
                        lad.type = 1;
                        if (!a["file"].IsDefined()) {
                            std::cout << "log config error: FileAppender file is null, " << a
                                      << std::endl;
                            continue;
                        }
                        lad.file = a["file"].as<std::string>();
                        if (a["level"].IsDefined()) {
                            lad.level = logLevelFromString(a["level"].as<std::string>());
                        }
                    } else if (type == "StdoutLogAppender") {
                        lad.type = 2;
                        if (a["level"].IsDefined()) {
                            lad.level = logLevelFromString(a["level"].as<std::string>());
                        }
                    } else {
                        std::cout << "log config error: appender type is invalid, " << a
                                  << std::endl;
                        continue;
                    }

                    ld.appenders.push_back(lad);
                }
            }
            return ld;
        }
    };

    template<>
    class LexicalCast<LogDefine, std::string> {
    public:
        std::string operator()(const LogDefine &i) {
            YAML::Node n;
            n["name"] = i.name;
            if (i.level != (LogLevel) -1) {
                n["level"] = logLevelToString(i.level);
            }
            if (!i.formatter.empty()) {
                n["formatter"] = i.formatter;
            }

            for (auto &a: i.appenders) {
                YAML::Node na;
                if (a.type == 1) {
                    na["type"] = "FileLogAppender";
                    na["file"] = a.file;
                } else if (a.type == 2) {
                    na["type"] = "StdoutLogAppender";
                }
                if (a.level != (LogLevel) -1) {
                    na["level"] = logLevelToString(a.level);
                    FOXZT_INFO(logLevelToString(a.level));//调试
                }

                n["appenders"].push_back(na);
            }
            std::stringstream ss;
            ss << n;
            return ss.str();
        }
    };

    template<class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
    class ConfigVar : public ConfigVarBase {
    public:
        using ptr = std::shared_ptr<ConfigVar>;
        using MutexType = std::shared_mutex;
        using on_change_cb = std::function<void(const T &old_value, const T &new_value)>;

        ConfigVar(const std::string &name, T mVal, const std::string &description = "") : ConfigVarBase(name,
                                                                                                        description),
                                                                                          m_val(mVal) {}

        std::string toString() override {
            try {
                std::shared_lock<MutexType> lock(m_mutex);
                return ToStr()(m_val);
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::toString exception {} convert: {} to string", e.what(), typeid(m_val).name());
            }
            return "";
        }

        bool fromString(const std::string &val) override {
            try {
                setMVal(FromStr()(val));
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::fromString exception {} convert: string to {}", e.what(), typeid(m_val).name());
            }
            return false;
        }

        [[nodiscard]] std::string getTypeName() const override {
            return TypeToName<T>();
        }

        void setMVal(T mVal) {
            {
                std::shared_lock<MutexType> lock(m_mutex);
                if (mVal == m_val) {
                    return;
                }
                for (auto &i: m_cbs) {
                    i.second(m_val, mVal);
                }
            }
            std::unique_lock<MutexType> lock(m_mutex);
            m_val = mVal;
        }

        [[nodiscard]] T getMVal() {
            std::shared_lock<MutexType> lock(m_mutex);
            return m_val;
        }

        uint64_t addListener(on_change_cb cb) {
            static uint64_t s_fun_id = 0;
            std::unique_lock<MutexType> lock(m_mutex);
            ++s_fun_id;
            m_cbs[s_fun_id] = cb;
            return s_fun_id;
        }

        void delListener(uint64_t key) {
            std::unique_lock<MutexType> lock(m_mutex);
            m_cbs.erase(key);
        }

        void clearListener() {
            std::unique_lock<MutexType> lock(m_mutex);
            m_cbs.clear();
        }

        on_change_cb getListener(uint64_t key) {
            std::shared_lock<MutexType> lock(m_mutex);
            auto it = m_cbs.find(key);
            return it == m_cbs.end() ? nullptr : it->second;
        }

    private:
        T m_val;
        std::map<uint64_t, on_change_cb> m_cbs;
        MutexType m_mutex;
    };

    class Config {
    public:
        using ConfigVarMap = std::map<std::string, ConfigVarBase::ptr>;
        using MutexType = std::shared_mutex;

        template<class T>
        static typename ConfigVar<T>::ptr
        Lookup(const std::string &name, const T &default_value, const std::string &description = "") {
            std::unique_lock<MutexType> lock(GetMutex());
            auto it = GetDatas().find(name);
            if (it != GetDatas().end()) {
                auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
                if (tmp) {
                    FOXZT_INFO("Lookup name={} exists", name);
                    return tmp;
                } else {
                    FOXZT_ERROR("Lookup name={} exists but type not {}, real_type={} {}", name, TypeToName<T>(),
                                TypeToName<T>(), it->second->toString());
                    return nullptr;
                }
            }

            if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
                != std::string::npos) {
                FOXZT_ERROR("Lookup name invalid {}", name);
                throw std::invalid_argument(name);
            }
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
            GetDatas()[name] = v;
            return v;
        }

        template<class T>
        static typename ConfigVar<T>::ptr
        Lookup(const std::string &name) {
            std::shared_lock<MutexType> lock(GetMutex());
            auto it = GetDatas().find(name);
            if (it == GetDatas().end()) {
                return nullptr;
            }
            return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
        }

        static ConfigVarBase::ptr LookupBase(const std::string &name);

        static void LoadFromYaml(const YAML::Node &root);

    private:
        static ConfigVarMap &GetDatas();
        static MutexType& GetMutex();
    };

    extern foxzt::ConfigVar<std::set<foxzt::LogDefine>>::ptr g_log_defines;
}

#endif //CPPPROJ_CONFIG_H
