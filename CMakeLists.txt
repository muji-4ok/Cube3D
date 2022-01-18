cmake_minimum_required(VERSION 3.17)
project(Cube3D)

set(CMAKE_CXX_STANDARD 20)

find_package(Freetype REQUIRED)
find_package(OpenCV REQUIRED core imgproc imgcodecs highgui video videoio)
find_package(Threads REQUIRED)
find_package(glfw3 REQUIRED)

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/fonts/arial.ttf)
    message(FATAL_ERROR "Arial font is required to run but wasn't found! Find it and copy from your"
            " system folder to ./fonts/arial.ttf")
endif ()

include_directories(${PROJECT_SOURCE_DIR}/lib/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/FastKociemba/kociemba/ckociemba/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/OptimalCubeSolver)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
# FIXME: find_package for glfw doesn't set the include directory...
include_directories(/usr/include/GLFW)

add_library(glad STATIC ${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c)

file(GLOB OptimalCubeSolver_SOURCES ${PROJECT_SOURCE_DIR}/lib/OptimalCubeSolver/*.c)
add_library(OptimalCubeSolver STATIC ${OptimalCubeSolver_SOURCES})

file(GLOB ckociemba_SOURCES ${PROJECT_SOURCE_DIR}/lib/FastKociemba/kociemba/ckociemba/*.c)
add_library(ckociemba STATIC ${ckociemba_SOURCES})

file(GLOB Cube3D_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(Cube3D ${Cube3D_SOURCES})
target_link_libraries(Cube3D PRIVATE OptimalCubeSolver)
target_link_libraries(Cube3D PRIVATE ckociemba)
target_link_libraries(Cube3D PUBLIC glad ${FREETYPE_LIBRARIES} ${OpenCV_LIBS} Threads::Threads ${CMAKE_DL_LIBS} glfw)
