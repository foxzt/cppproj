#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "foxzt/util.h"
#include "foxzt/log.h"

class LogFormatter {
public:
    LogFormatter(const std::string &pattern) : pattern(pattern) {}

    std::string format(const std::string &level, const std::string &message) const {
        std::ostringstream oss;
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        for (std::size_t i = 0; i < pattern.length(); ++i) {
            if (pattern[i] == '%') {
                if (i + 1 < pattern.length()) {
                    switch (pattern[i + 1]) {
                        case 'Y': // Year (e.g., 2023)
                            oss << std::put_time(std::localtime(&now_c), "%Y");
                            break;
                        case 'm': // Month (01-12)
                            oss << std::put_time(std::localtime(&now_c), "%m");
                            break;
                        case 'd': // Day of the month (01-31)
                            oss << std::put_time(std::localtime(&now_c), "%d");
                            break;
                        case 'H': // Hour (00-23)
                            oss << std::put_time(std::localtime(&now_c), "%H");
                            break;
                        case 'M': // Minute (00-59)
                            oss << std::put_time(std::localtime(&now_c), "%M");
                            break;
                        case 'S': // Second (00-59)
                            oss << std::put_time(std::localtime(&now_c), "%S");
                            break;
                        case 'e': // Milliseconds (000-999)
                            oss << std::setfill('0') << std::setw(3) << ms.count();
                            break;
                        case 'l': // Log level placeholder
                            oss << level;
                            break;
                        case '@': // Placeholder for additional information
                            oss << __FILE__ << ":" << __LINE__;
                            break;
                        case 'v': // Log message placeholder
                            oss << message;
                            break;
                        case '%': // Log message placeholder
                            oss << '%';
                            break;
                        default:
                            oss << pattern[i]; // Treat unknown format specifier as a regular character
                            break;
                    }
                    ++i; // Skip the next character since it has been processed
                } else {
                    oss << pattern[i]; // Treat '%' at the end of the pattern as a regular character
                }
            } else {
                oss << pattern[i]; // Regular character
            }
        }

        return oss.str();
    }

private:
    std::string pattern;
};

void getTime() {
    std::ostringstream oss;

    // 获取当前时间点
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // 将时间点转换为 time_t 类型
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // 计算时间点相对于纪元时间的毫秒数
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // 格式化输出日期和时间
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    oss << ms.count();

    std::cout << oss.str() << std::endl;
}


void test_getNowTime() {
    std::ostringstream oss;

    // 获取当前时间点
    std::chrono::system_clock::time_point now = foxzt::getNowTime();

    // 将时间点转换为 time_t 类型
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // 计算时间点相对于纪元时间的毫秒数
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // 格式化输出日期和时间
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    //oss << ms.count();

    std::cout << oss.str() << std::endl;
}

void test_log() {
    foxzt::Logger::ptr logger = std::make_shared<foxzt::Logger>();
    auto event = std::make_shared<foxzt::LogEvent>(
            foxzt::LogLevel::DEBUG, __FILE__, __LINE__, foxzt::GetThreadId(),
            foxzt::GetFiberId(), foxzt::getNowTime(), logger->getMName());
    event->getMSs() << "test";
    logger->log(foxzt::LogLevel::DEBUG, event);

}

int main() {
    test_log();
    return 0;
}
