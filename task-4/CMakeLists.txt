cmake_minimum_required(VERSION 3.7)

project (dpll VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_executable(dpll src/dpll.c)

