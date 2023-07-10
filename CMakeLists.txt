#本项目使用的cmake版本不低于3.15
cmake_minimum_required(VERSION 3.15)

#设置项目名称
project(cppproj)

#设置 CMake 构建过程中生成详细输出
set(CMAKE_VERBOSE_MAKEFILE ON)

#添加项目搜索头文件的位置
include_directories(.)

#设置可执行文件与库文件输出的位置
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#添加测试环境代码可执行文件
add_executable(test_evn tests/test_environment/test_evn.cpp)