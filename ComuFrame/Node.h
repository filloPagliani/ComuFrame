#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include "tinyxml2.h"
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace zmq;

class Node
{
public:
	Node(std::string identity);
	~Node();
	void initNode();
	std::string getDataConfig();


	//getter
	std::string getUrl();
	std::string getidentity();
	std::string getDataNodeRequest();
private:
	std::string url;
	std::string identity;
	std::string dataNodeRequest;
	context_t ctx;
	void initServiceThread();

};