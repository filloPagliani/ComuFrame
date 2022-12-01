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
		std::thread registerThread(&Central::initRegisterThread, this);
		registerThread.join();
		std::cout << "Central, Main: join eseguito\n";
	}

	 void Central::initRegisterThread() {
		 std::cout << "Central, RegisterThread: appena partito\n";
		socket_t registerSocket(Central::ctx, ZMQ_ROUTER);
		registerSocket.bind(url);
		std::cout << "Central, RegisterThread: bindato a " << url << " e pronto a ricevere mex \n";
		Central::registerCliet(&registerSocket);
		
		//cleaning up
		registerSocket.close();
	 }

	 std::vector<std::string> Central::registerCliet(socket_t * sock) {
		 std::vector<std::string> clients;
		 multipart_t regMsg;
		 while (true) {		 

			 auto res = recv_multipart( *sock, std::back_inserter(regMsg));
			 clients.push_back(regMsg.popstr());
			 std::cout << "Central, RegisterThread: Messaggio ricevuto: " << clients.back();
			 regMsg.pop();
			 std::cout << " : " << regMsg.popstr() << "\n";
		 }
		 return clients;
	 }

	 //Getter
	 std::string Central::getUrl() {
		 return Central::url;
	 }


	 //TODO funzione per chiudere e pulire tutto quindi tenendo traccia di thread, socket e ctx poi chiudere tutto sia qua che in node