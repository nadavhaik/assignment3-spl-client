cmake_minimum_required(VERSION 3.10.2)
project(assignment3-spl-client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(
        assignment3-spl-client
        src/EchoClient.cpp
        include/ConnectionHandler.h
        src/ConnectionHandler.cpp
        include/ClientToServerMessage.h src/ClientToServerMessage.cpp
        include/ServerToClientMessage.h src/ServerToClientMessage.cpp include/SessionData.h src/SessionData.cpp src/Caster.cpp include/Caster.h)

