#pragma once
#include <regex>

#include "Interval.h"

namespace logic {
	struct Regex {
		const static std::regex INTERVALS_OPEN;
		const static std::regex INTERVALS_CLOSE;
		const static std::regex INTERVAL_OPEN;
		const static std::regex INTERVAL_CLOSE;
		const static std::regex HIGH;
		const static std::regex LOW;
	};

	void	readFromFile(char const * fileName);
	void	writeInFIle(char const * fileName);

	void	checkForRecursive(std::ifstream&, const std::string&);
	void	intervalsProcesser(std::ifstream&);
	void	intervalProcesser(std::ifstream&);

	void	initThread(int, int);

	void	countPrimes(Interval const *);
	int		getNextPrime(int from, const int to);
	bool	isPrime(int);
}