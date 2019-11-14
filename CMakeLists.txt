cmake_minimum_required(VERSION 3.15)
project(BGEToolSolution)

add_subdirectory("${PROJECT_SOURCE_DIR}/BGETool" "${PROJECT_SOURCE_DIR}/BGETool/build")
add_subdirectory("${PROJECT_SOURCE_DIR}/BGEToolInjector" "${PROJECT_SOURCE_DIR}/BGEToolInjector/build")
add_subdirectory("${PROJECT_SOURCE_DIR}/uplay32" "${PROJECT_SOURCE_DIR}/uplay32/build")