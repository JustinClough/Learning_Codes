set( HW6_SOURCES
eig_wrap.hpp
driver.hpp
driver.cpp
mesh.hpp
mesh.cpp
fea_hw6.cpp)

add_executable(hw6 ${HW6_SOURCES})

target_include_directories(hw6 PUBLIC ${EIG_DIR})
