//
// Created by fox on 2023/7/26.
//

#include "foxzt/include/fiber.h"
#include <cstdlib>
#include <atomic>

#include "foxzt/include/config.h"


namespace foxzt {

    Logger::ptr g_logger = DEFAULT_LOGGER_RAW();

    static std::atomic<uint64_t> s_fiber_id = 0;
    static std::atomic<uint64_t> s_fiber_count = 0;

    static thread_local Fiber *t_fiber = nullptr;
    static thread_local Fiber::ptr t_threadFiber = nullptr;

    static ConfigVar<uint32_t>::ptr g_fiber_stack_size =
            Config::Lookup<uint32_t>("fiber.stack_size", 1024 * 1024,
                                     "fiber stack size");

    class MallocStackAllocator {
    public:
        static void *Alloc(size_t size) {
            return malloc(size);
        }

        static void Dealloc(void *vp, size_t size) {
            free(vp);
        }
    };

    using StackAlloactor = MallocStackAllocator;

    Fiber::Fiber() {
        m_state = RUNNING;
        setThis(this);
        if (getcontext(&m_ctx)) {
            FOXZT_ASSERT2(false, "getcontext");
        }
        s_fiber_count++;
    }

    Fiber::Fiber(const std::function<void()> &cb, size_t stacksize)
            : m_id(++s_fiber_count),
              m_cb(cb) {
        ++s_fiber_count;
        m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getMVal();
        m_stack = StackAlloactor::Alloc(m_stacksize);
        if (getcontext(&m_ctx)) {
            FOXZT_ASSERT2(false, "getcontext");
        }
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_sp = m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;

        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    }

    void Fiber::setThis(Fiber *fiber) {
        t_fiber = fiber;
    }

    Fiber::ptr Fiber::GetThis() {
        if (t_fiber) {
            return t_fiber->shared_from_this();
        }
        Fiber::ptr main_fiber(new Fiber);
        FOXZT_ASSERT(t_fiber == main_fiber.get());
        return t_fiber->shared_from_this();
    }

    void Fiber::YieldSuspended() {
        Fiber::ptr cur = GetThis();
        cur->m_state = SUSPENDED;
        cur->swapOut();
    }

    void Fiber::YieldReady() {
        Fiber::ptr cur = GetThis();
        cur->m_state = READY;
        cur->swapOut();
    }

    uint64_t Fiber::FiberCounts() {
        return s_fiber_count;
    }

    void Fiber::MainFunc() {
        Fiber::ptr cur = GetThis();
        FOXZT_ASSERT(cur);
        try {
            cur->m_cb();
            cur->m_cb = nullptr;
            cur->m_state = DEAD;
        } catch (std::exception &exception) {
            cur->m_state = EXCEP;
            FOXZT_LOGGER_ERROR(g_logger, "Fiber Except: {}", exception.what());
        } catch (...) {
            cur->m_state = EXCEP;
            FOXZT_LOGGER_ERROR(g_logger, "Fiber Except");
        }
    }

    Fiber::~Fiber() {
        --s_fiber_count;
        if (m_stack) {
            FOXZT_ASSERT(m_state == DEAD || m_state == INIT || m_state == EXCEP);

            StackAlloactor::Dealloc(m_stack, m_stacksize);
        } else {
            FOXZT_ASSERT(!m_cb);
            FOXZT_ASSERT(m_state == RUNNING);
            Fiber *cur = t_fiber;
            if (cur == this) {
                setThis(nullptr);
            }
        }
    }

    void Fiber::reset(const std::function<void()> &cb) {
        FOXZT_ASSERT(m_stack);
        FOXZT_ASSERT(m_state == INIT || m_state == DEAD);
        m_cb = cb;
        if (getcontext(&m_ctx)) {
            FOXZT_ASSERT2(false, "getcontext");
        }
        m_ctx.uc_link = nullptr;
        m_ctx.uc_stack.ss_sp = m_stack;
        m_ctx.uc_stack.ss_size = m_stacksize;

        makecontext(&m_ctx, &Fiber::MainFunc, 0);
        m_state = INIT;
    }

    void Fiber::swapIn() {
        setThis(this);
        FOXZT_ASSERT(m_state != RUNNING);
        m_state = RUNNING;
        if (swapcontext(&t_threadFiber->m_ctx, &m_ctx)) {
            FOXZT_ASSERT2(false, "swapcontext");
        }
    }

    void Fiber::swapOut() {
        setThis(t_threadFiber.get());
        if (swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
            FOXZT_ASSERT2(false, "swapcontext");
        }
    }
}