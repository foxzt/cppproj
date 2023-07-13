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

    template<class T>
    class ConfigVar : public ConfigVarBase {
    public:
        using ptr = std::shared_ptr<ConfigVar>;

        ConfigVar(const std::string &name, T mVal, const std::string &description = "") : ConfigVarBase(name,
                                                                                                        description),
                                                                                          m_val(mVal) {}

        std::string toString() override {
            try {
                return boost::lexical_cast<std::string>(m_val);
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::toString exception {} convert: {} to string", e.what(), typeid(m_val).name());
            }
            return "";
        }

        bool fromString(const std::string &val) override {
            try {
                m_val = boost::lexical_cast<T>(val);
            } catch (std::exception &e) {
                FOXZT_ERROR("ConfigVar::fromString exception {} convert: string to {}", e.what(), typeid(m_val).name());
            }
            return "";
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

    private:
        static ConfigVarMap s_datas;
    };
    Config::ConfigVarMap Config::s_datas;
}

#endif //CPPPROJ_CONFIG_H
