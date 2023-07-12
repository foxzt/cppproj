#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

int main() {
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto logger = spdlog::rotating_logger_mt("some_logger_name", "logs/rotating.txt", max_size, max_files);
    auto console_logger = spdlog::stdout_color_mt("console_logger");
    spdlog::set_default_logger(console_logger);
    spdlog::get("console_logger")->sinks().push_back(logger->sinks()[0]);
    spdlog::get("console_logger")->sinks().push_back(console_logger->sinks()[0]);
    auto l = spdlog::logger("name");
    l.sinks().push_back(logger->sinks()[0]);

    l.error("aaaa");

    console_logger->set_pattern("[%Y-%m-%d %M:%S.%e] [%^%l%$] [%K] [%s:%#] %v%]");
    console_logger->set_level(spdlog::level::err);
    console_logger->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>("_function_name_")},
                        spdlog::level::err, "a{}", "hi");

    spdlog::default_logger_raw();
    SPDLOG_INFO("a");

    return 0;
}
