cmake_minimum_required(VERSION 3.30)
project(FOFIS)

set(CMAKE_CXX_STANDARD 20)

add_executable(FOFIS main.cpp
        aircraft.h)
target_include_directories(FOFIS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})