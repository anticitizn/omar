
cmake_minimum_required(VERSION 3.7)
project(omar)

find_package(SDL2 REQUIRED)
include_directories(omar ${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp)
add_executable(omar ${SOURCES} main.cpp)

target_link_libraries(omar ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})
target_include_directories(omar PUBLIC "${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
