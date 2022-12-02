#include "ComuFrame.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "Node.h"
#include "Central.h"
#include "tinyxml2.h"
#include <thread>
using namespace std;
using namespace zmq;

//default settings
std::string url = "tcp://127.0.0.1";
std::string servicePort = ":1000";
std::string pubPort = ":1001";

int main()
{
	tinyxml2::XMLDocument configDoc;
	if (tinyxml2::XML_SUCCESS != configDoc.LoadFile("C:/Users/pagliani/source/repos/ComuFrame/Config.xml")) { std::cout << "can't load configration file, using default options"; }
	else {
		tinyxml2::XMLElement * urlElement = configDoc.RootElement()->FirstChildElement("url");
		std::cout << urlElement->FirstAttribute()->Value();
		servicePort = urlElement->FirstChildElement("servicePort")->FirstAttribute()->Value();
		std::cout << servicePort;
		pubPort = urlElement->FirstChildElement("pubPort")->FirstAttribute()->Value();
		std::cout << pubPort;
	}
	
	Central central(url+servicePort);
	std::thread centralThread(&Central::initCentral, &central);
	centralThread.detach();

	Node luca(url+servicePort, "luca");
	std::thread lucaThread(&Node::initNode, &luca);
	lucaThread.detach();

	Node fillo(url+servicePort, "Fillo");
	std::thread filloThread(&Node::initNode, &fillo);
	filloThread.detach();

	string exit;
	std::cin >> exit ;
	
	return 0;
}

