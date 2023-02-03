#pragma once
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include "jsoncons/json.hpp"
class DataBase {
public:
	typedef std::pair<std::string, bool> packet;
	typedef std::unordered_map < std::string, std::vector<std::pair<std::string, bool*>>> dbMap;
	bool addPacket(jsoncons::json jPack, std::string sender);
	std::vector<std::string> syncroNode(jsoncons::json requestedData);

	//getter
	dbMap getIndex();
	std::unordered_map<std::string, bool> getAvaiablePacks();
private:
	dbMap index;
	void resetPackets();
	std::unordered_map<std::string, bool> avaiablePacks;
	std::vector<std::string> providePackets(std::vector<std::string> requestedData);
};


//    std::vector<std::vector<bool*>> ands; le clausole da creare mentre controllo
