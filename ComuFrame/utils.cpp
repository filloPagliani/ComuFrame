#include "utils.h"


zmq::socket_t initInprocSocket(zmq::context_t* ctx, std::string identity, std::string url, bool isConnect) {
	zmq::socket_t sock(*ctx, ZMQ_PAIR);
	sock.set(zmq::sockopt::routing_id, identity);
	if (isConnect) {
		sock.connect(url);
	}
	else {
		sock.bind(url);
	}
	return (sock);
}

void joinThreads( std::list<std::thread> threads) {
	std::list<std::thread>::iterator i;
	for (i = threads.begin(); i != threads.end(); i++) {
		std::cout << "ciao " << (*i).get_id();
		//(*i).join();
	}
}