#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/DataNode.h"


TEST_CASE("DataNode unit test") {
	DataNode dataTest("fakeNode");
	REQUIRE(dataTest.getSender() == "fakeNode");

	std::unordered_map<std::string, std::string> aPacket({ {"aString", "string"}, {"anInt", "int"}});
	dataTest.addPacket(aPacket);
	REQUIRE(dataTest.getAllPacket().size() == 0); //aPacket doesn't have the mandatory field "nameID"
	aPacket["nameID"] = "aPacket";
	dataTest.addPacket(aPacket);
	REQUIRE(dataTest.getAllPacket().size() == 1);

	std::unordered_map<std::string, std::string> anotherPacket({ {"nameID", "anotherPacket"}, {"aFloat", "float"}, {"aVector", "vector"}, {"aList", "list"}});
	dataTest.addPacket(anotherPacket);
	REQUIRE(dataTest.getAllPacket().size() == 2);
	REQUIRE(dataTest.getAllPacket()[1] == anotherPacket);
	REQUIRE(dataTest.getAllPacket()[1]["aFloat"] == "float");

	dataTest.addPacket(anotherPacket);
	REQUIRE(dataTest.getAllPacket().size() == 2);

	//testing findPacket and findPacketName
	std::unordered_map<std::string,std::string> foundData = dataTest.findPacket(std::unordered_map<std::string, std::string>{ {"aFloat", "float"}, { "aVector","vector" }});
	REQUIRE(foundData == anotherPacket);
	foundData = dataTest.findPacket(std::unordered_map<std::string, std::string>{ {"aFloat", "float"}, { "wrongKey","wrongType" }});
	std::unordered_map<std::string, std::string> emptypack;
	REQUIRE(foundData == emptypack);
	std::string foundName = dataTest.findPacketName(std::unordered_map<std::string, std::string>{ {"aFloat", "float"}, { "aVector","vector" }});
	REQUIRE(foundName == "anotherPacket");
	foundName = dataTest.findPacketName(std::unordered_map<std::string, std::string>{ {"aFloat", "float"}, { "wrongKey","wrongType" }});
	REQUIRE(foundName == "");
}