cmake_minimum_required(VERSION 3.7)
project(server C)

set(CMAKE_C_STANDARD 99)

add_executable(server main.c funkcie.c funkcie.h)

target_link_libraries(server pthread)