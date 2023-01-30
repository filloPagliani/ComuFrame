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
	"NodeRegistrationTest.cpp"
	"FsmTest.cpp"
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
)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain tinyxml2 libzmq jsoncons)
target_link_libraries(LaunchNode tinyxml2 jsoncons)
target_link_libraries(LaunchCentral tinyxml2 jsoncons)