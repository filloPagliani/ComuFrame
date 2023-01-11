cmake_minimum_required (VERSION 3.18)
target_link_libraries(ComuFrame)

add_executable(LaunchCentral "LaunchCentral.cpp" "../ComuFrame/Central.cpp" "../ComuFrame/Central.h" "../ComuFrame/utils.cpp" "../ComuFrame/utils.h")
add_executable(LaunchNode "LaunchNode.cpp" "../ComuFrame/Node.cpp" "../ComuFrame/Node.h" "../ComuFrame/utils.cpp" "../ComuFrame/utils.h")

target_link_libraries(libzmq)