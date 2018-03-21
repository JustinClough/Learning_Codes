set( HW5_SOURCES
coord.cpp
coord.hpp
driver.cpp
driver.hpp
element.cpp
element.hpp
vertex.cpp
vertex.hpp
eig_wrap.hpp
fea_hw5.cpp)

add_executable(hw5 ${HW5_SOURCES})

target_include_directories(hw5 PUBLIC ${EIG_DIR})
