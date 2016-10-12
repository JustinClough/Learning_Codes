cmake_minimum_required(VERSION 3.0.0)

project(Pet_Project1)

set(SOURCES Addition.cpp Subtraction.cpp Multiply.cpp Division.cpp)

add_library(AlgebraLibrary ${SOURCES})

add_executable(AlgebraExecutable Pet_project1.cpp)

target_link_libraries(AlgebraExecutable AlgebraLibrary)


