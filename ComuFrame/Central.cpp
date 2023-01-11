#include "Central.h"

using namespace zmq;
	
	Central::Central(std::string url)
	{
		this->url = url;
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
		std::cout << "Central, ServiceThread: Registration completed, registered " << connectedClients.size() << " nodes\n";

		socket_t toMainSocket = initInprocSocket(&(Central::ctx), "inproc://serviceChannel", true);
	 }

	 std::vector<std::string> Central::registerClient(socket_t * sock) {
		 std::vector<std::string> clients;
		 multipart_t registrationMSG;
		 while (clients.size() < expectedClient) {    //prima di inizare la fase dovra esere trovato un modo per definire quanti node ci si aspetta il due è provvisiorio e per testing

			 auto res = recv_multipart( *sock, std::back_inserter(registrationMSG));
			 clients.push_back(registrationMSG.popstr());
			 registrationMSG.clear();
			 registrationMSG.pushstr("connected");
			 registrationMSG.pushstr("");
			 registrationMSG.pushstr(clients.back());
			 send_multipart(*sock, registrationMSG);
			 registrationMSG.clear();
			 std::cout << "Central, serviceThread: Registration request from " << clients.back() << ", registration completed. \n";
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


	