#include "Central.h"

using namespace zmq;
	
	Central::Central()
	{
		try {
			YAML::Node config = YAML::LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config/Central.yaml");
			this->url = config["url"].as<std::string>() + config["servicePort"].as<std::string>();
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
					std::cout << "messaggo arrivato cosi " << YDataRequested << "\n";
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
			std::cout << request.first << " : ";
			std::vector<std::string> strings = db.providePackets(request.second);
			for (auto& str : strings) {
				std::cout << str << " ";
			}
			std::cout << "\n";
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
					 std::cout << "node " << clients.back() << "is already registered, sending confirmations again";
					 clients.pop_back();
				 }
				 else if (registrationMSG.popstr() == state.toString()) {
					 YAML::Node YData = YAML::Load(registrationMSG.popstr());
					 for (YAML::const_iterator i = YData.begin(); i != YData.end(); i++) {
						 db.addPacket(i->second,clients.back(),i->first.as<std::string>());
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

	