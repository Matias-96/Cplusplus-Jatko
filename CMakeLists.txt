cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
        TaskQueue.cpp
        TaskQueue.h
        Game_Task.h
        Simple.cpp
        Simple.h
        Log_Ptr.h)
