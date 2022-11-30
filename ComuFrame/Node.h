#include "zmq.hpp"
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace zmq;

class Node
{
public:
	Node(std::string url, std::string identity);
	void initNode();
	void initRegisterThread();

	//getter
	std::string getUrl();
	std::string getidentity();
private:
	std::string url;
	std::string identity;
	context_t ctx;

};