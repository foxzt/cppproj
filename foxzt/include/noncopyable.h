//
// Created by fox on 2023/7/20.
//

#ifndef CPPPROJ_NONCOPYABLE_H
#define CPPPROJ_NONCOPYABLE_H

namespace foxzt {

    class Noncopyable {
    public:
        Noncopyable() = default;

        ~Noncopyable() = default;

        Noncopyable(const Noncopyable &) = delete;

        Noncopyable &operator=(const Noncopyable &) = delete;
    };

} // foxzt

#endif //CPPPROJ_NONCOPYABLE_H
