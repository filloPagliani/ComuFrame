
#-----------libzmq---------
add_subdirectory("libzmq")
install(
    TARGETS libzmq
    RUNTIME COMPONENT Public
    LIBRARY COMPONENT Public
    NAMELINK_COMPONENT Public
    ARCHIVE COMPONENT Public
)


#-----------cppzmq---------
add_subdirectory("cppzmq")
add_library(cppzmq INTERFACE)

target_include_directories(cppzmq
    INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/zeromq/cppzmq/include"
)