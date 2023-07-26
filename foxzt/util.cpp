//
// Created by fox on 2023/7/10.
//

#include "foxzt/include/util.h"

namespace foxzt {
    Logger::ptr g_logger = LOGGER_NAME_RAW("system");

    long GetThreadId() {
        return syscall(SYS_gettid);
    }

    uint32_t GetFiberId() {
        return 0;
    }

    std::chrono::time_point<std::chrono::system_clock> getNowTime() {
        return std::chrono::system_clock::now();
    }

    void BackTrace(std::vector<std::string> &bt, int size, int skip) {
        void **array = (void **) malloc(sizeof(void *) * size);
        size_t s = ::backtrace(array, size);

        char **strings = backtrace_symbols(array, s);
        if (strings == nullptr) {
            FOXZT_LOGGER_ERROR(g_logger, "backtrace_symbols error");
            //std::cout << "backtrace_symbols error" << std::endl;
            return;
        }

        for (size_t i = skip; i < s; i++) {
            bt.emplace_back(strings[i]);
        }
        free(strings);
        free(array);
    }

    std::string BackTrace(int size, int skip, const std::string &prefix) {
        std::vector<std::string> bt;
        BackTrace(bt, size, skip);
        std::stringstream ss;
        for (const auto &i: bt) {
            ss << prefix << i << std::endl;
        }
        return ss.str();
    }

}

