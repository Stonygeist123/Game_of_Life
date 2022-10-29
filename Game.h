#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <random>
#include <sstream>
using namespace std::chrono_literals;
constexpr unsigned short columns = 50, rows = 50;

template <class T, class Compare> constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
	return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}
template <class T> constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	return clamp(v, lo, hi, std::less<T>{});
}

class Game {
public:
	void start();

private:
	std::array<std::array<bool, columns>, rows> table{};
	std::array<std::array<bool, 3>, 3> get_sub_field(unsigned short, unsigned short);
	void print();
};
