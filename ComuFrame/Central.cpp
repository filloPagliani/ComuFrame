#include "Central.h"

using namespace zmq;
	
	Central::Central()
	{
		tinyxml2::XMLDocument configDoc;
		if (tinyxml2::XML_SUCCESS != configDoc.LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config.xml")) {
			std::cout << "can't load configration file, using default options";
			this->url = "tcp://127.0.0.1:5555";
		}
		else {
			tinyxml2::XMLElement* urlElement = configDoc.RootElement()->FirstChildElement("url");
			this->url = urlElement->FirstAttribute()->Value();
			this->url += urlElement->FirstChildElement("servicePort")->FirstAttribute()->Value();
			//pubPort = urlElement->FirstChildElement("pubPort")->FirstAttribute()->Value();
		}
	}

	Central::~Central()
	{
		Central::ctx.close();
	}

	void Central::initCentral() {
		//init service thread
		std::cout << "How many node are you expecting? \n";
		std::cin >> Central::expectedClient;
		while(!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "insert a number greater than 0 please \n";
			std::cin >> Central::expectedClient;
		}
		std::thread serviceThread(&Central::initServiceThread, this);

		//init all inproc comunication sockets
		socket_t InternalServiceSocket = initInprocSocket(&(Central::ctx), "inproc://serviceChannel", false);
		socket_t InternalTimerSocket = initInprocSocket(&(Central::ctx), "inproc://timerCannel", false);

		//wait for all the secondary threads to join
		serviceThread.join();
		std::cout << "Central, Main: all threads joined\n";
		InternalServiceSocket.close();
		InternalTimerSocket.close();
	}

	 void Central::initServiceThread() {
		
		socket_t serviceSocket(Central::ctx, ZMQ_ROUTER);
		serviceSocket.set(sockopt::routing_id, "Central");
		serviceSocket.bind(url);
		Central::connectedClients = Central::registerClient(&serviceSocket);
		Central::state.nextState();
		std::cout << "Central, ServiceThread: Registration completed, registered " << connectedClients.size() << " nodes\n";
		for (std::string node : this->connectedClients) {
			multipart_t syncroMSG;
			syncroMSG.pushstr(state.toString());
			syncroMSG.pushstr(node);
		}
		socket_t toMainSocket = initInprocSocket(&(Central::ctx), "inproc://serviceChannel", true);
	 }

	 std::vector<std::string> Central::registerClient(socket_t * sock) {
		 std::vector<std::string> clients;
		 multipart_t registrationMSG;
		 while (clients.size() < expectedClient) {    //prima di inizare la fase dovra esere trovato un modo per definire quanti node ci si aspetta il due è provvisiorio e per testing

			 if(registrationMSG.recv(*sock))
			 {
				 clients.push_back(registrationMSG.popstr());
				 if (std::count(clients.begin(), clients.end(), clients.back()) > 1) {
					 registrationMSG.clear();
					 registrationMSG.pushstr("connected");
					 registrationMSG.pushstr(clients.back());
					 std::cout << "node " << clients.back() << "is already registered, trying to send confirmations again";
					 clients.pop_back();
				 }
				 if (registrationMSG.popstr() == state.toString()) {
					 this->dataNodes.push_back(DataNode(clients.back()));
					 jsoncons::json jData = jsoncons::json::parse(registrationMSG.popstr());
					 for (auto i = jData["Packets"].begin_elements(); i < jData["Packets"].end_elements(); i++) {
						 i->insert_or_assign("nameID", this->dataNodes.back().getSender() + "_" + i->at("nameID").as_string());
						 this->dataNodes[dataNodes.size() - 1].addPacket(*i);
					 }
					 registrationMSG.clear();
					 registrationMSG.pushstr("connected");
					 registrationMSG.pushstr(clients.back());
					 std::cout << "Central, serviceThread: Registration request from " << clients.back() << ", registration completed. with sending info:\n";

				 }
				 else { //send back to the sender the message with the currstate TODO handle the reception of that error msg
					 registrationMSG.clear();
					 registrationMSG.pushstr(state.toString());
					 registrationMSG.pushstr(clients.back());
					 clients.pop_back();
				 }
				 send_multipart(*sock, registrationMSG);
				 registrationMSG.clear();
				 for (const auto packs : this->dataNodes[dataNodes.size() - 1].getAllPacket()) {
					 std::cout << packs.at("nameID") << "\n";
				 }
			 }
			 else {
				 //handle error message not received
				 std::cout << "Central, serviceThread: Registration received wasn't received correctly";
			 }
		 }
		 return clients;
	 }


	 //Getter
	 std::string Central::getUrl() {
		 return Central::url;
	 }

	 int Central::getExpectedClient() {
		 return Central::expectedClient;
	 }

	 std::vector<DataNode> Central::getDataNodes()
	 {
		 return this->dataNodes;
	 }
	