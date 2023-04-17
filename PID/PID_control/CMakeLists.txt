cmake_minimum_required(VERSION 3.5)

project(PID_control LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PID_control main.cpp ../pid_control.h ../pid_control.cpp)


install(TARGETS PID_control
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
