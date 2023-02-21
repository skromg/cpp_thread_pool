cmake_minimum_required(VERSION 3.21)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRC_LIST
        main.cpp
        threadpool.h)

add_executable(thread_pool ${SRC_LIST})
target_link_libraries(thread_pool -lpthread Qt5::Widgets)