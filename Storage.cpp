#include "Storage.h"

std::atomic<Storage*> Storage::instance = nullptr;
std::mutex Storage::block;

Storage* Storage::getInstance() {
	Storage* storage = instance.load(std::memory_order_acquire);

	if (!storage) {
		std::lock_guard<std::mutex> lock(block);
		storage = instance.load(std::memory_order_relaxed);
		if (!storage) {
			storage = new Storage();
			instance.store(storage, std::memory_order_release);
		}
	}
	return storage;
}

void	Storage::addToVector(int prime) {
	std::lock_guard<std::mutex> lock(block);
	primes.push_back(prime);
}

const std::vector<int>& Storage::getVector() const {
	std::lock_guard<std::mutex> lock(block);
	return primes;
}

void	Storage::addToMap(Interval const * interval, std::unique_ptr<std::thread> thread) {
	std::lock_guard<std::mutex> lock(block);
	threads.emplace(interval, std::move(thread));
}

const std::map<Interval const  *, std::unique_ptr<std::thread>>& Storage::getMap() const {
	std::lock_guard<std::mutex> lock(block);
	return threads;
}