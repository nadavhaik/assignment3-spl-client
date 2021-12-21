cmake_minimum_required(VERSION 3.16)
project(assignment3_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(assignment3_server main.cpp include/ConnectionHandler.h src/ConnectionHandler.cpp)
