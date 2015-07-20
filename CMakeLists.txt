cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(baker-samefringe CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

enable_testing()

set(WARNING_FLAGS "-Wall -Wextra")
set(SHARED_FLAGS "${WARNING_FLAGS} ${SECURITY_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS}")

add_subdirectory(test)
