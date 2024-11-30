cmake_minimum_required(VERSION 3.29)
project(traveling_salesman_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(traveling_salesman_project main.cpp
        TimeWindow.cpp
        TimeWindow.h
        Node.cpp
        Node.h
        Edge.cpp
        Edge.h
        Graph.cpp
        Graph.h
        TestDataHelper.cpp
        TestDataHelper.h
        Greedy.cpp
        Greedy.h)
