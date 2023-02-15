#include "DataBase.h"
//take in imput a YAML containing one packet and its info and add it to the db
void DataBase::addPacket(YAML::Node YPack,std::string sender,std::string name) {
	std::unordered_map<std::string, std::string> packToAdd = YPack.as< std::unordered_map<std::string, std::string>>();
	std::string packName = name + "_" + sender;
	this->avaiablePacks.insert({packName,false});
	for (auto it = packToAdd.begin(); it!=packToAdd.end(); it++) {
		std::string dataName = it->first + "_" + it->second;
		auto dataToUpdate = this->index.find(it->first + "_" + it->second);
		if (dataToUpdate != this->index.end()) {
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

//just reset all the packets to "not taken"
void DataBase::resetPackets() {
	for (auto it = this->avaiablePacks.begin(); it != this->avaiablePacks.end(); it++) {
		it->second = false;
	}
}

//given a set of data requested from a node provide the packets in wich they can be found. if they are not in the DB return "can't your data in the DB"
std::vector<std::string> DataBase::providePackets(std::vector<std::string> requestedData) {
	std::vector<std::vector<bool*>> andClause;
	int maxSize = 0;
	for (auto& data : requestedData) {
		std::vector<bool*> orClause;
		std::vector<std::pair<std::string, bool*>> packetsWitData = index[data];
		for (auto& pack : packetsWitData) {
			orClause.push_back(pack.second);
		}
		if (orClause.size() == 1) {
			*(orClause[0]) = true;
		}
		else {
			andClause.push_back(orClause);
			if (orClause.size() > maxSize) { maxSize = orClause.size(); }
		}
	}//hai fatto la matrice devi iterare i test hint: funz che ti crei e scorre e ne mette uno true alla volta poi testa lo setta false e va a quello dopo; questa funzione poi la fai ricorsiva che si chiama da sola una volta prima poi due per le coppie e cosi via a ogni chiamata escludendo le prime x colonne ricorda di non settare a 1 le cose gia a uno perche singole clause che non sono presenti nella tabella
	std::vector<std::string> result;
	for (int i = 0; i < andClause.size();i++) {
		if (tuneFunctions(&andClause, maxSize, i, 0)) {
			break;
		}
		else if (i == andClause.size() - 1) { result.push_back("can't find your data in the DB"); return result; }
	}

	for (auto& i : this->avaiablePacks) {
		if (i.second) {
			result.push_back(i.first);
		}
	}
	resetPackets();
	return result;
}

bool DataBase::tuneFunctions(std::vector<std::vector<bool*>>* formula, int maxSize, int recCallNum, int callNumber) {
	for (int row = 0; row < maxSize; row++) {
		for (int column = callNumber; column < formula->size();column++) {
			if (row < (*formula)[column].size()) {
				(*(*formula)[column][row]) = true;
				bool res = false;
				if (recCallNum == callNumber) {
					res = evaluateAND(*formula);
				}
				else {
					res = tuneFunctions(formula, maxSize, recCallNum, callNumber + 1);
				}
				if (res) {
					return res;
				}
				else {
					(*(*formula)[column][row]) = false;
				}
			}
		}
	}
}

bool DataBase::evaluateOR(std::vector<bool*> OR) {
	for (auto& clause : OR ) {
		if (clause) {
			return true;
		}
	}
	return false;
}

bool DataBase::evaluateAND(std::vector<std::vector<bool*>> AND) {
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