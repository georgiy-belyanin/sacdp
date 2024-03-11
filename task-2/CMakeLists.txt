cmake_minimum_required(VERSION 3.7)

enable_testing()

project (ffield VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -fsanitize=undefined -fsanitize=address")

add_library(ffield src/field.c)

target_include_directories(ffield PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(ffield_tests tests/field.c)

target_include_directories(ffield_tests PRIVATE ${PROJECT_SOURCE_DIR}/vendor)
target_link_libraries(ffield_tests ffield)

add_test(NAME ffield COMMAND ffield_tests)

