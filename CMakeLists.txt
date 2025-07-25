cmake_minimum_required(VERSION 3.25)
project(neuralNetwork)

set(CMAKE_CXX_STANDARD 17)

include_directories(headers)

file(GLOB_RECURSE SRCS "src/*.c")

add_executable(ml_exec ${SRCS})

target_link_libraries(ml_exec m)

target_compile_options(ml_exec PUBLIC -Wall -Werror)

set(CMAKE_BUILD_TYPE Debug)
