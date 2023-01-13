cmake_minimum_required (VERSION 3.18)
target_link_libraries(ComuFrame)

add_executable(LaunchCentral 
	"LaunchCentral.cpp"
	"../ComuFrame/Central.cpp"
	"../ComuFrame/Central.h" 
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h" 
	"../ComuFrame/DataNode.cpp" 
	"../ComuFrame/DataNode.h")
add_executable(LaunchNode 
	"LaunchNode.cpp" 
	"../ComuFrame/Node.cpp" 
	"../ComuFrame/Node.h" 
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h")
add_executable(Tests
	"DataNodeTest.cpp"
	"../ComuFrame/DataNode.cpp"
	"../ComuFrame/DataNode.h")

target_link_libraries(libzmq)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)