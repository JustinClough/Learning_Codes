set( HW1_SOURCES
eig_wrap.hpp
driver.hpp
driver.cpp
errorCalcs.hpp
errorCalcs.cpp
element1D.hpp
element1D.cpp
forcing.cpp
forcing.hpp
mesh1D.hpp
mesh1D.cpp
stiffness.hpp
stiffness.cpp
fea_hw1.cpp)

add_executable(hw1 ${HW1_SOURCES})

target_include_directories(hw1 PUBLIC ${EIG_DIR})
