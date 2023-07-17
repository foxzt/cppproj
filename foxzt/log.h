//
// Created by fox on 2023/7/10.
//

#ifndef CPPPROJ_LOG_H
#define CPPPROJ_LOG_H

#include <memory>
#include <string>
#include <memory>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <cstdint>
#include <cstdarg>
#include <list>
#include <fmt/core.h>

#include <ostream>
#include "singleton.h"
#include "util.h"

/**
 * @brief 获取默认日志器
 */
#define DEFAULT_LOGGER_RAW() foxzt::LoggerMgr::GetInstance()->getDefault()

/**
 * @brief 获取name的日志器
 */
#define LOGGER_NAME_RAW(name) foxzt::LoggerMgr::GetInstance()->getLogger(name)

#define FOXZT_LOGGER_CALL(logger, level, fmts, ...) \
do { \
    auto event = std::make_shared<foxzt::LogEvent>( \
        level, __FILE__, __LINE__, foxzt::GetThreadId(), \
        foxzt::GetFiberId(), foxzt::getNowTime(), (logger)->getMName()); \
    event->getMSs() << fmt::format(fmts, ##__VA_ARGS__); \
    (logger)->log(level, event); \
} while(0)

#define FOXZT_LOGGER_DEBUG(logger, ...) FOXZT_LOGGER_CALL(logger, foxzt::LogLevel::DEBUG, __VA_ARGS__)

#define FOXZT_DEBUG(...) FOXZT_LOGGER_DEBUG(DEFAULT_LOGGER_RAW(), __VA_ARGS__)

#define FOXZT_LOGGER_INFO(logger, ...) FOXZT_LOGGER_CALL(logger, foxzt::LogLevel::INFO, __VA_ARGS__)

#define FOXZT_INFO(...) FOXZT_LOGGER_INFO(DEFAULT_LOGGER_RAW(), __VA_ARGS__)

#define FOXZT_LOGGER_WARN(logger, ...) FOXZT_LOGGER_CALL(logger, foxzt::LogLevel::WARN, __VA_ARGS__)

#define FOXZT_WARN(...) FOXZT_LOGGER_WARN(DEFAULT_LOGGER_RAW(), __VA_ARGS__)

#define FOXZT_LOGGER_ERROR(logger, ...) FOXZT_LOGGER_CALL(logger, foxzt::LogLevel::ERROR, __VA_ARGS__)

#define FOXZT_ERROR(...) FOXZT_LOGGER_ERROR(DEFAULT_LOGGER_RAW(), __VA_ARGS__)

#define FOXZT_LOGGER_FATAL(logger, ...) FOXZT_LOGGER_CALL(logger, foxzt::LogLevel::FATAL, __VA_ARGS__)

#define FOXZT_FATAL(...) FOXZT_LOGGER_FATAL(DEFAULT_LOGGER_RAW(), __VA_ARGS__)

namespace foxzt {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    std::string logLevelToString(LogLevel level);

    LogLevel logLevelFromString(const std::string &levelString);


    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent(LogLevel mLevel, const char *mFile, int32_t mLine, long mThreadId, uint32_t mFiberId,
                 const std::chrono::time_point<std::chrono::system_clock> &mTime,
                 std::string mLoggerName) : m_level(mLevel), m_file(mFile),
                                            m_line(mLine),
                                            m_threadId(mThreadId),
                                            m_fiberId(mFiberId),
                                            m_time(mTime),
                                            m_loggerName(std::move(mLoggerName)) {}

        LogLevel getMLevel() const {
            return m_level;
        }

        long getMThreadId() const {
            return m_threadId;
        }

        const std::chrono::time_point<std::chrono::system_clock> &getMTime() const {
            return m_time;
        }

        const std::string &getMLoggerName() const {
            return m_loggerName;
        }

        const char *getMFile() const {
            return m_file;
        }

        int32_t getMLine() const {
            return m_line;
        }

        uint32_t getMFiberId() const {
            return m_fiberId;
        }

        const std::string &getLoggerName() const {
            return m_loggerName;
        }

        std::stringstream &getMSs() {
            return m_ss;
        }

        std::string getContent() const { return m_ss.str(); }

    private:
        LogLevel m_level;
        const char *m_file;
        int32_t m_line;
        long m_threadId;
        uint32_t m_fiberId;
        std::chrono::time_point<std::chrono::system_clock> m_time;
        std::string m_loggerName;
        std::stringstream m_ss;
    };

    class LogFormatter {
    public:
        using ptr = std::shared_ptr<LogFormatter>;

        explicit LogFormatter(std::string mPattern) : m_pattern(std::move(mPattern)) { init(); }

        void init();

        std::string format(LogLevel level, const LogEvent::ptr &event);

        std::ostream &format(std::ostream &ofs, LogLevel level, const LogEvent::ptr &event);

        const std::string &getMPattern() const;

    public:
        class FormatItem {
        public:
            using ptr = std::shared_ptr<FormatItem>;

            virtual ~FormatItem() = default;

            virtual void format(std::ostream &os, LogLevel level, LogEvent::ptr event) = 0;
        };

    private:
        /// 日志格式
        std::string m_pattern;
        /// 日志格式解析后格式
        std::vector<FormatItem::ptr> m_items;
    };

    class LogAppender {
    public:
        using ptr = std::shared_ptr<LogAppender>;

        virtual ~LogAppender() = default;

        virtual void log(LogLevel level, LogEvent::ptr event) = 0;

        void setMLevel(LogLevel mLevel) {
            m_level = mLevel;
        }

        void setMFormatter(LogFormatter::ptr mFormatter) {
            m_formatter = std::move(mFormatter);
        }

        virtual std::string toYamlString() = 0;

    protected:
        /// 日志级别
        LogLevel m_level = LogLevel::DEBUG;
        /// 日志格式器
        LogFormatter::ptr m_formatter;
    };

    class StdoutLogAppender : public LogAppender {
    public:
        using ptr = std::shared_ptr<StdoutLogAppender>;

        void log(LogLevel level, LogEvent::ptr event) override;

        std::string toYamlString() override;
    };

    class FileLogAppender : public LogAppender {
    public:
        using ptr = std::shared_ptr<FileLogAppender>;

        explicit FileLogAppender(std::string mFilename) : m_filename(std::move(mFilename)) {
            m_filestream.open(m_filename, std::ios::app | std::ios::out);
        }

        ~FileLogAppender() override {
            m_filestream.close();
        }

        void log(LogLevel level, LogEvent::ptr event) override;

        std::string toYamlString() override;

    private:
        /// 文件路径
        std::string m_filename;
        /// 文件流
        std::ofstream m_filestream;
    };

    class Logger {
    public:
        using ptr = std::shared_ptr<Logger>;

        explicit Logger(std::string name = "default") : m_name(std::move(name)) {

            /**
             * %Y：年份（四位数）。
             * %m：月份。
             * %d：日期。
             * %H：小时（24小时制）。
             * %M：分钟。
             * %S：秒。
             * %e：毫秒。
             * %l：日志级别。
             * %@：源代码文件名和行号。
             * %v：实际的日志消息。
             * %%：百分号
             * 以后还能拓展，可以支持更多的格式……
             */
            ///输出示例：[2023-07-12 07:35:22.123][info](main.cpp:8): This is an info message.
            ///默认日志名称是default，默认日志输出地是stdout，默认输出格式是[%Y-%m-%d %H:%M:%S.%e] [%l] [%@]: %v，日志器的默认级别是DEBUG
            m_level = LogLevel::DEBUG;
            m_formatter.reset(new LogFormatter("[%Y-%m-%d %H:%M:%S.%e] [%l] [%@]: %v"));
            m_appenders.push_back(std::make_shared<StdoutLogAppender>());
            for (auto &i: m_appenders) {
                i->setMFormatter(m_formatter);
            }
        }


        void log(LogLevel level, const LogEvent::ptr &event);

        void debug(const LogEvent::ptr &event);

        void info(const LogEvent::ptr &event);

        void warn(const LogEvent::ptr &event);

        void error(const LogEvent::ptr &event);

        void fatal(const LogEvent::ptr &event);

        void addAppender(const LogAppender::ptr &appender);

        void delAppender(const LogAppender::ptr &appender);

        void clearAppenders();

        const std::string &getMName() const {
            return m_name;
        }

        LogLevel getMLevel() const {
            return m_level;
        }

        void setMLevel(LogLevel mLevel) {
            m_level = mLevel;
        }

        void setFormatter(const std::string &val);

        std::string toYamlString();

    private:
        /// 日志名称
        std::string m_name;
        /// 日志级别
        LogLevel m_level;
        /// 日志目标集合
        std::vector<LogAppender::ptr> m_appenders;
        /// 日志格式器
        LogFormatter::ptr m_formatter;
    };

    class LoggerManager {
    public:
        LoggerManager();

        Logger::ptr getLogger(const std::string &name);

        Logger::ptr getDefault() const { return m_default; }

        std::string toYamlString();
    private:
        /// 日志器容器
        std::map<std::string, Logger::ptr> m_loggers;
        /// 主日志器
        Logger::ptr m_default;
    };

    typedef Singleton<LoggerManager> LoggerMgr;

    struct LogAppenderDefine {
        int type = 0; //1 File, 2 Stdout
        LogLevel level = (LogLevel) -1;
        std::string file;

        bool operator==(const LogAppenderDefine &oth) const {
            return type == oth.type
                   && level == oth.level
                   && file == oth.file;
        }
    };

    struct LogDefine {
        std::string name;
        LogLevel level = (LogLevel) -1;
        std::string formatter;
        std::vector<LogAppenderDefine> appenders;

        bool operator==(const LogDefine &oth) const {
            return name == oth.name;
        }

        bool operator<(const LogDefine &oth) const {
            return name < oth.name;
        }
    };
}

#endif //CPPPROJ_LOG_H
