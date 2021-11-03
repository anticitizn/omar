project(omar)

file(GLOB SOURCES src/*.cpp)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

find_package(SDL2 REQUIRED)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" ${SDL2_INCLUDE_DIRS})
