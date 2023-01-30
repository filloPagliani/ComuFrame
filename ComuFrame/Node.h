#define ZMQ_BUILD_DRAFT_API
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
	Node(std::string identity);
	~Node();
	void initNode();
	//getter
	std::string getUrl();
	std::string getidentity();
	jsoncons::json getDataNodeRequest();
private:
	Fsm state;
	std::string url;
	std::string identity;
	jsoncons::json dataNodeRequest;
	bool sendRegistration(socket_t* sock);
	context_t ctx;
	void initServiceThread();

};