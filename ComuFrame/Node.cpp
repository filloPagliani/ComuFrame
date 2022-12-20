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
	//init serviceThread
	std::thread serviceThread(&Node::initServiceThread, this);

	//init all comunication sockets
	socket_t inprocServiceSocket = initInprocSocket(&(Node::ctx), "main", "inproc://serviceChannel", false);

	//wait for all the secondary thread
	serviceThread.join();
	std::cout << identity << " MainThread, all threads joined\n";
}

void Node::initServiceThread() {
	socket_t serviceSocket(Node::ctx, ZMQ_DEALER);
	if (Node::identity != "") {
		serviceSocket.set(sockopt::routing_id, Node::identity);
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

	socket_t inprocServiceSocket = initInprocSocket(&(Node::ctx), "service", "inproc://serviceChannel", true);
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

