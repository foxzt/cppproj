#本项目使用的cmake版本不低于3.15
cmake_minimum_required(VERSION 3.15)

#设置项目名称
project(cppproj)

#设置 CMake 构建过程中生成详细输出
set(CMAKE_VERBOSE_MAKEFILE ON)

#启动测试
enable_testing()

#添加项目搜索头文件的位置
include_directories(.)
include_directories(/root/thirdlib/include)
link_directories(/root/thirdlib/lib)

#设置可执行文件与库文件输出的位置
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置编译库文件所需要的文件
set(LIB_SRC
        foxzt/log.cpp
        foxzt/util.cpp)

#编译成库
add_library(foxzt SHARED ${LIB_SRC})

link_libraries(foxzt)

#添加测试环境代码可执行文件
add_executable(test_evn tests/test_environment/test_evn.cpp)
target_link_libraries(test_evn)
add_test(NAME "Test_Evn" COMMAND test_evn)

add_executable(test_boost tests/test_environment/test_boost.cpp)
target_link_libraries(test_boost)
add_test(NAME "Test_Boost" COMMAND test_boost)

add_executable(test_yamlcpp tests/test_environment/test_yamlcpp.cpp)
target_link_libraries(test_yamlcpp yaml-cpp)
add_test(NAME "Test_YAMLCPP" COMMAND test_boost)

add_executable(test_json tests/test_environment/test_json.cpp)
target_link_libraries(test_json jsoncpp)
add_test(NAME "Test_JSONCPP" COMMAND test_json)