#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/Node.h"
#include "yaml-cpp/yaml.h"

TEST_CASE("Node unit test") {
	Node nodeTest1("nodeTest1");
	REQUIRE(nodeTest1.getidentity() == "nodeTest1");
	YAML::Node sendingPackets = nodeTest1.getSendingPackets();
	YAML::Node requestedData = nodeTest1.getrequestedData();
	REQUIRE(sendingPackets.IsMap());
	REQUIRE(requestedData.IsMap());
}