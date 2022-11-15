cmake_minimum_required(VERSION 3.16)

project(omar)

file(GLOB SOURCES src/*.cpp)

if(${CMAKE_CURRENT_SOURCE_DIR} MATCHES ${CMAKE_SOURCE_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
	set(EXT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/")
	add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)
else()
	add_library(${PROJECT_NAME} OBJECT ${SOURCES})
endif()

find_package(SDL2 REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" ${SDL2_INCLUDE_DIRS})
