#include "utils.h"

void joinThreads( std::list<std::thread> threads) {
	std::list<std::thread>::iterator i;
	for (i = threads.begin(); i != threads.end(); i++) {
		std::cout << "ciao " << (*i).get_id();
		//(*i).join();
	}
}