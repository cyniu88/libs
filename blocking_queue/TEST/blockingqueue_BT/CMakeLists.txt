cmake_minimum_required(VERSION 2.8)

project(blockingqueue_BT)
add_executable(${PROJECT_NAME} "main.cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBT_TEST -DNOSSL")
