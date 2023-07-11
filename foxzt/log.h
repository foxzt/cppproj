//
// Created by fox on 2023/7/10.
//

#ifndef CPPPROJ_LOG_H
#define CPPPROJ_LOG_H

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
#include "singleton.h"
#include "util.h"

namespace foxzt {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent(LogLevel level, const char *file, int32_t line, uint32_t elapse,
                 uint32_t thread_id, uint32_t fiber_id, uint64_t time,
                 std::string thread_name)
                : m_level(level),
                  m_file(file),
                  m_line(line),
                  m_elapse(elapse),
                  m_threadId(thread_id),
                  m_fiberId(fiber_id),
                  m_time(time),
                  m_threadName(std::move(thread_name)) {}

        const char *getFile() const { return m_file; }

        int32_t getLine() const { return m_line; }

        uint32_t getElapse() const { return m_elapse; }

        uint32_t getThreadId() const { return m_threadId; }

        uint32_t getFiberId() const { return m_fiberId; }

        uint64_t getTime() const { return m_time; }

        const std::string &getThreadName() const { return m_threadName; }

        std::string getContent() const { return m_ss.str(); }

        LogLevel getLevel() const { return m_level; }

        std::stringstream &getSS() { return m_ss; }

        void format(const char *fmt, ...) {
            va_list args;
            va_start(args, fmt);
            format(fmt, args);
            va_end(args);
        }

        void format(const char *fmt, va_list args) {
            char buffer[1024];
            int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
            if (len != -1 && len < sizeof(buffer)) {
                m_ss << buffer;
            }
        }

    private:
        LogLevel m_level;
        const char *m_file;
        int32_t m_line;
        uint32_t m_elapse;
        uint32_t m_threadId;
        uint32_t m_fiberId;
        uint64_t m_time;
        std::string m_threadName;
        std::stringstream m_ss;
    };

    class Logger {
        // Logger 类的定义
    };

    class LogEventWrap {
    public:
        explicit LogEventWrap(LogEvent::ptr e) : m_event(std::move(e)) {}

        ~LogEventWrap() = default;

        LogEvent::ptr getEvent() const { return m_event; }

        std::stringstream& getSS() { return m_event->getSS(); }

    private:
        LogEvent::ptr m_event;
    };

    class LogFormatter{};

    class LogAppender{};

    class StdoutLogAppender : public LogAppender{};

    class FileLogAppender : public LogAppender{};

    class LoggerManager{};

    typedef foxzt::Singleton<LoggerManager> LoggerMgr;
}

#endif //CPPPROJ_LOG_H
