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
        explicit Semaphore(uint32_t count = 0);

        ~Semaphore();

        void wait();

        void notify();

    private:
        sem_t m_semaphore{};
    };
}

#endif //CPPPROJ_SEMAPHORE_H
