#include "../ComuFrame/Node.h"

int main(int argc, char ** argv) {
	Node node1("nodeTest1");
	Node node2("nodeTest2");
	std::thread node2Thread(&Node::initNode,&node2);
	node2Thread.detach();
	node1.initNode();

	std::cout << "waiting fo a key to close";
	std::string exit;
	std::cin >> exit;
	return 0;
}