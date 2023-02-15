#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/DataBase.h"
#include "yaml-cpp/yaml.h"

TEST_CASE("database test") {


	YAML::Node fakeNodeData1 = YAML::Load("{PackTest1: {aInt: int, aDouble: double}, PackTest2: {fuel: int, posx: double, posy: double}}");
	YAML::Node fakeNodeData2 = YAML::Load("{PackTest3: {aInt: int, anotherDouble: double}, PackTest4: {display: int, lights: bool, intensity: double}}");

	DataBase db;
	db.addPacket(fakeNodeData1["PackTest1"], "fakeNode1", "PackTest1");
	db.addPacket(fakeNodeData1["PackTest2"], "fakeNode1", "PackTest2");
	db.addPacket(fakeNodeData2["PackTest3"], "fakeNode2", "PackTest3");
	db.addPacket(fakeNodeData2["PackTest4"], "fakeNode2", "PackTest4");
	REQUIRE(db.getAvaiablePacks()["PackTest1"] == false);
	REQUIRE(db.getAvaiablePacks()["PackTest2"] == false);
	REQUIRE(db.getAvaiablePacks()["PackTest3"] == false);
	REQUIRE(db.getAvaiablePacks()["PackTest4"] == false);
	REQUIRE(((db.getIndex()["aInt_int"][0].first == "PackTest1_fakeNode1") || (db.getIndex()["aInt_int"][0].first == "PackTest3_fakeNode2")));
	REQUIRE(((db.getIndex()["aInt_int"][1].first == "PackTest1_fakeNode1") || (db.getIndex()["aInt_int"][1].first == "PackTest3_fakeNode2")));
	REQUIRE(db.getIndex()["display_int"][0].first == "PackTest4_fakeNode2");
	std::vector<std::string> requestedData = { "aDouble_double","display_int","fuel_int" };
	std::vector<std::string> retrievedPackets = db.providePackets(requestedData);
	REQUIRE(std::find(retrievedPackets.begin(), retrievedPackets.end(), "PackTest1_fakeNode1")!=retrievedPackets.end());
	REQUIRE(std::find(retrievedPackets.begin(), retrievedPackets.end(), "PackTest2_fakeNode1") != retrievedPackets.end());
	REQUIRE(std::find(retrievedPackets.begin(), retrievedPackets.end(), "PackTest4_fakeNode2") != retrievedPackets.end());
	REQUIRE(retrievedPackets.size() == 3);
	requestedData = { "aInt_int", "aDouble_double" };
	retrievedPackets = db.providePackets(requestedData);
	REQUIRE(std::find(retrievedPackets.begin(), retrievedPackets.end(), "PackTest1_fakeNode1") != retrievedPackets.end());
	REQUIRE(retrievedPackets.size() == 1);
	requestedData = { "notPresentData" };
	retrievedPackets = db.providePackets(requestedData);
	REQUIRE(retrievedPackets[0] == "can't find your data in the DB");
}