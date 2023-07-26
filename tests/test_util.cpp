#include <cassert>

#include "foxzt/include/log.h"
#include "foxzt/include/config.h"
#include "foxzt/include/macro.h"

foxzt::Logger::ptr g_logger = DEFAULT_LOGGER_RAW();

void test_assert() {
    FOXZT_ASSERT2(1 == 1, "asd asd");
}

void fun() {
    test_assert();
}

int main() {
    test_assert();

    return 0;
}