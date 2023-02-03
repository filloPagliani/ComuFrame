#include "DataBase.h"
//take in imput a json version extracted from DataNode and insert the packet in the db
bool DataBase::addPacket(jsoncons::json jPack,std::string sender) {
	std::unordered_map<std::string, std::string> packToAdd = jPack.as< std::unordered_map<std::string, std::string>>();
	std::string packName = packToAdd["nameID"] + "_" + sender;
	if (packToAdd.find("nameID") == packToAdd.end()) {
		return false;
	}
	else {
		this->avaiablePacks.insert({packName,false});
		for (auto it = packToAdd.begin(); it!=packToAdd.end(); it++) {
			std::string dataName = it->first + "_" + it->second;
			auto dataToUpdate = this->index.find(it->first + "_" + it->second);
			if ( it->first == "nameID") {
				continue;
			}
			else if (dataToUpdate != this->index.end()) {
				std::pair<std::string, bool*> pairToAdd = { packName,&(avaiablePacks[packName])};
				dataToUpdate->second.push_back(pairToAdd);
			}
			else {
				std::vector<std::pair<std::string, bool *>> pairToAdd;
				pairToAdd.push_back({ packName,&avaiablePacks[packName] });
				index[dataName]=pairToAdd;
			}
		}
	}
}

//just reset all the packets to "not taken"
void DataBase::resetPackets() {
	for (auto it = this->avaiablePacks.begin(); it != this->avaiablePacks.end(); it++) {
		it->second = false;
	}
}

//given a set of data requested from a node provide the packet in wichhe can find them.
std::vector<std::string> providePackets(std::vector<std::string> requestedData);


//getter
DataBase::dbMap DataBase::getIndex() {
	return this->index;
}
std::unordered_map<std::string, bool> DataBase::getAvaiablePacks() {
	return this->avaiablePacks;
}