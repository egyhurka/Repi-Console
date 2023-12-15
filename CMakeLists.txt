cmake_minimum_required(VERSION 3.27)
project(v0_03)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp fileHandler.cpp utilities.cpp variables.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
add_executable(v0_03 ${SOURCE_FILES})
set_property(TARGET v0_03 PROPERTY CXX_STANDARD 20)