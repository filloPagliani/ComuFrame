#include <stdio.h>
#include <iostream>
#include <thread>
#include <list>
#include "zmq.hpp"
#include "zmq_addon.hpp"

using namespace zmq;

void joinThreads(std::list<std::thread> threads);
zmq::socket_t initInprocSocket(zmq::context_t* ctx, std::string url, bool isConnect);
bool sendStrMSG(zmq::socket_t* sock, std::string msg,std::string state);
bool receiveStrMsgTimeout(socket_t* sock, int timeout);

