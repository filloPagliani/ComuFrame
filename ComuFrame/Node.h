#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace zmq;

class Node
{
public:
	Node(std::string url, std::string identity);
	~Node();
	void initNode();
	void initServiceThread();

	//getter
	std::string getUrl();
	std::string getidentity();
private:
	std::string url;
	std::string identity;
	context_t ctx;

};