set( HW1_SOURCES
eig_wrap.hpp
fea_hw1.cpp)

add_executable(fea_hw1 ${HW1_SOURCES})

target_include_directories(fea_hw1 PRIVATE
  ${EIG_SRC}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
