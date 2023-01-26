#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/Node.h"
#include "../ComuFrame/Central.h"

TEST_CASE("Registration of a node tests") {
	//va multithreaddato per ora debugga su launch
	Node nodeTest1("nodeTest1");
	nodeTest1.initNode();
	Central central;
	central.initCentral();
}