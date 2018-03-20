cmake_minimum_required(VERSION 3.0.0)

project(FEA)

set(CMAKE_BUILD_TYPE debug)

add_subdirectory(${EIG_DIR})

add_subdirectory(${HW1_SRC})
add_subdirectory(${HW5_SRC})
