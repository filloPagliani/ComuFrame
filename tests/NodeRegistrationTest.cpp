#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/Node.h"
#include "../ComuFrame/Central.h"
#include "jsoncons/json.hpp"

TEST_CASE("Registration of a node tests") {
	std::string nameNode1 = "nodeTest1";
	std::string nameNode2 = "nodeTest2";
	jsoncons::json dataNodeTest1;
	jsoncons::json dataNodeTest2;
	Node nodeTest1("nodeTest1");
	Node nodeTest2("nodeTest2");
	Central central;

	std::thread node1Thread(&Node::initNode, &nodeTest1);
	std::thread node2Thread(&Node::initNode, &nodeTest2);
	central.initCentral();

	tinyxml2::XMLDocument configDoc;
	if (tinyxml2::XML_SUCCESS != configDoc.LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config.xml")) {
		std::cout << "can't load configration file, using default options";
	}
	else {
		try {
			tinyxml2::XMLElement* nodeElement1 = configDoc.RootElement()->FirstChildElement(nameNode1.c_str());
			nodeElement1 = nodeElement1->FirstChildElement("data");
			//TODO sistema le eccezzioni es: se non c'è l'array, se non ci sono i nomi dei pack ecc
			dataNodeTest1 = jsoncons::json::parse(nodeElement1->GetText());
			for (auto i = dataNodeTest1["Packets"].begin_elements(); i < dataNodeTest1["Packets"].end_elements(); i++)
			{
				i->insert_or_assign("nameID", nameNode1 + "_" + i->at("nameID").as_string());
				
			}
			tinyxml2::XMLElement* nodeElement2 = configDoc.RootElement()->FirstChildElement(nameNode2.c_str());
			nodeElement2 = nodeElement2->FirstChildElement("data");
			//TODO sistema le eccezzioni es: se non c'è l'array, se non ci sono i nomi dei pack ecc
			dataNodeTest2 = jsoncons::json::parse(nodeElement2->GetText());
			for (auto i = dataNodeTest2["Packets"].begin_elements(); i < dataNodeTest2["Packets"].end_elements(); i++)
			{
				i->insert_or_assign("nameID", nameNode2 + "_" + i->at("nameID").as_string());
			}

		}
		catch (std::exception e) {
			std::cout << "cannot find the configuration of " << nameNode1 << " inside the configuration file. exception: " << e.what();
		}	
	}
	jsoncons::json receivedData1;
	jsoncons::json receivedData2;
	if (central.getDataNodes()[0].getSender() == nameNode1) {
		receivedData1 = jsoncons::json(central.getDataNodes()[0].getAllPacket()[0]);
		receivedData2 = jsoncons::json(central.getDataNodes()[1].getAllPacket()[0]);
	}
	else if (central.getDataNodes()[0].getSender() == nameNode2) {
		receivedData1 = jsoncons::json(central.getDataNodes()[1].getAllPacket()[0]);
		receivedData2 = jsoncons::json(central.getDataNodes()[0].getAllPacket()[0]);
	}
	REQUIRE(receivedData1.as_string() == dataNodeTest1["Packets"][0].as_string());
	REQUIRE(receivedData2.as_string() == dataNodeTest2["Packets"][0].as_string());


	node1Thread.join();
	node2Thread.join();
}