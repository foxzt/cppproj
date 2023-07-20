//
// Created by fox on 2023/7/10.
//

#include "foxzt/include/util.h"

namespace foxzt {
    long GetThreadId() {
        return syscall(SYS_gettid);
    }

    uint32_t GetFiberId(){
        return 0;
    }
    std::chrono::time_point<std::chrono::system_clock> getNowTime(){
        return std::chrono::system_clock::now();
    }

}

