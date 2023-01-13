#include "../ComuFrame/Node.h"

int main() {
	std::string url = "tcp://127.0.0.1";
	std::string servicePort = ":1000";
	std::string pubPort = ":1001";
	Node node1(url + servicePort, "node1");
	node1.initNode();

	std::cout << "waiting fo a key to close";
	std::string exit;
	std::cin >> exit;
	return 0;
}