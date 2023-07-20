//
// Created by fox on 2023/7/10.
// 单例模式封装

#ifndef CPPPROJ_SINGLETON_H
#define CPPPROJ_SINGLETON_H

#include <memory>

namespace foxzt {
    template<class T, class X = void, int N = 0>
    class Singleton {
    public:
        /**
         * @brief 返回单例裸指针
         */
        static T *GetInstance() {
            static T v;
            return &v;
        }
    };

    template<class T, class X = void, int N = 0>
    class SingletonPtr {
    public:
        /**
         * @brief 返回单例智能指针
         */
        static std::shared_ptr<T> GetInstance() {
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };

} // foxzt

#endif //CPPPROJ_SINGLETON_H
