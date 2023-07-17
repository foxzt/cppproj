//
// Created by fox on 2023/7/13.
//

#include "config.h"

namespace foxzt {
    Config::ConfigVarMap &Config::GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }

    ConfigVarBase::ptr Config::LookupBase(const std::string &name) {
        auto it = GetDatas().find(name);
        return it == GetDatas().end() ? nullptr : it->second;
    }


    void ListAllMember(const std::string &prefix,
                       const YAML::Node &node,
                       std::list<std::pair<std::string, const YAML::Node>> &output) {
        if (prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
            != std::string::npos) {
            std::stringstream ss;
            ss << node;
            FOXZT_ERROR("Config invalid name: {} : {}", prefix, ss.str());
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

    ///为日志系统配置添加回调函数（冷知识，静态变量的初始化在main函数之前）
    foxzt::ConfigVar<std::set<foxzt::LogDefine>>::ptr g_log_defines =
            foxzt::Config::Lookup("logs", std::set<foxzt::LogDefine>(), "logs config");

    struct LogIniter {
        LogIniter() {
            g_log_defines->addListener([](const std::set<foxzt::LogDefine> &old_value,
                                          const std::set<foxzt::LogDefine> &new_value) {
                //FOXZT_INFO("on_logger_conf_changed");
                for (auto &i: new_value) {
                    foxzt::Logger::ptr logger;
                    logger = LOGGER_NAME_RAW(i.name);
                    logger->setMLevel(i.level);
                    if (!i.formatter.empty()) {
                        logger->setFormatter(i.formatter);
                    }

                    logger->clearAppenders();
                    for (auto &a: i.appenders) {
                        foxzt::LogAppender::ptr ap;
                        if (a.type == 1) {
                            ap.reset(new foxzt::FileLogAppender(a.file));
                        } else if (a.type == 2) {
                            ap.reset(new foxzt::StdoutLogAppender);
                        }
                        ap->setMLevel(a.level);
                        logger->addAppender(ap);
                    }
                }

                for (auto &i: old_value) {
                    auto it = new_value.find(i);
                    if (it == new_value.end()) {
                        //删除logger
                        auto logger = LOGGER_NAME_RAW(i.name);
                        logger->clearAppenders();
                    }
                }
            });
        }
    };

    static LogIniter log_init;

}