set( HW1_SOURCES
eig_wrap.hpp
fea_hw1.cpp)

add_executable(hw1 ${HW1_SOURCES})

target_include_directories(hw1 PUBLIC ${EIG_DIR})
