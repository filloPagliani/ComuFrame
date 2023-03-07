cmake_minimum_required (VERSION 3.18)

set(source_file_launchcentral
	"LaunchCentral.cpp"
	"../ComuFrame/Central.cpp"
	"../ComuFrame/Central.h" 
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h" 
	"../Comuframe/Fsm.h"
	"../ComuFrame/Fsm.cpp"
	"../ComuFrame/Database.h"
	"../ComuFrame/Database.cpp"
)

set(source_file_launchnode
	"LaunchNode.cpp" 
	"../Comuframe/Node.h"
	"../Comuframe/Node.cpp"
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h"
	"../Comuframe/Fsm.h"
	"../ComuFrame/Fsm.cpp"
)

add_executable(LaunchCentral 
	${source_file_launchcentral}
)
add_executable(LaunchNode 
	${source_file_launchnode}
)
add_executable(Tests
	"FsmTest.cpp"
	"DataBaseTest.cpp"
	"CentralTest.cpp"
	"NodeTest.cpp"
	"../ComuFrame/Node.h" 
	"../ComuFrame/Node.cpp"
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h"
	"../ComuFrame/Central.cpp"
	"../ComuFrame/Central.h" 
	"../Comuframe/Fsm.h"
	"../ComuFrame/Fsm.cpp"
	"../ComuFrame/Database.h"
	"../ComuFrame/Database.cpp"
 )

find_package(eCAL REQUIRED)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain tinyxml2 libzmq jsoncons yaml-cpp)
target_link_libraries(LaunchNode yaml-cpp eCAL::core protobuf::libprotobuf)
target_link_libraries(LaunchCentral yaml-cpp eCAL::core protobuf::libprotobuf)