cmake_minimum_required(VERSION 3.28)

project(measurements VERSION 1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build tests" ON)

add_library(measurements INTERFACE)
target_sources(
    measurements
    PUBLIC
    FILE_SET HEADERS BASE_DIRS include)

add_subdirectory(include)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
