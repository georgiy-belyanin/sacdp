cmake_minimum_required(VERSION 3.7)

project (bigint VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -fsanitize=undefined -fsanitize=address")

add_library(bigint src/bigint.c)

target_include_directories(bigint PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(bigint_tests tests/bigint.c)

target_include_directories(bigint_tests PRIVATE ${PROJECT_SOURCE_DIR}/vendor)
target_link_libraries(bigint_tests bigint)

