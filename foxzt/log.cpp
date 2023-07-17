//
// Created by fox on 2023/7/10.
//

#include "log.h"
#include "config.h"

namespace foxzt {
    std::string logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARN:
                return "WARN";
            case LogLevel::ERROR:
                return "ERROR";
            case LogLevel::FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    LogLevel logLevelFromString(const std::string &levelString) {
        static const std::unordered_map<std::string, LogLevel> levelMap{
                {"DEBUG", LogLevel::DEBUG},
                {"INFO",  LogLevel::INFO},
                {"WARN",  LogLevel::WARN},
                {"ERROR", LogLevel::ERROR},
                {"FATAL", LogLevel::FATAL},
                {"debug", LogLevel::DEBUG},
                {"info",  LogLevel::INFO},
                {"warn",  LogLevel::WARN},
                {"error", LogLevel::ERROR},
                {"fatal", LogLevel::FATAL}
        };

        auto iter = levelMap.find(levelString);
        if (iter != levelMap.end()) {
            return iter->second;
        } else {
            // 处理无效的日志级别字符串
            throw std::runtime_error("Invalid log level");
            // 或者返回默认级别
            //return LogLevel::DEBUG;
        }
    }

    std::string LogFormatter::format(LogLevel level, const LogEvent::ptr &event) {
        std::stringstream ss;
        for (auto &i: m_items) {
            i->format(ss, level, event);
        }
        return ss.str();
    }

    std::ostream &LogFormatter::format(std::ostream &ofs, LogLevel level, const LogEvent::ptr &event) {
        for (auto &i: m_items) {
            i->format(ofs, level, event);
        }
        return ofs;
    }

    void StdoutLogAppender::log(LogLevel level, LogEvent::ptr event) {
        if (level >= m_level) {
            m_formatter->format(std::cout, level, event);
        }
    }

    std::string StdoutLogAppender::toYamlString() {
        YAML::Node node;
        node["type"] = "StdoutLogAppender";
        if (m_level != (LogLevel) -1) {
            node["level"] = logLevelToString(m_level);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    void FileLogAppender::log(LogLevel level, LogEvent::ptr event) {
        if (level >= m_level) {
            m_formatter->format(m_filestream, level, event);
        }
    }

    std::string FileLogAppender::toYamlString() {
        YAML::Node node;
        node["type"] = "FileLogAppender";
        node["file"] = m_filename;
        if (m_level != (LogLevel) -1) {
            node["level"] = logLevelToString(m_level);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    void Logger::setFormatter(const std::string &val) {
        LogFormatter::ptr new_val(new LogFormatter(val));
        m_formatter = new_val;
        for (auto &i: m_appenders) {
            i->setMFormatter(m_formatter);
        }
    }

    void Logger::clearAppenders() {
        m_appenders.clear();
    }

    void Logger::delAppender(const LogAppender::ptr &appender) {
        for (auto it = m_appenders.begin();
             it != m_appenders.end(); ++it) {
            if (*it == appender) {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void Logger::addAppender(const LogAppender::ptr &appender) {
        m_appenders.push_back(appender);
        m_appenders.back()->setMFormatter(m_formatter);
    }

    void Logger::log(LogLevel level, const LogEvent::ptr &event) {
        if (level >= m_level) {
            if (!m_appenders.empty()) {
                for (auto &i: m_appenders) {
                    i->log(level, event);
                }
            }
        }
    }

    void Logger::debug(const LogEvent::ptr &event) {
        log(LogLevel::DEBUG, event);
    }

    void Logger::info(const LogEvent::ptr &event) {
        log(LogLevel::INFO, event);
    }

    void Logger::warn(const LogEvent::ptr &event) {
        log(LogLevel::WARN, event);
    }

    void Logger::error(const LogEvent::ptr &event) {
        log(LogLevel::ERROR, event);
    }

    void Logger::fatal(const LogEvent::ptr &event) {
        log(LogLevel::FATAL, event);
    }

    std::string Logger::toYamlString() {
        YAML::Node node;
        node["name"] = m_name;
        if (m_level != (LogLevel) -1) {
            node["level"] = logLevelToString(m_level);
        }
        if (m_formatter) {
            node["formatter"] = m_formatter->getMPattern();
        }

        for (auto &i: m_appenders) {
            node["appenders"].push_back(YAML::Load(i->toYamlString()));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }

    //日志级别
    class LevelFormatItem : public LogFormatter::FormatItem {
    public:
        explicit LevelFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << logLevelToString(event->getMLevel());
        }
    };

    //日志器名称
    class LoggerNameFormatItem : public LogFormatter::FormatItem {
    public:
        explicit LoggerNameFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMLoggerName();
        }
    };

    //线程号
    class ThreadIdFormatItem : public LogFormatter::FormatItem {
    public:
        explicit ThreadIdFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMThreadId();
        }
    };

    //协程号
    class FiberIdFormatItem : public LogFormatter::FormatItem {
    public:
        explicit FiberIdFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMFiberId();
        }
    };

    //文件名
    class FilenameFormatItem : public LogFormatter::FormatItem {
    public:
        explicit FilenameFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMFile();
        }
    };

    //行号
    class LineFormatItem : public LogFormatter::FormatItem {
    public:
        explicit LineFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMLine();
        }
    };

    //换行
    class NewLineFormatItem : public LogFormatter::FormatItem {
    public:
        explicit NewLineFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << std::endl;
        }
    };

    //模式串中的非占位符
    class CharFormatItem : public LogFormatter::FormatItem {
    public:
        explicit CharFormatItem(char c) : m_char(c) {}

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << m_char;
        }

    private:
        char m_char;
    };

    //消息
    class MessageFormatItem : public LogFormatter::FormatItem {
    public:
        explicit MessageFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getContent();
        }
    };

    //年
    class YearFormatItem : public LogFormatter::FormatItem {
    public:
        explicit YearFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%Y");
        }
    };

    //月
    class MonthFormatItem : public LogFormatter::FormatItem {
    public:
        explicit MonthFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%m");
        }
    };

    //日
    class DayFormatItem : public LogFormatter::FormatItem {
    public:
        explicit DayFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%d");
        }
    };

    //小时
    class HourFormatItem : public LogFormatter::FormatItem {
    public:
        explicit HourFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%H");
        }
    };

    //分钟
    class MinuteFormatItem : public LogFormatter::FormatItem {
    public:
        explicit MinuteFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%M");
        }
    };

    //秒
    class SecondFormatItem : public LogFormatter::FormatItem {
    public:
        explicit SecondFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            std::time_t now_c = std::chrono::system_clock::to_time_t(event->getMTime());
            os << std::put_time(std::localtime(&now_c), "%S");
        }
    };

    //毫秒
    class MillisecondsFormatItem : public LogFormatter::FormatItem {
    public:
        explicit MillisecondsFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            auto ms =
                    std::chrono::duration_cast<std::chrono::milliseconds>(event->getMTime().time_since_epoch()) % 1000;
            os << std::setfill('0') << std::setw(3) << ms.count();
        }
    };

    //文件名：行号
    class FilenameAndLineFormatItem : public LogFormatter::FormatItem {
    public:
        explicit FilenameAndLineFormatItem() = default;

        void format(std::ostream &os, LogLevel level, LogEvent::ptr event) override {
            os << event->getMFile() << ":" << event->getMLine();
        }
    };

    void LogFormatter::init() {
        for (std::size_t i = 0; i < m_pattern.length(); ++i) {
            if (m_pattern[i] == '%') {
                if (i + 1 < m_pattern.length()) {
                    switch (m_pattern[i + 1]) {
                        case 'Y': // 年
                            m_items.push_back(std::shared_ptr<FormatItem>(new YearFormatItem));
                            break;
                        case 'm': // 月
                            m_items.push_back(std::shared_ptr<FormatItem>(new MonthFormatItem));
                            break;
                        case 'd': // 日
                            m_items.push_back(std::shared_ptr<FormatItem>(new DayFormatItem));
                            break;
                        case 'H': // 小时（24小时制）
                            m_items.push_back(std::shared_ptr<FormatItem>(new HourFormatItem));
                            break;
                        case 'M': // 分钟
                            m_items.push_back(std::shared_ptr<FormatItem>(new MinuteFormatItem));
                            break;
                        case 'S': // 秒
                            m_items.push_back(std::shared_ptr<FormatItem>(new SecondFormatItem));
                            break;
                        case 'e': // 毫秒
                            m_items.push_back(std::shared_ptr<FormatItem>(new MillisecondsFormatItem));
                            break;
                        case 'l': // 日志事件等级
                            m_items.push_back(std::shared_ptr<FormatItem>(new LevelFormatItem));
                            break;
                        case '@': // 文件名：行号
                            m_items.push_back(std::shared_ptr<FormatItem>(new FilenameAndLineFormatItem));
                            break;
                        case 'v': // 日志消息
                            m_items.push_back(std::shared_ptr<FormatItem>(new MessageFormatItem));
                            break;
                        case 'n': // 日志器名称
                            m_items.push_back(std::shared_ptr<FormatItem>(new LoggerNameFormatItem));
                            break;
                        case 't': // 线程号
                            m_items.push_back(std::shared_ptr<FormatItem>(new ThreadIdFormatItem));
                            break;
                        case 'F': // 协程号
                            m_items.push_back(std::shared_ptr<FormatItem>(new FiberIdFormatItem));
                            break;
                        case 'g': // 文件名（带路径）
                            m_items.push_back(std::shared_ptr<FormatItem>(new FilenameFormatItem));
                            break;
                        case '#': // 文件名（带路径）
                            m_items.push_back(std::shared_ptr<FormatItem>(new LineFormatItem));
                            break;
                        default:
                            m_items.push_back(std::shared_ptr<FormatItem>(new CharFormatItem(m_pattern[i + 1])));
                            break;
                    }
                    ++i; // Skip the next character since it has been processed
                } else {
                    m_items.push_back(std::shared_ptr<FormatItem>(new CharFormatItem(
                            m_pattern[i]))); // Treat '%' at the end of the pattern as a regular character
                }
            } else {
                m_items.push_back(std::shared_ptr<FormatItem>(new CharFormatItem(m_pattern[i]))); // Regular character
            }
        }
        m_items.push_back(std::shared_ptr<FormatItem>(new NewLineFormatItem));
    }

    const std::string &LogFormatter::getMPattern() const {
        return m_pattern;
    }

    LoggerManager::LoggerManager() {
        m_default.reset(new Logger);
        m_loggers[m_default->getMName()] = m_default;
    }

    Logger::ptr LoggerManager::getLogger(const std::string &name) {
        auto it = m_loggers.find(name);
        if (it != m_loggers.end()) {
            return it->second;
        }

        Logger::ptr logger(new Logger(name));
        m_loggers[name] = logger;
        return logger;
    }

    std::string LoggerManager::toYamlString() {
        YAML::Node node;
        for (auto &i: m_loggers) {
            node.push_back(YAML::Load(i.second->toYamlString()));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
}