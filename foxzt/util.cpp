//
// Created by fox on 2023/7/10.
//

#include "util.h"

namespace foxzt {
    pid_t GetThreadId() {
        return syscall(SYS_gettid);
    }

    uint32_t GetFiberId(){
        return 0;
    }

    uint64_t GetCurrentMS(){
        return 0;
    }
    uint64_t GetCurrentUS(){
        return 0;
    }
}

