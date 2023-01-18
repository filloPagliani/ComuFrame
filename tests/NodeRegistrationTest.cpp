#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/Node.h"

TEST_CASE("Registration of a node tests") {
	Node nodeTest1("nodeTest1");
	nodeTest1.initNode();
}