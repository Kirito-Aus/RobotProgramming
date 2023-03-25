#pragma once
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

class CoordinatePos
{
public:
	int x, y;
	CoordinatePos(int x = 0, int y = 0)
		:x(x), y(y) {
	}

	string toString() {
		stringstream buffer;
		buffer << "CoordinatePos{x=" << x << ", y=" << y << "}";
		return buffer.str();
	}
};
