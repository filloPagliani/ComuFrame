#include "DataBase.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "tinyxml2.h"
#include "jsoncons/json.hpp"
#include "Fsm.h"


using namespace zmq;

class Central
{
public:
	//constructor
	Central();
	~Central();
	void initCentral();

	//Getter
	std::string getUrl();
	int getExpectedClient();

private:
	Fsm state;
	std::string url;
	int expectedClient = 0;
	context_t ctx;
	std::vector<std::string> connectedClients;
	DataBase db;
	void initServiceThread();
	std::vector<std::string> registerClient(socket_t * sock);
	bool sendStrMSG(socket_t* sock, std::string msg, std::string adressee);

};