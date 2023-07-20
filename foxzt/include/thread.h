//
// Created by fox on 2023/7/20.
//

#ifndef CPPPROJ_THREAD_H
#define CPPPROJ_THREAD_H

#include <functional>
#include <utility>
#include <thread>

#include "util.h"
#include "noncopyable.h"
#include "semaphore.h"

namespace foxzt {
    class Thread : Noncopyable {
    public:
        typedef std::shared_ptr<Thread> ptr;

        Thread(const std::string &name, std::function<void()> cb);

        ~Thread();

        long getMId() const;

        const std::string &getMName() const;

        static Thread *GetThis();

        static const std::string &GetName();

        void detach();

    private:
        void run();

    private:
        long m_id;
        std::thread m_thread;
        std::function<void()> m_cb;
        std::string m_name;
        Semaphore m_semaphore;
    };
}

#endif //CPPPROJ_THREAD_H
