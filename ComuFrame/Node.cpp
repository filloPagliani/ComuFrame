#include "Node.h"

using namespace zmq;

Node::Node(std::string url, std::string identity)
{
	this->url = url;
	this->identity = identity;
}

Node::~Node()
{
	Node::ctx.close();
}

void Node::initNode() {
	std::thread serviceThread(&Node::initServiceThread, this);
	serviceThread.join();
	std::cout << identity << ", Main: serviceThread joined\n";
}

void Node::initServiceThread() {
	socket_t serviceSocket(Node::ctx, ZMQ_DEALER);
	if (Node::identity != "") {
		serviceSocket.setsockopt(ZMQ_IDENTITY, Node::identity.data(), sizeof(Node::identity.data()));
	}
	serviceSocket.connect(url);
	std::cout << identity << ", serviceThread: Initialized and connected to " << Node::url <<"\n";

	multipart_t registrationMSG("RegistrationMSG", sizeof("RegistrationMSG"));
	registrationMSG.pushstr("");
	if (registrationMSG.size() == send_multipart(serviceSocket, registrationMSG)) {
		std::cout << identity << ", ServiceThread: message sent \n";
	}
	else {//manage sending error
		std::cout << "something went wrong with sending";
	}

	//cleaning up
	
	//ricezione va spostata da qua
	multipart_t msgRecv;
	auto res1 = recv_multipart(serviceSocket, std::back_inserter(msgRecv));
	msgRecv.popstr();
	std::cout << identity << " serviceThread: connesso a " << msgRecv.popstr() << "\n";
	serviceSocket.close();
}

//getter
std::string Node::getUrl() {
	return Node::url;
}

std::string Node::getidentity() {
	return Node::identity;
}

