#include "DataNode.h"
DataNode::DataNode(std::string sender)
{
	DataNode::sender = sender;
}

bool DataNode::addPacket(std::unordered_map<std::string, std::string> packToAdd) {
	if ((this->packetTypes.empty()) || (std::count(this->packetTypes.begin(), this->packetTypes.end(), packToAdd) == 0) ){
		this->packetTypes.push_back(packToAdd);
		return true;
	}
	else {
		return false;
	}
}

//getter

std::string DataNode::getSender()
{
	return DataNode::sender;
}

std::vector<std::unordered_map<std::string, std::string>> DataNode::getData()
{
	return DataNode::packetTypes;
}