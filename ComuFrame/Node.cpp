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
	//init all internal comunication sockets
	socket_t InternalServiceSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", false);
	socket_t InternalTimerSocket = initInprocSocket(&(Node::ctx), "inproc://timerChannel", false);
	//init serviceThread
	std::thread serviceThread(&Node::initServiceThread, this);
  
	InternalServiceSocket.close();
	InternalTimerSocket.close();
	serviceThread.join();
	std::cout << identity << " MainThread, all threads joined\n";
	InternalServiceSocket.close();
	InternalTimerSocket.close();
}

void Node::initServiceThread() {
	socket_t serviceSocket(Node::ctx, ZMQ_DEALER);
	if (Node::identity != "") {
		serviceSocket.set(sockopt::routing_id, Node::identity);
	}
	serviceSocket.connect(url);
	std::cout << identity << ", serviceThread: Initialized and connected to " << Node::url <<"\n";

	message_t registrationMSG("RegistrationMSG", sizeof("RegistrationMSG"));
	if (true == serviceSocket.send(registrationMSG)) {
		std::cout << identity << ", ServiceThread: message sent \n";
	}
	else {
		//manage sending error
		std::cout << "something went wrong with sending";
	}

	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", true);
	//cleaning up

	//ricezione va spostata da qua
	multipart_t msgRecv;
	auto res1 = recv_multipart(serviceSocket, std::back_inserter(msgRecv));
	msgRecv.popstr();
	std::cout << identity << " serviceThread: messaggio ricevuto: " << msgRecv.popstr() << "\n";

}

//getter
std::string Node::getUrl() {
	return Node::url;
}

std::string Node::getidentity() {
	return Node::identity;
}

