cmake_minimum_required(VERSION 3.15)
project(BGEToolInjector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(BGEToolInjector main.cpp BGEToolInjector.rc)