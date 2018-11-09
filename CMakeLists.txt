cmake_minimum_required(VERSION 2.6)

# Project's name

project(Heap)
# Set the output folder where your program will be created

set (CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -std=c++11")
set(CMAKE_BUILD_TYPE Release)

include_directories(header)

file(GLOB SOURCES src/*.cpp)


add_executable(../heap.o ${SOURCES})
