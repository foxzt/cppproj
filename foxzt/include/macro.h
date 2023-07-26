//
// Created by fox on 2023/7/26.
//

#ifndef CPPPROJ_MACRO_H
#define CPPPROJ_MACRO_H

#include <cassert>
#include <cstring>

#include "util.h"

#define FOXZT_ASSERT(x) \
    if(!(x)) { \
        FOXZT_ERROR("ASSERTION: {}\n{}\n{}", \
                                    #x,        \
                                    "backtrace", \
                                    foxzt::BackTrace(100, 2, "    ")); \
        assert(x); \
    }

#define FOXZT_ASSERT2(x, w) \
    if(!(x)) { \
        FOXZT_ERROR("ASSERTION: {}\n{}\n{}\n{}", \
                                    #x,            \
                                    w, \
                                    "backtrace", \
                                    foxzt::BackTrace(100, 2, "    ")); \
        assert(x); \
    }

#endif //CPPPROJ_MACRO_H
