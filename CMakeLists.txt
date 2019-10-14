cmake_minimum_required(VERSION 3.5)

project(baker-samefringe LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Catch2 REQUIRED)
add_executable(tests tests.cpp samefringe.hpp)
target_link_libraries(tests Catch2::Catch2)

set(WARNING_FLAGS "-Wall -Wextra")
set(SHARED_FLAGS "${WARNING_FLAGS} ${SECURITY_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS}")

include(CTest)
include(Catch)
catch_discover_tests(tests)
