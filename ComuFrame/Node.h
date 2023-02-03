#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include "tinyxml2.h"
#include "jsoncons/json.hpp"
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
	jsoncons::json getSendingData();
	jsoncons::json getrequestedDataa();
private:
	//properties
	Fsm state;
	std::string url;
	std::string identity;
	jsoncons::json sendingData;
	jsoncons::json requestedData;
	//methods
	bool receiveStrMsgTimeout(socket_t* sock, int timeout);
	bool sendStrMSG(socket_t* sock, std::string msg);
	context_t ctx;
	void initServiceThread();
	bool waitForAck(std::string ackName,socket_t* sock);


};