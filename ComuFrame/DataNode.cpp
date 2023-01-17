#include "DataNode.h"
DataNode::DataNode(std::string sender)
{
	DataNode::sender = sender;
}

bool DataNode::addPacket(std::unordered_map<std::string, std::string> packToAdd) {
	if (packToAdd.find("nameID") == packToAdd.end()) {
		return false;
	}
	else if ((this->packetTypes.empty()) || (std::count(this->packetTypes.begin(), this->packetTypes.end(), packToAdd) == 0) ){
		for (auto i = this->packetTypes.begin(); i != packetTypes.end(); i++) {
			if ((*i)["nameID"] == packToAdd["nameID"]) {
				return false;
			}
		}
		this->packetTypes.push_back(packToAdd);
		return true;
	}
	else {
		return false;
	}
}

//find the packet with the data that you need and return that packet, if it doesnt exixts return the empty map
std::unordered_map<std::string, std::string> DataNode::findPacket(std::unordered_map<std::string, std::string> dataToFind) { 
	for (auto i = DataNode::packetTypes.begin(); i != DataNode::packetTypes.end(); i++) {
		bool found = true;
		for (auto j = dataToFind.begin(); j != dataToFind.end(); j++) {
			if ((*i)[j->first] != j->second) {
				found = false;
				break; 
			};
		}
		if (found) { return (*i); }
	}
	std::unordered_map<std::string, std::string> emptyMap;
	return emptyMap;
}

//find the name of the packet with the data that you need and return that name, if it doesnt exixts return the empty string ""
std::string DataNode::findPacketName(std::unordered_map<std::string, std::string> dataToFind) { 
	for (auto i = DataNode::packetTypes.begin(); i != DataNode::packetTypes.end(); i++) {
		bool found = true;
		for (auto j = dataToFind.begin(); j != dataToFind.end(); j++) {
			if ((*i)[j->first] != j->second) {
				found = false;
				break;
			};
		}
		if (found) { return (*i)["nameID"]; }
	}
	return "";
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