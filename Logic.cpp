#include <fstream>
#include <string>

#include "Storage.h"
#include "Logic.h"

using std::regex;

const regex logic::Regex::INTERVALS_OPEN(R"((\s*)<intervals>(\s*))");
const regex logic::Regex::INTERVALS_CLOSE(R"((\s*)<intervals>(\s*))");
const regex logic::Regex::INTERVAL_OPEN(R"(\s*<interval>\s*)");
const regex logic::Regex::INTERVAL_CLOSE(R"(\s*</interval>\s*)");
const regex logic::Regex::HIGH(R"(<high>(\s*\d+\s*)</high>)");
const regex logic::Regex::LOW(R"(<low>(\s*\d+\s*)</low>)");

void logic::countPrimes(Interval const * interval) {
	Storage *storage = Storage::getInstance();

	int start = interval->get_low();
	int end = interval->get_high();

	for (; start <= end; ++start) {
		start = logic::getNextPrime(start, end);
		if (!start)
			return;
		storage->addToVector(start);
	}
}

void logic::checkForRecursive(std::ifstream& in, const std::string& line) {
	if (std::regex_match(line, logic::Regex::INTERVALS_OPEN))
		logic::intervalsProcesser(in);

	if (std::regex_match(line, logic::Regex::INTERVAL_OPEN))
		logic::intervalProcesser(in);
}

// need EOF check
void logic::intervalsProcesser(std::ifstream& in) {
	
	std::string line;
	while (getline(in, line)) { // need eof check
		if (std::regex_match(line, logic::Regex::INTERVALS_CLOSE)) break;

		logic::checkForRecursive(in, line);

		if (std::regex_match(line, logic::Regex::INTERVAL_OPEN))
			logic::intervalProcesser(in);
	}
}

void logic::initThread(int low, int high) {
	if (low < 0 || high < low)
		return;

	Storage* storage = Storage::getInstance();
	for (auto it = storage->getMap().begin(); it != storage->getMap().end(); ++it) {
		if (low > it->first->get_high() || high < it->first->get_low())
			continue;

		if (low >= it->first->get_low()) {
			if (high <= it->first->get_high())
				return;
			else
				low = it->first->get_high() + 1;
		}
		else {
			if (high >= it->first->get_high()) {
				logic::initThread(low, it->first->get_low() - 1);
				logic::initThread(it->first->get_high() + 1, high);
				return;
			}
			else if (high >= it->first->get_low())
				high = it->first->get_low() - 1;
		}
	}

	Interval* interval = new Interval(low, high);
	storage->addToMap(interval, std::make_unique<std::thread>(logic::countPrimes, interval));
}

// need EOF check
void logic::intervalProcesser(std::ifstream& in) {

	int low = -1, high = -1, tmp = -1;
	std::string line;
	while (getline(in, line)) {
		if (std::regex_match(line, logic::Regex::INTERVAL_CLOSE))
			break;
		
		logic::checkForRecursive(in, line);

		if (std::regex_search(line, logic::Regex::LOW)) {
			tmp = std::stoi(std::sregex_iterator(line.begin(), line.end(), logic::Regex::LOW)->operator[](1));
			low = (low < 0) ? tmp : ((low < tmp) ? low : tmp);
		}
		else if (std::regex_search(line, logic::Regex::HIGH)) {
			tmp = std::stoi(std::sregex_iterator(line.begin(), line.end(), logic::Regex::HIGH)->operator[](1));
			high = (high < 0) ? tmp : ((high > tmp) ? high : tmp);
		}
	}

	initThread(low, high);
}

void logic::readFromFile(char const * fileName) {

	std::ifstream in(fileName);
	if (!in.is_open())
		return;

	std::string line;
	while (getline(in, line)) {
		if (std::regex_match(line, logic::Regex::INTERVALS_OPEN))
			logic::intervalsProcesser(in);
	}
}

void logic::writeInFIle(char const * fileName) {
	Storage *storage = Storage::getInstance();

	auto vector = storage->getVector();

	std::ofstream out(fileName, std::ios::app);
	if (!out.is_open())
		return;

	out << std::endl << "<root>" << std::endl;
	out << "\t<primes>";
	for (int number : vector)
		out << ' ' << number;
	out << "</primes>" << std::endl;
	out << "</root>";

	out.close();
}

int	 logic::getNextPrime(int start, const int end) {

	while (start <= end) {
		if (logic::isPrime(start))
			return start;
		start++;
	}
	return 0;
}

bool logic::isPrime(const int num) {

	if (!num)
		return false;

	int del = 2, border = num / 2;
	while (del <= border) {
		if (num % del == 0)
			return false;
		del++;
	}
	return true;
}