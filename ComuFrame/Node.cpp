#include "Node.h"

using namespace zmq;

Node::Node(std::string identity)
{
	this->identity = identity;
	tinyxml2::XMLDocument configDoc;
	if (tinyxml2::XML_SUCCESS != configDoc.LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config.xml")) {
		std::cout << "can't load configration file, using default options";
		this->url = "tcp://127.0.0.1:5555";
	}
	else {
		tinyxml2::XMLElement* urlElement = configDoc.RootElement()->FirstChildElement("url");
		this->url = urlElement->FirstAttribute()->Value();
		this->url += urlElement->FirstChildElement("servicePort")->FirstAttribute()->Value();
		try {
			tinyxml2::XMLElement* nodeElement = configDoc.RootElement()->FirstChildElement("nodes")->FirstChildElement(this->identity.c_str());
			nodeElement = nodeElement->FirstChildElement("data");
			//TODO sistema le eccezzioni es: se non c'è l'array, se non ci sono i nomi dei pack ecc
			this->sendingData = jsoncons::json::parse(nodeElement->FirstChildElement("sendingData")->GetText());
			this->requestedData = jsoncons::json::parse(nodeElement->FirstChildElement("requestedData")->GetText());
		}
		catch (std::exception e) {
			std::cout << "cannot find the configuration of " << this->identity << " inside the configuration file. exception: " << e.what()<<"\n";
		}
		//pubPort = urlElement->FirstChildElement("pubPort")->FirstAttribute()->Value();
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
	sendStrMSG(&serviceSocket, this->sendingData.as_string()); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config


	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", true);

	if (receiveStrMsgTimeout(&serviceSocket, 5000)) {
		state.nextState();
	}
	
	//TODO wrappa il while dentro al sendstrmsg
	multipart_t msgRecv;
	recv_multipart(serviceSocket, std::back_inserter(msgRecv));
	sendStrMSG(&serviceSocket, this->requestedData.as_string());//TODO : scegli numero di tries che non sia a caso come ora e estrai da config
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
			sendStrMSG(sock, this->sendingData.as_string()); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
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

jsoncons::json Node::getrequestedDataa() {
	return this->requestedData;
}

jsoncons::json Node::getSendingData() {
	return this->sendingData;
}