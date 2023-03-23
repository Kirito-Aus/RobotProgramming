#pragma once
#include <string>
#include <sstream>
#include "Position.h"
using std::string;
using std::stringstream;

class WorkStation {
private:
	// get the tier of workstation on production tree
	static int getTier(int type)
	{
		if (type >= 8) return 0;
		if (type == 7) return 1;
		return 2;
	}

public:
	static WorkStation nullWorkStation;

	// status
	int id;
	int type;
	Position position;
	int remainingProduceTime;
	int materialStatus;
	int producionStatus;

	int priority;
	// the priority of task which this workstation request from others
	int requestTaskPriority;
	// wheather the workStation's production is registered
	bool resourcePushed = true;

	WorkStation():id(0), type(0), priority(1000) {}

	void updateRequestPriority() {
		requestTaskPriority = priority * 3 + getTier(type);
	}

	std::string toString() {
		std::stringstream buffer;
		buffer << "WorkStation{id=" << id << ", type=" << type << ", priority=" << priority	<< "}";
		return buffer.str();
	}
};

WorkStation WorkStation::nullWorkStation;
