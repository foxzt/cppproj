#include <iostream>
#include <memory>

template<class T>
class Base {
public:
    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }
};

template<class T>
class Derived : public Base<T> {
public:
    void foo() override {
        std::cout << "Derived::foo()" << std::endl;
    }
};

int main() {
    std::shared_ptr<Base<int>> basePtr = std::make_shared<Derived<int>>();

    // dynamic_pointer_cast 进行类型转换
    std::shared_ptr<Derived<float>> derivedPtr = std::dynamic_pointer_cast<Derived<float>>(basePtr);

    if (derivedPtr) {
        derivedPtr->foo();
    } else {
        std::cout << "Failed to cast." << std::endl;
    }

    return 0;
}