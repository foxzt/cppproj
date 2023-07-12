#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "foxzt/foxzt.h"


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
    logger->setFormatter("[%@]");
    logger->log(foxzt::LogLevel::DEBUG, event);
}

int main() {
    test_getNowTime();
    test_log();
    return 0;
}
