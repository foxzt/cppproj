//
// Created by fox on 2023/7/26.
//

#ifndef CPPPROJ_FIBER_H
#define CPPPROJ_FIBER_H

#include <ucontext.h>
#include <memory>
#include <mutex>
#include <functional>
#include "thread.h"
#include "macro.h"
#include "log.h"

namespace foxzt {

    class Fiber : public std::enable_shared_from_this<Fiber> {
    public:
        using ptr = std::shared_ptr<Fiber>;
        enum State {
            INIT,      //刚创建
            READY,     //就绪
            SUSPENDED, //挂起
            RUNNING,   //运行
            DEAD,      //结束
            WAITING,    //等待
            EXCEP
        };
    private:
        Fiber();

    public:
        explicit Fiber(const std::function<void()> &cb, size_t stacksize = 0);

        ~Fiber();

        void reset(const std::function<void()> &cb);

        void swapIn();

        void swapOut();

    public:
        static void setThis(Fiber *fiber);

        static Fiber::ptr GetThis();

        static void YieldSuspended();

        static void YieldReady();

        static uint64_t FiberCounts();

        static void MainFunc();

    private:
        uint64_t m_id = 0;
        uint32_t m_stacksize = 0;
        State m_state = INIT;

        ucontext_t m_ctx{};
        void *m_stack = nullptr;

        std::function<void()> m_cb;
    };

}

#endif //CPPPROJ_FIBER_H
