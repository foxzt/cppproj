#本项目使用的cmake版本不低于3.15
cmake_minimum_required(VERSION 3.15)

#设置项目名称
project(cppproj)

#设置 CMake 构建过程中生成详细输出
#set(CMAKE_VERBOSE_MAKEFILE ON)

#添加项目搜索头文件的位置
include_directories(.)

#设置可执行文件与库文件输出的位置
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置编译库文件所需要的文件
set(LIB_SRC
        foxzt/log.cpp
        foxzt/util.cpp foxzt/config.cpp)

#设置需要链接的库
set(LIB
        jsoncpp
        foxzt
        spdlog
        )

#编译成库
add_library(foxzt STATIC ${LIB_SRC})
target_link_libraries(foxzt yaml-cpp fmt)

link_libraries(${LIB})

#添加测试环境代码可执行文件
add_executable(test_evn tests/test_evn.cpp)

add_executable(test_boost tests/test_boost.cpp)

add_executable(test_yamlcpp tests/test_yamlcpp.cpp)

add_executable(test_json tests/test_json.cpp)

add_executable(mytest tests/mytest.cpp)

add_executable(test_spdlog tests/test_spdlog.cpp)

add_executable(test_fmt tests/test_fmt.cpp)

add_executable(test_config tests/test_config.cpp)

set(CMAKE_INSTALL_PREFIX "/thirdlib" CACHE PATH "Installation prefix")
install(TARGETS foxzt LIBRARY DESTINATION /thirdlib/lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/foxzt
        DESTINATION /thirdlib/include
        FILES_MATCHING PATTERN "*.h")
