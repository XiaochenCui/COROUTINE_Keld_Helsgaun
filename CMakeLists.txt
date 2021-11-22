cmake_minimum_required(VERSION 3.10)

# set the project name
project(COROUTINE)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set CPPFLAGS
# -g for debugging
set(CMAKE_CXX_FLAGS "-g")

set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(COROUTINE_LIB_DIR ${CMAKE_SOURCE_DIR}/SRC/copy-stack)
add_library(coroutine STATIC ${COROUTINE_LIB_DIR}/coroutine.h ${COROUTINE_LIB_DIR}/coroutine.cpp)

set(TARGET_LIST TestProgram MinimalExample TextTransformation TelegamFilter)
foreach (TARGET ${TARGET_LIST})
    add_executable(${TARGET} EXAMPLES/${TARGET}.cpp)
    target_include_directories(${TARGET} PRIVATE ${COROUTINE_LIB_DIR})
    target_link_libraries(${TARGET} coroutine)
endforeach()

set(SIMSET_LIB_DIR ${CMAKE_SOURCE_DIR}/SRC/simset)
add_library(simset STATIC ${SIMSET_LIB_DIR}/simset.h ${SIMSET_LIB_DIR}/simset.cpp)

set(RANDOM_LIB_DIR ${CMAKE_SOURCE_DIR}/SRC/random)
add_library(random STATIC ${RANDOM_LIB_DIR}/random.h ${RANDOM_LIB_DIR}/random.cpp)

set(SIMULATION_LIB_DIR ${CMAKE_SOURCE_DIR}/SRC/simulation)
add_library(simulation STATIC ${SIMULATION_LIB_DIR}/simulation.h ${SIMULATION_LIB_DIR}/simulation.cpp)
target_include_directories(simulation PRIVATE ${COROUTINE_LIB_DIR})
target_include_directories(simulation PRIVATE ${SIMSET_LIB_DIR})
target_include_directories(simulation PRIVATE ${RANDOM_LIB_DIR})

add_executable(CarWash EXAMPLES/CarWash.cpp)
target_include_directories(CarWash PRIVATE ${SIMULATION_LIB_DIR})
target_include_directories(CarWash PRIVATE ${COROUTINE_LIB_DIR})
target_include_directories(CarWash PRIVATE ${SIMSET_LIB_DIR})
target_include_directories(CarWash PRIVATE ${RANDOM_LIB_DIR})
target_link_libraries(CarWash simulation simset coroutine random)