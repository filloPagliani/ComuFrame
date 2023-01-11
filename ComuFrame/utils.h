#include <stdio.h>
#include <iostream>
#include <thread>
#include <list>
#include "zmq.hpp"

void joinThreads(std::list<std::thread> threads);
zmq::socket_t initInprocSocket(zmq::context_t* ctx, std::string url, bool isConnect);