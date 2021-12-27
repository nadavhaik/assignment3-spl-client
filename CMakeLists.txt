cmake_minimum_required(VERSION 3.16)
project(assignment3-spl-client)

set(CMAKE_CXX_STANDARD 11)

add_executable(assignment3_server main.cpp include/ConnectionHandler.h src/ConnectionHandler.cpp src/Casters.cpp)
