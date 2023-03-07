#include "Central.h"

using namespace zmq;
	
	//constructor of Central, create the node and take the config from the file
	Central::Central()
	{
		try {
			YAML::Node config = YAML::LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config/Central.yaml");
			this->url = config["url"].as<std::string>() + config["servicePort"].as<std::string>();
			this->expectedClient = config["expectedClient"].as<int>();
		}
		catch (std::exception e) {
			std::cout << "can't load configration file, using default options. Error : " << e.what();
			this->url = "tcp://127.0.0.1:5555";
		}
	}

	Central::~Central()
	{
		Central::ctx.close();
	}

	//inizialize the central node with all his thread
	void Central::initCentral() {

		//init service thread
		std::thread serviceThread(&Central::initServiceThread, this);

		//init all inproc comunication sockets
		socket_t InternalServiceSocket = initInprocSocket(&(Central::ctx), "inproc://serviceChannel", false);
		socket_t InternalTimerSocket = initInprocSocket(&(Central::ctx), "inproc://timerCannel", false);

		message_t msg;
		InternalServiceSocket.recv(msg);
		while (strcmp(msg.to_string().c_str(), "syncronization Completed")) {
			InternalServiceSocket.recv(msg);
		}
		std::thread timerThread(&Central::initTimerThread, this);
		auto time = std::chrono::high_resolution_clock::now();
		while (true) {
			pollitem_t pollFromThreads[]{
					{InternalServiceSocket, 0, ZMQ_POLLIN, 0},
					{InternalTimerSocket, 0, ZMQ_POLLIN, 0}
			};
			poll(pollFromThreads, 2, -1);
			if (pollFromThreads[0].revents & ZMQ_POLLIN) {
				//da fare
			}
			if (pollFromThreads[1].revents & ZMQ_POLLIN) {
				InternalTimerSocket.recv(msg);
			}
		}
		//wait for all the secondary threads to join
		serviceThread.join();
		timerThread.join();
		std::cout << "Central, Main: all threads joined\n";
		InternalServiceSocket.close();
		InternalTimerSocket.close();
	}

	//initialize the service thread
	 void Central::initServiceThread() {
		
		socket_t serviceSocket(Central::ctx, ZMQ_ROUTER);
		serviceSocket.set(sockopt::routing_id, "Central");
		serviceSocket.bind(url);
		socket_t toMainSocket = initInprocSocket(&(Central::ctx), "inproc://serviceChannel", true);
		Central::connectedClients = Central::registerClient(&serviceSocket);
		Central::state.nextState();
		std::cout << "Central, ServiceThread: Registration completed, registered " << connectedClients.size() << " nodes\n";
		for (std::string node : this->connectedClients) {
			sendStrMSG(&serviceSocket, state.toString(),node);
		}
		std::unordered_map<std::string, std::vector<std::string>> requestMap;

		while (true) {
			pollitem_t receivingDataRequest[]{
				{serviceSocket, 0, ZMQ_POLLIN, 0}
			};
			poll(receivingDataRequest, 1, 10000);//TODO : scegli numero di tries che non sia a caso come ora e estrai da config
			if (receivingDataRequest[0].revents & ZMQ_POLLIN) {
				multipart_t request;
				recv_multipart(serviceSocket, std::back_inserter(request));
				std::string nodeName = request.popstr();
				if ((request.popstr() == state.toString())&&(requestMap.find(nodeName) == requestMap.end()))   //da qua esco se il request map non ha il nome o se il messaggio ricevuto non è in stato syncro, probabilmente vanno divisi i due casi per reagire in modo diverso
				{
					std::vector<std::string> dataRequested;
					YAML::Node YDataRequested = YAML::Load(request.popstr());
					for(auto it = YDataRequested.begin(); it != YDataRequested.end();it++){
						dataRequested.push_back(it->first.as<std::string>() + "_" + it->second.as<std::string>());
					}
					requestMap[nodeName] = dataRequested;
					if (requestMap.size() == connectedClients.size()) {
						break;
					}
				}
				else {
					std::cout <<"discarded "<< request.popstr()<< "\n";
					continue;//si potrebbe rimandare e fare in modo che di la si esce solo con ack attento qua da valutare entrambi i casi che ti portano qua
				}
			}
			else {
				std::cout << "timeout elapsed without the ack, sending registration again\n";//ricordati che qua bisona mandare solo a chi manca
			}
		}
		
		for (auto& request : requestMap) {
			multipart_t msg(request.first);
			std::cout << request.first << " needs : ";
			std::vector<std::string> packetNeeded = db.providePackets(request.second);
			for (auto& str : packetNeeded) {
				msg.append(str);
				std::cout << str << " ";
			}
			std::cout << "\n";
			msg.send(serviceSocket);
		}

		toMainSocket.send(message_t("syncronization Completed"));
		std::vector<std::string> toCheckSubscriptions = this->connectedClients;
		while (true) {
			pollitem_t msgreceived[]{
				{toMainSocket, 0, ZMQ_POLLIN,0},
				{serviceSocket,0 , ZMQ_POLLIN,0}
			};
			poll(msgreceived, 2, -1);
			if (msgreceived[0].revents & ZMQ_POLLIN) {
				//handle messages from Main Socket

			}
			else if (msgreceived[1].revents & ZMQ_POLLIN) {
				//handle messages from Service socket
				multipart_t msg;
				recv_multipart(serviceSocket, std::back_inserter(msg));
				std::string sender = msg.popstr();
				std::string state = msg.popstr();
				std::string data = msg.popstr();
				std::cout << "Central serviceThread : message received from : " << sender << " during " << state << " with data : " << data << "\n";
				if (strcmp(state.c_str(), "syncronization")==0) {
					bool nodeFound = false;
					for (auto it = toCheckSubscriptions.begin(); it != toCheckSubscriptions.end(); it++) {
						if (strcmp(sender.c_str(), (*it).c_str())==0) {
							nodeFound = true;
							toCheckSubscriptions.erase(it);
							if (toCheckSubscriptions.size() == 0) {
								for (int i = 0; i < this->connectedClients.size(); i++) {
									if (!sendStrMSG(&serviceSocket, "AllSubscribed", this->connectedClients[i])) {
										std::cout << "Central, ServiceThread: error occoured trying to send a message to " << this->connectedClients[i]<<"\n";
									}
								}
								break;
							}
							else
							{
								break;
							}
						}
					}
					if (!nodeFound) {
						std::cout << sender << " has already been signed as subscribed or has never connected\n"; //altro errore da hanlde
					}
				}
			}
		}
	 }

	 //inizialize the timer thread
	 void Central::initTimerThread() {
		 socket_t toMain = initInprocSocket(&(Central::ctx), "inproc://timerCannel", true);
		 timeBeginPeriod(1);
		 for (int i = 0; i < 10; i++) {
			 auto start = std::chrono::high_resolution_clock::now();
			 toMain.send(message_t("message"));
			 std::this_thread::sleep_for(std::chrono::milliseconds(8) - std::chrono::milliseconds(2) - (start- std::chrono::high_resolution_clock::now()));
			 while ((std::chrono::high_resolution_clock::now() - start) < std::chrono::milliseconds(8)) {
				 continue;
			 }
			 auto end = std::chrono::high_resolution_clock::now();


		 }
		 timeEndPeriod(1);
	 }

	 //when its called register the nodes that have done a request at the given socket
	 std::vector<std::string> Central::registerClient(socket_t* sock) {
		 std::vector<std::string> clients;
		 multipart_t registrationMSG;
		 while (clients.size() < expectedClient) {    //prima di inizare la fase dovra esere trovato un modo per definire quanti node ci si aspetta il due è provvisiorio e per testing

			 if (registrationMSG.recv(*sock))
			 {
				 clients.push_back(registrationMSG.popstr());
				 if (std::count(clients.begin(), clients.end(), clients.back()) > 1) {
					 registrationMSG.clear();
					 registrationMSG.pushstr("connected");
					 registrationMSG.pushstr(clients.back());
					 std::cout << "node " << clients.back() << "is already registered, sending confirmations again";
					 clients.pop_back();
				 }
				 else if (registrationMSG.popstr() == state.toString()) {
					 YAML::Node YData = YAML::Load(registrationMSG.popstr());
					 for (YAML::const_iterator i = YData.begin(); i != YData.end(); i++) {
						 db.addPacket(i->second, clients.back(), i->first.as<std::string>());
					 }
					 registrationMSG.clear();
					 registrationMSG.pushstr("connected");
					 registrationMSG.pushstr(clients.back());
					 std::cout << "Central, serviceThread: Registration request from " << clients.back() << ", registration completed.\n";

				 }
				 else { //send back to the sender the message with the currstate TODO handle the reception of that error msg
					 registrationMSG.clear();
					 registrationMSG.pushstr(state.toString());
					 registrationMSG.pushstr(clients.back());
					 clients.pop_back();
				 }
				 send_multipart(*sock, registrationMSG);
				 registrationMSG.clear();

			 }
			 else {
				 //handle error message not received
				 std::cout << "Central, serviceThread: Registration received wasn't received correctly";
			 }
		 }
		 //printing the new db
		 std::unordered_map<std::string, bool> availablPack = db.getAvaiablePacks();
		 std::cout << "new DB: \n";
		 for (auto it = availablPack.begin(); it != availablPack.end(); it++) {
			 std::cout << it->first << " : " << it->second << "\n";
		 }
		 DataBase::dbMap index = db.getIndex();
		 std::cout << "\nindex: \n";
		 for (auto it = index.begin(); it != index.end(); it++) {
			 std::cout << it->first << " : ";
			 for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				 std::cout << it2->first << " " << *(it2->second) << " || ";
			 }
			 std::cout << "\n";
		 }
		 return clients;
	 }

	 //function used to send a string msg through the socket, it will try to send it n times if it fails and if after n times keeps failing it will return false
	 bool Central::sendStrMSG(socket_t* sock, std::string msg, std::string addressee) {
		 multipart_t registrationMSG(state.toString());
		 registrationMSG.addstr(msg);
		 registrationMSG.pushstr(addressee);
		 int tries = 0;
		 while (!registrationMSG.send(*sock)) {
			 tries++;
			 if (tries > 4) {
				 std::cout << "cant send the message:" << msg;
				 return false;
			 }
		 }
		 std::cout << "Central, ServiceThread: message sent to " << addressee << " with message " << msg << "\n";
		 return true;
	 }

	 //Getter
	 std::string Central::getUrl() {
		 return this->url;
	 }

	 int Central::getExpectedClient() {
		 return this->expectedClient;
	 }

	 void Central::preciseSleep(std::chrono::milliseconds millisecond, std::chrono::milliseconds higResTime) {
		 auto start = std::chrono::high_resolution_clock::now();
		 std::this_thread::sleep_for(millisecond - higResTime);
		 auto mid = std::chrono::high_resolution_clock::now();
		 while ((std::chrono::high_resolution_clock::now() - start) < millisecond) {
		 continue;
		}
		 auto end = std::chrono::high_resolution_clock::now();
		 std::cout << "sleep duration " << (std::chrono::duration_cast<std::chrono::microseconds>(mid - start)).count() << " total duration " << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << "\n";
	 }

	