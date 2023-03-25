#pragma once
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

class Position {
public:
	float x, y;

	Position(float x=0, float y=0)
		:x(x), y(y){
	}

	string toString() {
		stringstream buffer;
		buffer << "Position{x=" << x << ", y=" << y << "}";
		return buffer.str();
	}
};