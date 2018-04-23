set( BONUS_SOURCES
eig_wrap.hpp
element.cpp
element.hpp
mesh.cpp
mesh.hpp
solution.cpp
solution.hpp
driver.cpp
driver.hpp
fea_bonus.cpp)

add_executable(bonus ${BONUS_SOURCES})

target_include_directories(bonus PUBLIC ${EIG_DIR})
