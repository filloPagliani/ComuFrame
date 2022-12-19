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
		std::thread serviceThread(&Central::initServiceThread, this);
		serviceThread.join();
		std::cout << "Central, Main: serviceThread joined\n";
	}

	 void Central::initServiceThread() {

		socket_t serviceSocket(Central::ctx, ZMQ_ROUTER);
		serviceSocket.setsockopt(ZMQ_IDENTITY, "Central",sizeof("central"));
		serviceSocket.bind(url);
		std::cout << "Central, ServiceThread: initialized and binded to " << url << "\n";

		Central::connectedClients = Central::registerClient(&serviceSocket);
		std::cout << "Central, ServiceThread: Registration completed, registered " << connectedClients.size() << " nodes\n";

		for (int i = 0; i < connectedClients.size(); i++) {
			multipart_t msgToSend("rispostona", sizeof("rispostona"));
			msgToSend.pushstr("");
			msgToSend.pushstr(connectedClients[i]);
			send_multipart(serviceSocket, msgToSend);
			std::cout << "Central, ServiceThread: messaggio mandato a " << connectedClients[i] << "\n";
		}
		serviceSocket.close();
	 }

	 std::vector<std::string> Central::registerClient(socket_t * sock) {
		 std::vector<std::string> clients;
		 multipart_t registrationMSG;
		 while (clients.size() < 2) {    //prima di inizare la fase dovra esere trovato un modo per definire quanti node ci si aspetta il due � provvisiorio e per testing

			 auto res = recv_multipart( *sock, std::back_inserter(registrationMSG));
			 clients.push_back(registrationMSG.popstr());
			 std::cout << "Central, ServiceThread: message received from " << clients.back();
			 registrationMSG.pop();		//popping out the frame empty message
			 std::cout << " : " << registrationMSG.popstr() << "\n";
			 registrationMSG.clear();
		 }
		 return clients;
	 }

	 //Getter
	 std::string Central::getUrl() {
		 return Central::url;
	 }


	