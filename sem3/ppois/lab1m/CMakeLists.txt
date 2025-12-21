cmake_minimum_required(VERSION 3.14)

project(lab1m)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab1m main.cpp multiset.cpp multiset.hpp)

target_include_directories(lab1m PUBLIC ${PROJECT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.12.1.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/multisetTests.cpp multiset.cpp)

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(tests gtest_main)

include(GoogleTest)

gtest_discover_tests(tests)