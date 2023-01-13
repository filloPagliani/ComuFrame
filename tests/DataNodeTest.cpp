#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/DataNode.h"


TEST_CASE("DataNode unit test") {
	DataNode dataTest("fakeNode");
	REQUIRE(dataTest.getSender() == "fakeNode");

	std::unordered_map<std::string, std::string> aPacket({{"aString", "string"}, {"anInt", "int"}});
	dataTest.addPacket(aPacket);
	REQUIRE(dataTest.getData().size() == 1);

	std::unordered_map<std::string, std::string> anotherPacket({ {"aFloat", "float"}, {"aVector", "vector"}, {"aList", "list"} });
	dataTest.addPacket(anotherPacket);
	REQUIRE(dataTest.getData().size() == 2);
	REQUIRE(dataTest.getData()[1] == anotherPacket);
	REQUIRE(dataTest.getData()[0]["aString"] == "string");

	dataTest.addPacket(anotherPacket);
	REQUIRE(dataTest.getData().size() == 2);
}