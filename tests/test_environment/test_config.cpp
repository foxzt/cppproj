//
// Created by fox on 2023/7/13.
//
#include "foxzt/foxzt.h"

foxzt::ConfigVar<int>::ptr g_int = foxzt::Config::Lookup<int>("system.port", 8080, "system port");
foxzt::ConfigVar<float>::ptr g_float = foxzt::Config::Lookup<float>("system.float", 1.23, "system float");


int main() {

    FOXZT_WARN("val: {} type: {}", g_int->getMVal(), g_int->getTypeName());
    FOXZT_WARN("val: {} type: {}", g_float->getMVal(), g_float->getTypeName());
    return 0;
}