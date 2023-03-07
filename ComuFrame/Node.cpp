#include "Node.h"

using namespace zmq;

//Node constructor
Node::Node(std::string identity)
{
	this->identity = identity;
	try {
		YAML::Node config = YAML::LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config/" + identity+".yaml");
		this->url = config["url"].as<std::string>();
		this->pubPort = config["pubPort"].as<std::string>();
		this->servicePort = config["servicePort"].as<std::string>();
		this->sendingPackets = config["sendingPackets"];
		this->requestedData = config["requestedData"];
	}
	catch (std::exception e) {
		std::cout << "can't load configration file, using default options. Error : " << e.what() << "\n";
		this->url = "tcp://127.0.0.1";
		this->pubPort = "5556";
		this->servicePort = "5555";
	}
}

//Node destructor
Node::~Node()
{
	Node::ctx.close();
}

//Initialize the node and all his threads
void Node::initNode() {
	//init all internal comunication sockets
	socket_t InternalServiceSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", false);
	socket_t InternalTimerSocket = initInprocSocket(&(Node::ctx), "inproc://timerChannel", false);
	socket_t InternalPubSocket = initInprocSocket(&(Node::ctx), "inproc://pubChannel", false);
	socket_t InternalSubSocket = initInprocSocket(&(Node::ctx), "inproc://subChannel", false);

	//init Threads
	std::thread serviceThread(&Node::initServiceThread, this);
	std::thread pubThread(&Node::initPubThread, this);
	std::thread subThread(&Node::initSubThread, this);

	//polling for message on each socket
	while (true) {
		pollitem_t msgPoll[]{
			{InternalServiceSocket, 0 ,ZMQ_POLLIN, 0},
			{InternalTimerSocket, 0 ,ZMQ_POLLIN, 0},
			{InternalPubSocket, 0 ,ZMQ_POLLIN, 0},
			{InternalSubSocket, 0 ,ZMQ_POLLIN, 0}
		};
		poll(msgPoll, 4, -1);
		multipart_t msg;
		if (msgPoll[0].revents & ZMQ_POLLIN) {
			// handle message from service socket
			recv_multipart(InternalServiceSocket, std::back_inserter(msg));
			std::string state = msg.popstr();
			std::string data = msg.popstr();
			if (strcmp(state.c_str(), "syncronization")==0) {
				if (strcmp(data.c_str(), "received subscriptions")==0) {
					std::cout << this->identity << " MainThread : received subscriptions\n";
					sendStrMSG(&InternalSubSocket, "received subscriptions","MainThread","subThread");
				}
//				if (strcmp(data.c_str(), "AllSubscribed") == 0) {
//					sendStrMSG(&InternalPubSocket, "AllSubscribed", "MainThread", "pubThread");
//				}
				else {
					std::cout << this->identity << " MainThread, received message from serviceThread in syncrophase but dont know how to handle it\n";
				}
			}
		}
		else if (msgPoll[1].revents & ZMQ_POLLIN) {
			// handle message from Timer socket
		}
		else if (msgPoll[2].revents & ZMQ_POLLIN) {
			// handle message from pub socket
		}
		else if (msgPoll[3].revents & ZMQ_POLLIN) {
			// handle message from sub socket
			recv_multipart(InternalSubSocket, std::back_inserter(msg));
			std::string state = msg.popstr();
			std::string data = msg.popstr();
			if (strcmp(state.c_str(), "syncronization")==0) {
				if (strcmp(data.c_str(), "subscribed")==0) {
					while (!sendStrMSG(&InternalServiceSocket, "subscribed","MainThread","ServiceThread"));
				}
			}
		}
	}


	serviceThread.join();
	pubThread.join();
	subThread.join();
	InternalServiceSocket.close();
	InternalTimerSocket.close();
	InternalPubSocket.close();
	InternalSubSocket.close();
	eCAL::Finalize();
	std::cout << identity << " MainThread, all threads joined\n";
}

//Function called from initNode ti inizialize the service thread
void Node::initServiceThread() {
	socket_t serviceSocket(this->ctx, ZMQ_DEALER);
	if (Node::identity != "") {
		serviceSocket.set(sockopt::routing_id, Node::identity);
	}
	serviceSocket.connect(url+servicePort);
	std::cout << identity << ", serviceThread: Initialized and connected to " << this->url+this->servicePort <<"\n";
	YAML::Emitter yEmitter;
	yEmitter << this->sendingPackets;
	sendStrMSG(&serviceSocket, yEmitter.c_str(),"ServiceThread","Central"); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config


	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://serviceChannel", true);

	if (receiveStrMsgTimeout(&serviceSocket, 5000)) {
		state.nextState();
	}
	
	multipart_t msg;
	recv_multipart(serviceSocket, std::back_inserter(msg));
	YAML::Emitter yEmitterrequested;
	yEmitterrequested << this->requestedData;
	sendStrMSG(&serviceSocket, yEmitterrequested.c_str(),"ServiceThread","Central");//TODO : scegli numero di tries che non sia a caso come ora e estrai da config
	msg.clear();
	recv_multipart(serviceSocket, std::back_inserter(msg));
	while (!msg.empty()) {
		this->subscriptions.push_back(msg.popstr());
	}
	sendStrMSG(&toMainSocket, "received subscriptions","ServiceThread","MaainThread");

	while (true) {
		pollitem_t msgreceived[]{
			{toMainSocket, 0, ZMQ_POLLIN,0},
			{serviceSocket,0 , ZMQ_POLLIN,0}
		};
		poll(msgreceived, 2, 0);
		if (msgreceived[0].revents & ZMQ_POLLIN) {
			multipart_t msg;
			recv_multipart(toMainSocket, std::back_inserter(msg));
			std::string state = msg.popstr();
			std::string data = msg.popstr();
			sendStrMSG(&serviceSocket, data,"serviceThread", "MainThread");
		}
		else if(msgreceived[1].revents & ZMQ_POLLIN) {
			multipart_t msg;
			recv_multipart(serviceSocket, std::back_inserter(msg));
			std::string state = msg.popstr();
			std::string data = msg.popstr();
			if (strcmp(state.c_str(), "syncronization") == 0) {
				if (strcmp(data.c_str(), "AllSubscribed") == 0) {
					if (!sendStrMSG(&toMainSocket, "AllSubscribed","ServiceThread","MainThread")) {
						std::cout << this->identity << ", ServiceThread : error occoured trying to send a message through toMainSocket\n";
					};
				}
			}
		}
		else {
		}
	}
}

//fucntion used to receive a string ack with a timeout, if the msg is received within the timeout indicated will return true, and go ahead,it will keep trying TODO: inserisci limite di tries.
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
			sendStrMSG(sock, yEmitter.c_str(),"",""); //TODO : scegli numero di tries che non sia a caso come ora e estrai da config
		}
	}
}

//function used to send a string msg through the socket, it will try to send it n times if it fails and if after n times keeps failing it will return false
bool Node::sendStrMSG(socket_t* sock, std::string msg, std::string sendingThread, std::string addressee) {
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
	std::cout << this->identity << "serviceThread: message sent.\nfrom :" 
		<< sendingThread
		<< "\nto :" <<addressee
		<< "\ndata :" << msg << "\n\n";
	return true;
}

void Node::initPubThread() {
	socket_t pubSocket(this->ctx, ZMQ_PUB);
	std::cout << this->pubPort;
	pubSocket.bind(this->url + this->pubPort);
	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://pubChannel", true);
	multipart_t msg;
	recv_multipart(toMainSocket, std::back_inserter(msg));
	pubSocket.send(message_t("ciao a tutti"));
}

void Node::initSubThread() {
	socket_t subSocket(this->ctx, ZMQ_SUB);
	subSocket.connect(this->url + this->pubPort);
	socket_t toMainSocket = initInprocSocket(&(Node::ctx), "inproc://subChannel", true);
	multipart_t msg;
	recv_multipart(toMainSocket, std::back_inserter(msg));
	subSocket.setsockopt(ZMQ_SUBSCRIBE, "");
	for (std::string topic : this->subscriptions) {
		subSocket.setsockopt(ZMQ_SUBSCRIBE, &topic, sizeof(topic));
		std::cout << this->identity << ", SubThread : subscribed to " << topic << "\n";
	}
	if (!sendStrMSG(&toMainSocket, "subscribed","subThread","MainThread")) {
		std::cout << this->identity << " subThread : ERROR occoured while sending message to mainSocket\n";
	}
	message_t simplemsg;
	subSocket.recv(simplemsg);
	std::cout << "messaggio ricevuto " << simplemsg.to_string();
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