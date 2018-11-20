cmake_minimum_required(VERSION 3.0)
project(jc)

include_directories(.)
include_directories(Common)
include_directories(Frontend)
include_directories(bytecode)
include_directories(lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE Src
    "*.cpp"
    "*.h"
    "*.hpp")

message(${Src})

find_library(READLINE_LIB readline) 

if (NOT READLINE_LIB)
    find_library(READLINE_LIB edit)
endif()

add_executable(main ${Src})
target_link_libraries(main ${READLINE_LIB})

add_custom_target(runit)
add_custom_command(TARGET runit
    POST_BUILD
    COMMAND $ { CMAKE_BINARY_DIR }
    / exp / main)
