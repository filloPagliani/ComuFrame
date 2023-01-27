#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "jsoncons/json.hpp"

class DataNode {
public:
	//constructor
	DataNode(std::string sender);

	//add a packet of data in the datanode given a json
	bool addPacket(std::unordered_map<std::string, std::string> packToAdd);

	//add a packet of data in the datanode given a json
	bool addPacket(jsoncons::json jPackToAdd);

	//given the unordered map representing the data that i'm searching for it retrieves the pack in wich you can found them (empty map if it doesn't exists
	std::unordered_map<std::string, std::string> findPacket(std::unordered_map<std::string, std::string> dataToFind);

	//given the unordered map representing the data that i'm searching for it retrieves the name (string) of the packet that contains them (empty string if it doesnt exists)
	std::string findPacketName(std::unordered_map<std::string, std::string> dataToFind);
	
	//getter
	std::string getSender();
	std::vector<std::unordered_map<std::string, std::string>> getAllPacket();
	std::unordered_map<std::string, std::string> getPacket(std::string packetName);
	
	//setter
	void setSender(std::string sender);
private:
	std::string sender;
	std::vector<std::unordered_map<std::string,std::string>> packetTypes;
};