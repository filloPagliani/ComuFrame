#include "ComuFrame.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "Node.h"
#include "Central.h"
#include <thread>

using namespace std;
using namespace zmq;

const std::string url = "tcp://127.0.0.1";
const std::string servicePort = ":5555";

int main()
{
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

