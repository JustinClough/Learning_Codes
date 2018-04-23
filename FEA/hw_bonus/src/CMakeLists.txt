set( BONUS_SOURCES
eig_wrap.hpp
fea_bonus.cpp)

add_executable(bonus ${BONUS_SOURCES})

target_include_directories(bonus PUBLIC ${EIG_DIR})
