#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "Fsm.h"
#include <chrono>


using namespace zmq;

class Node
{
public:
	//methods
	Node(std::string identity);
	~Node();
	void initNode();
	//getter
	std::string getUrl();
	std::string getidentity();
	YAML::Node getSendingPackets();
	YAML::Node getrequestedDataa();
private:
	//properties
	Fsm state;
	std::string url;
	std::string identity;
	YAML::Node sendingPackets;
	YAML::Node requestedData;
	//methods
	bool receiveStrMsgTimeout(socket_t* sock, int timeout);
	bool sendStrMSG(socket_t* sock, std::string msg);
	context_t ctx;
	void initServiceThread();
	bool waitForAck(std::string ackName,socket_t* sock);


};