cmake_minimum_required (VERSION 3.18)

add_executable(LaunchCentral 
	"LaunchCentral.cpp"
	"../ComuFrame/Central.cpp"
	"../ComuFrame/Central.h" 
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h" 
	"../ComuFrame/DataNode.cpp" 
	"../ComuFrame/DataNode.h"
	"../Comuframe/Fsm.h"
	"../ComuFrame/Fsm.cpp"
	"../ComuFrame/Database.h"
	"../ComuFrame/Database.cpp"


)
add_executable(LaunchNode 
	"LaunchNode.cpp" 
	"../ComuFrame/Node.cpp" 
	"../ComuFrame/Node.h" 
	"../ComuFrame/utils.cpp" 
	"../ComuFrame/utils.h"
	"../Comuframe/Fsm.h"
	"../ComuFrame/Fsm.cpp"

)
add_executable(Tests
	"DataNodeTest.cpp"
	"FsmTest.cpp"
	"DataBaseTest.cpp"
	"CentralTest.cpp"
	"NodeTest.cpp"
	"../ComuFrame/DataNode.cpp"
	"../ComuFrame/DataNode.h"
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

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain tinyxml2 libzmq jsoncons yaml-cpp)
target_link_libraries(LaunchNode yaml-cpp)
target_link_libraries(LaunchCentral yaml-cpp)