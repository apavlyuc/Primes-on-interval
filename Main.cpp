#include "Storage.h"
#include "Logic.h"

int main(int ac, char** av) {
	if (ac != 2) return -1;

	// read data from xml-based file
	logic::readFromFile(av[1]);

	Storage* storage = Storage::getInstance();
	for (auto it = storage->getMap().begin(); it != storage->getMap().end(); ++it) {
		if (it->second.get()->joinable())
			it->second.get()->join();
		delete it->first;
	}
	
	// write primes at the end of the given file
	logic::writeInFIle(av[1]);
	return 0;
}