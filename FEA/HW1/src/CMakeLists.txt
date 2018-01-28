set( HW1_SOURCES
eig_wrap.hpp
element1D.hpp
element1D.cpp
mesh1D.hpp
mesh1D.cpp
fea_hw1.cpp)

add_executable(hw1 ${HW1_SOURCES})

target_include_directories(hw1 PUBLIC ${EIG_DIR})
