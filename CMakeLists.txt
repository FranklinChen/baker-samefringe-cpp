cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(baker-samefringe CXX)

# Only works for cmake 3.1+!
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

set(WARNING_FLAGS "-Wall -Wextra")
set(SHARED_FLAGS "${WARNING_FLAGS} ${SECURITY_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS}")

enable_testing()

add_subdirectory(test)
