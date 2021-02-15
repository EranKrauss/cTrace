cmake_minimum_required(VERSION 3.17)
project(cTrace)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(cTrace src/main.cpp
        src/Graph.cpp
        include/Graph.h
        include/json.hpp
        src/Session.cpp
        include/Session.h
        src/Tree.cpp
        include/Tree.h
        src/Agent.cpp include/Agent.h)