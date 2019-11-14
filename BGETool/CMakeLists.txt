cmake_minimum_required(VERSION 3.15)
project(BGETool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

include_directories(${PROJECT_SOURCE_DIR}/lib/include)
find_library(DETOURS_LIBRARY NAMES detours HINTS "${PROJECT_SOURCE_DIR}/lib")

add_library(BGETool SHARED error.cpp dllmain.cpp Structures.h magic_enum.hpp Offsets.cpp Input.cpp mem.cpp Camera.cpp main.cpp)
target_link_libraries(BGETool ${DETOURS_LIBRARY})