cmake_minimum_required(VERSION 3.10)
project(CS40F18 LANGUAGES CXX C)

#Weird Opengl library changes on cmake 3.10/3.11 boundary
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

FIND_PACKAGE(OpenGL REQUIRED)    #for graphics
FIND_PACKAGE(Qt5OpenGL REQUIRED) #for window UI

#get symlink_data macro, compile info target, default build type
include(${CMAKE_SOURCE_DIR}/cmake_modules/helpers.cmake)
#include(${CMAKE_SOURCE_DIR}/cmake_modules/detectCUDA.cmake)

#default parameters that almost every program will need
target_link_libraries(compile_info INTERFACE Qt5::OpenGL OpenGL::GL)

#things to actually build. The top level CMake parameters defined here
#will be propagated to the subdirectories below
add_subdirectory(common)
add_subdirectory(metaballs)
