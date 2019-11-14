cmake_minimum_required(VERSION 3.15)
project(uplay32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_library(uplay32 SHARED main.c)