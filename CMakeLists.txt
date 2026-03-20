cmake_minimum_required(VERSION 4.0.2)
project(project2b)

set(CMAKE_CXX_STANDARD 20)

add_executable(project2b
        main.cpp
        Parser.cpp
        CLI.cpp
        HashTable.cpp
        RBT.cpp
        RBT.h
)
