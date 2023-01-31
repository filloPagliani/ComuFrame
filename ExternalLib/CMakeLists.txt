#-----------tinyXML2-------
option(tinyxml2_BUILD_TESTING "" OFF) 
option(tinyxml2_SHARED_LIBS "" ON)
add_subdirectory("tinyxml2")
target_link_libraries(ComuFrame tinyxml2)

install(TARGETS tinyxml2
    COMPONENT Public
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES tinyxml2/tinyxml2.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ThirdParties
    COMPONENT Common
)

if (MSVC)
    install(
        FILES $<TARGET_PDB_FILE:tinyxml2>
        COMPONENT PDB
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

#-----------jsoncons-------

add_library(jsoncons INTERFACE)
target_link_libraries(ComuFrame jsoncons)
target_include_directories(jsoncons
    INTERFACE
        jsoncons/include
)

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
add_library(cppzmq INTERFACE)
find_package(cppzmq)
option(-DENABLE_DRAFTS ON)
option(-DZMQ_BUILD_DRAFT_API 1)
target_link_libraries(ComuFrame cppzmq)
target_include_directories(cppzmq
    INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/cppzmq/include"
) 

#-----------Catch2---------

add_subdirectory("Catch2")

