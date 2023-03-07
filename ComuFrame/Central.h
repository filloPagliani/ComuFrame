#include "DataBase.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "utils.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "Fsm.h"
#include "yaml-cpp/yaml.h"
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace zmq;

class Central
{
public:
	//constructor
	Central();
	~Central();
	//methods
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
	void initTimerThread();
	void preciseSleep(std::chrono::milliseconds millisecond, std::chrono::milliseconds higResTime);
	std::vector<std::string> registerClient(socket_t * sock);
	bool sendStrMSG(socket_t* sock, std::string msg, std::string adressee);

};