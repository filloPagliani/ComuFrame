#include "Node.h"

using namespace zmq;

Node::Node(std::string url, std::string identity)
{
	this->url = url;
	this->identity = identity;
}

void Node::initNode() {
	std::thread registerThread(&Node::initRegisterThread, this);
	registerThread.join();
	std::cout << identity << ", Main: join eseguito\n";
}

void Node::initRegisterThread() {
	std::cout <<identity << ", RegisterThread: appena partito \n";
	socket_t registerSocket(Node::ctx, ZMQ_REQ);
	if (Node::identity != "") {
		registerSocket.setsockopt(ZMQ_IDENTITY, Node::identity.data(), 4);
	}

	registerSocket.connect(url);
	std::cout << identity << ", RegisterThread: connesso a " << Node::url <<" e sto per inviare\n";
	message_t msg("ciaone", sizeof("ciaone"));
	registerSocket.send(msg);
	std::cout << identity << ", RegisterThread: messaggio inviato\n";
	//cleaning up
	registerSocket.close();
}

//getter
std::string Node::getUrl() {
	return Node::url;
}

std::string Node::getidentity() {
	return Node::identity;
}