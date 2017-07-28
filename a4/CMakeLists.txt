cmake_minimum_required(VERSION 3.0.0)

project(A4 VERSION 0.0.1 LANGUAGES CXX C)

include(cmake/bob.cmake)

bob_begin_package()

bob_begin_cxx_flags()
bob_cxx11_flags()
bob_end_cxx_flags()

set(SCOREC_PREFIX "" CACHE PATH "Scorec/core installation")
set(Trilinos_PREFIX "" CACHE PATH "Trilinos installation")

include(cmake/depends.cmake)
add_subdirectory(src)

bob_end_package()
