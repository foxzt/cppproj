//
// Created by fox on 2023/7/20.
//

#include <utility>

#include "foxzt/include/thread.h"

namespace foxzt {

    static thread_local Thread *t_thread = nullptr;
    static thread_local std::string t_thread_name = "UNKNOW";

    Thread::Thread(const std::string &name, std::function<void()> cb) : m_cb(std::move(cb)), m_name(name), m_id(0) {
        if (name.empty()) {
            m_name = "UNKNOWN";
        }
        m_thread = std::thread(&Thread::run, this);
        m_semaphore.wait();
    }

    Thread::~Thread() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    long Thread::getMId() const {
        return m_id;
    }

    const std::string &Thread::getMName() const {
        return m_name;
    }

    void Thread::run() {
        t_thread = this;
        t_thread_name = m_name;
        m_id = foxzt::GetThreadId();

        std::function<void()> cb;
        cb.swap(m_cb);

        m_semaphore.notify();

        cb();
    }

    Thread *Thread::GetThis() {
        return t_thread;
    }

    const std::string &Thread::GetName() {
        return t_thread_name;
    }

    void Thread::detach() {
        if (m_thread.joinable()) {
            m_thread.detach();
        }
    }
}