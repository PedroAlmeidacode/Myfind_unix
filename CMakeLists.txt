cmake_minimum_required(VERSION 3.16)
project(Myfind C)

set(CMAKE_C_STANDARD 99)

add_executable(Myfind main.c library.h lib_util.c)


target_link_libraries(${PROJECT_NAME} pthread)