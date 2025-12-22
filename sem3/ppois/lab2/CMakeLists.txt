cmake_minimum_required(VERSION 3.14)

project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp gallery.cpp gallery.hpp)

target_include_directories(lab2 PUBLIC ${PROJECT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.12.1.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/galleryTests.cpp gallery.cpp)

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(tests gtest_main)

# Coverage support
option(ENABLE_COVERAGE "Enable code coverage reporting" ON)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Coverage enabled")
        target_compile_options(tests PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(tests PRIVATE --coverage)
    endif()
endif()

include(GoogleTest)

gtest_discover_tests(tests)