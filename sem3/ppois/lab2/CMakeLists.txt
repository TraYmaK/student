cmake_minimum_required(VERSION 3.14)

project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp gallery.cpp gallery.hpp)

target_include_directories(lab2 PUBLIC ${PROJECT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.12.1.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/galleryTests.cpp gallery.cpp main.cpp)

target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR})

target_compile_definitions(tests PRIVATE TESTING)

target_compile_options(tests PRIVATE -g)

target_link_libraries(tests gtest_main)

include(GoogleTest)

gtest_add_tests(TARGET tests
    TEST_PREFIX ""
    TEST_SUFFIX ""
    EXTRA_ARGS ""
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)