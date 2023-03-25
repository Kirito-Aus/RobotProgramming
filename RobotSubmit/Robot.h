#pragma once
#include <string>
#include <sstream>
#include "Task.h"
#include "LinearSpeed.h"
#include"Position.h"

class Robot
{
public:
	// status
	int id;
	int nearbyWorkStationId;
	int loadType;
	float timeValue;
	float collisionValue;
	float angularSpeed;
	LinearSpeed linearSpeed;
	float facing;
	Position position;

	Task* assignedTask = &Task::nullTask;

	Robot() {}

	string toString() {
		stringstream ss;
		ss << "Robot{id=" << id << ", load=" << loadType << ", assignedTask=" << assignedTask.toString() << "}";
		return ss.str();
	}
};
