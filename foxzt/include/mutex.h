//
// Created by fox on 2023/7/21.
//

#ifndef CPPPROJ_MUTEX_H
#define CPPPROJ_MUTEX_H

#include <atomic>
#include <mutex>
#include "noncopyable.h"

namespace foxzt {

    // 自旋锁
    class SpinLock : Noncopyable {
    public:
        void lock() {
            while (flag.test_and_set(std::memory_order_acquire)) {
                // 自旋等待直到获取到自旋锁
            }
        }

        void unlock() {
            flag.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
}

#endif //CPPPROJ_MUTEX_H
