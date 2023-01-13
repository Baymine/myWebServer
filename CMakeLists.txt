cmake_minimum_required(VERSION 3.22)
project(clion_web_server_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(clion_web_server_project main.cpp  src/ThreadPool.h src/ThreadPool.cpp ThreadPoolTest.cpp)
