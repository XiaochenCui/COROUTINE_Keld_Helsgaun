cmake_minimum_required(VERSION 3.10)

# set the project name
project(coroutine)

# set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SRC/share-stack)
set(LIB_FILES ${LIB_DIR}/coroutine.cpp ${LIB_DIR}/coroutine.h)

set(TARGET_LIST TestProgram MinimalExample TextTransformation TelegamFilter memcpy)

foreach (TARGET ${TARGET_LIST})
    # add source files
    add_executable(${TARGET} EXAMPLES/${TARGET}.cpp ${LIB_FILES})

    # Add include directories to a target.
    target_include_directories(${TARGET} PRIVATE ${LIB_DIR})
endforeach()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set CPPFLAGS
# -g for debugging
set(CMAKE_CXX_FLAGS "-g")