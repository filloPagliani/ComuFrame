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
std::vector<std::string> DataBase::providePackets(std::vector<std::string> requestedData) {
	std::vector<std::string> packList;
	std::vector<std::vector<std::pair<std::string, bool*>>> andClause;
	for (auto& data : requestedData) {
		std::vector<std::pair<std::string, bool*>> orClause;
		std::vector<std::pair<std::string, bool*>> packetsWitData = index[data];
		for (auto& pack : packetsWitData) {
			orClause.push_back(pack);
		}
		if (orClause.size() == 1) {
			packList.push_back(orClause[0].first);
			*(orClause[0].second) = true;
		}
		else {
			andClause.push_back(orClause);
		}
	}//hai fatto la matrice devi iterare i test hint: funz che ti crei e scorre e ne mette uno true alla volta poi testa lo setta false e va a quello dopo; questa funzione poi la fai ricorsiva che si chiama da sola una volta prima poi due per le coppie e cosi via a ogni chiamata escludendo le prime x colonne ricorda di non settare a 1 le cose gia a uno perche singole clause che non sono presenti nella tabella

}

bool evaluateOR(std::vector<std::pair<std::string, bool*>> OR) {
	for (auto& clause : OR ) {
		if (clause.second) {
			return true;
		}
	}
	return false;
}

bool evaluateAND(std::vector<std::vector<std::pair<std::string, bool*>>> AND) {
	for (auto& clause : AND) {
		if (!evaluateOR(clause)) {
			return false;
		}
	}
	return true;
}




//getter
DataBase::dbMap DataBase::getIndex() {
	return this->index;
}
std::unordered_map<std::string, bool> DataBase::getAvaiablePacks() {
	return this->avaiablePacks;
}