#include <iostream>
#include <shared_mutex>
#include <thread>

std::shared_mutex rwLock;
int sharedData = 0;

// 读线程函数
void readerThread() {
    std::shared_lock<std::shared_mutex> lock(rwLock);
    std::cout << "Read data: " << sharedData << std::endl;
}

// 写线程函数
void writerThread() {
    std::unique_lock<std::shared_mutex> lock(rwLock);
    sharedData++;
    std::cout << "Write data: " << sharedData << std::endl;
}

int main() {
    // 创建多个读线程和写线程
    std::thread t1(readerThread);
    std::thread t2(readerThread);
    std::thread t3(writerThread);
    std::thread t4(readerThread);

    // 等待线程完成
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
