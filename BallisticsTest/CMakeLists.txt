cmake_minimum_required(VERSION 4.0)
project(BallisticsTest)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BallisticsTest
        ballistic.cpp
        ballistics.h
        BallisticsTest.cpp
        globalConstants.h
        objects.cpp
        objects.h
        windowFunctions.cpp
        windowFunctions.h)
