//
// Created by fox on 2023/7/11.
//
#include<foxzt/log.h>

int main() {
    // 创建一个LogEvent对象
    foxzt::LogEvent::ptr event(
            new foxzt::LogEvent(foxzt::LogLevel::INFO, "example.cpp", 42, 100, 1234, 5678, 1626014699, "Main"));

    // 创建一个LogEventWrap对象，并传入LogEvent对象
    foxzt::LogEventWrap eventWrap(event);

    // 使用LogEventWrap对象获取日志内容流，并写入日志消息
    std::stringstream &ss = eventWrap.getSS();
    ss << "This is a log message.";

    // 获取LogEvent对象
    foxzt::LogEvent::ptr wrappedEvent = eventWrap.getEvent();

    // 输出日志信息
    std::cout << "File: " << wrappedEvent->getFile() << std::endl;
    std::cout << "Line: " << wrappedEvent->getLine() << std::endl;
    std::cout << "Elapse: " << wrappedEvent->getElapse() << std::endl;
    std::cout << "Thread ID: " << wrappedEvent->getThreadId() << std::endl;
    std::cout << "Fiber ID: " << wrappedEvent->getFiberId() << std::endl;
    std::cout << "Time: " << wrappedEvent->getTime() << std::endl;
    std::cout << "Thread Name: " << wrappedEvent->getThreadName() << std::endl;
    std::cout << "Content: " << wrappedEvent->getContent() << std::endl;

    return 0;
}



