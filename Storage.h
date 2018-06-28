#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <map>

class Interval;

class Storage {
public:
	static Storage*													getInstance();

	void															addToVector(int);
	void															addToMap(Interval const *, std::unique_ptr<std::thread>);
	const std::vector<int>&											getVector() const;
	const std::map<Interval const *, std::unique_ptr<std::thread>>&	getMap() const;

private:
	static std::atomic<Storage*> instance;
	static std::mutex block;

	std::vector<int> primes;
	std::map<Interval const *, std::unique_ptr<std::thread>> threads;

	Storage() = default;
	Storage(const Storage&) = delete;
	Storage(const Storage&&) = delete;
	~Storage() = default; // there is only one instance, and this instance must live to the end of the progam, so this method private

	Storage& operator=(const Storage&) = delete;
};