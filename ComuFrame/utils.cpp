#include "utils.h"


zmq::socket_t initInprocSocket(zmq::context_t* ctx, std::string url, bool isConnect) {
	zmq::socket_t sock(*ctx, ZMQ_PAIR);
	if (isConnect) {
		sock.connect(url);
	}
	else {
		sock.bind(url);
	}
	return (sock);
}

