#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
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
	int getExpectedClient();

private:
	std::string url;
	int expectedClient = 0;
	context_t ctx;
	std::vector<std::string> connectedClients;

	void initServiceThread();
	std::vector<std::string> registerClient(socket_t * sock);
};