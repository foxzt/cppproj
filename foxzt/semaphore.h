//
// Created by fox on 2023/7/20.
//

#ifndef CPPPROJ_SEMAPHORE_H
#define CPPPROJ_SEMAPHORE_H

#include <cstdint>
#include <semaphore.h>
#include "noncopyable.h"

namespace foxzt {

    class Semaphore : Noncopyable {
    public:
        /**
         * @brief 构造函数
         * @param[in] count 信号量值的大小
         */
        explicit Semaphore(uint32_t count = 0);

        /**
         * @brief 析构函数
         */
        ~Semaphore();

        /**
         * @brief 获取信号量
         */
        void wait();

        /**
         * @brief 释放信号量
         */
        void notify();

    private:
        sem_t m_semaphore{};
    };
}

#endif //CPPPROJ_SEMAPHORE_H
