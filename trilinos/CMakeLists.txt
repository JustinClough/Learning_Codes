cmake_minimum_required(VERSION 3.0.0)

set(Trilinos_PREFIX "" CACHE PATH "Trilinos Instalation Prefix")
find_package(Trilinos REQUIRED PATHS ${Trilinos_PREFIX} NO_DEFAULT_PATH) 
set(CMAKE_CXX_COMPILER ${Trilinos_CXX_COMPILER} )
set(CMAKE_CXX_FLAGS ${Trilinos_CXX_COMPILER_FLAGS} )
set(ALL_INCS ${Trilinos_INCLUDE_DIRS} ${Trilinos_TPL_INCLUDE_DIRS})

set(ALL_LIBS ${Trilinos_LIBRARIES} ${Trilinos_TPL_LIBRARIES})

list( REMOVE_DUPLICATES ALL_INCS )

list( REMOVE_DUPLICATES ALL_LIBS )

message( STATUS "ALL_INCS ${ALL_INCS}" ) 

message( STATUS "ALL_LIBS ${ALL_LIBS}" ) 

add_executable( tpetra1 tpetra1.cpp)
target_include_directories( tpetra1 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra1 ${ALL_LIBS})

add_executable( tpetra2 tpetra2.cpp)
target_include_directories( tpetra2 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra2 ${ALL_LIBS})

add_executable( tpetra3 tpetra3.cpp)
target_include_directories( tpetra3 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra3 ${ALL_LIBS})

#No code for exercise 4

add_executable( tpetra5 tpetra5.cpp)
target_include_directories( tpetra5 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra5 ${ALL_LIBS})

add_executable( tpetra6 tpetra6.cpp)
target_include_directories( tpetra6 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra6 ${ALL_LIBS})

add_executable( tpetra7 tpetra7.cpp)
target_include_directories( tpetra7 PRIVATE ${ALL_INCS} ) 
target_link_libraries( tpetra7 ${ALL_LIBS})

add_executable( belos_practice belos_practice.cpp)
target_include_directories( belos_practice PRIVATE ${ALL_INCS} ) 
target_link_libraries( belos_practice ${ALL_LIBS})
