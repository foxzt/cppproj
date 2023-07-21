#include <iostream>
#include <thread>
#include <mutex>

class MyMutex {
public:
    static void lock() {
        // 实现获取锁的逻辑
        std::cout << "MyMutex: lock acquired" << std::endl;
    }

    static void unlock() {
        // 实现释放锁的逻辑
        std::cout << "MyMutex: lock released" << std::endl;
    }
};

void workerThread() {
    MyMutex mtx;
    std::lock_guard<MyMutex> lock(mtx);
    // 执行需要保护的操作
    std::cout << "Worker thread: Got the lock!" << std::endl;
}

int main() {
    std::thread worker(workerThread);
    worker.join();

    return 0;
}
