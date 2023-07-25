#include <iostream>
#include <boost/coroutine2/all.hpp>

void fun() {
    std::cout << "Hello from fun!" << std::endl;
}

int main() {
    auto coroutine_func = [](
            boost::coroutines2::coroutine<void>::push_type &yield) {
        fun();
        yield();
        std::cout << "Coroutine: Finish" << std::endl;
    };

    boost::coroutines2::coroutine<void>::pull_type coroutine(coroutine_func);

    std::cout << "Main: Start" << std::endl;
    coroutine();
    std::cout << "Main: Resume" << std::endl;
    coroutine();
    std::cout << "Main: Finish" << std::endl;

    return 0;
}
