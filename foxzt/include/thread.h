//
// Created by fox on 2023/7/20.
//

#ifndef CPPPROJ_THREAD_H
#define CPPPROJ_THREAD_H

#include <functional>
#include <utility>
#include <thread>
#include <memory>

#include "util.h"
#include "noncopyable.h"
#include "semaphore.h"

namespace foxzt {
    class Thread : Noncopyable {
    public:
        typedef std::shared_ptr<Thread> ptr;

        template<typename... Args>
        Thread(const std::string &name, void (*cb)(Args...), Args... args)
                : m_cb(std::bind(cb, std::forward<Args>(args)...)), m_name(name), m_id(0) {
            if (name.empty()) {
                m_name = "UNKNOWN";
            }
            m_thread = std::thread(&Thread::run, this);
            m_semaphore.wait();
        }


        ~Thread();

        long getMId() const;

        const std::string &getMName() const;

        static Thread *GetThis();

        static const std::string &GetName();

        void detach();

        void join();

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
