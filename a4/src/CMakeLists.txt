set( A4_SOURCES
A4_DBCs.cpp
A4_NBCs.cpp
A4_Control.cpp
A4_Disc.cpp
A4_ElasticStiffness.cpp
A4_LinAlg.cpp
A4_LinSolve.cpp
A4_FESolver.cpp
A4_PostProc.cpp
A4_Util.cpp
a4.cc
)

add_executable(a4 ${A4_SOURCES})
target_include_directories(a4 PRIVATE
  ${Trilinos_INCLUDE_DIRS}
  ${Trilinos_TPL_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(a4 PRIVATE
  ${Trilinos_LIBRARIES}
  ${Trilinos_TPL_LIBRARIES}
  ${Trilinos_EXTRA_LD_FLAGS}
  SCOREC::core)

bob_export_target(a4)

bob_end_subdir()
