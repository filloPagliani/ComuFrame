#include "zmq.hpp"
#include "zmq_addon.hpp"
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace zmq;

class Central
{
public:

	Central(std::string url);
	~Central();
	void initCentral();

	//Getter
	std::string getUrl();

private:
	std::string url;
	context_t ctx;

	void initServiceThread();
	std::vector<std::string> registerCliet(socket_t * sock);
};