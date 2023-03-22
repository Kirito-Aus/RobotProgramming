#pragma once
#include "Task.h"
#include "LinearSpeed.h"

class Robot {
public:
	// status
	int nearbyWorkStationId;
	int loadType;
	float timeValue;
	float collisionValue;
	float angularSpeed;
	LinearSpeed linearSpeed;
	float facing;
	Position position;

	Task* assignedTask = &Task::nullTask;

	Robot(){}
};