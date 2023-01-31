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
			std::cout << "cannot find the configuration of " << this->identity << " inside the configuration file. exception: " << e.what();
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
	int tries = 0;
	while (!sendStrMSG(&serviceSocket,this->sendingData.as_string())) //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
	{
		tries++;
		sendStrMSG(&serviceSocket, this->sendingData.as_string());
		if (tries > 4) { std::cout << "cant send registration message"; }
	}

	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", true);


	pollitem_t ackreceive[]{
		{serviceSocket, 0, ZMQ_POLLIN, }
	};
	while (true) {
		message_t ack;
		poll(ackreceive, 1, 5000);//TODO : scegli numero di tries che non sia a caso come ora e estrai da config
		if (ackreceive[0].revents & ZMQ_POLLIN) {
			multipart_t msgRecv;
			recv_multipart(serviceSocket, std::back_inserter(msgRecv));
			std::cout << identity << " serviceThread: messaggio ricevuto: " << msgRecv.popstr() << "\n";
			state.nextState();
			break;
		}
		else {
			std::cout << "timeout elapsed without the ack, sending registration again";
			while (!sendStrMSG(&serviceSocket, this->sendingData.as_string())) //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
			{
				tries++;
				sendStrMSG(&serviceSocket, this->sendingData.as_string());
				if (tries > 4) { std::cout << "cant send registration message"; }
			}
		}
	}
	//TODO wrappa il while dentro al sendstrmsg
	multipart_t msgRecv;
	recv_multipart(serviceSocket, std::back_inserter(msgRecv));
	while (!sendStrMSG(&serviceSocket, this->requestedData.as_string())) //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
	{
		tries++;
		sendStrMSG(&serviceSocket, this->requestedData.as_string());
		if (tries > 4) { std::cout << "cant send syncronization message"; }
	}
}

bool Node::sendStrMSG(socket_t* sock, std::string msg) {
	multipart_t registrationMSG(state.toString());
	registrationMSG.addstr(msg);
	if (registrationMSG.send(*sock)) {
		std::cout << this->identity << ", ServiceThread: registration message sent";
		return true;
	}
	else {
		std::cout << this->identity << ", ServiceThread: failed trying to send registration message";
		return false;
	}
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