#pragma once
#include "yaml-cpp/yaml.h"
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <iostream>
class DataBase {
public:
	typedef std::pair<std::string, bool> packet;
	typedef std::unordered_map < std::string, std::vector<std::pair<std::string, bool*>>> dbMap;
	void addPacket(YAML::Node YPack, std::string sender, std::string name);
	std::vector<std::string> providePackets(std::vector<std::string> requestedData);

	//getter
	dbMap getIndex();
	std::unordered_map<std::string, bool> getAvaiablePacks();
private:
	dbMap index;
	std::unordered_map<std::string, bool> avaiablePacks;
	//methods
	bool tuneFunctions(std::vector<std::vector<bool*>> * formula, int maxSize, int recCallNum, int callNumber);
	void resetPackets();
	bool evaluateOR(std::vector<bool*> OR);
	bool evaluateAND(std::vector<std::vector<bool*>> AND);
};


//    std::vector<std::vector<bool*>> ands; le clausole da creare mentre controllo
