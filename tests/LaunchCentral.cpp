#include "../ComuFrame/Central.h"

int main() {
	Central central;
	central.initCentral();
	std::cout << "waiting fo a key to close";
	std::string exit;
	std::cin >> exit;
	return 0;
}