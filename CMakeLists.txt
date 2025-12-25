CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

project(datastructure-b)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(matrix matrix.cc)

add_executable(avl_tree avl_tree.cc)
