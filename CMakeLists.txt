cmake_minimum_required(VERSION 3.16)
project(assignment3-spl-client)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        assignment3-spl-client
        src/EchoClient.cpp
        include/ConnectionHandler.h
        src/ConnectionHandler.cpp
        src/Casters.cpp
        include/ClientToServerMessage.h
        include/ServerToClientMessage.h src/ServerToClientMessage.cpp include/SessionData.h src/SessionData.cpp)

