#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include "yaml-cpp/yaml.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "Fsm.h"
#include <chrono>
#include <ecal/ecal.h>
#include <ecal/msg/publisher.h>
#include <ecal/msg/subscriber.h>

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
	YAML::Node getrequestedData();
private:
	//properties
	Fsm state;
	std::string url;
	std::string pubPort;
	std::string servicePort;
	std::string identity;
	YAML::Node sendingPackets;
	YAML::Node requestedData;
	std::vector<std::string> subscriptions;
	//methods
	bool receiveStrMsgTimeout(socket_t* sock, int timeout);
	bool sendStrMSG(socket_t* sock, std::string msg, std::string sendingThread, std::string addressee);
	context_t ctx;
	void initServiceThread();
	void initPubThread();
	void initSubThread();
};