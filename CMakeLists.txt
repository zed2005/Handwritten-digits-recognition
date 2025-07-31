# A simple cMake file for compiling the code
# Only the non-ordinary things are commented

cmake_minimum_required(VERSION 3.25)
project(neuralNetwork)

set(CMAKE_CXX_STANDARD 17)

# all .h files are in the headers directory
include_directories(headers)

# all .c files are in the src directory
file(GLOB_RECURSE SRCS "src/*.c")

add_executable(ml_exec ${SRCS})

target_link_libraries(ml_exec m)

target_compile_options(ml_exec PUBLIC -Wall -Werror)

# set so can easily debug with vscode (oprional) 
set(CMAKE_BUILD_TYPE Debug)
