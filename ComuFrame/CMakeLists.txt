﻿# CMakeList.txt : CMake project for ComuFrame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
# Add source to this project's executable.
add_executable (ComuFrame 
"ComuFrame.cpp" 
"ComuFrame.h" 
"Central.cpp"
"Central.h" 
"Node.cpp" 
"Node.h" 
"utils.cpp" 
"utils.h" 
"Fsm.cpp" 
"Fsm.h" 
"DataBase.h"
"DataBase.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ComuFrame PROPERTY CXX_STANDARD 20)
endif()
find_package(eCAL REQUIRED)
target_link_libraries(ComuFrame eCAL::core)
# TODO: Add tests and install targets if needed.
