#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "tinyxml2.h"
#include "jsoncons/json.hpp"
#include "DataNode.h"

using namespace zmq;

class Central
{
public:
	//constructor
	Central();
	~Central();
	void initCentral();

	//Getter
	std::vector<DataNode> getDataNodes();
	std::string getUrl();
	int getExpectedClient();

private:
	std::string url;
	int expectedClient = 0;
	context_t ctx;
	std::vector<std::string> connectedClients;
	std::vector<DataNode> dataNodes;

	void initServiceThread();
	std::vector<std::string> registerClient(socket_t * sock);
};