#pragma once

class Interval {
public:
	inline Interval(int low, int high) : _low(low), _high(high) {}
	~Interval() = default;

	inline int get_low() const { return _low; }
	inline int get_high() const { return _high; }

private:
	int _low;
	int _high;

	Interval() = delete;
};