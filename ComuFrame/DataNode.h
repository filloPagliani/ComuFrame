#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

class DataNode {
public:
	DataNode(std::string sender);

	bool addPacket(std::unordered_map<std::string, std::string> packToAdd);
	std::unordered_map<std::string, std::string> findPacket(std::unordered_map<std::string, std::string> dataToFind);
	std::string findPacketName(std::unordered_map<std::string, std::string> dataToFind);
	//getter
	std::string getSender();
	std::vector<std::unordered_map<std::string, std::string>> getData();
	//setter
	void setSender(std::string sender);
private:
	std::string sender;
	std::vector<std::unordered_map<std::string,std::string>> packetTypes;
};