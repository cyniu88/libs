cmake_minimum_required(VERSION 3.5)

project(BT-statistic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILES  "main.cpp"
    "../../statistic_test/statistic_BT.cpp")
add_executable( ${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  pthread gtest  )

