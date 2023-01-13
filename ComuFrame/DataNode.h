#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

class DataNode {
public:
	DataNode(std::string sender);
	std::string getSender();
	std::vector<std::unordered_map<std::string, std::string>> getData();
	bool addPacket(std::unordered_map<std::string, std::string> packToAdd);
private:
	std::string sender;
	std::vector<std::unordered_map<std::string,std::string>> packetTypes;
};