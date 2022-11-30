
#-----------libzmq---------
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_CPACK    OFF CACHE BOOL "" FORCE)
set(ENABLE_CLANG    OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED    ON  CACHE BOOL "" FORCE)
set(BUILD_STATIC    OFF CACHE BOOL "" FORCE)
set(WITH_PERF_TOOL  OFF CACHE BOOL "" FORCE)
set(WITH_LIBSODIUM  OFF CACHE BOOL "" FORCE)
set(WITH_DOCS       OFF CACHE BOOL "" FORCE)
set(ENABLE_WS       OFF CACHE BOOL "" FORCE)
add_subdirectory("libzmq")


#-----------cppzmq---------
set(ZeroMQ_DIR C:/Users/pagliani/src/vcpkg/packages/zeromq_x64-windows/share/zeromq)
set(cppzmq_DIR C:/Users/pagliani/src/vcpkg/packages/cppzmq_x64-windows/share/cppzmq)
find_package(cppzmq)
target_link_libraries(ComuFrame cppzmq)
#add_library(cppzmq INTERFACE)
#
target_include_directories(cppzmq
    INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/cppzmq/include"
)