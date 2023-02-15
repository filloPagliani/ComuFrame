#include "Node.h"

using namespace zmq;

Node::Node(std::string identity)
{
	this->identity = identity;
	try {
		YAML::Node config = YAML::LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config/" + identity+".yaml");
		this->url = config["url"].as<std::string>() + config["servicePort"].as<std::string>();
		this->sendingPackets = config["sendingPackets"];
		this->requestedData = config["requestedData"];
	}
	catch (std::exception e) {
		std::cout << "can't load configration file, using default options. Error : " << e.what() << "\n";
		this->url = "tcp://127.0.0.1:5555";
	}
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
	YAML::Emitter yEmitter;
	yEmitter << this->sendingPackets;
	sendStrMSG(&serviceSocket, yEmitter.c_str()); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config


	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", true);

	if (receiveStrMsgTimeout(&serviceSocket, 5000)) {
		state.nextState();
	}
	
	//TODO wrappa il while dentro al sendstrmsg
	multipart_t msgRecv;
	recv_multipart(serviceSocket, std::back_inserter(msgRecv));
	YAML::Emitter yEmitterrequested;
	yEmitterrequested << this->requestedData;
	sendStrMSG(&serviceSocket, yEmitterrequested.c_str());//TODO : scegli numero di tries che non sia a caso come ora e estrai da config
}

//fucntion used to receive a string with a timeout, if the msg is received within the timeout indicated will return true, and go ahead,it will keep trying TODO: inserisci limite di tries.
bool Node::receiveStrMsgTimeout(socket_t* sock, int timeout) {
	pollitem_t ackreceive[]{
		{*sock, 0, ZMQ_POLLIN, }
	};
	while (true) {
		poll(ackreceive, 1, timeout);//TODO : scegli timeout che non sia a caso come ora e estrai da config
		if (ackreceive[0].revents & ZMQ_POLLIN) {
			multipart_t msgRecv;
			recv_multipart(*sock, std::back_inserter(msgRecv));
			std::cout << identity << " serviceThread: message received: " << msgRecv.popstr() << "\n";
			return true;
		}
		else {
			std::cout << "timeout elapsed without the ack, sending registration again\n";
			YAML::Emitter yEmitter;
			yEmitter << this->sendingPackets;
			sendStrMSG(sock, yEmitter.c_str()); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
		}
	}
}


//function used to send a string msg through the socket, it will try to send it n times if it fails and if after n times keeps failing it will return false
bool Node::sendStrMSG(socket_t* sock, std::string msg) {
	multipart_t registrationMSG(state.toString());
	registrationMSG.addstr(msg);
	int tries = 0;
	while (!registrationMSG.send(*sock)) {
		tries++;
		if (tries > 4) { 
			std::cout << "cant send the message:" << msg; 
			return false;
		}
	}
	std::cout << this->identity << "serviceThread: message sent :\n" << msg<< "\n";
	return true;
}

//getter
std::string Node::getUrl() {
	return Node::url;
}

std::string Node::getidentity() {
	return Node::identity;
}

YAML::Node Node::getrequestedData() {
	return this->requestedData;
}

YAML::Node Node::getSendingPackets() {
	return this->sendingPackets;
}