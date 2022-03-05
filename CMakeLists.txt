cmake_minimum_required(VERSION 3.22)
project(thcsv)

set (CMAKE_CXX_STANDARD 17)
add_executable(thcsv main.cpp thcsv.cpp common.cpp thdecode.cpp)